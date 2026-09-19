/**
 * UDP 接收端 - C++ OpenCV 版
 * 接收 ESP32-CAM 的 JPEG 视频流并实时显示
 *
 * 编译:
 *   g++ udp_receiver_opencv.cpp -o udp_receiver_opencv.exe ^
 *       -I <opencv_include> -L <opencv_lib> -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lws2_32
 *
 * 运行:
 *   udp_receiver_opencv.exe [端口]
 *   默认端口: 8080
 */

#include <iostream>
#include <vector>
#include <cstring>
#include <thread>
#include <chrono>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define SOCKET int
    #define INVALID_SOCKET -1
    #define closesocket close
#endif

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#define DEFAULT_PORT     14567
#define MAX_PACKET_SIZE  1400
#define RECV_TIMEOUT_SEC 3
#define MAX_FRAME_SIZE   (300 * 1024)      // 最大帧 300KB (ESP32-CAM JPEG 通常 ~50KB)
#define MIN_FRAME_SIZE   256                // 最小有效帧

static bool g_running = true;

int main(int argc, char *argv[])
{
    int port = (argc > 1) ? std::atoi(argv[1]) : DEFAULT_PORT;

#ifdef _WIN32
    // 初始化 Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[!] WSAStartup failed" << std::endl;
        return -1;
    }
#endif

    // 创建 UDP socket
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "[!] socket() failed" << std::endl;
#ifdef _WIN32
        WSACleanup();
#endif
        return -1;
    }

    // 增大接收缓冲区以避免 UDP 丢包 (默认 8KB 太小)
    int rcvbuf = 512 * 1024; // 512KB
    setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const char *)&rcvbuf, sizeof(rcvbuf));

    // 设置接收超时
#ifdef _WIN32
    int timeout = RECV_TIMEOUT_SEC * 1000;
#else
    struct timeval timeout = {RECV_TIMEOUT_SEC, 0};
#endif
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));

    // 绑定
    struct sockaddr_in local_addr;
    std::memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family      = AF_INET;
    local_addr.sin_port        = htons(port);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        std::cerr << "[!] bind() failed on port " << port << std::endl;
        closesocket(sock);
#ifdef _WIN32
        WSACleanup();
#endif
        return -1;
    }

    std::cout << "[*] Listening on UDP 0.0.0.0:" << port << std::endl;
    std::cout << "[*] Press 'q' or ESC to exit" << std::endl;

    // 接收缓冲区
    char        recv_buf[MAX_PACKET_SIZE];
    std::vector<uint8_t> frame_buf;       // 当前帧累积数据
    uint32_t    expected_total = 0;        // 期望总字节数
    int         frame_count = 0;
    double      fps = 0.0;
    auto        last_fps_time = std::chrono::steady_clock::now();

    cv::namedWindow("ESP32-CAM UDP Stream", cv::WINDOW_NORMAL);

    while (g_running) {
        struct sockaddr_in src_addr;
        socklen_t src_len = sizeof(src_addr);

        int len = recvfrom(sock, recv_buf, MAX_PACKET_SIZE, 0,
                           (struct sockaddr *)&src_addr, &src_len);

        if (len < 0) {
            // 超时：丢弃不完整帧
            if (!frame_buf.empty()) {
                std::cout << "[!] Frame incomplete (timeout), discarding "
                          << frame_buf.size() << " bytes" << std::endl;
                frame_buf.clear();
                expected_total = 0;
            }
            continue;
        }

        if (len == 0) continue;

        bool is_first = frame_buf.empty();

        if (is_first) {
            // 第一个分片：前4字节 = JPEG总长度(大端)
            if (len < 4) continue;

            expected_total = ((uint8_t)recv_buf[0] << 24) |
                             ((uint8_t)recv_buf[1] << 16) |
                             ((uint8_t)recv_buf[2] <<  8) |
                             ((uint8_t)recv_buf[3]);

            // 帧大小合法性检查：防止丢包导致错误解析出巨型尺寸
            if (expected_total < MIN_FRAME_SIZE || expected_total > MAX_FRAME_SIZE) {
                std::cout << "[!] Invalid frame size " << expected_total
                          << ", discarding" << std::endl;
                frame_buf.clear();
                expected_total = 0;
                continue;
            }

            frame_buf.reserve(expected_total);   // 预分配减少重分配
            frame_buf.insert(frame_buf.end(), recv_buf + 4, recv_buf + len);
        } else {
            frame_buf.insert(frame_buf.end(), recv_buf, recv_buf + len);

            // 防呆：累积远超预期 → 帧同步丢失, 丢弃重建
            if (expected_total > 0 && frame_buf.size() > expected_total + MAX_PACKET_SIZE * 2) {
                std::cout << "[!] Frame desync detected, discarding buffer" << std::endl;
                frame_buf.clear();
                expected_total = 0;
                continue;
            }
        }

        // 检查是否收完一帧
        if (expected_total > 0 && frame_buf.size() >= expected_total) {
            // 解码 JPEG
            cv::Mat img = cv::imdecode(frame_buf, cv::IMREAD_COLOR);
            if (!img.empty()) {
                frame_count++;

                // 帧率计算（指数移动平均，平滑显示）
                auto now = std::chrono::steady_clock::now();
                double dt = std::chrono::duration<double>(now - last_fps_time).count();
                last_fps_time = now;
                if (dt > 0) {
                    double inst_fps = 1.0 / dt;
                    fps = (fps == 0.0) ? inst_fps : fps * 0.9 + inst_fps * 0.1;
                }

                // 叠加信息
                char info[128];
                std::snprintf(info, sizeof(info), "FPS:%.1fSize:%uB",
                              fps, expected_total);
                cv::putText(img, info, cv::Point(10, 30),
                            cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);

                char src[64];
                std::snprintf(src, sizeof(src), "IP:%s:%d",
                              inet_ntoa(src_addr.sin_addr), ntohs(src_addr.sin_port));
                cv::putText(img, src, cv::Point(10, 60),
                            cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);

                cv::imshow("ESP32-CAM UDP Stream", img);

                int key = cv::waitKey(1) & 0xFF;
                if (key == 'q' || key == 27) {
                    g_running = false;
                }
            } else {
                std::cout << "[!] Failed to decode JPEG, size=" << expected_total << std::endl;
            }

            // 重置，准备下一帧
            frame_buf.clear();
            expected_total = 0;
        }
    }

    closesocket(sock);
    cv::destroyAllWindows();
#ifdef _WIN32
    WSACleanup();
#endif
    std::cout << "[*] Done. Total frames displayed: " << frame_count << std::endl;
    return 0;
}

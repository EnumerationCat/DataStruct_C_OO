/**
 * TCP 接收端 - C++ OpenCV 版
 * 接收 ESP32-CAM 通过 TCP 发送的 JPEG 视频流并实时显示
 *
 * ESP32 TCP 协议:
 *   [4字节大端帧长度] + [JPEG数据]
 *
 * 编译:
 *   g++ tcp_receiver_opencv.cpp -o tcp_receiver_opencv.exe ^
 *       -I <opencv_include> -L <opencv_lib> -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lws2_32
 *
 * 运行:
 *   tcp_receiver_opencv.exe [端口]
 *   默认端口: 14567
 */

#include <iostream>
#include <vector>
#include <cstring>
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
#define RECV_TIMEOUT_SEC 3
#define MAX_FRAME_SIZE   (300 * 1024)      // 最大帧 300KB

static bool g_running = true;

/**
 * @brief 从TCP socket精确读取n字节
 * @return 成功返回true, 连接断开或出错返回false
 */
static bool recv_all(SOCKET sock, uint8_t *buf, size_t n)
{
    while (n > 0) {
        int len = recv(sock, (char *)buf, n, 0);
        if (len <= 0)
            return false;   // 连接断开或错误
        buf += len;
        n   -= len;
    }
    return true;
}

int main(int argc, char *argv[])
{
    int port = (argc > 1) ? std::atoi(argv[1]) : DEFAULT_PORT;

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[!] WSAStartup failed" << std::endl;
        return -1;
    }
#endif

    // 创建 TCP listening socket
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_sock == INVALID_SOCKET) {
        std::cerr << "[!] socket() failed" << std::endl;
        WSACleanup();
        return -1;
    }

    // 允许地址重用，避免重启时 "Address in use"
    int optval = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR,
               (const char *)&optval, sizeof(optval));

    // 绑定
    struct sockaddr_in local_addr;
    std::memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family      = AF_INET;
    local_addr.sin_port        = htons(port);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        std::cerr << "[!] bind() failed on port " << port << std::endl;
        closesocket(listen_sock);
        WSACleanup();
        return -1;
    }

    // 监听
    if (listen(listen_sock, 1) < 0) {
        std::cerr << "[!] listen() failed" << std::endl;
        closesocket(listen_sock);
        WSACleanup();
        return -1;
    }

    std::cout << "[*] Listening on TCP 0.0.0.0:" << port << std::endl;
    std::cout << "[*] Waiting for ESP32-CAM connection..." << std::endl;
    std::cout << "[*] Press 'q' or ESC to exit" << std::endl;

    cv::namedWindow("ESP32-CAM TCP Stream", cv::WINDOW_NORMAL);

    while (g_running) {
        // 接受客户端连接
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        SOCKET client_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &client_len);

        if (client_sock == INVALID_SOCKET) {
            std::cerr << "[!] accept() failed" << std::endl;
            break;
        }

        char client_ip[64];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        std::cout << "[+] Client connected: " << client_ip << ":"
                  << ntohs(client_addr.sin_port) << std::endl;

        // 设置接收超时
#ifdef _WIN32
        int timeout = RECV_TIMEOUT_SEC * 1000;
#else
        struct timeval timeout = {RECV_TIMEOUT_SEC, 0};
#endif
        setsockopt(client_sock, SOL_SOCKET, SO_RCVTIMEO,
                   (const char *)&timeout, sizeof(timeout));

        // 循环接收帧
        int         frame_count  = 0;
        double      fps          = 0.0;
        auto        last_fps_time = std::chrono::steady_clock::now();

        while (g_running) {
            // 1. 读取4字节帧长度(大端)
            uint8_t header[4];
            if (!recv_all(client_sock, header, 4)) {
                std::cout << "[-] Client disconnected (" << client_ip << ")" << std::endl;
                break;
            }

            uint32_t frame_len = ((uint32_t)header[0] << 24) |
                                ((uint32_t)header[1] << 16) |
                                ((uint32_t)header[2] <<  8) |
                                ((uint32_t)header[3]);

            if (frame_len < 256 || frame_len > MAX_FRAME_SIZE) {
                std::cout << "[!] Invalid frame size " << frame_len
                          << ", discarding" << std::endl;
                // 跳过错误帧
                continue;
            }

            // 2. 读取JPEG数据
            std::vector<uint8_t> jpeg_buf(frame_len);
            if (!recv_all(client_sock, jpeg_buf.data(), frame_len)) {
                std::cout << "[-] Client disconnected during frame (" << client_ip << ")" << std::endl;
                break;
            }

            // 3. 解码并显示
            cv::Mat img = cv::imdecode(jpeg_buf, cv::IMREAD_COLOR);
            if (!img.empty()) {
                frame_count++;

                // 帧率计算(指数移动平均)
                auto now = std::chrono::steady_clock::now();
                double dt = std::chrono::duration<double>(now - last_fps_time).count();
                last_fps_time = now;
                if (dt > 0) {
                    double inst_fps = 1.0 / dt;
                    fps = (fps == 0.0) ? inst_fps : fps * 0.9 + inst_fps * 0.1;
                }

                // 叠加OSD信息
                char info[128];
                std::snprintf(info, sizeof(info), "FPS:%.1f Size:%uB",
                              fps, frame_len);
                cv::putText(img, info, cv::Point(10, 30),
                            cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);

                char src[64];
                std::snprintf(src, sizeof(src), "IP:%s", client_ip);
                cv::putText(img, src, cv::Point(10, 60),
                            cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);

                cv::imshow("ESP32-CAM TCP Stream", img);

                int key = cv::waitKey(1) & 0xFF;
                if (key == 'q' || key == 27) {
                    g_running = false;
                    break;
                }
            } else {
                std::cout << "[!] Failed to decode JPEG, size=" << frame_len << std::endl;
            }
        }

        closesocket(client_sock);
        std::cout << "[*] Connection closed, waiting for next client..." << std::endl;
    }

    closesocket(listen_sock);
    cv::destroyAllWindows();
#ifdef _WIN32
    WSACleanup();
#endif
    std::cout << "[*] Done." << std::endl;
    return 0;
}

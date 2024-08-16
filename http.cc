#include <signal.h>
#include <iostream>
#include <workflow/WFFacilities.h>
#include <wfrest/HttpServer.h>

using std::cout;
using std::string;

static WFFacilities::WaitGroup waitGroup(1);

void sighandler(int signum) {
    cout << "Server stopped!\n";
    waitGroup.done();
}

int main() {
    // 捕获SIGINT信号（通常是Ctrl+C）
    signal(SIGINT, sighandler);

    wfrest::HttpServer server;

    // 处理GET请求 /search
    server.GET("/search", [](const wfrest::HttpReq *req, wfrest::HttpResp *resp) {
        string query = req->query("q"); // 从查询参数中获取搜索关键字
        cout << "Search query: " << query << "\n";

        // 简单搜索逻辑（可以替换为实际的搜索处理逻辑）
        string result = "Results for: " + query;

        // 返回搜索结果
        resp->String(result);
    });

    // 启动服务器并监听指定的端口（例如12345）
    if (server.track().start(8080) == 0) {
        cout << "Server started at http://127.0.0.1:8080\n";
        waitGroup.wait(); // 等待信号停止服务器
        cout << "Server stopped.\n";
        server.stop();
        return 0;
    } else {
        perror("Server start failed!");
        return -1;
    }
}


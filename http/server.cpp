// ��unixϵͳ
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(WIN32)

#include <ws2tcpip.h>
#include <windows.h>

#define close closesocket

class WinSockInit {
    WSADATA _wsa;
public:
    WinSockInit() { // �����׽��ְ汾��Ϣ2.0 WSA������ַ
        WSAStartup(MAKEWORD(2, 0), &_wsa);

    }

    ~WinSockInit() {
        WSACleanup(); // ��ֹws2_32.dll
    }
};

// Unixϵͳ
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <iostream>
#include <string>

using namespace std;

// ����URL
void router(int clientSock, string const &url) {
    string hint;
    cout << url << " �յ���Ϣ\n";
    if (url == "/") {
        hint = "<h1 style=\"text-align: center;\">��ҳ</h1>";
        send(clientSock, hint.c_str(), hint.length(), 0);
    } else if (url == "/test") {
        hint = "���Խӿ�";
        send(clientSock, hint.c_str(), hint.length(), 0);
    } else {
        hint = "404";
        send(clientSock, hint.c_str(), hint.length(), 0);
    }
}

int main() {
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(WIN32)
    WinSockInit sockInit; // Ϊwindowsϵͳ ����WSAStartup
#endif

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // �����׽��� ʧ�ܷ���-1
    sockaddr_in addr = {0};
    addr.sin_family = AF_INET; // ָ����ַ��
    addr.sin_addr.s_addr = INADDR_ANY; // ip��ʼ��
    addr.sin_port = htons(8080); // �˿ںų�ʼ��

    bind(sock, (sockaddr *) &addr, sizeof(addr)); // ����ip�Ͷ˿�

    listen(sock, 0); // ���ü���

    // ���ÿͻ���
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    int clientSock;
    // ���ܿͻ�������
    while (-1 != (clientSock = accept(sock, (sockaddr *) &clientAddr, (socklen_t *) &clientAddrSize))) {
        // ������
        string requestStr;
        int bufSize = 4096;
        requestStr.resize(bufSize);
        // ��������
        recv(clientSock, &requestStr[0], bufSize, 0);

        // ȡ�õ�һ��
        string firstLine = requestStr.substr(0, requestStr.find("\r\n"));
        // ��ȡurl substr ���ƺ��� ����Ϊ��ʼλ��(Ĭ��0) ���Ƶ��ַ���Ŀ
        firstLine = firstLine.substr(firstLine.find(" ") + 1);
        // find�����ҵ��ĵ�һ��ƥ���ַ�����λ�� ����������Ƿ�����ƥ����ַ�����
        string url = firstLine.substr(0, firstLine.find(" "));

        // ������Ӧͷ
        string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html; charset=gbk\r\n"
                "Connection: close\r\n"
                "\r\n";
        send(clientSock, response.c_str(), response.length(), 0);
        // ����url
        router(clientSock, url);
        close(clientSock); // �رտͻ����׽���
    }
    close(sock); // �رշ������׽���
    return 0;
}
#include "HttpHeader.h"
#include <string>
#include <sstream>

std::string HttpHeader::GetHeader() {
    std::stringstream  stream;
    stream << "HTTP/1.1 200 OK\r\n";
    stream << "Host: 172.24.5.55:9006\r\n";
    stream << "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.66 Safari/537.36\r\n";
    stream << "Accept-Encoding: gzip, deflate\r\n";
    stream << "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8\r\n";
    stream << "\r\n";
    return stream.str();
}

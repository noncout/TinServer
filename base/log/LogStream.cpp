#include "LogStream.h"
#include <string.h>

using  namespace  Log;
LogStream &LogStream::operator<<(int value)
{
    buf_.append(std::to_string(value));
    return *this;
}

LogStream &LogStream::operator<<(double value)
{
    buf_.append(std::to_string(value));
    return *this;
}

LogStream &LogStream::operator<<(long value)
{
    buf_.append(std::to_string(value));
    return *this;
}

LogStream &LogStream::operator<<(float value)
{
    buf_.append(std::to_string(value));
    return *this;
}

LogStream &LogStream::operator<<(unsigned int value)
{
    buf_.append(std::to_string(value));
    return *this;
}
LogStream &LogStream::operator<<(unsigned long value)
{
    buf_.append(std::to_string(value));
    return *this;
}
LogStream &LogStream::operator<<(long long int value)
{
    buf_.append(std::to_string(value));
    return *this;
}
LogStream &LogStream::operator<<(long double value)
{
    buf_.append(std::to_string(value));
    return *this;
}

LogStream &LogStream::operator<<(char * value) {
    buf_.append(value);
    return *this;
}

LogStream &LogStream::operator<<(void * value) {
    char * v = reinterpret_cast<char *>(value);
    return operator<<(v);
}

LogStream &LogStream::operator<<(std::string value) {
    buf_.append(value);
    return *this;
}

LogStream &LogStream::operator<<(bool value) {
    if (value) {
        buf_.append("true");
    } else {
        buf_.append("false");
    }
    return *this;
}

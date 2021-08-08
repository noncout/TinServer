#ifndef LOGGING_LOGSTREAM_H
#define LOGGING_LOGSTREAM_H

#include <string>
#include "../noncopyable/noncopyable.h"
namespace  Log {

class LogStream : noncopyable {
    typedef LogStream self;
public:

    self & operator<<(int);

    self & operator<<(double);

    self & operator<<(long);

    self & operator<<(float);

    self & operator<<(unsigned int);

    self & operator<<(unsigned long);

    self & operator<<(long long int);

    self & operator<<(long double);

    self & operator<<(void * value);

    self & operator<<(char * value);

    self & operator<<(std::string value);

    self & operator<<(bool value);

    std::string getData() {
        return buf_;
    }

private:
    std::string buf_;
};

} // Log

#endif //LOGGING_LOGSTREAM_H

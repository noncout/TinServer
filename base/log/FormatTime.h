//
// Created by admin on 2020/10/13.
//

#ifndef LOGGING_FORMATTIME_H
#define LOGGING_FORMATTIME_H

#include <chrono>
#include <sstream>
#include <string>
#include "../noncopyable/noncopyable.h"

using namespace std;
using namespace chrono;

/**
 * 控制日志的格式
 */
namespace Log {
    class FormatTime : public noncopyable {
    public:
       static std::string getCurrentTime(int format = 0);
    };

} // namespace Log
#endif //LOGGING_FORMATTIME_H

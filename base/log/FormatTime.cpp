#include "FormatTime.h"

std::string Log::FormatTime::getCurrentTime(int format) {
    const auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);
    const auto nowMs = std::chrono::duration_cast<std::chrono::microseconds>(
            now.time_since_epoch()) % 100000;
    auto tm = localtime(&t);
    char buffer[100];
    std::string time;
    if(format == 0) {
        strftime (buffer,100,"%F %T",tm);
        time = buffer;
        time.append(".");
    } else {
        strftime (buffer,100,"%F_%H-%M-%S",tm);
        time = buffer;
        time.append("-");
    }

    time.append(std::to_string(nowMs.count()));
    return time;
}

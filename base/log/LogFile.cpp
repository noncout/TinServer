#include "LogFile.h"

#include <time.h>
#include <sys/time.h>
#include <iostream>

#include "FormatTime.h"

LogFile::LogFile(const std::string &fileName,
                 uint64_t maxWritenSize)
        : max_writenSize_(maxWritenSize),
          file_base_name_(fileName) {
    rollFile();
}

void LogFile::append(const char *logline, int len) {
    file_->append(logline,len);
    // 根据大小判断
    if (file_->getWritten() >= max_writenSize_) {
        rollFile();
    } else {
        // 根据时间判断
        time_t  value = time(NULL);
        if (value >= roll_time_) {
            rollFile();
        }
    }
}

void LogFile::flush() {
    file_->flush();
}

bool LogFile::rollFile() {
    std::string appen_name = FormatTime::getCurrentTime(1);
    file_.reset(new FileUtil(appen_name  + "_" + file_base_name_));
    struct timeval tv;
    ::gettimeofday(&tv, nullptr);
    start_time_ = tv.tv_sec;
    roll_time_ = start_time_ + (24 * 60 * 60);
    return false;
}


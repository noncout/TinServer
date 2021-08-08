#ifndef LOGGING_LOGFILE_H
#define LOGGING_LOGFILE_H

/**
 * 控制文件的回滚,同样只能一个线程写，如果需要用到多线程
 * 这个文件的写需要加锁
 *
 */

#include <mutex>
#include <string>
#include <memory>

#include "FileUtil.h"

// 500M = 500 * 1000 * 1000
namespace Log {
#define MAXSIZE  500000000
}

using namespace Log;

class LogFile {
public:
    explicit LogFile(const std::string & fileName,
            uint64_t maxWritenSize = MAXSIZE);

    void append(const char *logline, int len);

    void flush();

private:

    bool rollFile();

private:
    std::unique_ptr<FileUtil> file_; // 真正写文件的位置
    uint64_t max_writenSize_;
    std::string file_base_name_; // 文件的基本名字
    uint64_t  start_time_; // 起始时间
    uint64_t  roll_time_; // 切换时间
};


#endif //LOGGING_LOGFILE_H

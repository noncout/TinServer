#ifndef LOGGING_FILEUTIL_H
#define LOGGING_FILEUTIL_H
#include <stdio.h>
#include <string>

/**
 * 为了提高效率，整个类使用无锁api
 * 整个函数需要在单线程里边进行写
 */
#include <sys/types.h>

namespace  Log {
    class FileUtil {
    public:
        FileUtil(const std::string &fileName);

        ~FileUtil();

        void flush();

        size_t append(const char *line, int len);

        off_t getWritten();

    private:
        FILE *fp_;
        std::string file_name_;
        char buffer_[BUFSIZ * 8]; // 8*8*1024
        off_t writtenBytes_;
    };

} // namespace Log

#endif //LOGGING_FILEUTIL_H

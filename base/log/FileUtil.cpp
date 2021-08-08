#include "FileUtil.h"
#include <assert.h>
#include <stdio.h>
using namespace Log;

FileUtil::FileUtil(const std::string &fileName)
        : fp_(fopen(fileName.c_str(), "ae")),  // append and O_CLOSEXEC
          file_name_(fileName),
          writtenBytes_(0)
{
    assert(fp_);
    ::setbuffer(fp_, buffer_, sizeof(buffer_));
}

FileUtil::~FileUtil() {
    this->flush();
    ::fclose(fp_);
}

void FileUtil::flush() {
    ::fflush(fp_);
}

size_t FileUtil::append(const char *line, int len) {
    // 提高性能
    int n = ::fwrite_unlocked(line, 1, len, fp_);
    writtenBytes_ += n;
    return n;
}

off_t FileUtil::getWritten() {
    return writtenBytes_;
}



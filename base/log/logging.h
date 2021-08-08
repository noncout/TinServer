#ifndef LOGGING_LOGGING_H
#define LOGGING_LOGGING_H

#include <string>
#include <thread>
#include <unistd.h>
#include <sys/syscall.h>

#include "./LogStream.h"
#include "./LogFile.h"
#include "./FormatTime.h"
#include "../Singleton/Singleton.h"
#include "AsyncLog.h"


namespace  Log {
    enum class Level {
        INFO = 4,
        DEBUG = 3,
        WARNING = 2,
        ERROR = 1,
        FATAL = 0,
    };

    class LogImpl : public LogStream {
    public:
        LogImpl(const std::shared_ptr<AsyncLog> & logFile) {
            logFile_ = logFile;
        }

        ~LogImpl() {
            // 每次临时对象销毁时，输出到写文件中
            if (logFile_ != nullptr) {
                *this << "\n";  // 手动写入换行符
                logFile_->append(this->getData().c_str(), getData().size());
                printf("%s", this->getData().c_str());
            }
        }

    private:
        std::shared_ptr<AsyncLog> logFile_;
    };

    // TODO:add level
    class Logging : public Singleton<Logging> {
        public:
        static bool init(const std::string & fileName = "logging.txt",
                         int flushInterval = 1000,
                         uint64_t maxSize = MAXSIZE) {
            file_ptr_.reset(new AsyncLog(fileName,flushInterval, maxSize));
            file_ptr_->start();
            return true;
        }

        static bool stop() {
            file_ptr_->stop();
            return true;
        }

         static void setLogLevel(Level level) {
                out_level_ = level;
         }

         static Level getLogLevel()  {
             return out_level_;
         }

         static void flush() {
             file_ptr_->flush();
         }

         static std::shared_ptr<AsyncLog> getLogFile() {
             return file_ptr_;
         }

        private:
            static std::shared_ptr<AsyncLog> file_ptr_;  // log file
            static Level                     out_level_; // bottom of output
    };

//#define __FUNCTION__ ""

// todo:添加日志等级控制
#define LOG_INFO    LogImpl(Log::Logging::Instance()->getLogFile()) << "["<< FormatTime::getCurrentTime() << "] " \
                    << "[" << syscall(SYS_gettid) << " | " <<  std::string(__FUNCTION__) << " | " << __LINE__ << \
                    "] [INFO]:"

#define LOG_DEBUG   LogImpl(Log::Logging::Instance()->getLogFile()) << "["<< FormatTime::getCurrentTime() << "] " \
                    << "[" << syscall(SYS_gettid) << " | " <<  std::string(__FUNCTION__) << " | " << __LINE__ << \
                    "] [DEBUG]:"

#define LOG_WARNING LogImpl(Log::Logging::Instance()->getLogFile()) << "["<< FormatTime::getCurrentTime() << "] "\
                    << "[" << syscall(SYS_gettid) << " | " <<  std::string(__FUNCTION__) << " | " << __LINE__ << \
                    "] [WARNING]:"

#define LOG_ERROR   LogImpl(Log::Logging::Instance()->getLogFile()) << "["<< FormatTime::getCurrentTime() << "] " \
                    << "[" << syscall(SYS_gettid) << " | " <<  std::string(__FUNCTION__) << " | " << __LINE__ << \
                    "] [ERROR]:"

#define LOG_FATAL   LogImpl(Log::Logging::Instance()->getLogFile()) << "["<< FormatTime::getCurrentTime() << "] "  \
                    << "[" << syscall(SYS_gettid) << " | " <<  std::string(__FUNCTION__) << " | " << __LINE__ << \
                    "] [FATAL]:"

} // namespace Log

#endif //LOGGING_LOGGING_H

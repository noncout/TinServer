#include "logging.h"

std::shared_ptr<AsyncLog> Logging::file_ptr_ = nullptr;
Level Logging::out_level_ = Level::INFO;

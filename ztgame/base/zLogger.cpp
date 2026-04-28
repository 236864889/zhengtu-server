#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include "zLogger.h"

const zLogger::zLevel* zLogger::zLevel::OFF = NULL;
const zLogger::zLevel* zLogger::zLevel::FATAL = NULL;
const zLogger::zLevel* zLogger::zLevel::ERROR = NULL;
const zLogger::zLevel* zLogger::zLevel::ALARM = NULL;
const zLogger::zLevel* zLogger::zLevel::WARN = NULL;
const zLogger::zLevel* zLogger::zLevel::IFFY = NULL;
const zLogger::zLevel* zLogger::zLevel::INFO = NULL;
const zLogger::zLevel* zLogger::zLevel::TRACE = NULL;
const zLogger::zLevel* zLogger::zLevel::DEBUG = NULL;
const zLogger::zLevel* zLogger::zLevel::GBUG = NULL;
const zLogger::zLevel* zLogger::zLevel::ALL = NULL;

const LevelPtr zLogger::zLevel::LEVELALARM;
const LevelPtr zLogger::zLevel::LEVELIFFY;
const LevelPtr zLogger::zLevel::LEVELTRACE;
const LevelPtr zLogger::zLevel::LEVELGBUG;

zLogger::zLevel::zLevel(LevelPtr level) : zlevel(level) {}

zLogger::zLoggerLocalFileAppender::zLoggerLocalFileAppender() {}
zLogger::zLoggerLocalFileAppender::~zLoggerLocalFileAppender() {}
void zLogger::zLoggerLocalFileAppender::activateOptions() {}

zLogger::zLogger(const log4cxx::LogString& name) {
    bzero(message, sizeof(message));
}

zLogger::~zLogger() {}

void zLogger::setName(const log4cxx::LogString& name) {}

bool zLogger::addConsoleLog() { return true; }
void zLogger::removeConsoleLog() {}

bool zLogger::addLocalFileLog(const log4cxx::LogString& filename) { return true; }
void zLogger::removeLocalFileLog(const log4cxx::LogString& filename) {}

bool zLogger::addSysLog(const log4cxx::LogString& facility) { return true; }
void zLogger::removeSysLog(const log4cxx::LogString& facility) {}

static void printLog(const char* level, const char* fmt, va_list ap) {
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char time_str[26];
    strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    char msg[4096];
    vsnprintf(msg, sizeof(msg), fmt, ap);
    printf("[%s] %s: %s\n", time_str, level, msg);
}

bool zLogger::log(const zLevel* level, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printLog("LOG", fmt, ap);
    va_end(ap);
    return true;
}

bool zLogger::forceLog(const zLevel* level, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printLog("FORCE", fmt, ap);
    va_end(ap);
    return true;
}

bool zLogger::fatal(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printLog("FATAL", fmt, ap);
    va_end(ap);
    return true;
}

bool zLogger::error(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printLog("ERROR", fmt, ap);
    va_end(ap);
    return true;
}

bool zLogger::error_out(const char* str) {
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char time_str[26];
    strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("[%s] ERROR: %s\n", time_str, str);
    return true;
}

bool zLogger::warn(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printLog("WARN", fmt, ap);
    va_end(ap);
    return true;
}

bool zLogger::info(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printLog("INFO", fmt, ap);
    va_end(ap);
    return true;
}

bool zLogger::debug(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printLog("DEBUG", fmt, ap);
    va_end(ap);
    return true;
}

bool zLogger::alarm(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printLog("ALARM", fmt, ap);
    va_end(ap);
    return true;
}

bool zLogger::iffy(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printLog("IFFY", fmt, ap);
    va_end(ap);
    return true;
}

bool zLogger::trace(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printLog("TRACE", fmt, ap);
    va_end(ap);
    return true;
}

bool zLogger::gbug(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printLog("GBUG", fmt, ap);
    va_end(ap);
    return true;
}
void zLogger::setLevel(const std::string& level) {
    // 简化实现
}

#ifndef LOGGER_H
#define LOGGER_H

#include <time.h>
#include <stdio.h>

typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogType;

#define __LOG(logType, msg, ...) do {               \
    time_t timeTicks = time(NULL);                  \
    struct tm* timeInfo = localtime(&timeTicks);    \
    printf("[%04d-%02d-%02d %02d:%02d:%02d] ",      \
            timeInfo->tm_year + 1900,               \
            timeInfo->tm_mon + 1,                   \
            timeInfo->tm_mday,                      \
            timeInfo->tm_hour,                      \
            timeInfo->tm_min,                       \
            timeInfo->tm_sec);                      \
    switch (logType) {                              \
        case LOG_INFO:                              \
            printf("[INFO] ");                      \
            break;                                  \
        case LOG_WARNING:                           \
            printf("\033[0;33m[WARNING]\033[0m ");  \
            break;                                  \
        case LOG_ERROR:                             \
            printf("\033[0;31m[ERROR]\033[0m ");    \
            break;                                  \
    }                                               \
    printf(msg, ##__VA_ARGS__);                     \
    printf("\n");                                   \
    } while (0)                                     \

#define LOG_INFO(msg, ...) __LOG(LOG_INFO, msg, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...) __LOG(LOG_WARNING, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) __LOG(LOG_ERROR, msg, ##__VA_ARGS__)
#endif
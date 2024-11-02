//
//  logger.c
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include "logger.h"

#define MAX_LOG_LENGTH 1024
#define LOG_TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"

static FILE* log_file = NULL;
static LogLevel current_log_level = LOG_INFO;

void logger_init(const char* filename, LogLevel level) {
    log_file = fopen(filename, "a");
    if (log_file == NULL) {
        fprintf(stderr, "Error: Could not open log file %s\n", filename);
        return;
    }
    current_log_level = level;
}

void logger_close(void) {
    if (log_file != NULL) {
        fclose(log_file);
        log_file = NULL;
    }
}

void logger_set_level(LogLevel level) {
    current_log_level = level;
}

static const char* level_to_string(LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO:  return "INFO";
        case LOG_WARN:  return "WARN";
        case LOG_ERROR: return "ERROR";
        default:        return "UNKNOWN";
    }
}

void logger_log(LogLevel level, const char* file, int line, const char* format, ...) {
    if (level < current_log_level || log_file == NULL) {
        return;
    }

    time_t now;
    time(&now);
    struct tm* timeinfo = localtime(&now);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), LOG_TIMESTAMP_FORMAT, timeinfo);

    char message[MAX_LOG_LENGTH];
    va_list args;
    va_start(args, format);
    vsnprintf(message, MAX_LOG_LENGTH, format, args);
    va_end(args);

    fprintf(log_file, "[%s] [%s] %s:%d - %s\n", 
            timestamp, 
            level_to_string(level), 
            file, 
            line, 
            message);
    fflush(log_file);
}

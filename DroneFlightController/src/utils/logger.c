#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include "logger.h"

#define MAX_LOG_LENGTH 1024
#define LOG_TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"

static FILE* log_file = NULL;
static LogLevel current_log_level = LOG_INFO;
static LogDestination current_log_destination = LOG_TO_FILE;

void logger_init(const char* destination, LogLevel level, LogDestination log_dest) {
    current_log_level = level;
    current_log_destination = log_dest;

    if (log_dest == LOG_TO_FILE) {
        log_file = fopen(destination, "a");
        if (log_file == NULL) {
            fprintf(stderr, "Error: Could not open log file %s\n", destination);
            return;
        }
    } else if (log_dest == LOG_TO_FLASH || log_dest == LOG_TO_EXTERNAL_STORAGE) {
        // Initialize flash memory or external storage logging
        // This is a placeholder for actual implementation
    }
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
    if (level < current_log_level) {
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

    if (current_log_destination == LOG_TO_FILE && log_file != NULL) {
        fprintf(log_file, "[%s] [%s] %s:%d - %s\n", 
                timestamp, 
                level_to_string(level), 
                file, 
                line, 
                message);
        fflush(log_file);
    } else if (current_log_destination == LOG_TO_FLASH || current_log_destination == LOG_TO_EXTERNAL_STORAGE) {
        // Log to flash memory or external storage
        // This is a placeholder for actual implementation
    }
}

void log_to_flash(const char* message) {
    // Placeholder function to log data to flash memory
    // Implement actual flash memory logging here
}

void log_to_external_storage(const char* message) {
    // Placeholder function to log data to external storage
    // Implement actual external storage logging here
}

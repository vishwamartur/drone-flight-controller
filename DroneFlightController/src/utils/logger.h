//
//  logger.h
//  DroneFlightController
//
//  Created by Vishwanath Martur on 11/1/24.
//

#ifndef logger_h
#define logger_h

// Log levels
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN, 
    LOG_ERROR
} LogLevel;

// Initialize logger with filename and minimum log level
void logger_init(const char* filename, LogLevel level);

// Close logger and free resources
void logger_close(void);

// Set minimum log level
void logger_set_level(LogLevel level);

// Log a message with specified level, source file, line number and format string
void logger_log(LogLevel level, const char* file, int line, const char* format, ...);

#endif /* logger_h */

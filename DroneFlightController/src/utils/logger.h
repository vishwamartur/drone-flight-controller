#ifndef logger_h
#define logger_h

// Log levels
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN, 
    LOG_ERROR
} LogLevel;

// Log destinations
typedef enum {
    LOG_TO_FILE,
    LOG_TO_FLASH,
    LOG_TO_EXTERNAL_STORAGE
} LogDestination;

// Initialize logger with destination, minimum log level, and log destination
void logger_init(const char* destination, LogLevel level, LogDestination log_dest);

// Close logger and free resources
void logger_close(void);

// Set minimum log level
void logger_set_level(LogLevel level);

// Log a message with specified level, source file, line number and format string
void logger_log(LogLevel level, const char* file, int line, const char* format, ...);

// Log data to flash memory
void log_to_flash(const char* message);

// Log data to external storage
void log_to_external_storage(const char* message);

#endif /* logger_h */

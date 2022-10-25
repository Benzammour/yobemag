#ifndef YOBEMAG_LOG_H
#define YOBEMAG_LOG_H

#include <stdio.h>

typedef enum LoggingLevel {
    DEBUG = -1,
    INFO,
    WARNING,
    ERROR,
    FATAL
} LoggingLevel;

/**
 * @brief   Set the minimum required of a log message to be printed to the console
 *
 * @param   log_lvl The desired minimal logging level
 */
void log_set_lvl(LoggingLevel log_lvl);

/**
 * @brief   Flash stdout and stderr on exit
 */
void log_teardown(void);

/**
 * @brief 	Log a string to stderr with logging level ::FATAL formatted as
 * 			@p msg with parameters @p ...
 * 			This function is invoked by `#YOBEMAG_EXIT(msg, ...)`.
 *
 *
 * @param 	file_path 	Passed by `#YOBEMAG_EXIT(msg, ...)`. Indicates invocation site
 * @param 	line_number Passed by `#YOBEMAG_EXIT(msg, ...)`. Indicates specific invocation line
 * @param 	msg 		Format string for the log message
 * @param 	...			Parameters for format string
 */
_Noreturn __attribute__((format(printf, 3, 4))) void log_exit(char const *file_path, int line_number, char const *msg,
                                                              ...);

/**
 * @brief   Log a string to @p stream with logging level @p dbg_lvl, (printed as @p log_lvl_str),
 * 			formatted as @p msg with parameters @p ...
 *
 * @param   log_lvl     Logging level, represented as ::LoggingLevel
 * @param   log_lvl_str Logging level string representation
 * @param   stream      Stream to print to (commonly stderr/stdout)
 * @param   msg         Format string for the log message
 * @param   ...         Parameters for format string
 */
__attribute__((format(printf, 4, 5))) void log_str(LoggingLevel log_lvl, char const *log_lvl_str, FILE *stream,
                                                   char const *msg, ...);

/*
 * This is necessary, since __VA_ARGS__ is a C GNU extension.
 * clang will not compile with -Werror, if this warning is not ignored here.
 * Ignoring is fine, since clang can handle this GNU extension.
 *
 * The second ignored diagnostic surpresses "Macro not used" messages.
 */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#pragma clang diagnostic ignored "-Wunused-macros"

#define LOG_DEBUG(msg, ...)   log_str(DEBUG, "DEBUG", stdout, msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...)    log_str(INFO, "INFO", stdout, msg, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...) log_str(WARNING, "WARNING", stdout, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...)   log_str(ERROR, "ERROR", stderr, msg, ##__VA_ARGS__)
#define LOG_FATAL(msg, ...)   log_str(FATAL, "FATAL", stderr, msg, ##__VA_ARGS__)

#define YOBEMAG_EXIT(msg, ...) log_exit(__FILE__, __LINE__, msg, ##__VA_ARGS__)

#pragma clang diagnostic pop

#endif // YOBEMAG_LOG_H

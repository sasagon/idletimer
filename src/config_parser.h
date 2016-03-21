#ifndef __CONFIG_PARSER_H__
#define __CONFIG_PARSER_H__
#include <stdio.h>
#include <stdbool.h>
#include "command_map.h"

#define CONFIG_PARSER_MAX_LINE_LENGTH   4096
#define CONFIG_PARSER_COMMENT_CHAR      '#'
#define CONFIG_PARSER_DELIMITER         ":"

typedef enum {
    NO_ERROR,
    TOO_LONG_LINE,
    TOO_SHORT_MINUTES,
    TOO_LONG_MINUTES,
    ILLEGAL_COMMAND_TYPE,
    ILLEGAL_MINUTES,
    ILLEGAL_LINE_FORMAT
} ConfigErrorType;

typedef struct {
    CommandMap* idle_commands;
    CommandMap* wakeup_commands;
} Config; 

typedef bool (*config_error_handler_t)(
    ConfigErrorType, const char* filename, int line_number);

Config* parse_config(FILE*, const char* filename, config_error_handler_t);
void delete_config(Config*);

#endif

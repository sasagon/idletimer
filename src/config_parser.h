#ifndef __CONFIG_PARSER_H__
#define __CONFIG_PARSER_H__
#include <stdio.h>
#include "command_map.h"

typedef struct {
    CommandMap* idle_commands;
    CommandMap* wakeup_commands;
} Config;

Config* parse_config(FILE*, const char* filename);
void delete_config(Config*);

#endif

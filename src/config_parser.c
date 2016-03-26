#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <glib.h>
#include "config_parser.h"

#define NUMBER_OF_COLUMNS 3


static bool parse_line(Config* p, const char* s,
    config_error_handler_t error_handler,
    const char* filename, int line_number, void* data)
{
    assert(s);
    assert(error_handler);

    gchar** t = g_strsplit(s, CONFIG_PARSER_DELIMITER, NUMBER_OF_COLUMNS);
    char* endptr = NULL;

    if (g_strv_length(t) == NUMBER_OF_COLUMNS) {
        unsigned long minute = strtoul(t[1], &endptr, 10);
        if (endptr == t[1] || *endptr != '\0') {
            g_strfreev(t);
            return error_handler(ILLEGAL_MINUTES, filename, line_number, data);
        } else if (minute == 0) {
            g_strfreev(t);
            return error_handler(TOO_SHORT_MINUTES, filename, line_number, data);
        } else if (minute == ULONG_MAX) {
            g_strfreev(t);
            return error_handler(TOO_LONG_MINUTES, filename, line_number, data);
        }
        if (strcmp(t[0], "idle") == 0) {
            add_command(p->idle_commands, minute, t[2]);
        } else if (strcmp(t[0], "wakeup") == 0) {
            add_command(p->wakeup_commands, minute, t[2]);
        } else {
            g_strfreev(t);
            return error_handler(ILLEGAL_COMMAND_TYPE, filename, line_number, data);
        }
    } else {
        g_strfreev(t);
        return error_handler(ILLEGAL_LINE_FORMAT, filename, line_number, data);
    }
    g_strfreev(t);
    return true;
}


Config* parse_config(
    FILE* f, const char* filename,
    config_error_handler_t error_handler, void* data)
{
    assert(f);
    assert(error_handler);

    Config* p = malloc(sizeof(Config));
    p->idle_commands = create_command_map();
    p->wakeup_commands = create_command_map();

    int line_number = 0;
    char line_buffer[CONFIG_PARSER_MAX_LINE_LENGTH + 2];
    bool line_continued = false;

    while (fgets(line_buffer, sizeof(line_buffer), f) != NULL) {
        line_number++;
        if (strlen(line_buffer) > CONFIG_PARSER_MAX_LINE_LENGTH) {
            if (!error_handler(TOO_LONG_LINE, filename, line_number, data)) {
                break;
            }
            line_continued = true;
            continue;
        }
        if (line_continued) {
            line_continued = false;
            continue;
        }
        char* t = strpbrk(line_buffer, "\r\n");
        if (t != NULL) {
            *t = '\0';
        }
        if (*line_buffer == CONFIG_PARSER_COMMENT_CHAR) {
            continue;
        } else if (*line_buffer == '\0') {
            continue;
        }
        if (!parse_line(p, line_buffer, error_handler, filename, line_number, data)) {
            break;
        }
    }
    return p;
}


void delete_config(Config* p)
{
    if (p) {
        delete_command_map(p->wakeup_commands);
        delete_command_map(p->idle_commands);
    }
}


int get_num_of_entries_in_config(Config* p)
{
    assert(p);
    return
      get_num_of_entries_in_command_map(p->idle_commands) +
      get_num_of_entries_in_command_map(p->wakeup_commands);
}

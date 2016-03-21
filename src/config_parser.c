#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <glib.h>
#include "config_parser.h"

#define MAX_LINE_LENGTH   4096
#define COMMENT_CHAR      '#'
#define DELIMITER         ":"
#define NUMBER_OF_COLUMNS 3

#define SUCCESS 0
#define FAIL    -1

static int parse_line(Config* p, const char* s)
{
    assert(p);
    assert(s);

    gchar** t = g_strsplit(s, DELIMITER, NUMBER_OF_COLUMNS);

    if (g_strv_length(t) == NUMBER_OF_COLUMNS) {
      unsigned long minute = strtoul(t[1], NULL, 10);
      if (strcmp(t[0], "idle") == 0) {
          add_command(p->idle_commands, minute, t[2]);
      } else if (strcmp(t[0], "wakeup") == 0) {
          add_command(p->wakeup_commands, minute, t[2]);
      } else {
          g_strfreev(t);
          return FAIL;
      }
    }
    g_strfreev(t);
    return SUCCESS;
}


Config* parse_config(FILE* f, const char* filename)
{
    assert(f);

    Config* p = malloc(sizeof(Config));
    p->idle_commands = create_command_map();
    p->wakeup_commands = create_command_map();

    int line_number = 0;
    char line_buffer[MAX_LINE_LENGTH + 2];
    bool line_continued = false;

    while (fgets(line_buffer, sizeof(line_buffer), f) != NULL) {
        line_number++;
        if (strlen(line_buffer) > MAX_LINE_LENGTH) {
            if (filename != NULL) {
              fprintf(stderr, "Warning: %s:%d: too long line\n", filename, line_number);
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
        if (*line_buffer == COMMENT_CHAR) {
            continue;
        } else if (*line_buffer == '\0') {
            continue;
        }
        if (parse_line(p, line_buffer) != SUCCESS) {
            fprintf(stderr, "Warning: %s:%d: illegal line\n", filename, line_number);
        }
    }
    if (is_command_map_empty(p->idle_commands)
     && is_command_map_empty(p->wakeup_commands)) {
        fprintf(stderr, "Error: %s: no effective lines\n", filename);
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

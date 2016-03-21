#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "command_map.h"

typedef struct {
  unsigned long idle_minutes;
  int num_of_commands;
  char** commands;
} CommandArray;


static void add_command_to_command_array(CommandArray* p, const char* command)
{
    assert(p);
    assert(command);

    int i = p->num_of_commands++;
    p->commands = realloc(p->commands, sizeof(char*[p->num_of_commands + 1]));
    p->commands[i] = strdup(command);
    p->commands[i + 1] = NULL;
}


static CommandArray* create_command_array(unsigned long idle_minutes, const char* command)
{
    CommandArray* p = malloc(sizeof(CommandArray));
    p->idle_minutes = idle_minutes;
    p->num_of_commands = 0;
    p->commands = NULL;
    add_command_to_command_array(p, command);
    return p;
}


static void delete_command_array(CommandArray* p)
{
    int i;

    if (p) {
        for (i = 0; i < p->num_of_commands; i++) {
            free(p->commands[i]);
        }
        free(p->commands);
        free(p);
    }
}


struct _CommandMap {
  int num_of_command_arrays;
  CommandArray** command_arrays;
};


CommandMap* create_command_map()
{
    CommandMap* p = malloc(sizeof(CommandMap));
    p->num_of_command_arrays = 0;
    p->command_arrays = NULL;
    return p;
}


void delete_command_map(CommandMap* p)
{
    int i;

    if (p) {
        for (i = 0; i < p->num_of_command_arrays; i++) {
            delete_command_array(p->command_arrays[i]);
        }
        free(p->command_arrays);
        free(p);
    }
}


static CommandArray* find_command_array(const CommandMap* p, unsigned long idle_minutes)
{
    int i;

    for (i = 0; i < p->num_of_command_arrays; i++) {
        if (p->command_arrays[i]->idle_minutes == idle_minutes) {
            return p->command_arrays[i];
        }
    }
    return NULL;
}


void add_command(CommandMap* p, unsigned long idle_minutes, const char* command)
{
    assert(p);
    assert(command);

    CommandArray* q = find_command_array(p, idle_minutes);

    if (q) {
        add_command_to_command_array(q, command);
    } else {
        int i = p->num_of_command_arrays++;
        p->command_arrays = realloc(p->command_arrays, sizeof(CommandArray[i + 1]));
        p->command_arrays[i] = create_command_array(idle_minutes, command);
    }
}


bool is_command_map_empty(const CommandMap* p)
{
    assert(p);
    return p->num_of_command_arrays <= 0;
}


const char*const* find_equals(const CommandMap* p, unsigned long idle_minutes)
{
    assert(p);

    const CommandArray* q = find_command_array(p, idle_minutes);

    if (q) {
        return (const char**)q->commands;
    } else {
        static const char* empty_string_array[] = { NULL };
        return empty_string_array;
    }
}


const char** find_less_equals(const CommandMap* p, unsigned long idle_minutes)
{
    assert(p);

    int i, j;
    const char** found_commands = NULL;

    for (i = j = 0; i < p->num_of_command_arrays; i++) {
        assert(p->command_arrays);
        if (p->command_arrays[i]->idle_minutes <= idle_minutes) {
            char** q = p->command_arrays[i]->commands;
            int k = p->command_arrays[i]->num_of_commands;
            found_commands = realloc(found_commands, sizeof(char*[j + k + 1]));
            while (*q) {
                found_commands[j++] = *q++;
            }
            found_commands[j] = NULL;
        }
    }
    if (found_commands) {
        return found_commands;
    } else {
        const char** q = malloc(sizeof(char*[1]));
        q[0] = NULL;
        return q;
    }
}


void delete_found_commands(const char** p)
{
    free(p);
}

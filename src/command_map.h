#ifndef __COMMAND_MAP__
#define __COMMAND_MAP__
#include <stdbool.h>

typedef struct _CommandMap CommandMap;

CommandMap* create_command_map();
void delete_command_map(CommandMap*);

void add_command(CommandMap*, unsigned long minutes, const char* command);
bool is_command_map_empty(const CommandMap*);
int get_num_of_entries_in_command_map(const CommandMap*);
const char*const* find_equals(const CommandMap*, unsigned long minutes);
const char** find_less_equals(const CommandMap*, unsigned long minutes);
void delete_found_commands(const char**);

void traverse_command_map(
    const CommandMap*,
    bool (*handler)(void* data, unsigned long minutes, const char* command),
    void* data);

#endif

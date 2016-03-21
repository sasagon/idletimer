#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <gtk/gtk.h>
#include "idle_timer.h"
#include "config_parser.h"

#define DOTFILE_NAME   ".idle_timer"

static void sigint_handler(int sig)
{
    kill(getpid(), SIGTERM);
}


static void sigterm_handler(int sig)
{
    gtk_main_quit();
}


static void set_signal_handlers()
{
    if (signal(SIGINT, sigint_handler) == SIG_ERR
     || signal(SIGTERM, sigterm_handler) == SIG_ERR) {
        perror("signal()");
        exit(EXIT_FAILURE);
    }
}


static void exec_commands(const char*const* commands)
{
    assert(commands);

    int i;

    for (i = 0; commands[i]; i++) {
        if (system(commands[i]) != 0) {
          fprintf(stderr, "Error: command execution failed: %s\n", commands[i]);
        }
    }
}


static void idle_listener(unsigned long idle_minutes, void* data)
{
    assert(data);
    const CommandMap* idle_commands = (CommandMap*)data;
    exec_commands(find_equals(idle_commands, idle_minutes));
}


static void wakeup_listener(unsigned long idle_minutes, void* data)
{
    assert(data);
    const CommandMap* wakeup_commands = (CommandMap*)data;
    const char** commands = find_less_equals(wakeup_commands, idle_minutes);
    exec_commands(commands);
    delete_found_commands(commands);
}


static void set_timer_handlers(IdleTimer* idle_timer, const Config* config)
{
    add_idle_listener(idle_timer, idle_listener, config->idle_commands);
    add_wakeup_listener(idle_timer, wakeup_listener, config->wakeup_commands);
}


static Config* load_config()
{
    gchar* dotfile_path = g_build_filename(g_get_home_dir(), DOTFILE_NAME, NULL);
    FILE* f = fopen(dotfile_path, "r");
    if (f == NULL) {
        fprintf(stderr, "Error: %s: %s\n", dotfile_path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    Config* config = parse_config(f, dotfile_path);
    fclose(f);
    g_free(dotfile_path);
    return config;
}


int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);
    set_signal_handlers();

    Config* config = load_config();
    IdleTimer* idle_timer = create_idle_timer();
    set_timer_handlers(idle_timer, config);

    gtk_main();

    delete_idle_timer(idle_timer);
    delete_config(config);

    return EXIT_SUCCESS;
}

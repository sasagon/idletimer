#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include "idle_timer.h"
#include "config_parser.h"

#define VERSION        "0.1"
#define DOTFILE_NAME   ".idletimer"

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


static bool verbose_mode = false;

static void set_verbose_mode(bool b)
{
    verbose_mode = b;
    if (b) {
        setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
    }
}

static void print_verbose_log(const char* format, ...)
{
    assert(format);

    if (verbose_mode) {
        va_list arg;
        va_start(arg, format);
        vfprintf(stdout, format, arg);
        va_end(arg);
    }
}

static void print_error_log(const char* format, ...)
{
    assert(format);

    va_list arg;
    va_start(arg, format);
    vfprintf((verbose_mode)? stdout : stderr, format, arg);
    va_end(arg);
}


static void exec_commands(const char*const* commands)
{
    assert(commands);

    int i;

    for (i = 0; commands[i]; i++) {
        print_verbose_log("Execute: %s\n", commands[i]);
        if (system(commands[i]) != 0) {
            print_error_log("Execution failed: %s\n", commands[i]);
        }
    }
}


static void idle_listener(unsigned long idle_minutes, void* data)
{
    assert(data);

    const CommandMap* idle_commands = (CommandMap*)data;
    const char*const* commands = find_equals(idle_commands, idle_minutes);
    if (commands[0] != NULL) {
        print_verbose_log("Idling for %lu minute(s).\n", idle_minutes);
        exec_commands(commands);
    }
}


static void wakeup_listener(unsigned long idle_minutes, void* data)
{
    assert(data);

    const CommandMap* wakeup_commands = (CommandMap*)data;
    const char** commands = find_less_equals(wakeup_commands, idle_minutes);
    if (commands[0] != NULL) {
        print_verbose_log("Wake up after %lu minute(s) idling.\n", idle_minutes);
        exec_commands(commands);
    }
    delete_found_commands(commands);
}


static void set_timer_handlers(IdleTimer* idle_timer, const Config* config)
{
    add_idle_listener(idle_timer, idle_listener, config->idle_commands);
    add_wakeup_listener(idle_timer, wakeup_listener, config->wakeup_commands);
}


static const char* config_error_format(ConfigErrorType type)
{
    switch (type) {
    case NO_ERROR:
        return "";
    case TOO_LONG_LINE:
        return "Warning: %s:%d: too long line.\n";
    case TOO_SHORT_MINUTES:
        return "Warning: %s:%d: 0 minute not allowed.\n";
    case TOO_LONG_MINUTES:
        return "Warning: %s:%d: too long minute specified.\n";
    case ILLEGAL_COMMAND_TYPE:
        return "Warning: %s:%d: line must start with 'idle' or 'wakeup'.\n";
    case ILLEGAL_MINUTES:
        return "Warning: %s:%d: illegal minutes string found.\n";
    case ILLEGAL_LINE_FORMAT:
        return "Warning: %s:%d: illegal line format.\n";
    default:
        return NULL;
    }
}


static bool config_error_handler(
    ConfigErrorType type, const char* filename, int line_number)
{
    assert(filename);

    const char* format = config_error_format(type);

    if (format != NULL) {
        fprintf(stderr, format, filename, line_number);
        return true; 
    } else {
        fprintf(stderr,
            "Error: %s:%d: unknown error.\n", filename, line_number);
        return false;
    }
}


static Config* load_config(const char* config_file_path)
{
    assert(config_file_path);

    FILE* f = fopen(config_file_path, "r");
    if (f == NULL) {
        fprintf(stderr, "Error: %s: %s\n", config_file_path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    Config* config = parse_config(f, config_file_path, &config_error_handler);
    fclose(f);

    if (is_command_map_empty(config->idle_commands)
     && is_command_map_empty(config->wakeup_commands)) {
        fprintf(stderr, "Error: %s: no effective lines.\n", config_file_path);
        exit(EXIT_FAILURE);
    }
    return config;
}


static bool print_command(
    void* data, unsigned long minutes, const char* command)
{
    assert(data);
    assert(command);

    const char* command_type = (char*)data;
    fprintf(stdout, "%s:%lu:%s\n", command_type, minutes, command);
    return true;
}


static void print_config(const Config* config)
{
    assert(config);
    traverse_command_map(config->idle_commands, print_command, "idle");
    traverse_command_map(config->wakeup_commands, print_command, "wakeup");
}


static void print_usage(const char* prgname)
{
    fprintf(
        stderr,
        "Usage:\n"
        "  %s [-h] [-v] [-c config_file]\n"
        "\n"
        "Options:\n"
        "  -h                 print this message.\n"
        "  -v                 print configurations and actions verbosely.\n"
        "  -c config_file     specify a config file instead of ~/%s\n"
        "\n"
        "Version:\n"
        "  %s"
        "\n",
        prgname, DOTFILE_NAME, VERSION);
}


typedef struct {
    bool help;
    bool verbose;
    gchar* config_file_path;
} Options;


Options parse_commandline_options(int argc, char* argv[])
{
    Options options = {
        false,
        false,
        g_build_filename(g_get_home_dir(), DOTFILE_NAME, NULL)
    };

    for (;;) {
        switch (getopt(argc, argv, "hvc:")) {
        case 'h':
            options.help = true;
            break;
        case 'v':
            options.verbose = true;
            break;
        case 'c':
            g_free(options.config_file_path);
            options.config_file_path = g_strdup(optarg);
            break;
        case -1:
            return options;
        }
    }
}


int main(int argc, char* argv[])
{
    Options options = parse_commandline_options(argc, argv);

    if (options.help) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    gtk_init(&argc, &argv);
    set_signal_handlers();

    Config* config = load_config(options.config_file_path);

    if (options.verbose) {
        set_verbose_mode(true);
        print_verbose_log("%s loaded.\n", options.config_file_path);
        print_config(config);
        print_verbose_log("Watching your input...\n");
    }

    IdleTimer* idle_timer = create_idle_timer();
    set_timer_handlers(idle_timer, config);

    gtk_main();

    delete_idle_timer(idle_timer);
    delete_config(config);

    g_free(options.config_file_path);
    return EXIT_SUCCESS;
}

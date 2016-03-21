TESTS = idle_timer_test command_map_test config_parser_test
#dist_noinst_DATA = $(TESTS)

AM_CFLAGS = -I..

check_PROGRAMS = idle_timer_test command_map_test config_parser_test

idle_timer_test_SOURCES = idle_timer_test.c
idle_timer_test_CFLAGS = $(GTK_CFLAGS) $(X11_CFLAGS)
idle_timer_test_LDADD = ../libidletimer.a $(GTK_LIBS) $(X11_LIBS) -lcunit 

command_map_test_SOURCES = command_map_test.c
command_map_test_CFLAGS = $(GTK_CFLAGS) $(X11_CFLAGS)
command_map_test_LDADD = ../libidletimer.a $(GTK_LIBS) $(X11_LIBS) -lcunit 

config_parser_test_SOURCES = config_parser_test.c
config_parser_test_CFLAGS = $(GTK_CFLAGS) $(X11_CFLAGS)
config_parser_test_LDADD = ../libidletimer.a $(GTK_LIBS) $(X11_LIBS) -lcunit 

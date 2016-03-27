TESTS = idle_timer_test command_map_test config_parser_test run-test.sh
dist_noinst_SCRIPTS = run-test.sh
dist_noinst_DATA = run-test.config
CLEANFILES=idletimer.log idletimer-command.log idletimer-error.log

run-test-warning:
	@echo "******************************************************"
	@echo " Out tests may take more than 2 minutes. Please wait. "
	@echo "******************************************************"

check_PROGRAMS = idle_timer_test command_map_test config_parser_test
check_DATA = run-test-warning

idle_timer_test_SOURCES = idle_timer_test.c
idle_timer_test_CFLAGS = $(GTK_CFLAGS) $(X11_CFLAGS)
idle_timer_test_LDADD = ../libidletimer.a $(GTK_LIBS) $(X11_LIBS) -lcunit 

command_map_test_SOURCES = command_map_test.c
command_map_test_CFLAGS = $(GTK_CFLAGS) $(X11_CFLAGS)
command_map_test_LDADD = ../libidletimer.a $(GTK_LIBS) $(X11_LIBS) -lcunit 

config_parser_test_SOURCES = config_parser_test.c
config_parser_test_CFLAGS = $(GTK_CFLAGS) $(X11_CFLAGS)
config_parser_test_LDADD = ../libidletimer.a $(GTK_LIBS) $(X11_LIBS) -lcunit 

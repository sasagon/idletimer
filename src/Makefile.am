SUBDIRS = . test

bin_PROGRAMS = idletimer
idle_timer_SOURCES = main.c
idle_timer_CFLAGS = $(GTK_CFLAGS) $(X11_CFLAGS)
idle_timer_LDADD = libidletimer.a $(GTK_LIBS) $(X11_LIBS) 

noinst_LIBRARIES = libidletimer.a
libidletimer_a_SOURCES = idle_timer.c command_map.c config_parser.c
libidletimer_a_CFLAGS = $(GTK_CFLAGS) $(X11_CFLAGS)

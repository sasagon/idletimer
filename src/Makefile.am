SUBDIRS = . test

bin_PROGRAMS = idletimer
idletimer_SOURCES = main.c
idletimer_CFLAGS = $(GTK_CFLAGS) $(X11_CFLAGS)
idletimer_LDADD = libidletimer.a $(GTK_LIBS) $(X11_LIBS)

noinst_LIBRARIES = libidletimer.a
libidletimer_a_SOURCES = idle_timer.c command_map.c config_parser.c
libidletimer_a_CFLAGS = $(GTK_CFLAGS) $(X11_CFLAGS)

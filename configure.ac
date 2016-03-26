#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([idle_timer], [0.1], [sasagon@gmail.com])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_AUX_DIR([config.gnu])
AM_INIT_AUTOMAKE([subdir-objects])

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for modules.
PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES(X11, x11,, AC_MSG_ERROR([X11 required.]))
PKG_CHECK_MODULES(GTK3, gtk+-3.0, [gtk3_enabled=yes], [gtk3_enabled=no])
PKG_CHECK_MODULES(GTK2, gtk+-2.0, [gtk2_enabled=yes], [gtk2_enabled=no])

# Checks for libraries.
AC_CHECK_LIB(Xss, XScreenSaverQueryInfo,,
             AC_MSG_ERROR([libxss required. You should install libxss package.]))

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADER_STDBOOL
AC_CHECK_HEADERS([stdlib.h string.h limits.h])
AC_CHECK_HEADERS([CUnit/CUnit.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([strdup, strpbrk, strtoul, strerror, memset])

case "$gtk3_enabled" in
yes)
  GTK_CFLAGS="$GTK3_CFLAGS"
  GTK_LIBS="$GTK3_LIBS"
  AC_DEFINE(HAVE_GTK3, [1], [Define to 1 if you have libgtk3])
  AC_DEFINE(HAVE_GTK, [1], [Define to 1 if you have libgtk2 or 3])
  ;;
no)
  case "$gtk2_enabled" in
  yes)
    GTK_CFLAGS="$GTK2_CFLAGS"
    GTK_LIBS="$GTK2_LIBS"
    AC_DEFINE(HAVE_GTK2, [1], [Define to 1 if you have libgtk2])
    AC_DEFINE(HAVE_GTK, [1], [Define to 1 if you have libgtk2 or 3])
    ;;
  no)
    AC_MSG_ERROR(["libgtk2 or 3 required. You should install a package named like libgtk-3-dev."])
    ;;
  esac
esac
AC_SUBST([GTK_CFLAGS])
AC_SUBST([GTK_LIBS])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/test/Makefile])
AC_OUTPUT

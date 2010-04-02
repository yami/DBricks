AC_PREREQ([2.63])
AC_INIT([DBricks], [0.1.0.0], [yamisoe@gmail.com])
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([src/geom/Point.hxx])
AC_CONFIG_HEADERS([config.h])



# Checks for programs.
AC_PROG_CXX
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.

# pkg-config checks
# FIXME: now these versions are the one in my own system

PKG_CHECK_MODULES(PANGOMM, pangomm-1.4 >= 0.22)

PKG_CHECK_MODULES(GTKMM, gtkmm-2.4 >= 2.16.0)

PKG_CHECK_MODULES(RSVG, librsvg-2.0 >= 2.26.0)

PKG_CHECK_MODULES(CAIROMM, cairomm-1.0 >= 1.6.4)

AC_CONFIG_FILES([
Makefile
src/Makefile
src/diagram/Makefile
src/logging/Makefile
src/ssexp/Makefile
src/sml/Makefile
])

AC_OUTPUT

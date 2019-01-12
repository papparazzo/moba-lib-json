AC_PREREQ([2.69])
AC_INIT([moba-lib-json], [0.1.0-0000], [pappi-@gmx.de])

AC_CONFIG_AUX_DIR([.])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

AC_PROG_RANLIB

AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CXX

AX_CXX_COMPILE_STDCXX_11()

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lMoBaCommon':
AC_CHECK_LIB([main])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_UINT8_T

# Checks for library functions.

AC_CONFIG_FILES([
  Makefile
  src/Makefile
])

AC_OUTPUT

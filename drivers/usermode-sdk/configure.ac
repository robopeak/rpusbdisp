#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT(edison_native_playground, 0.1, http://www.csksoft.net)

AC_SYS_LARGEFILE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB
AC_PATH_PROG(AR,ar,ar)

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdint.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T


# check target platform via current machine type
machine=`uname -m`
if test x$machine = xarmv7l; then
  ARCH_PLATFORM=armv7l
else
  ARCH_PLATFORM=x86
fi

# check target platform type based on the host arch

case "$host" in
  arm-*-*)
   ARCH_PLATFORM=armv7l
   ;;
  *86* )
   ARCH_PLATFORM=x86
  ;;
esac

AC_SUBST(ARCH_PLATFORM)

COMPILER_PREFIX=$host
AC_SUBST(COMPILER_PREFIX)



# enable debug
AC_MSG_CHECKING(for debug)
AC_ARG_ENABLE([debug],
	[AC_HELP_STRING([--enable-debug],
	[Turn off optimization flag and Turn on debug flag. (DEFINE _DEBUG)(default=disable)])],
	[OPT_DBG="$enableval"],
	[OPT_DBG=no] )

AC_SUBST(OPT_DBG)

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([clock_gettime ftruncate gettimeofday memset usleep])

AC_CONFIG_FILES([Makefile.conf
                 Makefile])
AC_OUTPUT



#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([eqbeats], [3.0], [contact@eqbeats.org])
AM_INIT_AUTOMAKE([foreign])
LT_INIT
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for libraries.
missing=''
AC_CHECK_LIB([pq], [PQconnectdb], [], [missing="$missing pq"])
AC_CHECK_LIB([fcgi], [FCGX_Init], [], [missing="$missing fcgi"])
AS_IF([test -n "$missing"], [
    AC_MSG_ERROR([cannot find the following libraries:$missing])
])
PKG_CHECK_MODULES([MAGICK], [MagickWand])
PKG_CHECK_MODULES([TAGLIB], [taglib])
PKG_CHECK_MODULES([CGICC], [cgicc])
CGICC_LIBS='-lcgicc -lfcgi++' # cgicc.pc doesn't do its job
AC_SUBST([CGICC_LIBS])
PKG_CHECK_MODULES([CTEMPLATE], [libctemplate])
PKG_CHECK_MODULES([PCRECPP], [libpcrecpp])
PKG_CHECK_MODULES([NETTLE], [nettle])
AC_CHECK_LIB([hiredis], [redisConnectUnix])
AM_CONDITIONAL([HIREDIS], [test "x$ac_cv_lib_hiredis_redisConnection" = xyes])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stddef.h stdlib.h string.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_CHECK_FUNCS([dup2 gettimeofday memset mkdir select socket strchr])

# Paths
AC_ARG_WITH([redis-socket],
    AC_HELP_STRING([--with-redis-socket=PATH], [Unix socket for Redis @<:@/run/eqbeats/redis.sock@:>@]),
    [redis_socket="$withval"],
    [redis_socket='/run/eqbeats/redis.sock'])
AC_SUBST([redis_socket])

AC_ARG_WITH([eqbeats-dir],
    AC_HELP_STRING([--with-eqbeats-dir=DIR], [Writable site-specific files @<:@/var/lib/eqbeats@:>@]),
    [eqbeats_dir="$withval"],
    [eqbeats_dir='/var/lib/eqbeats'])
AC_SUBST([eqbeats_dir])

AC_ARG_WITH([templates],
    AC_HELP_STRING([--with-templates=DIR], [path to the CTemplate files @<:@PKGDATADIR/templates@:>@]),
    [templates_dir="$withval"],
    [templates_dir='${pkgdatadir}/templates'])
AC_SUBST([templates_dir])

AC_CONFIG_FILES([
    Makefile
    include/Makefile
    lib/Makefile
    src/Makefile
    src/account/Makefile
    src/account/pages/Makefile
    src/core/Makefile
    src/log/Makefile
    src/misc/Makefile
    src/pages/Makefile
    src/playlist/Makefile
    src/playlist/pages/Makefile
    src/render/Makefile
    src/social/Makefile
    src/social/pages/Makefile
    src/stat/Makefile
    src/text/Makefile
    src/track/Makefile
    src/track/pages/Makefile
    src/userfeature/Makefile
    src/userfeature/pages/Makefile
    src/youtube/Makefile
    src/youtube/pages/Makefile
    src/tools/Makefile
    static/Makefile
    static/icons/Makefile
    templates/Makefile
    templates/html/Makefile
    templates/json/Makefile
    templates/oembed/Makefile
    sql/Makefile
])
AC_OUTPUT

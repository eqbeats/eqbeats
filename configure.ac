#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([eqbeats], [3.0], [contact@eqbeats.org])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

AC_GNU_SOURCE
LT_INIT

AC_PROG_CXX
AC_PROG_CC_C99
AC_PROG_INSTALL
AC_PROG_LN_S

missing=''
AC_CHECK_LIB([pq], [PQconnectdb], [PQ_LIBS="-lpq"], [missing="$missing pq"])
AC_SUBST([PQ_LIBS])
AC_CHECK_LIB([fcgi], [FCGX_Init], [FCGI_LIBS="-lfcgi"], [missing="$missing fcgi"])
AC_SUBST([FCGI_LIBS])
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
AC_CHECK_LIB([hiredis], [redisConnectUnix], [HIREDIS_LIBS="-lhiredis"])
AC_SUBST([HIRDIS_LIBS])
AM_CONDITIONAL([HIREDIS], [test "x$ac_cv_lib_hiredis_redisConnection" = xyes])

AC_CHECK_FUNC([asprintf], [], [AC_MSG_ERROR([missing asprintf])])

AC_ARG_WITH([redis-socket],
    AC_HELP_STRING([--with-redis-socket=PATH], [Unix socket for Redis @<:@/run/eqbeats/redis.sock@:>@]),
    [redis_socket="$withval"],
    [redis_socket='/run/eqbeats/redis.sock'])
AC_SUBST([redis_socket])

AC_ARG_WITH([eqbeats-dir],
    AC_HELP_STRING([--with-eqbeats-dir=DIR], [Default writable website-specific directory @<:@/var/lib/eqbeats@:>@]),
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
    conf/Makefile
])
AC_CONFIG_FILES([lib/eqbeats.pc])
AC_OUTPUT

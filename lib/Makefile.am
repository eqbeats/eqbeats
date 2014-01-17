lib_LTLIBRARIES = libeqbeats.la
libeqbeats_la_SOURCES = eqbeats.c
libeqbeats_la_LIBADD = $(PQ_LIBS)
AM_CFLAGS = -I$(top_srcdir)/include -DDEFAULT_EQBEATS_DIR=\"$(eqbeats_dir)\"

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = eqbeats.pc

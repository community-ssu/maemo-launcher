## Process this file with automake to produce Makefile.in
#
# $Id$
#

bin_PROGRAMS = maemo-client

sources = client.c
version_script = client.Version

EXTRA_DIST = $(version_script)

maemo_client_SOURCES = $(sources)
maemo_client_CPPFLAGS = $(CLIENT_CFLAGS) $(PIC)
maemo_client_LDFLAGS = \
	-rdynamic -Wl,--version-script=$(version_script) \
	-shared $(PIC)
maemo_client_LDADD = $(CLIENT_LIBS)


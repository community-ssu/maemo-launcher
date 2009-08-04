## Process this file with automake to produce Makefile.in
#

helperdir = $(libdir)/maemo-launcher
helper_PROGRAMS = defender

DEFENDER = "\"$(helperdir)/defender\""
LAUNCHER = "\"$(bindir)/maemo-launcher\""

defender_SOURCES = defender.c report.c report.h
defender_CPPFLAGS = \
	-DPROG_NAME="\"defender\"" \
	-DLAUNCHER=$(LAUNCHER) \
	-DDEFENDER=$(DEFENDER)

boosterdir =  $(libdir)/maemo-launcher
booster_PROGRAMS = booster-null.so booster-gtk.so

BOOSTER_DIR = "\"$(boosterdir)\""

booster_null_so_SOURCES = booster_null.c
booster_null_so_CFLAGS = -fPIC
booster_null_so_LDFLAGS = -shared -fPIC

booster_gtk_so_SOURCES = booster_gtk.c booster_gtk.h
booster_gtk_so_CFLAGS = -fPIC
booster_gtk_so_LDFLAGS = -shared -fPIC
booster_gtk_so_CPPFLAGS = $(BOOSTER_GTK_CFLAGS) $(BOOSTER_HILDON_CFLAGS)
booster_gtk_so_LDADD = $(BOOSTER_GTK_LIBS) $(BOOSTER_HILDON_LIBS)

if USE_QT_BOOSTER
booster_PROGRAMS += booster-qt.so
endif

booster_qt_so_SOURCES = booster_qt.c
booster_qt_so_CFLAGS = -fPIC
booster_qt_so_LDFLAGS = -shared -fPIC
booster_qt_so_CPPFLAGS = $(BOOSTER_QT_CFLAGS)
booster_qt_so_LDADD = $(BOOSTER_QT_LIBS)

if USE_CPP_BOOSTER
booster_PROGRAMS += booster-cpp.so
endif

booster_cpp_so_SOURCES = booster_cpp.cpp
booster_cpp_so_CPPFLAGS = -fPIC
booster_cpp_so_LDFLAGS = -shared -fPIC

bin_PROGRAMS = maemo-launcher maemo-invoker maemo-summoner

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = \
	maemo-launcher-app.pc \
	maemo-launcher-booster.pc

maemo_launcherdir = $(pkgincludedir)
maemo_launcher_HEADERS = booster_api.h
maemo_launcher_SOURCES = \
	launcher.c report.c report.h booster.c booster.h \
	invokelib.c invokelib.h \
	comm_msg.c comm_msg.h comm_dbus.c comm_dbus.h \
	prog.c prog.h
maemo_launcher_CPPFLAGS = \
	$(DBUS_CFLAGS) \
	-DPROG_NAME="\"maemo-launcher\"" \
	-DBOOSTER_DIR=$(BOOSTER_DIR) \
	-DBOOSTER_DEFAULT="\"null\"" \
	-DDEFENDER=$(DEFENDER) \
	-DHAVE_PRCTL_SET_NAME
maemo_launcher_LDADD = -ldl $(DBUS_LIBS)
# FIXME: Use rdynamic for now until the boosters do not use the report
#        functions.
maemo_launcher_LDFLAGS = -rdynamic

maemo_invoker_SOURCES = \
	invoker.c invokelib.c invokelib.h report.c report.h \
	search.c search.h
maemo_invoker_CPPFLAGS = -DPROG_NAME="\"maemo-invoker\""

maemo_summoner_SOURCES = \
	summoner.c report.c report.h prog.c prog.h \
	search.c search.h
maemo_summoner_CPPFLAGS = -DPROG_NAME="\"maemo-summoner\""
maemo_summoner_LDADD = -ldl

check_PROGRAMS = test-msg test-msg-perf

test_msg_SOURCES = test_lib.h test_lib.c test_msg.c comm_msg.c report.c
test_msg_CPPFLAGS = -DPROG_NAME="\"test-msg\""

test_msg_perf_SOURCES = test_msg_perf.c comm_msg.c report.c
test_msg_perf_CPPFLAGS = -DPROG_NAME="\"test-msg-perf\""

TESTS = $(check_PROGRAMS)

man_MANS = \
	maemo-launcher.1 \
	maemo-invoker.1 \
	maemo-summoner.1

EXTRA_DIST = \
	booster.pc.in \
	launcher.pc.in \
	launcher.1.in \
	invoker.1.in \
	summoner.1.in


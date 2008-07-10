#! /bin/sh
#
# $Id$
#

NAME=maemo-launcher
DESC="Maemo Launcher"
DAEMON=/usr/bin/$NAME
# XXX: Because we take quality seriously let's disable the application crash
# notification (you might want to add '--send-app-died' back...)
DAEMON_BASE_OPTS="--daemon --booster gtk"
PIDFILE=/tmp/$NAME.pid

# Those files set needed environment variables for the Maemo applications
# FIXME: but this should not be needed either, and should be inherited from
#        the session instead.

# OSSO AF Init definitions
DEFSDIR=/etc/osso-af-init/

if [ -e $DEFSDIR/af-defines.sh ]
then
  . $DEFSDIR/af-defines.sh
else
  echo "$DEFSDIR/af-defines.sh not found!"
  exit 1
fi

# When inside scratchbox we are not really root nor do we have 'user' user
if [ -f /targets/links/scratchbox.config ]; then
  DAEMON_OPTS="$DAEMON_BASE_OPTS"
else
  # FIXME: this is wrong wrong wrong, and should not be hardcoded, this script
  #        belongs in the session level instead.
  PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin
  export USER=user
  export LOGNAME=$USER
  PWENTRY=`getent passwd $USER`
  export HOME=`echo $PWENTRY | cut -d: -f6`
  export SHELL=`echo $PWENTRY | cut -d: -f7`
  if [ `id -u` = 0 ]; then
    CHUID="--chuid $USER"
    # Make sure the launcher is not an OOM candidate
    NICE="--nicelevel -1"
  fi
  DAEMON_OPTS="$DAEMON_BASE_OPTS --quiet"
fi

test -x $DAEMON || exit 0

set -e

case "$1" in
  start)
    echo -n "Starting $DESC: $NAME"
    start-stop-daemon --start --quiet --pidfile $PIDFILE $CHUID $NICE \
      --exec $DAEMON -- $DAEMON_OPTS || echo -n " start failed"
    echo "."
    ;;
  stop)
    echo -n "Stopping $DESC: $NAME"
    start-stop-daemon --stop --quiet --pidfile $PIDFILE --exec $DAEMON \
      || echo -n " not running"
    echo "."
    ;;
  restart)
    $0 stop
    sleep 1
    $0 start
    ;;
  reload|force-reload)
    echo -n "Reloading $DESC: $NAME"
    start-stop-daemon --stop --signal USR1 --quiet --pidfile $PIDFILE \
      --exec $DAEMON || echo -n " not running"
    echo "."
    ;;
  *)
    N=/etc/init.d/$NAME
    echo "Usage: $N {start|stop|restart|force-reload}" >&2
    exit 1
    ;;
esac

exit 0

# vim: syn=sh


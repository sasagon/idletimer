#!/bin/sh --
TESTDIR=`dirname $0`
PIDFILE=/tmp/xvfb-99.pid

export DISPLAY=:99.0

/sbin/start-stop-daemon --start --quiet \
  --pidfile "$PIDFILE" --make-pidfile --background \
  --exec /usr/bin/Xvfb -- :99 -ac -screen 0 1280x1024x16
sleep 3 # give xvfb some time to start

../idletimer -v -c "${TESTDIR}/run-test.config" > idletimer.log 2> idletimer-error.log &
IDLETIMER_PID=$!
sleep 125

xdotool click 1
sleep 3

kill "$IDLETIMER_PID" 2> /dev/null
sleep 3

/sbin/start-stop-daemon --stop --quiet --pidfile "$PIDFILE"

if ! cmp idletimer.log - <<EOT > /dev/null ; then
*** 2 entries found in ${TESTDIR}/run-test.config
  idle:1:echo "idling for 1 minute." > idletimer-command.log
  wakeup:2:echo "wakeup after idling for more then 2 minutes." >> idletimer-command.log
*** Idletimer watching your operation...
*** Idling for 1 minute(s).
Execute: echo "idling for 1 minute." > idletimer-command.log
*** Wake up after 2 minute(s) idling.
Execute: echo "wakeup after idling for more then 2 minutes." >> idletimer-command.log
EOT
  echo "Illegal idletimer.log found as below:" 1>&2
  cat idletimer.log 1>&2
  exit 1
elif ! cmp idletimer-command.log <<EOT > /dev/null ; then
idling for 1 minute.
wakeup after idling for more then 2 minutes.
EOT
  echo "Illegal idletimer-command.log found as below:" 1>&2
  cat idletimer-command.log 1>&2
  exit 1
fi

exit 0

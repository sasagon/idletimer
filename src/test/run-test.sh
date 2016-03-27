#!/bin/sh --
TESTDIR=`dirname $0`

../idletimer -v -c "${TESTDIR}/run-test.config" > idletimer.log 2> idletimer-error.log &
IDLETIMER_PID=$!
sleep 5
kill $IDLETIMER_PID 2> /dev/null

cat <<EOT > idletimer.log.expected
*** 1 entry found in ${TESTDIR}/run-test.config
  idle:1:echo "foo" > /dev/null
*** Idletimer watching your operation...
EOT
if !cmp idletimer.log.expected idletimer.log > /dev/null ; then
  echo "Illegal execution log file found as below:" 1>&2
  cat idletimer.log 1>&2
  exit 1
#elif [ -s idletimer-error.log ]; then
#  echo "Idletimer error log found as below:" 1>&2
#  cat idletimer-error.log 1>&2
#  exit 1
fi

exit 0

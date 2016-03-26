#!/bin/sh --
echo PWD: `pwd`
echo '$0:' $0
TESTDIR=`dirname $0`

../idletimer -v -c "${TESTDIR}/run-test.config" > idletimer.log 2>&1 &
IDLETIMER_PID=$!
sleep 5
kill $IDLETIMER_PID 2> /dev/null

cat <<EOT > idletimer.log.expected
*** 1 entry found in ${TESTDIR}/run-test.config
  idle:1:echo "foo" > /dev/null
*** Idletimer watching your operation...
EOT
if cmp idletimer.log.expected idletimer.log > /dev/null ; then
  exit 0
else
  echo "Idletimer execution failed because of illegal log file as below:" 1>&2
  cat idletimer.log 1>&2
  exit 1
fi

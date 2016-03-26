IDLE TIMER
==========

[![Build Status](https://travis-ci.org/sasagon/idletimer?branch=master)]
(https://travis-ci.org/sasagon/idletimer)

Idletimer behaves like a screen saver except that it does not blank screens
and wake them up but executes preconfigured commands in `~/.idletimer`.

## Usage & Examples

    $ idletimer

or

    $ idletimer > /dev/null &

for background.

`~/.idletimer` contains one or more lines which consist of 3 itmes below:

  - execution type (`idle` or `wakeup`)
  - idling minute(s)
  - command line text

which are separated with `:`.

Idletimer passes whole text after the second `:` to `system()` function.
So you can use shell functions here.


Ex.1)

    idle:3:~/bin/idling3min.sh

If your system does not detect any keyboard or mouse input for 3 minutes,
then idletimer executes `~/bin/idling3min.sh`


Ex.2)

    idle:10:date "+%F %T idle" -d "@$((`date +%s` - 60 * 10))" >> ~/idle.log

If your system has been idle for just 10 minutes,
then write the time to start idling (10 minutes before) to a log file.


Ex.3)

    wakeup:300:python ~/bin/timecard.py >> ~/timecard.log

If you wake up your system after idling in 300 minutes or above, 
then idletimer executes `~/bin/timecard.py` with python interpreter.


## Command Line Options

    -h                 print help message.
    -v                 print configurations and actions verbosely.
    -c config_file     specify a config file instead of ~/.idletimer


## Requirements

 - X11
 - GTK+2 or 3
 - libxss

## Install 

see Install.md

## License

MIT License

## Author

SASAKI Hiroshi <sasagon@gmail.com>

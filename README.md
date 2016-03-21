IDLE TIMER
==========

Idletimer behaves like a screen saver except that it does not blank screens
and wake them up but executes preconfigured commands in `~/.idletimer`.

## Usage & Examples

    $ idletimer

or

    $ idletimer &

for background.

`~/.idletimer` contains one or more lines which consist of 3 itmes below:

  - execution type (`idle` or `wakeup`)
  - idling minute(s)
  - command line text

which are separated with `:`.


Ex.1)

    idle:3:~/bin/idling3min.sh

If your system does not detect any keyboard or mouse input for 3 minutes,
then idletimer executes `~/bin/idling3min.sh`


Ex.2)

    wakeup:60:python ~/bin/wakeup.py

If you wake up your system after idling in 60 minutes or above, 
then idletimer executes `~/bin/wakeup.py` with python.


Idletimer passes a whole text after the second `:` to `system()` function.


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

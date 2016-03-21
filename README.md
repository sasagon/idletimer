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

  - execution type ('idle' or 'wakeup')
  - idling minute(s)
  - command line

which are separated with ':'.


ex.1)

    idle:3:idling3min.sh

If your system does not detect any keyboard or mouse input for 3 minutes,
then idletimer executes "idling3min.sh".


ex.2)

    wakeup:60:wakeup.sh

If you wake up your system after idling in 60 minutes or above, 
then idletimer executes "wakeup.sh".


Idletimer command passes a whole string after second ':' to system() function.


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

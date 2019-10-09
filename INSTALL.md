INSTALLATION
============

## Prerequisites

 - X11
 - libgtk2.0 or libgtk-3
 - libxss

For example, if you use APT then run below to install the development packages.

    $ sudo apt-get install libx11-dev libgtk-3-dev libxss-dev

If your system do not have libgtk-3 package, you should use libgtk2.0-dev
instead.

The following tools and library are required in some cases.

 - autotools (if you get the source tree from github instead of tarball)
 - CUnit (if you want to run unit tests)
 - xdotool, xvfb (if you want to run binary execution test)

For APT example below:

    $ sudo apt-get install autotools-dev libcunit1-dev xdotool xvfb


## Compile

If you get the source tree from tarball,
just run `configure` script and `make` command like below:

    $ ./configure
    $ make

If you get the source tree from git directly instead of tarball,
you must execute `automake` and `autoreconf` command in autotools
before running `configure`.

    $ automake --add-missing
    $ autoreconf -v

You can also execute

    $ make check

to run tests if you have CUnit and xdotool.

## Install

    $ sudo make install

or

    $ make install bindir=$HOME/bin

And you must prepare a configuration file (normally `~/.idletimer`).

    $ cp dot.idletimer.sample ~/.idletimer
    $ vi ~/.idletimer     # edit with your favorite editor

## Run

    $ idletimer

or

    $ idletimer &

You can use some options, which are described in README.md.

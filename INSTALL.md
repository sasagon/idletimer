INSTALLATION
============

## Prerequisites

 - X11
 - libgtk2 or libgtk3
 - libxss

For example, if you use APT then run below to install the development packages.

    $ sudo apt-get install libx11-dev libgtk3-dev libxss-dev

The following tools and library are required in some cases:

 - autotools (if you get the source tree from git directly instead of tarball)
 - CUnit (if you want to run unit tests)


## Compile

Just run `configure` script and `make` command like below:

    $ ./configure
    $ make

If you get the source tree from git directly instead of tarball,
you must execute `autoreconf` command in autotools before running `configure`.

    $ autoreconf -v

You can also execute

    $ make check

to run tests if you have CUnit.

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

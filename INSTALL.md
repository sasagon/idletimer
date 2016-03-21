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


## Compile & Install

Just run `configure` script and `make` command like below:

    $ ./configure
    $ make
    $ sudo make install   # or $ make install bindir=$HOME/bin
    $ cp dot.idletimer.sample ~/.idletimer

You can also execute `make check` to run unit tests if you have CUnit.

If you get the source tree from git directly instead of tarball,
you must execute `autoreconf` command in autotools before running `configure`.

    $ autoreconf -v

INSTALLATION
============

## Prerequisites

 - X11
 - libgtk2 or libgtk3
 - libxss
 - CUnit (Optinal)

For example, if you use APT then run below to install the development packages.

    $ sudo apt-get install libx11-dev libgtk3-dev libxss-dev


## Compile & Install

Just run configure script and make command like below:

    $ ./configure
    $ make
    $ sudo make install

or

    $ make install bindir=$HOME/bin

If you want to execute `make check`, you need CUnit.

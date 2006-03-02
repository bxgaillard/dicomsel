#!/bin/sh
cd "`dirname "$0"`" &&
aclocal &&
autoconf &&
autoheader &&
automake -a -c

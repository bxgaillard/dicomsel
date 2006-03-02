#!/bin/sh
cd "`dirname "$0"`" &&
{ [ ! -f Makefile ] || make distclean; } &&
exec rm -rf autom4te.cache config.h.in `find -name Makefile.in` \
    aclocal.m4 configure depcomp install-sh missing

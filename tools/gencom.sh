#!/bin/bash

# ----------------------------------------------------------------------------
#
# GenCom: Comment Generator for Source File Head
# Copyright (C) 2005, 2006 Benjamin Gaillard
#
# ----------------------------------------------------------------------------
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc., 59
# Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# ----------------------------------------------------------------------------


# Recognized filenames
FILENAMES='Makefile makefile Makefile.* GNUmakefile *.mk *.mak *.am \
configure.ac *.c *.cpp *.h *.s *.rc'

# Line length
LENGTH=78


NORMAL=$'\e[00m'
RED=$'\e[01;31m'
GREEN=$'\e[01;32m'
CYAN=$'\e[01;36m'


dirs=''
files=''

while [ $# -gt 0 ]; do
    case "$1" in
	-i | --info)
	    infofile="$2"
	    shift
	    ;;
	-l | --license)
	    licensefile="$2"
	    shift
	    ;;
	*)
	    if [ -d "$1" ]; then
		dirs="$dirs"$'\n'"$1"
	    else
		files="$files"$'\n'"$1"
	    fi
	    ;;
    esac
    shift
done

if [ -z "$infofile" -o -z "$licensefile" -o -z "$dirs" ]; then
    echo "Syntax: $0" >&2
    echo '        {-i|--info} <info file> {-l|--license} <license file>' >&2
    echo '        <directory> [directory...]' >&2
    exit 1
fi

if [ ! -f "$infofile" ]; then
    echo "Error: \"$infofile\" doesn't exist." >&2
    exit 2
fi
if [ ! -f "$licensefile" ]; then
    echo "Error: \"$licensefile\" doesn't exist." >&2
    exit 3
fi

INFO="`cat "$infofile"`"
LICENSE="`cat "$licensefile"`"

for filename in $FILENAMES; do
    files="$files"$'\n'"`find $dirs -name "$filename"`"
done
files="`echo "$files" | sort | uniq`"


BIFS="$IFS"
IFS=$'\n'
TMP="/tmp/`basename "$0"`.tmp"

for file in $files; do
    if [ ! -r "$file" ]; then
	echo "${file}: ${RED}cannot open!${NORMAL}"
	continue
    fi

    case "`basename "$file"`" in
	[Mm]akefile | Makefile.* | GNUmakefile | *.mk | *.mak | *.am)
	    BEGIN=''
	    LINE='#'
	    END=''
	    FILEEND='# End of File'
	    ;;

	configure.ac)
	    BEGIN=''
	    LINE='dnl'
	    END=''
	    FILEEND='dnl End of File'
	    ;;

	*.c | *.cpp | *.h | *.s | *.rc)
	    BEGIN='/*'
	    LINE=' *'
	    END=' */'
	    FILEEND='/* End of File */'
	    ;;
    esac

    # Make separation line
    i=$(( $LENGTH - ${#LINE}))
    SEP=''
    while [ $(( --i )) -gt 0 ]; do
	SEP=$SEP-
    done
    unset i

    # Echo filename and create temporary file
    echo -n "$file:"
    exec 3>&1 >"$TMP"

    step=0
    prev=''
    ended=n
    desc=''

    while read -r line; do
	if [ $step = 9 ]; then
	    if [ "$line" = "$FILEEND" ]; then
		ended=y
	    fi
	    echo "$line"
	else
	    case $step in
		0)
		    if [ -n "$line" ]; then
			if [ -z "$BEGIN" ]; then
			    step=2
			elif [ "$line" = "$BEGIN" ]; then
			    step=1
			else
			    step=8
			fi
		    fi
		    ;;

		1)
		    step=2
		    ;;
	    esac

	    if [ $step -ge 2 -a $step -lt 8 ]; then
		case "$line" in
		    "$LINE"* | "$END")
			case $step in
			    2)
				case "$line" in
				    "$LINE ----------"*)
					step=3
					;;
				    *)
					step=8
					;;
				esac
				;;

			    3)
				case "$line" in
				    "$LINE ----------"*)
					step=4
					;;
				    *)
					;;
				esac
				;;

			    4)
				case "$line" in
				    "$LINE ----------"*)
					step=5
					;;
				    *)
					desc="$desc$line"$'\n'
					;;
				esac
				;;

			    5)
				case "$line" in
				    "$LINE ----------"*)
					step=6
					;;
				    "$END")
					desc=''
					step=7
					;;
				esac
				;;

			    6)
				if [ "$line" = "$END" ]; then
				    step=7
				else
				    step=8
				fi
				;;

			    7)
				step=8
				;;
			esac
			;;

		    *)
			if [ $step = 5 ]; then
			    spec=''
			fi
			step=8
			;;
		esac
	    fi

	    if [ $step = 8 -a -n "$line" ]; then
		# File head
		if [ -n "$BEGIN" ]; then
		    echo "$BEGIN"
		fi
		echo "$LINE $SEP"
		echo "$LINE"

		# Informations
		echo "$INFO" | while read -r iline; do
		    if [ -n "$iline" ]; then
			echo "$LINE $iline"
		    else
			echo "$LINE"
		    fi
		done

		# Separation
		echo "$LINE"
		echo "$LINE $SEP"

		# File description
		if [ -n "$desc" ]; then
		    echo -n "$desc"

		    # Separation
		    echo "$LINE $SEP"
		fi

		# License
		echo "$LINE"
		echo "$LICENSE" | while read -r iline; do
		    if [ -n "$iline" ]; then
			echo "$LINE $iline"
		    else
			echo "$LINE"
		    fi
		done

		# File head finalization
		echo "$LINE"
		echo "$LINE $SEP"
		if [ -n "$END" ]; then
		    echo "$END"
		fi

		# File beginning
		echo
		echo
		echo "$line"

		step=9
	    fi
	fi

	prev="$line"
    done < "$file"

    if [ -n "$FILEEND" -a $ended = n ]; then
        if [ -n "$prev" ]; then
	    echo
	fi
	echo "$FILEEND"
    fi

    # Close file
    exec 1>&3-

    # Move file
    if diff -a -q "$file" "$TMP" > /dev/null; then
	rm -f "$TMP"
	echo -n " ${CYAN}no change."
    elif mv -f "$TMP" "$file"; then
	echo -n " ${GREEN}updated."
    else
	rm -f "$TMP"
	echo -n " ${RED}could not update!"
    fi
    echo "$NORMAL"
done

IFS="$BIFS"
exit 0

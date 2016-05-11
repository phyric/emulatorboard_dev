#!/bin/sh

#
# Run this to dump and follow the most recent log file
#

lastlog="..."

# enter logs/ directory (if it exists)
if [ -d logs ] ; then
    cd logs;
fi

# get just most recent log file
for f in `ls -t RadTest*log`; do 
    lastlog=$f
    break
done

if [ ! -e $lastlog ] ; then
    echo "Did not find log file!"
    exit 1
fi

echo "--- $lastlog ---"
lines=`wc -l $lastlog|awk {'print $1'}`

head -n$((lines-10)) $lastlog
tail -f $lastlog

exit 0

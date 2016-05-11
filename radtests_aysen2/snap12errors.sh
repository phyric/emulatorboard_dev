#!/bin/sh

#
# This script will report the number of times each snap12 channel had
# an error that persisted and cause the run to be paused
#

if [ $# -ne 1 ]; then
    echo "You must specify the logfile to search."
fi

logfile=$1

echo ""
echo "Number of times logfile says the channel had a non-zero error count just before the run was paused:"
for i in 0 1 2 3 4 5 6 7; do 
    echo -n "$i: " 
    grep -A22 "ntry=0" $logfile | grep -B22 "ntry=2" | grep -A9 "ntry=0" | grep "snap12_counts\[$i\]" |grep -v "=  0" -c; 
done
echo ""
echo "Note: this is not exactly the same as the number of times a channel *caused* a run pause, but should be close and at least it will give you an idea of a particular channel is never getting any errors."
echo ""

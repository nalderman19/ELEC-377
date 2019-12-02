#!/bin/bash

# Print banner to console
echo -e "PID\tUSER\tRSS\tCOMMAND"

# Only loop through files in proc directory that consist of digits only
for FILE in $(ls -vd /proc/[1-9]*); do
    # if process has a 'status' directory in it
    if test -f "$FILE/status"; then
        # Get PID for proc
        PID=${FILE/\/proc\//}
        # Get user using the stat command
        USER=$(stat -c %U $FILE)
        # Get RSS value and remove 'VmRSS' and 'kB'
        RSS=$(grep -w VmRSS: $FILE/status | sed -e 's/VmRSS:\t *//'| sed -e 's/ [kK]B//')
        # Check if RSS is empty
        if test -z "$RSS"; then
            RSS=0 
        fi
        # Get command from 'cmdline' in proc
        CMD=$(cat $FILE/cmdline | tr '\0' ' ')
        # Check if CMD is empty if it is then check the status files for the details
        if test -z "$CMD"; then
            CMD="[$(grep 'Name' $FILE/'status' | sed -e 's/.*\t *//')]"
        fi
        # Print the line with the process information
        echo -e "$PID\t$USER\t$RSS\t$CMD"
    fi
done 

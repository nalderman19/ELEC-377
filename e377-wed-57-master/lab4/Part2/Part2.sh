#!/bin/bash

# Initialize empty string
MAINFILES=""
MODFILES=""
OTHERFILES=""

# Loop through files ending with '.c' extention using '-name' flag
for FILE in $(find $1 -type f -name "*.c"); do
    FILE=`readlink -f $FILE`
    # Search for main method (can be void or int)
    if grep -wq "[i,n,t,v,o,i,d]* main" $FILE; then
        # Add directory path, # of printf, # of fprintf to MAINFILES using cat pipe grep
        MAINFILES="$MAINFILES$FILE:$(cat $FILE | grep -w printf | wc -l),$(cat $FILE | grep -w fprintf | wc -l)\n"
    elif grep -wq "init_module" $FILE; then
        # Add directory path, line # of all printk to MODFILES using grep pipe sed
        MODFILES="$MODFILES$FILE:$(grep -wn "printk" $FILE | sed -e 's/:.*//' | tr '\n' ',' | sed -e 's/,$//')\n"
    else
        # Add directory path to OTHERFILES
        OTHERFILES="$OTHERFILES$FILE\n"
    fi
done

# Fallback procedures, print 'No ___ Files' if no files are found for a given source file type
if test -z $MAINFILES; then
    MAINFILES="No main files\n"
fi
if test -z $MODFILES; then
    MODFILES="No module files\n"
fi
if test -z $OTHERFILES; then
    OTHERFILES="No other files\n"
fi

# Final print statement
echo -e "Main Files:\n"$MAINFILES"\nModule Files:\n"$MODFILES"\nOther Files:\n$OTHERFILES"

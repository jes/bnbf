#!/bin/sh
#Automated testing for bnbf
#James Stanley 2010

for dir in test-*
do
    cd $dir

    echo -n `cat testname` "- "

    ../../src/bnbf `cat options` program >real-stdout 2>real-stderr <stdin

    cmp -s real-stderr stderr
    if [ "$?" -ne "0" ]
    then
        echo "FAIL"
        echo "Expected stderr:"
        cat stderr
        echo "Got:"
        cat real-stderr
     else
        cmp -s real-stdout stdout
        if [ "$?" -ne "0" ]
        then
            echo "FAIL"
            echo "Expected stdout:"
            cat stdout
            echo "Got:"
            cat real-stdout
        else
            echo "PASS"
        fi
    fi

    rm real-stdout real-stderr

    cd ..
done

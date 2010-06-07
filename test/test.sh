#!/bin/sh
#Automated testing for bnbf
#James Stanley 2010

fails=0

fail() {
    $file=$1
    echo "FAIL"
    echo "Expected $file:"
    cat $file
    echo "Got:"
    cat real-$file
    fails=$((fails+1))
}

for dir in test-*
do
    cd $dir

    echo -n `cat testname` "- "

    ../../src/bnbf `cat options` program >real-stdout 2>real-stderr <stdin

    cmp -s real-stderr stderr
    if [ "$?" -ne "0" ]
    then
        fail "stderr"
     else
        cmp -s real-stdout stdout
        if [ "$?" -ne "0" ]
        then
            fail "stdout"
        else
            echo "PASS"
        fi
    fi

    rm real-stdout real-stderr

    cd ..
done

echo "----------------"
echo "Failures: $fails"
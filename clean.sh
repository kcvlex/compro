#!/bin/sh
for i in $(find ./ -name '*_submit.cpp' -or -name 'a.out' -or -name 'input.txt')
do
    rm $i
done

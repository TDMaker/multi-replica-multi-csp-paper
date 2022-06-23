#!/bin/bash
for num in {100..1000..100}
do
echo $num >> proof_by_challenged_number.log
for index in {1..10..1}
do
str="${num}.out"
./$str >> proof_by_challenged_number.log
done
done
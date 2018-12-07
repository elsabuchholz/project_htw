#!/bin/bash
COUNTER=0
while [  $COUNTER -lt 10 ]; do
make src/bench_toupper
./out/bench_toupper  > output.txt
sleep 10s
rm -rf /lfs/sometext.txt
< /dev/urandom tr -dc "X" | head -c1000 > /lfs/sometext.txt
echo The counter is $COUNTER
let COUNTER=COUNTER+1 
done

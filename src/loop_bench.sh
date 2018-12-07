#!/bin/bash
COUNTER=1
while  [ $COUNTER -le 3 ]; do
make src/bench_toupper
echo "$COUNTER" >> output.txt
./out/bench_toupper  >> output.txt
sleep 10s
rm -rf /lfs/sometext.txt
< /dev/urandom tr -dc "X" | head -c1000 > /lfs/sometext.txt
echo The counter is $COUNTER
 COUNTER=$((COUNTER+1))
done

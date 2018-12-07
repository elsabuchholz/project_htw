#!/bin/bash
# make ist definiert in Makefile kompiliert bench-touper code
make bench-toupper-formatted
#counter um das skript x mal auszuführen in while do loop
COUNTER=1
while  [ $COUNTER -le 3 ]; do
echo "$COUNTER" >> 1000.txt
./out/bench_toupper_formatted  >> 1000.txt
#sleep um sicher zu stellen, dass sometext nicht entfernt wird vor Änderungen
sleep 10
rm -rf /lfs/sometext.txt
< /dev/urandom tr -dc "X" | head -c1000 > /lfs/sometext.txt
echo The counter is $COUNTER
 COUNTER=$((COUNTER+1))
done

#!/bin/bash
#create random file with characters
#< /dev/urandom tr -dc "X" | head -c1000 > /lfs/sometext.txt
#size 10K - 20M erst ab 4K kann gemessen werden, da character mehr als 4K groß ist
#10K
#size=10000
#100K
#size=100000
#1M
size=1000000
#10M
#size=10000000
#20M
#size=20000000
dd if=/dev/urandom bs=$size count=1 | base64 > /home/l4mdc/sometext.txt &&
cat /home/l4mdc/sometext.txt | tr 'A-Za-z0-9+/=' 'a-z' > /home/l4mdc/sometext.txt &&
truncate -s $size /home/l4mdc/sometext.txt
REAL_SIZE="$(du -h /home/l4mdc/sometext.txt)"
echo $REAL_SIZE

# make ist definiert in Makefile kompiliert bench-touper code
make bench-toupper-formatted

#counter um das skript x mal auszuführen in while do loop
COUNTER=1
while  [ $COUNTER -le 100 ]; do
  echo "cp sometext to lfs"
cp /home/l4mdc/sometext.txt /lfs/sometext.txt &&
./out/bench_toupper_formatted  >> rslt/$size.csv
#sleep um sicher zu stellen, dass sometext nicht entfernt wird vor Änderungen
sleep 10s
#entfernen der random datei und kopieren der vorher erstellten Datei
rm -rf /lfs/sometext.txt

echo The counter is $COUNTER
 COUNTER=$((COUNTER+1))
done

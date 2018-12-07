#!/bin/bash
#create random file with characters
#< /dev/urandom tr -dc "X" | head -c1000 > /lfs/sometext.txt
dd if=/dev/urandom bs=786438 count=1 | base64 > /home/l4mdc/sometext.txt
cat /home/l4mdc/sometext.txt | tr 'A-Za-z0-9+/=' 'a-z$%'

# make ist definiert in Makefile kompiliert bench-touper code
make bench-toupper-formatted

#counter um das skript x mal auszuführen in while do loop
COUNTER=1
while  [ $COUNTER -le 3 ]; do
cp /home/l4mdc/sometext.txt /lfs/sometext.txt
echo "$COUNTER" >> 1000.csv
./out/bench_toupper_formatted  >> 1000.csv
#sleep um sicher zu stellen, dass sometext nicht entfernt wird vor Änderungen
sleep 10s
#entfernen der random datei und kopieren der vorher erstellten Datei
rm -rf /lfs/sometext.txt

echo The counter is $COUNTER
 COUNTER=$((COUNTER+1))
done

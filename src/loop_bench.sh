#!/bin/bash
make src/bench_toupper
./out/bench_toupper  > output.txt
sleep 10s
rm -rf /lfs/sometext.txt
< /dev/urandom tr -dc "X" | head -c1000 > /lfs/sometext.txt

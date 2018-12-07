#!/bin/bash
make src/bench_toupper
./out/bench_toupper  > output.txt
sleep 10s
rm -rf /lfs/sometext.txt

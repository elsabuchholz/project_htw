#!/bin/bash
make src/bench_toupper
./out/bench_toupper  > output.txt
sleep(10);
rm -rf /lfs/sometext.txt

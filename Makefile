GCC=gcc

pre-build:
	mkdir -p ./out

toupper: pre-build
	$(GCC) src/toupper_file.c -o out/toupper_file

timer: pre-build
	$(GCC) src/timer.c -o out/timer

bench-toupper: pre-build
	$(GCC) src/bench_toupper.c -o out/bench_toupper

bench-toupper-formatted: pre-build
	$(GCC) src/bench_toupper_formatted.c -o out/bench_toupper_formatted
	
clean: pre-build
	rm -rf ./out/*

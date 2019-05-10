GCC=gcc
LOCFLAGS=pthread
pre-build:
	mkdir -p ./out-test

multi: pre-build
	$(GCC) $(LOCFLAGS) multithread-test.c -o out-test/multithread-test


clean: pre-build
	rm -rf ./out-test/*

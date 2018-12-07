GCC=gcc

pre-build:
	mkdir -p ./out

toupper: pre-build
	$(GCC) src/toupper_file.c -o out/toupper_file

clean: pre-build
	rm -rf ./out/*

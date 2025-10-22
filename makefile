all:
	mkdir -p bin
	gcc src/*.c -Iinclude -o bin/cryp
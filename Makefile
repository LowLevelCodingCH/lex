build:
	@echo "CC src/maina"
	@gcc -o src/main src/main.c -Iinclude -lc -lm

run: build
	./src/main

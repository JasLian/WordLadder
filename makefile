build:
	gcc main.c -o program.exe

run:
	./program.exe < input.txt

debug:
	gcc -g main.c -o program.exe

valgrind:
	valgrind ./program.exe

gdb:
	gdb program.exe

demo:
	./demo.exe
	
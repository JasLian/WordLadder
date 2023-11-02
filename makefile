build:
	gcc main.c -o program.exe

run:
	./program.exe

debug:
	gcc -g main.c -o program.exe

valgrind:
	valgrind ./program.exe

gdb:
	gdb program.exe

blank:
	gcc blank.c -o blank.exe

rblank:
	./blank.exe


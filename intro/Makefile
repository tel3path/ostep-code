
all: cpu_random cpu_stdin cpu_stdin_smashy mem threads io

clean:
	rm -f cpu mem threads io

cpu_random: cpu_random.c
	gcc -o cpu_random cpu_random.c -Wall

cpu_stdin: cpu_stdin.c
	gcc -o cpu_stdin cpu_stdin.c -Wall

cpu_stdin_smashy: cpu_stdin_smashy.c
	gcc -o cpu_stdin_smashy cpu_stdin_smashy.c -Wall

mem: mem.c
	gcc -o mem mem.c -Wall

threads: threads.c
	gcc -o threads threads.c -Wall -pthread

io: io.c
	gcc -o io io.c -Wall


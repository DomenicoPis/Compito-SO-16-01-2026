start: main.c procedure.c header.h monitor_hoare.h monitor_hoare.c
	gcc -o start main.c procedure.c monitor_hoare.c

clean:
	rm -f start

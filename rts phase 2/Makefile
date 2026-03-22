CC = gcc
CFLAGS = -Wall -Wextra

OBJ = main.o task.o edf.o rm.o queue.o actual.o parameters.o

scheduler: $(OBJ)
	$(CC) $(CFLAGS) -o scheduler $(OBJ)

main.o: main.c task.h
	$(CC) $(CFLAGS) -c main.c

task.o: task.c task.h
	$(CC) $(CFLAGS) -c task.c

edf.o: edf.c task.h
	$(CC) $(CFLAGS) -c edf.c

rm.o: rm.c task.h
	$(CC) $(CFLAGS) -c rm.c

queue.o: queue.c task.h
	$(CC) $(CFLAGS) -c queue.c

actual.o: actual.c actual.h
	$(CC) $(CFLAGS) -c actual.c

parameters.o: parameters.c parameters.h
	$(CC) $(CFLAGS) -c parameters.c

clean:
	rm -f *.o scheduler

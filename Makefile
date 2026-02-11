CC = gcc
CFLAGS = -Wall

OBJ = main.o task.o edf.o rm.o

scheduler: $(OBJ)
	$(CC) $(CFLAGS) -o scheduler $(OBJ)

main.o: main.c task.h
task.o: task.c task.h
edf.o: edf.c task.h
rm.o: rm.c task.h

clean:
	rm -f *.o scheduler


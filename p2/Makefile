CC = gcc
TARGET = smallsh

all: $(TARGET)

$(TARGET): main.o smallsh.o
	$(CC) -o $@ $^

%.o: %.c smallsh.h
	$(CC) -c -o $@ $<

clean:
	rm -f *.o $(TARGET)

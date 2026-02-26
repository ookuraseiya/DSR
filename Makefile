CC     = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
TARGET = dsr_test
OBJS   = dsr.o dsr_packet.o dsr_test.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

dsr.o: dsr.c dsr.h dsr_constants.h
	$(CC) $(CFLAGS) -c dsr.c -o dsr.o

dsr_packet.o: dsr_packet.c dsr_packet.h dsr.h dsr_constants.h
	$(CC) $(CFLAGS) -c dsr_packet.c -o dsr_packet.o

dsr_test.o: dsr_test.c dsr.h dsr_packet.h
	$(CC) $(CFLAGS) -c dsr_test.c -o dsr_test.o

run: all
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean

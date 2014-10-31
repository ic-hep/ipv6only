CC=gcc
LD=gcc
CFLAGS=-Wall -fPIC
LDFLAGS=-Wl,--wrap=connect
LIBS=-ldl
TARGET=libpreipv6.so
OBJS=preipv6.o

.PHONY: clean

default: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -shared -o $(TARGET) $(OBJS) $(LIBS)

clean:
	rm -f $(TARGET) $(OBJS)


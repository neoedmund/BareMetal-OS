TARGET = mm.o

ifndef CC1
	CC1=clang
endif

# -nostartfiles -nodefaultlibs 
CFLAGS = -I../std/ -std=c1x -Wall -m64 -nostdlib -fomit-frame-pointer -mno-red-zone -fno-builtin

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC1) -c -o $@ $< $(CFLAGS)

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	ld -r $(OBJECTS)  -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)

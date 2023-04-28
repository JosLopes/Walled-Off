IDIR = include
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2 -I$(IDIR)

all: walled-off

ODIR = src/obj

LIBS = -lm -lcurses

DEPS = $(wildcard $(IDIR)/*.h)

OBJ = $(patsubst src/%.c, $(ODIR)/%.o, $(wildcard src/*.c))

$(info $$OBJ is [${OBJ}])

$(ODIR)/%.o: src/%.c $(DEPS)
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

walled-off: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o walled-off

IDIR = include
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2 -I$(IDIR)

ODIR = src/obj

LIBS = -lm -lcurses

# main will be a file, this is just for demonstration purposes. 
.PHONY: clean

_DEPS = $(wildcard $(IDIR)/*.h)
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = $(wildcard src/*.c)
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

walled-off: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(ODIR)/*.o

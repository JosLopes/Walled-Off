IDIR = ../include
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2 -I$(IDIR)

ODIR = obj
LDIR = ../lib

LIBS = -lm -lcurses

# main will be a file, this is just for demonstration purposes. 
.PHONY: main clean

_DEPS = $(wildcard *.h)
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = $(wildcard *.c)
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

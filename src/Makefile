IDIR = ../include
CC = gcc
CFLAGS = -I$(IDIR)

ODIR = obj
LDIR = ../lib

LIBS = -lm -lcurses

_DEPS = datatypes.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = movement.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

datatypes: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
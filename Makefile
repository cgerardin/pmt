CC				= clang
CFLAGS			= -std=c11 -Wall
LFLAGS			= -lncurses
EXEC			= pmt
DEBUG			= true

CFILES			= main.c input.c map.c

ifeq ($(DEBUG), true)
	CFLAGS += -g
else
	CFLAGS += -O3
endif

all: $(EXEC)
	
$(EXEC): $(CFILES)
	@$(CC) $(CFLAGS) -o $@ $(CFILES) $(LFLAGS)

.PHONY: clean

clean:
	@rm -rf *.o

mrproper: clean
	@rm $(EXEC)


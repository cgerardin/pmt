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
	@mkdir build
	@$(CC) $(CFLAGS) -o build/$@ $(CFILES) $(LFLAGS)

.PHONY: clean

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf build


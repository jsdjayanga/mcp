# Makefile
# For CS5923, Assignment 1, Q4 (Monte Carlo)
# Based on Prof. Kathy Yelick's material (at UC Berkeley)
#
#
# Include platform-dependent settings.
#
include Makefile.include

#
# NOTE: This only works for GNU Make

LDFLAGS += -Ldcmt0.4/lib -ldcmt

Q4_INCS = black_scholes.h gaussian.h parser.h random.h timer.h util.h
Q4_C_SRCS = black_scholes.c gaussian.c main.c parser.c random.c dcmt0.4/lib/random_seed.c timer.c util.c
Q4_C_OBJS = $(Q4_C_SRCS:.c=.o)
Q4_EXE = Q4.exe


all: Q4.exe

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

Q4.exe: $(Q4_C_OBJS) dcmt0.4/lib/libdcmt.a
	$(LINKER) $(CFLAGS) $(LDFLAGS) $(Q4_C_OBJS) -o $@

dcmt0.4/lib/libdcmt.a: 
	make -C dcmt0.4/lib

black_scholes.o: black_scholes.c black_scholes.h gaussian.h random.h util.h

gaussian.o: gaussian.c gaussian.h util.h

main.o: main.c black_scholes.h parser.h random.h timer.h

parser.o: parser.c parser.h

random.o: random.c random.h

dcmt0.4/lib/random_seed.o: dcmt0.4/lib/random_seed.c

timer.o: timer.c timer.h

util.o: util.c util.h

clean:
	make -C dcmt0.4/lib clean
	rm -f $(Q4_C_OBJS) $(Q4_EXE)

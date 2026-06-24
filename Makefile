CC ?= cc
CFLAGS ?= -O2 -std=c11 -Wall -Wextra -Wpedantic
CPPFLAGS ?= -Iinclude
LDLIBS ?= -lm

.PHONY: all test clean
all: test_registration_transfer test_registration_newtonian

test_registration_transfer: source/registration_transfer.c tests/test_registration_transfer.c include/registration_transfer.h
	$(CC) $(CPPFLAGS) $(CFLAGS) source/registration_transfer.c tests/test_registration_transfer.c -o $@ $(LDLIBS)

test_registration_newtonian: source/registration_transfer.c source/registration_newtonian.c tests/test_registration_newtonian.c include/registration_transfer.h include/registration_newtonian.h
	$(CC) $(CPPFLAGS) $(CFLAGS) source/registration_transfer.c source/registration_newtonian.c tests/test_registration_newtonian.c -o $@ $(LDLIBS)

test: all
	./test_registration_transfer data/sm_thermodynamics_husdal2016.csv
	./test_registration_newtonian data/sm_thermodynamics_husdal2016.csv

clean:
	rm -f test_registration_transfer test_registration_newtonian

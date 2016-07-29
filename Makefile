CC := gcc
OUT := ./ypifajkb
CFLAGS := -g -Wall -std=c11
SRC := ypifajkb.c
TEST := test.ypi

all: ypifajkb test

ypifajkb:
	$(CC) -o $(OUT) $(SRC) $(CFLAGS)

test:
	$(OUT) $(TEST)

clean:
	rm $(OUT) || :
EXEC = v1 v2
CFLAGS = -pthread
FILES = general.h general.c gcd_struct.h gcd_struct.c

all: clean $(EXEC)

v1:
	gcc -o v1_parent v1.c $(FILES) $(CFLAGS)

v2:
	gcc -o v2_parent v2.c $(FILES) $(CFLAGS)

clean:
	rm -f $(EXEC)
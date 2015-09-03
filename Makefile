.PHONY: all clean

all:
	$(CC) neosprconv.c -o neosprconv

clean:
	$(RM) neosprconv

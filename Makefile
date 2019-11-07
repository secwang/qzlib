CC := g++

CFLAGS := -std=c++11 -O3 -DNDEBUG -lz
CFLAGS_32 := -m32
CFLAGS_64 := -m64

CFLAGS_M := -bundle -undefined dynamic_lookup
CFLAGS_L := -shared -fPIC -lstdc++

SRC = qzlib.cpp

m32: $(SRC)
	$(CC) $(CFLAGS) $(CFLAGS_32) $(CFLAGS_M) $(SRC) -o qzlib_m32.so

m64: $(SRC)
	$(CC) $(CFLAGS) $(CFLAGS_64) $(CFLAGS_M) $(SRC) -o qzlib_m64.so

l32: $(SRC)
	$(CC) $(CFLAGS) $(CFLAGS_32) $(CFLAGS_L) $(SRC) -o qzlib_l32.so

l64: $(SRC)
	$(CC) $(CFLAGS) $(CFLAGS_64) $(CFLAGS_L) $(SRC) -o qzlib_l64.so

clean:
	rm -f qzlib_*.so

.PHONY: clean

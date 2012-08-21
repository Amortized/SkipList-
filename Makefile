CC=/usr/bin/g++


all:
	make clean
	make skiplist

skiplist:
	$(CC) skiplist.cpp -o skiplist

clean:
	rm -f skiplist




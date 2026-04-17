CC = gcc

all: test_objects test_tree test_index test_commit test_log

test_objects: object.c test_objects.c
	$(CC) object.c test_objects.c -o test_objects -lssl -lcrypto

test_tree: object.c tree.c test_tree.c
	$(CC) object.c tree.c test_tree.c -o test_tree -lssl -lcrypto

test_index: object.c index.c test_index.c
	$(CC) object.c index.c test_index.c -o test_index -lssl -lcrypto

test_commit: object.c tree.c index.c commit.c test_commit.c
	$(CC) object.c tree.c index.c commit.c test_commit.c -o test_commit -lssl -lcrypto

test_log: object.c tree.c index.c commit.c test_log.c
	$(CC) object.c tree.c index.c commit.c test_log.c -o test_log -lssl -lcrypto

clean:
	rm -f test_objects test_tree test_index test_commit test_log

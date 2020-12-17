app:
	$(CC) todo.c -o todo -Wall -std=c99

clean:
	rm -f todo

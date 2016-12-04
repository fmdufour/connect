all:
	gcc -g -Wall \
		src/main.c \
		src/server.c \
		src/client.c \
		-o bin/connect

clean:
	-@rm src/*.o bin/* 2>/dev/null || true
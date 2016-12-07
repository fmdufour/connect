all:
	gcc -g  \
		-o bin/connect \
		src/main.c \
		src/server.c \
		src/client.c \
		src/util.c	\
		src/commands.c \
		src/socket.c \
		src/protocol.c
		

clean:
	-@rm src/*.o bin/* 2>/dev/null || true
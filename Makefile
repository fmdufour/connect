all:
	gcc -g -Wall \
		-o bin/connect \
		src/main.c \
		src/server.c \
		src/client.c \
		src/util.c		
		

clean:
	-@rm src/*.o bin/* 2>/dev/null || true
MKDIR_P = mkdir -p

main: main.c
	gcc main.c -lncurses -ltinfo -lglut -lGL -o GameEngine
	${MKDIR_P} tempProject/objects
	${MKDIR_P} tempProject/scripts


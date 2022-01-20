#ifndef NCURSES_C
#define NCURSES_C
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h> 
#include "polyEditor.c"

char **splitBySpace(char* in){
	char **ret = malloc(sizeof(char*)*strlen(in));
	char buffer[14];
	int j = 0;
	int x = 0;
	for(int i = 0; i < strlen(in); i++)
		ret[i] = malloc(sizeof(char*));
	for(int i = 2; in[i]; i++){
		if(in[i+1] == '\0'){
			buffer[j++] = in[i];
			buffer[j] = 0;
			strcpy(ret[x++], buffer);
			j = 0;
		} else if(in[i] != ' '){
			buffer[j++] = in[i];
		} else {
			buffer[j] = 0;
			strcpy(ret[x++], buffer);
			j = 0;
		}
	}
	return ret;

}

int main(){
	char c;
	char buffer[40];
	int i = 0;
	int j = 0;
	initscr();
	noecho();
	for(int j = 0; j<40; j++)
		buffer[j] = '\0';
	move(0,0);
	while(1){
		refresh();
		c = getch();
		if(c == ':'){
			move(LINES-1,i);
			printw(":");
			i++;
			move(LINES-1,i);
			while((c = getch()) != 27){
				if(c == '\n'){
					break;
				} else {
					printw("%c",c);
					buffer[j++] = c;
					i++;
					move(LINES-1,i);
				}
			}
			move(0,0);
			buffer[j] = '\0';
			switch(buffer[0]){
				case ('q'):
//					endwin();
//					exit(0);
					break;
				case ('n'):{
					char **tdchar = splitBySpace(buffer);
					move(0,0);
//					printw("%s %s %s", tdchar[0], tdchar[1], tdchar[2]);
					endwin();
					start(0,tdchar);
					initscr();
					free(tdchar);
					break;
				}
			}
			j=0;
		}
		i = 0;
	}
	endwin();
}
#endif

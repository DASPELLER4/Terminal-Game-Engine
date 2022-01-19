#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>

int main(){
	char c;
	int i = 0;
	initscr();
	noecho();
	while(1){
		move(0,0);
		c = getch();
		if(c == ':'){
			move(LINES-1,i);
			printw(":");
			i++;
			move(LINES-1,i);
			while((c = getch()) != 27){
				if(c == '\n'){
					break;
				} else if(c == KEY_BACKSPACE){
					i--;
					move(LINES-1,i);
					printw("%c",' ');
					i--;
					move(LINES-1,i);
					printw("%c",' ');
				} else {
					printw("%c",c);
					i++;
					move(LINES-1,i);
				}
			}
		}
		i = 0;
	}
	endwin();
}

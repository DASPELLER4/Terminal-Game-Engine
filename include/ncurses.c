#ifndef NCURSES_C
#define NCURSES_C
#ifdef __unix__
	#include <sys/types.h>
	#include <sys/stat.h>
#else
	#include <conio.h>
	#include <sys/types.h>
	#include <sys/stat.h>
#endif
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h> 
#include <signal.h>
#include "polyEditor.c"
#include "render.h"

#define MAX_LINE_LENGTH 80

int splitCount=0;
char project[40] = {0};
char projectFile[60] = {0};

void sigintHandler(int sig_num){
	signal(SIGINT, sigintHandler);
	move(LINES-1,0);
	printw("Cannot be terminated using Ctrl+C");
}

char **splitBySpace(char* in){
	char **ret = malloc(sizeof(char*)*strlen(in));
	char buffer[14];
	int j = 0;
	int x = 0;
	splitCount=0;
	for(int i = 0; i < strlen(in); i++)
		ret[i] = malloc(sizeof(char*));
	for(int i = 0; in[i]; i++){
		if(in[i+1] == '\0'){
			buffer[j++] = in[i];
			buffer[j] = 0;
			splitCount++;
			strcpy(ret[x++], buffer);
			j = 0;
		} else if(in[i] != ' '){
			buffer[j++] = in[i];
		} else {
			buffer[j] = 0;
			splitCount++;
			strcpy(ret[x++], buffer);
			j = 0;
		}
	}
	return ret;

}

int genProjectInfo(){
	struct dirent *files;
	struct dirent *filees;
	DIR *dir = opendir(project);
	DIR *deer;
	char deeer[30];
	if (dir == NULL){
		return 0;
	}
	char str[1024] = {0};
	while ((files = readdir(dir)) != NULL){
		if(files->d_name[0] != '.'){
			strcat(str, files->d_name);
			strcat(str, "\n");
			strcpy(deeer, project);
			strcat(deeer, "/");
			strcat(deeer, files->d_name);
			if((deer = opendir(deeer))){
				while ((filees = readdir(deer)) != NULL){
					if(filees->d_name[0] != '.'){
						strcat(str, "\t");
						strcat(str, filees->d_name);
						strcat(str, "\n");
					}
				}
				closedir(deer);
			}
		}
	}
	FILE *fp = fopen(projectFile, "w");
	if (fp == NULL)
		return 0;
	fprintf(fp, "%s\n", str);
	fclose(fp);
	free(files);
	free(filees);
	closedir(dir);
}

int genAllIncludes(){
        struct dirent *files;
	char topDir[30] = {0};
	char allIncludes[40] = {0};
	strcat(topDir, project);
	strcat(topDir, "/scripts");
	strcat(allIncludes, project);
	strcat(allIncludes, "/scripts/allIncludes.h");
        DIR *dir = opendir(topDir);
        if (dir == NULL){
                return 0;
        }
        char str[1024] = {0};
	strcat(str, "#include \"tfuncs.h\"\n");
        while ((files = readdir(dir)) != NULL){
                if(files->d_name[0] != '.' && strcmp(files->d_name, "allIncludes.h")){
			strcat(str, "#include \"");
			strcat(str, files->d_name);
			strcat(str, "\"\n");
                }
        }
        FILE *fp = fopen(allIncludes, "w");
        if (fp == NULL)
                return 0;
        fprintf(fp, "%s\n", str);
        fclose(fp);
        free(files);
        closedir(dir);
}

int genCFile(){
        struct dirent *files;
	char topDir[30] = {0};
	strcat(topDir, project);
	strcat(topDir, "/scripts");
	char CFile[40] = {0};
	strcat(CFile, project);
	strcat(CFile, "/project.c");
        DIR *dir = opendir(topDir);
        if (dir == NULL){
                return 0;
        }
        char str[1024] = {0};
	int x = 0;
        while ((files = readdir(dir)) != NULL){
                if(files->d_name[0] != '.' && strcmp(files->d_name, "allIncludes.h") && strcmp(files->d_name, "tfuncs.h")){
			x++;
                }
        }
	strcat(str, "#include <ncurses.h>\n#include \"scripts/allIncludes.h\"\n#include \"libs/object.h\"\n#include \"libs/render.h\"\n#include \"libs/scene.h\"\nint main(){\nfor(int x = 0; x < 100; x++)\n\tsleeps[x] = 0;\nScene scene = {");
	strcat(str, itoa0(x));
	strcat(str, ",{");
	rewinddir(dir);
        while ((files = readdir(dir)) != NULL){
                if(files->d_name[0] != '.' && strcmp(files->d_name, "allIncludes.h") && strcmp(files->d_name, "tfuncs.h")){
			strcat(str, "wut(\"");
			strcat(str, files->d_name);
			strcat(str, "\"),");
                }
        }
	str[strlen(str)-1] = 0;
	strcat(str,"}};\n");
        rewinddir(dir);
        while ((files = readdir(dir)) != NULL){
                if(files->d_name[0] != '.' && strcmp(files->d_name, "allIncludes.h") && strcmp(files->d_name, "tfuncs.h")){
			strcat(str, files->d_name);
                        strcat(str, "start(get_object(&scene, \"");
                        strcat(str, files->d_name);
                        strcat(str, "\"));\n");
                }
        }
	rewinddir(dir);
	strcat(str,"while (1){\nchar c = getch();\n");
        while ((files = readdir(dir)) != NULL){
                if(files->d_name[0] != '.' && strcmp(files->d_name, "allIncludes.h") && strcmp(files->d_name, "tfuncs.h")){
                        strcat(str, files->d_name);
                        strcat(str, "loop(get_object(&scene, \"");
                        strcat(str, files->d_name);
                        strcat(str, "\"),c);\n");
                }
        }
	strcat(str,"Screen screen = sceneToScreen(scene);render(screen);freeScreen(screen);}}");
        FILE *fp = fopen(CFile, "w");
        if (fp == NULL)
                return 0;
        fprintf(fp, "%s\n", str);
        fclose(fp);
        free(files);
        closedir(dir);
}

int uiStart(){
	char c;
	char buffer[40];
	int i = 0;
	int j = 0;
	initscr();
	noecho();
	cbreak();
	signal(SIGINT, sigintHandler);
	for(int j = 0; j<40; j++)
		buffer[j] = '\0';
	move(0,0);
	while(1){
		if(project[0] != 0){
			printw("Project directory: %s", project);
			move(1,0);
			if( access( projectFile, F_OK ) == 0 ) {
				genProjectInfo();
				FILE *file = fopen(projectFile, "r");
    				if (!file){
				        perror(projectFile);
				}
				unsigned int line_count = 0;
				char lines[MAX_LINE_LENGTH] = {0};
				while (fgets(lines, MAX_LINE_LENGTH, file)) {
					printw("%s", lines);
					if (lines[strlen(lines) - 1] != '\n')
						printw("\n");
				}
				fclose(file);
			} else {
				printw("project.info not found, is this a valid project? Looked for %s", projectFile);
			}
		}
		move(0,0);
		refresh();
		c = getch();
		if(c == ':'){
			move(LINES-1,i);
			clrtoeol();
			printw(":");
			i++;
			move(LINES-1,i);
			while((c = getch()) != 27){
				if (c == KEY_BACKSPACE || c == KEY_DC || c == 127){
					i--;
					move(LINES-1,i);
					printw("%c",' ');
					buffer[--j] = '\0';
				}else if (c == '\n'){
					break;
				} else {
					printw("%c",c);
					buffer[j++] = c;
					i++;
					move(LINES-1,i);
				}
			}
			buffer[j] = '\0';
			if(project[0] == 0 && (buffer[0]!='o' && buffer[0]!='q' && (buffer[0]!='n' && buffer[1]!='p') && buffer[0]!='h')){
				move(LINES-1,0);
				printw("NO PROJECT OPENED");
			}else{
				move(0,0);
				switch(buffer[0]){
					case ('q'):
						endwin();
						exit(0);
						break;
					case ('c'):{
						genAllIncludes();
						char command[100] = "gcc ";
						strcat(command, project);
						strcat(command, "/project.c -o ");
						strcat(command, project);
						strcat(command, "/bin -lncurses -ltinfo -lglut -lGL");
						system(command);
						move(LINES-2,0);
						printw("RUN IN THE SAME DIRECTORY AS THE BINARY");
						break;
					} case ('h'):
						move(LINES/2,0);
						printw("Help Page - h\nfull controls available at the readme");
						break;
					case ('n'):{
						switch(buffer[1]){
							case('o'):{
								char **tdchar = splitBySpace(buffer);
								move(LINES-1,0);
								endwin();
								if(!(splitCount<4)){
								char shit[30];
								strcpy(shit, project);
								strcat(shit, "/objects/");
								strcat(shit, tdchar[1]);
								strcpy(tdchar[1], shit);
								start(4,tdchar);
								}
								initscr();
								for(int i = 0; i<splitCount; i++)
									free(tdchar[i]);
								break;
							}case('p'):{
								char **tdchar = splitBySpace(buffer);
								mkdir(tdchar[1],0777);
								char command[50] = {0};
								strcat(command, "cp -r tempProject/* ");
								strcat(command, tdchar[1]);
								system(command);
								for(int i = 0; i<splitCount; i++)
								        free(tdchar[i]);
								break;
							}case('s'):{
								char **tdchar = splitBySpace(buffer);
								move(LINES-1,0);
								char shit[30];
								strcpy(shit, project);
								strcat(shit, "/scripts/");
								strcat(shit, tdchar[1]);
								FILE *newScript = fopen(shit, "w");
								char content[1024] = {0};
								strcat(content, "#include \"tfuncs.h\"\n#include \"../libs/object.h\"\nint ");
								strcat(content, tdchar[1]);
								strcat(content, "start(Object *self){\n}\nint ");
								strcat(content, tdchar[1]);
								strcat(content, "loop(Object *self, char c){\n}");
								fprintf(newScript, "%s", content);
								fclose(newScript);
								for(int i = 0; i<splitCount; i++)
									free(tdchar[i]);
								genAllIncludes();
								genCFile();
								break;
							}

						}
						break;
					} case('p'):{
						char **tdchar = splitBySpace(buffer);
						Screen screen = initScreen();
						char shit[30];
						strcpy(shit, project);
						strcat(shit, "/objects/");
						strcat(shit, tdchar[1]);
						readAndAdd(shit, &screen);
						putchar('\n');
						for(int i = 0; i<splitCount; i++)
							free(tdchar[i]);
						render(screen);
						freeScreen(screen);
						char cool;
						while((cool = getch()) != '\n');
						endwin();
						initscr();
						noecho();
						cbreak();
						break;
					} case ('o'):{
						char **tdchar = splitBySpace(buffer);
						strcpy(project, tdchar[1]);
						for(int i = 0; i<splitCount; i++)
							free(tdchar[i]);
						strcpy(projectFile, project);
						strcat(projectFile, "/project.info");
						break;
					}default:
						move(LINES-1,0);
						printw("Unknown command: %c",buffer[0]);
						break;
				}
				j=0;
			}
		}
		i = 0;
	}
	endwin();
}
#endif

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

#define PROJ_NAME_LENGTH 40
#define MAX_LINE_LENGTH 80

int splitCount=0;
char project[PROJ_NAME_LENGTH] = {0};
char projectFile[PROJ_NAME_LENGTH+20] = {0};

void sigintHandler(int sig_num){
	signal(SIGINT, sigintHandler);
	move(LINES-1,0);
	printw("Cannot be terminated using Ctrl+C");
}

char **splitBySpace(char* in){
	char **ret = malloc(sizeof(char*)*strlen(in));
	char buffer[100];
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
			strncpy(ret[x++], buffer, 100);
			j = 0;
		} else if(in[i] != ' '){
			buffer[j++] = in[i];
		} else {
			buffer[j] = 0;
			splitCount++;
			strncpy(ret[x++], buffer, 100);
			j = 0;
		}
	}
	return ret;

}

int genProjectInfo(){
	struct dirent *files;
	struct dirent *filees;
	DIR *dir = opendir(project);
	DIR *dirToReadForFile;
	char dirToReadForFilesName[PROJ_NAME_LENGTH+21];
	if (dir == NULL){
		return 0;
	}
	int lenOfStrVar = 0;
	while ((files = readdir(dir)) != NULL){
		if(files->d_name[0] != '.'){
			lenOfStrVar += strlen(files->d_name) + 2;
			strncpy(dirToReadForFilesName, project, PROJ_NAME_LENGTH);
			strcat(dirToReadForFilesName, "/");
			strcat(dirToReadForFilesName, files->d_name);
			if((dirToReadForFile = opendir(dirToReadForFilesName))){
				while ((filees = readdir(dirToReadForFile)) != NULL){
					if(filees->d_name[0] != '.'){
						lenOfStrVar += 4 + strlen(filees->d_name);
					}
				}
				closedir(dirToReadForFile);
			}
		}
	}
	rewinddir(dir);
	rewinddir(dirToReadForFile);
	char str[lenOfStrVar];
	while ((files = readdir(dir)) != NULL){
		if(files->d_name[0] != '.'){
			strcat(str, files->d_name);
			strcat(str, "\n");
			strncpy(dirToReadForFilesName, project, PROJ_NAME_LENGTH);
			strcat(dirToReadForFilesName, "/");
			strcat(dirToReadForFilesName, files->d_name);
			if((dirToReadForFile = opendir(dirToReadForFilesName))){
				while ((filees = readdir(dirToReadForFile)) != NULL){
					if(filees->d_name[0] != '.'){
						strcat(str, "\t");
						strcat(str, filees->d_name);
						strcat(str, "\n");
					}
				}
				closedir(dirToReadForFile);
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
	char topDir[PROJ_NAME_LENGTH+8];
	char allIncludes[PROJ_NAME_LENGTH+22];
	strncpy(topDir, project, PROJ_NAME_LENGTH);
	strcat(topDir, "/scripts");
	strncpy(allIncludes, project, PROJ_NAME_LENGTH);
	strcat(allIncludes, "/scripts/allIncludes.h");
        DIR *dir = opendir(topDir);
        if (dir == NULL){
                return 0;
        }
	int lenOfStrVar = 0;
       	while ((files = readdir(dir)) != NULL){
                if(files->d_name[0] != '.' && strcmp(files->d_name, "allIncludes.h")){
			lenOfStrVar += 12 + strlen(files->d_name);
                }
        }
	rewinddir(dir);
	char str[lenOfStrVar+20];
	strcpy(str, "#include \"tfuncs.h\"\n");
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
	char topDir[PROJ_NAME_LENGTH+8];
	strncpy(topDir, project, PROJ_NAME_LENGTH);
	strcat(topDir, "/scripts");
	char CFile[PROJ_NAME_LENGTH+10];
	strncpy(CFile, project, PROJ_NAME_LENGTH);
	strcat(CFile, "/project.c");
        DIR *dir = opendir(topDir);
        if (dir == NULL){
                return 0;
        }
	int lenOfStrVar = 347;
	while ((files = readdir(dir)) != NULL){
                if(files->d_name[0] != '.' && strcmp(files->d_name, "allIncludes.h") && strcmp(files->d_name, "tfuncs.h")){
			lenOfStrVar += strlen(files->d_name)*5+72;
                }
        }
        char str[lenOfStrVar];
	str[0] = 0;
	rewinddir(dir);
	int x = 0;
        while ((files = readdir(dir)) != NULL){
                if(files->d_name[0] != '.' && strcmp(files->d_name, "allIncludes.h") && strcmp(files->d_name, "tfuncs.h")){
			x++;
                }
        }
	strcat(str, "#include <ncurses.h>\n#include \"scripts/allIncludes.h\"\n#include \"libs/object.h\"\n#include \"libs/render.h\"\n#include \"libs/scene.h\"\nint main(){\nsignal(SIGINT, sigintHandler);for(int x = 0; x < 100; x++)\n\tsleeps[x] = 0;\nScene scene = {");
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
	strcat(str,"initscr();noecho();nodelay(stdscr, TRUE);scrollok(stdscr, TRUE);char c = 0;while (1){\nif(kbhit)c=getch();move(0,0);\n");
        while ((files = readdir(dir)) != NULL){
                if(files->d_name[0] != '.' && strcmp(files->d_name, "allIncludes.h") && strcmp(files->d_name, "tfuncs.h")){
                        strcat(str, files->d_name);
                        strcat(str, "loop(get_object(&scene, \"");
                        strcat(str, files->d_name);
                        strcat(str, "\"),c);\n");
                }
        }
	strcat(str,"Screen screen = sceneToScreen(scene);render(screen);freeScreen(screen);if(c=='q')break;}endwin();exit(0);}");
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
	char buffer[400];
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
					if(i>1){
					i--;
					delch();
					move(LINES-1,i);
					delch();
					buffer[--j] = '\0';}
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
			j=0;
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
						genCFile();
						char command[PROJ_NAME_LENGTH*2+14+34];
						strncpy(command,"gcc ",5);
						strncat(command, project, PROJ_NAME_LENGTH);
						strcat(command, "/project.c -o ");
						strncat(command, project, PROJ_NAME_LENGTH);
						strcat(command, "/bin -lncurses -ltinfo -lglut -lGL");
						system(command);
						move(LINES-2,0);
						printw("RUN IN THE SAME DIRECTORY AS THE BINARY");
						break;
					} case ('h'):
						move(LINES/2,0);
						printw("Help Page - h\nfull controls available at the readme"); // LOL PAST ME WAS AN ASSHOLE
						break;
					case ('e'):{
						char **tdchar = splitBySpace(buffer);
						if(splitCount==2){
							move(LINES-1,0);	
							char command[PROJ_NAME_LENGTH+5+11+strlen(tdchar[1])];
							strncpy(command, "vim ", 5);
							strcat(command, project);
							strcat(command, "/scripts/");
							strcat(command, tdchar[1]);
							endwin();
							system(command);
							initscr();
						}
						for(int i = 0; i<splitCount; i++)
						        free(tdchar[i]);
						break;
					}
					case ('r'):{
						move(LINES-1,0);
						char command[PROJ_NAME_LENGTH+4+8];
						strcpy(command, "cd ");
						strcat(command, project);
						strcat(command, "; ./bin");
						endwin();
						system(command);
						initscr();
						break;
					}
					case ('n'):{
						switch(buffer[1]){
							case('o'):{
								char **tdchar = splitBySpace(buffer);
								move(LINES-1,0);
								endwin();
								if(splitCount==4){
									char command[PROJ_NAME_LENGTH+12+strlen(tdchar[1])];
									strncpy(command, project, PROJ_NAME_LENGTH);
									strcat(command, "/objects/");
									strcat(command, tdchar[1]);
									strncpy(tdchar[1], command, 100); // really stupid, why'd i do that
									start(4,tdchar);
								}
								initscr();
								for(int i = 0; i<splitCount; i++)
									free(tdchar[i]);
								break;
							}case('p'):{
								char **tdchar = splitBySpace(buffer);
								if(splitCount==2){
									mkdir(tdchar[1],0777);
									char command[22+strlen(tdchar[1])];
									strcpy(command, "cp -r tempProject/* ");
									strcat(command, tdchar[1]);
									system(command);
								}
								for(int i = 0; i<splitCount; i++)
								        free(tdchar[i]);
								break;
							}case('s'):{
								char **tdchar = splitBySpace(buffer);
								if(splitCount==2){
									move(LINES-1,0);
									char fileName[PROJ_NAME_LENGTH+12+strlen(tdchar[1])];
									strncpy(fileName, project, PROJ_NAME_LENGTH);
									strcat(fileName, "/scripts/");
									strncat(fileName, tdchar[1], 100);
									FILE *newScript = fopen(fileName, "w");
									char content[strlen(tdchar[1])*2+2+53+28+30];
									strcpy(content, "#include \"tfuncs.h\"\n#include \"../libs/object.h\"\nint ");
									strncat(content, tdchar[1],100);
									strcat(content, "start(Object *self){\n}\nint ");
									strncat(content, tdchar[1],100);
									strcat(content, "loop(Object *self, char c){\n}");
									fprintf(newScript, "%s", content);
									fclose(newScript);
								}
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
						if(splitCount==2){
							Screen screen = initScreen();
							char prev[PROJ_NAME_LENGTH+13+strlen(tdchar[1])];
							strncpy(prev, project, PROJ_NAME_LENGTH);
							strcat(prev, "/objects/");
							strncat(prev, tdchar[1], 100);
							readAndAdd(prev, &screen);
							putchar('\n');
							render(screen);
							freeScreen(screen);
							char cool;
							while((cool = getch()) != '\n');
							refresh();
						}
						for(int i = 0; i<splitCount; i++)
							free(tdchar[i]);
						break;
					} case ('o'):{
						char **tdchar = splitBySpace(buffer);
						printf("%d",buffer);
						if(splitCount==2){
							strncpy(project, tdchar[1], PROJ_NAME_LENGTH);
							strncpy(projectFile, project, PROJ_NAME_LENGTH);
							strcat(projectFile, "/project.info");
						}
						for(int i = 0; i<splitCount; i++)
							free(tdchar[i]);
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

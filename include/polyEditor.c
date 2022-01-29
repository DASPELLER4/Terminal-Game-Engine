#ifndef POLYEDIT_C
#define POLYEDIT_C
#include <GL/freeglut.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "types.h"
#include "render.h"

typedef struct{
	Vector2d vertecies[3];
} glPoly;

glPoly Polygons[25];
int currVert = 0;
int currPoly = 0;
char** args;

void display(){
	if(currPoly>0)
	for(int i = 0; i<currPoly; i++){
		glBegin(GL_POLYGON);
                	glVertex3f(Polygons[i].vertecies[0].a,Polygons[i].vertecies[0].b,0);
	                glVertex3f(Polygons[i].vertecies[1].a,Polygons[i].vertecies[1].b,0);
        	        glVertex3f(Polygons[i].vertecies[2].a,Polygons[i].vertecies[2].b,0);
	        glEnd();
	}
	glFlush();
}

void onMouse(int button, int state, int x, int y){
	double w = glutGet( GLUT_WINDOW_WIDTH );
	double h = glutGet( GLUT_WINDOW_HEIGHT );
	if(button == 2 && state == 0){
		currPoly--;
		if(currPoly < 0)
			currPoly = 0;
		currVert = 0;
		glClear(GL_COLOR_BUFFER_BIT);
	}
	else if(button == 0 && state == 0){
		Polygons[currPoly].vertecies[currVert] = (Vector2d){((x/w * 2.0) / 1) - 1,((((h/2.0)-y)+h/2.0)/h * 2.0) - 1};
		currVert++;
		if(currVert > 2){
			currPoly++;
			currVert = 0;
		}
	}
}

void idle(){
	glutPostRedisplay();
}

char *formatToFile(){
	char *out = calloc(currPoly*50,1);
	double lowestX = 1;
	double lowestY = 1;
	for(int i = 0; i < currPoly; i++){
		for(int j = 0; j < 3; j++){
			if(Polygons[i].vertecies[j].a < lowestX)
			        lowestX = Polygons[i].vertecies[j].a;
			if(Polygons[i].vertecies[j].b < lowestY)
		        	lowestY = Polygons[i].vertecies[j].b;
		}
	}
	for(int i = 0; i < currPoly; i++){
		for(int j = 0; j < 3; j++){
			int k;
			k=(((((Polygons[i].vertecies[j].a + 1) * glutGet(GLUT_WINDOW_WIDTH)) / 2) - ((((lowestX + 1) * glutGet(GLUT_WINDOW_WIDTH)) / 2)))/(10*atof(args[3])))*2-1;
			strcat(out, itoa0(k));
			strcat(out, " ");
			k=((((Polygons[i].vertecies[j].b + 1) * glutGet(GLUT_WINDOW_HEIGHT)) / 2) - ((((lowestY + 1) * glutGet(GLUT_WINDOW_HEIGHT)) / 2)))/(10*atof(args[3]));
			strcat(out, itoa0(k));
			strcat(out, " ");
		}
		strcat(out, args[2]);
		strcat(out, "\n");
	}
	return out;
}

void keypress(unsigned  char key, int x, int y){
	switch(key){
		case('s'):{
			char *content = formatToFile();
			FILE *fp;
			fp = fopen(args[1], "w");
			if(fp == NULL){
				printf("file can't be opened\n");
				exit(1);
			}
			fprintf(fp, "%s", content);
			fclose(fp);
			free(content);
			break;}
		case('q'):
			glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
			if(glutGetWindow())
				glutDestroyWindow(glutGetWindow());
			break;
		case('r'):
			currPoly = currVert = 0;
			break;
		case('h'):
			printf("Object Generator Help Page\n\tThis tool is to assist in the creation of a 2D object\n\nControls:\n\tLMB-Places a new vertex, takes 3 vertecies to create a polygon\n\tRMB-Delete last polygon\n\tS-Save to the file supplied in the console\n\tQ-Quit the software\n\tR-Reset all polygons\n\tH-Display this page\n");
			break;
	}
}

int start(int argc, char** argv){
	args = argv;
	for(int i = 0; i < 25; i++)
		Polygons[i] = (glPoly){{{0,0},{0,0},{0,0}}};
	currVert = 0;
	currPoly = 0;
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowPosition(80, 80);
        glutInitWindowSize(400, 300);
        glutCreateWindow("Object Creator | h for Help");
	glClear(GL_COLOR_BUFFER_BIT);
        glutDisplayFunc(display);
	glutMouseFunc(onMouse);
        glutIdleFunc(idle);
	glutKeyboardFunc(keypress);
        glutMainLoop();
	return 0;
}
#endif

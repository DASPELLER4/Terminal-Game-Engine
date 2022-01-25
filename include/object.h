#ifndef OBJECT_H
#define OBJECT_H

#include "render.h"
#include "types.h"
#include <string.h>

typedef struct{
	char *name;
	int relPosX;
	int relPosY;
	Poly *polygons;
	int PolyCount;
} Object;

#include "scene.h"

Object* get_object(Scene scene, char name[]){
	for(int i = 0; i < scene.ObjectCount; i++)
		if(!strcmp(scene.objects[i].name, name))
			return &scene.objects[i];
}

Poly *readPolygons(char *fileName){ // read from a file polygon data
	char contents[MAXSIZE], color;
	Poly *polygons = malloc(40*sizeof(Polygon));
	int currentVector[] = {0,0};
	int tempCoord[3][2] = {{0,0},{0,0},{0,0}};
	FILE *fptr;
	if((fptr = fopen(fileName,"r")) == NULL){
		printf("Error! opening file");
		exit(1);
	}
	int curr = 0;
	while (fscanf(fptr, "%s", contents) != EOF) { // welcome to hell
		if (contents[0] > '9' || contents[0] < '0'){ // this means that the character is a "colour character" so it has reached the end of the polygon
			color = contents[0];
			polygons[curr++] = {(Poly) {{{tempCoord[0][0], tempCoord[0][1]},{tempCoord[1][0], tempCoord[1][1]},{tempCoord[2][0], tempCoord[2][1]}},color}}
			currentVector[0] = 0; // reset the position where the polygon data is written to
			currentVector[1] = 0;
		}
		else{
			tempCoord[currentVector[1]][currentVector[0]] = atoi(contents); // the current string is probably an integer, just write to the current coordinate value
			currentVector[0]++;
		}
		if(currentVector[0] > 1){ // if we have reached the end of the vertex, go onto the next field
			currentVector[1]++;
			currentVector[0] = 0;
		}
		if(currentVector[1] > 3){ // if it continues past the last position in tempCoord, just throw an error so it doesn't seg fault
			printf("Error! Syntax Error: colour not specified or more than 3 vertecies specified");
			exit(1);
		}
	}
	fclose(fptr);
	return readPolygons;
}

Object genObject(char dir[]){
	char fileName[40] = {};
	strcpy(fileName,"../objects/");
	strcat(fileName,dir);
	FILE *fp fopen(fileName,"r");;
	char ch;
	int lineCount = 0;
	while((ch=fgetc(fp))!=EOF) {
		if(ch=='\n')
			linesCount++;
	}
	fclose(fp);
	Object ret = {dir, 0, 0, readPolygon(fileName), lineCount};
	return ret;
}

#endif

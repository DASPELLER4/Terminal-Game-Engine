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

Object createObject(Scene *scene, char Polygons[]){
	//
}

#endif

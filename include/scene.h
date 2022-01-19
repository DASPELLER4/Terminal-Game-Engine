#ifndef SCENE_H
#define SCENE_H

#include "object.h"
#include "render.h"
#include "types.h"

typedef struct{
        int ObjectCount;
        Object *objects;
} Scene;

Screen sceneToScreen(Scene scene){
	Screen ret = initScreen();
	ret.PolyCount = scene.ObjectCount*3;
	ret.Polygons = (Poly *) malloc(sizeof(Poly) * ret.PolyCount);
	for(int x = 0; x < ret.PolyCount;)
		for(int i = 0; i < scene.ObjectCount; i++)
			for(int j = 0; j < scene.objects[i].PolyCount; j++)
				ret.Polygons[x++] = scene.objects[i].polygons[j];
	return ret;
}

#endif

#ifndef SCENE_H
#define SCENE_H

#include "object.h"
#include "render.h"
#include "types.h"

typedef struct{
        int ObjectCount;
        Object objects[100];
} Scene;

Screen sceneToScreen(Scene scene){
	Screen ret = initScreen();
	ret.PolyCount = 0;
	for(int i = 0; i < scene.ObjectCount; i++)
		for(int x = 0; x < scene.objects[i].PolyCount; x++)
			ret.PolyCount++;
	ret.Polygons = (Poly *) malloc(sizeof(Poly) * ret.PolyCount);
	for(int x = 0; x < ret.PolyCount;)
		for(int i = 0; i < scene.ObjectCount; i++)
			for(int j = 0; j < scene.objects[i].PolyCount; j++)
				ret.Polygons[x++] = (Poly){{{scene.objects[i].polygons[j].Coords[0].a+scene.objects[i].relPosX, scene.objects[i].polygons[j].Coords[0].b+scene.objects[i].relPosY},{scene.objects[i].polygons[j].Coords[1].a+scene.objects[i].relPosX, scene.objects[i].polygons[j].Coords[1].b+scene.objects[i].relPosY},{scene.objects[i].polygons[j].Coords[2].a+scene.objects[i].relPosX, scene.objects[i].polygons[j].Coords[2].b+scene.objects[i].relPosY}},scene.objects[i].polygons[j].Colour};
//				ret.Polygons[x++] = scene.objects[i].polygons[j];
	return ret;
}

#endif

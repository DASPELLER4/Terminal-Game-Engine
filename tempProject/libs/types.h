#ifndef TYPES_H
#define TYPES_H
typedef struct{
        int a;
        int b;
} Vector2;

typedef struct{
        double a;
        double b;
} Vector2d;

typedef struct{
        int a;
        int b;
        int c;
} Vector3;

typedef struct{
        Vector2 Coords[3];
        char Colour;
} Poly;
#endif

#include "include/types.h"
#include "include/object.h"
#include "include/scene.h"
#include "include/render.h"

int main(int argc, char** argv){
        Screen screen = initScreen();
        readAndAdd(argv[1], &screen);
        putchar('\n');
        render(screen);
        freeScreen(screen);
        return 0;
}

#include <siege/siege.h>
#include <sge/cave/cave.h>

#include <string.h>
#include <stdio.h>
#include <math.h>

#define WIDTH 1280
#define HEIGHT 800

#define CELL 16

#define DBORDER SG_TRUE
#define BORDER  SG_TRUE
#define DIAG    SG_TRUE

#define GRIDW (WIDTH / CELL - 2)
#define GRIDH (HEIGHT / CELL - 2)

SGECave* cave;

void SG_CALL evKeyboardKeyPress(SGEntity* entity, SGenum key)
{
    switch(key)
    {
    case SG_KEY_SPACE:
        //sgeCaveStep(cave);
        sgeCaveCalcSets(cave, DIAG);
        break;
    }
}

int main(void)
{
    sgLoadModule("SDL");
    sgLoadModule("OpenGL");
    sgInit(0);

    sgWindowOpen(WIDTH, HEIGHT, 32, 0);

    //SGERule rule;

    //rule = mkrule("678", "345678");   // rough cave
    //rule = sgeRule("5678", "45678");   // smooth cave

    cave = sgeCaveCreate(GRIDW, GRIDH, NULL, 0.45, BORDER);

    SGEntity* ctrl = sgEntityCreate();
    ctrl->evKeyboardKeyPress = evKeyboardKeyPress;
    ctrl->evKeyboardKeyRepeat = evKeyboardKeyPress;

    SGint ret;
    for(;;)
    {
        sgDrawClear1f(1.0);
        if(!sgLoop(&ret))
            break;

        //sgDrawColor1f(0.0);
        sgeCaveDrawGrayDBG(cave, CELL, DBORDER);
        sgeCaveDrawSetsDBG(cave, sgColor3f(1.0, 0.0, 0.0), CELL, DBORDER);

        sgWindowSwapBuffers();
    }

    sgDeinit();
    return ret;
}

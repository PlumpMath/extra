#include <sge/cave/cave.h>

#include <string.h>
#include <assert.h>
#include <math.h>

static void gridInit(SGECave* cave, float fill)
{
    int i, j;
    for(j = 1; j < cave->height + 1; j++)
    {
        for(i = 1; i < cave->width + 1; i++)
        {
            cave->gridf[i][j] = fmod(sgRandGenf(cave->rand), 1.0);
            cave->grid[cave->curr][i][j] = (1.0 - fill <= cave->gridf[i][j]);
            cave->grid[!cave->curr][i][j] = (1.0 - fill <= cave->gridf[i][j]);
        }
    }
    for(i = 0; i < cave->width + 2; i++)
        memset(cave->set[i], 0, (cave->height + 2) * sizeof(SGuint));
}
static void gridCalcPropagate(SGECave* cave, SGint x, SGint y, SGuint set, SGbool diag)
{
    SGuint stop = 1;
    cave->stack[0] = sgePoint2i(x, y);

    SGEPoint p;
    while(stop)
    {
        p = cave->stack[stop-1];
        cave->set[p.x][p.y] = set;
        cave->setstr[set]++;
        if(!sgeCaveGetSetCurr(cave, p.x+1, p.y))
            cave->stack[stop++] = sgePoint2i(p.x+1, p.y);
        else if(!sgeCaveGetSetCurr(cave, p.x-1, p.y))
            cave->stack[stop++] = sgePoint2i(p.x-1, p.y);
        else if(!sgeCaveGetSetCurr(cave, p.x, p.y+1))
            cave->stack[stop++] = sgePoint2i(p.x, p.y+1);
        else if(!sgeCaveGetSetCurr(cave, p.x, p.y-1))
            cave->stack[stop++] = sgePoint2i(p.x, p.y-1);
        else if(diag && !sgeCaveGetSetCurr(cave, p.x+1, p.y+1))
            cave->stack[stop++] = sgePoint2i(p.x+1, p.y+1);
        else if(diag && !sgeCaveGetSetCurr(cave, p.x-1, p.y+1))
            cave->stack[stop++] = sgePoint2i(p.x-1, p.y+1);
        else if(diag && !sgeCaveGetSetCurr(cave, p.x+1, p.y-1))
            cave->stack[stop++] = sgePoint2i(p.x+1, p.y-1);
        else if(diag && !sgeCaveGetSetCurr(cave, p.x-1, p.y-1))
            cave->stack[stop++] = sgePoint2i(p.x-1, p.y-1);
        else
            stop--;
    }
}

SGECave* SG_CALL sgeCaveCreate(size_t width, size_t height, SGRand* rand, float fill, SGbool border)
{
    SGECave* cave = malloc(sizeof(SGECave));
    if(!cave) return NULL;

    size_t awidth = width + 2;
    size_t aheight = height + 2;

    cave->width = width;
    cave->height = height;

    if(rand)
    {
        cave->rand = rand;
        cave->delrand = SG_FALSE;
    }
    else
    {
        cave->rand = sgRandCreate(SG_RAND_MERSENNE32);
        cave->delrand = SG_TRUE;
    }

    size_t i;

    cave->curr = 1;
    cave->grid[0] = malloc(awidth * sizeof(SGbool*));
    cave->grid[1] = malloc(awidth * sizeof(SGbool*));
    cave->gridf = malloc(awidth * sizeof(float*));
    cave->set = malloc(awidth * sizeof(SGuint*));
    for(i = 0; i < awidth; i++)
    {
        cave->grid[0][i] = malloc(aheight * sizeof(SGbool));
        cave->grid[1][i] = malloc(aheight * sizeof(SGbool));
        cave->gridf[i] = malloc(aheight * sizeof(float));
        cave->set[i] = malloc(aheight * sizeof(SGuint));
    }
    cave->setstr = malloc(awidth * aheight * sizeof(SGuint));  // NULL
    cave->stack = malloc(awidth * aheight * sizeof(SGEPoint)); // NULL

    sgeCaveSetBorder(cave, border);
    gridInit(cave, fill);

    return cave;
}
void SG_CALL sgeCaveDestroy(SGECave* cave)
{
    if(!cave) return;

    size_t i;

    if(cave->stack)
        free(cave->stack);
    if(cave->setstr)
        free(cave->setstr);
    for(i = 0; i < cave->width + 2; i++)
    {
        free(cave->set[i]);
        free(cave->gridf[i]);
        free(cave->grid[1][i]);
        free(cave->grid[0][i]);
    }
    free(cave->set);
    free(cave->gridf);
    free(cave->grid[1]);
    free(cave->grid[0]);

    if(cave->delrand)
        sgRandDestroy(cave->rand);
}

void SG_CALL sgeCaveSetBorder(SGECave* cave, SGbool border)
{
    size_t i, c;
    for(i = 1; i < cave->width + 1; i++)
    {
        for(c = 0; c < 2; c++)
        {
            cave->grid[c][i][0             ] = border;
            cave->grid[c][i][cave->height+1] = border;
        }
        cave->gridf[i][0             ] = border;
        cave->gridf[i][cave->height+1] = border;
        cave->set[i][0             ] = border ? 0xFFFFFFFF : 0;
        cave->set[i][cave->height+1] = border ? 0xFFFFFFFF : 0;
    }
    for(c = 0; c < 2; c++)
    {
        memset(cave->grid[c][0            ], border, (cave->height + 2) * sizeof(SGbool));
        memset(cave->grid[c][cave->width+1], border, (cave->height + 2) * sizeof(SGbool));
    }
    // can't use memset here because we're dealing with floats
    for(i = 0; i < cave->height + 2; i++)
    {
        cave->gridf[0            ][i] = border;
        cave->gridf[cave->width+1][i] = border;
        cave->set[0            ][i] = border ? 0xFFFFFFFF : 0;
        cave->set[cave->width+1][i] = border ? 0xFFFFFFFF : 0;
    }
}
SGbool SG_CALL sgeCaveGetCell(SGECave* cave, SGubyte sel, SGint i, SGint j)
{
    SGEPoint p = sgePointNearest(sgePoint2i(i, j), sgePoint2i(0, 0), sgePoint2i(cave->width + 1, cave->height + 1));
    return cave->grid[sel][p.x][p.y];
}
SGbool SG_CALL sgeCaveGetCellPrev(SGECave* cave, SGint i, SGint j)
{
    return sgeCaveGetCell(cave, !cave->curr, i, j);
}
SGbool SG_CALL sgeCaveGetCellCurr(SGECave* cave, SGint i, SGint j)
{
    return sgeCaveGetCell(cave, cave->curr, i, j);
}
SGuint SG_CALL sgeCaveGetSet(SGECave* cave, SGubyte sel, SGint i, SGint j)
{
    if(i < 0 || j < 0 || i >= cave->width + 2 || j >= cave->height + 2)
        return 0xFFFFFFFF;
    if(cave->grid[sel][i][j])
        return 0xFFFFFFFF;
    return cave->set[i][j];
}
SGuint SG_CALL sgeCaveGetSetCurr(SGECave* cave, SGint i, SGint j)
{
    return sgeCaveGetSet(cave, cave->curr, i, j);
}
SGuint SG_CALL sgeCaveGetSetPrev(SGECave* cave, SGint i, SGint j)
{
    return sgeCaveGetSet(cave, !cave->curr, i, j);
}

SGubyte SG_CALL sgeCaveCount(SGECave* cave, SGubyte sel, SGint i, SGint j)
{
    return sgeCaveGetCell(cave,sel,i-1,j-1)
        +  sgeCaveGetCell(cave,sel,i  ,j-1)
        +  sgeCaveGetCell(cave,sel,i+1,j-1)
        +  sgeCaveGetCell(cave,sel,i-1,j  )
        +  sgeCaveGetCell(cave,sel,i+1,j  )
        +  sgeCaveGetCell(cave,sel,i-1,j+1)
        +  sgeCaveGetCell(cave,sel,i  ,j+1)
        +  sgeCaveGetCell(cave,sel,i+1,j+1);
}

SGuint SG_CALL sgeCaveCalcSets(SGECave* cave, SGbool diag)
{
    SGint i, j;
    for(i = 0; i < cave->width + 2; i++)
        for(j = 0; j < cave->height + 2; j++)
            cave->set[i][j] = 0;
        //memset(&cave->set[i][1], 0, (cave->height + 1) * sizeof(SGuint));
    cave->setnum = 0;
    cave->setstr[0] = 0;
    for(i = 1; i < cave->width + 1; i++)
    {
        for(j = 1; j < cave->height + 1; j++)
        {
            if(!cave->grid[cave->curr][i][j] && !cave->set[i][j])
                gridCalcPropagate(cave, i, j, ++cave->setnum, diag);
            else
                cave->setstr[0]++;
        }
    }
    return cave->setnum;
}
void SG_CALL sgeCaveConnect(SGECave* cave, SGbool diag)
{
    assert(0); /* TODO */
}

void SG_CALL sgeCaveDrawDBG(SGECave* cave, float cell, SGbool border)
{
    SGint i, j;

    for(i = !border; i < cave->width + 1 + border; i++)
        for(j = !border; j < cave->height + 1 + border; j++)
            if(cave->grid[cave->curr][i][j])
                sgDrawRectangle2fWH(i * cell, j * cell, cell, cell, SG_TRUE);
}
void SG_CALL sgeCaveDrawGrayDBG(SGECave* cave, float cell, SGbool border)
{
    SGint i, j;

    for(i = !border; i < cave->width + 1 + border; i++)
    {
        for(j = !border; j < cave->height + 1 + border; j++)
        {
            if(cave->grid[cave->curr][i][j])
            {
                sgDrawColor2f(1.0 - cave->gridf[i][j], 0.25);
                sgDrawRectangle2fWH(i * cell, j * cell, cell, cell, SG_TRUE);
            }
        }
    }
}
void sgeCaveDrawSetsDBG(SGECave* cave, SGColor base, float cell, SGbool border)
{
    SGint i, j;
    for(i = !border; i < cave->width + 1 + border; i++)
    {
        for(j = !border; j < cave->height + 1 + border; j++)
        {
            if(cave->set[i][j])
            {
                sgDrawColor4f(base.r, base.g, base.b, base.a * (cave->set[i][j] / (float)cave->setnum));
                sgDrawRectangle2fWH(i * cell, j * cell, cell, cell, SG_TRUE);
            }
        }
    }
}

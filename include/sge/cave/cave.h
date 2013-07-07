#ifndef SGE_CAVE_CAVE_H_
#define SGE_CAVE_CAVE_H_

#include <siege/siege.h>

#include "../point.h"

typedef struct SGECave
{
    size_t width, height;

    SGRand*   rand;     // RNG to be used
    SGbool delrand;     // remove rand on destruction?

    SGbool    curr;     // current head
    SGbool**  grid[2];  // main cell grid
    float**   gridf;    // float cell grid
    SGuint    setnum;   // number of sets
    SGuint**  set;      // set data
    SGuint*   setstr;   // set strength data
    SGEPoint* stack;    // stack of points; used internally in set eval
} SGECave;

SGECave* SG_CALL sgeCaveCreate(size_t width, size_t height, SGRand* rand, float fill, SGbool border);
void SG_CALL sgeCaveDestroy(SGECave* cave);

void SG_CALL sgeCaveSetBorder(SGECave* cave, SGbool border);

SGbool SG_CALL sgeCaveGetCell(SGECave* cave, SGubyte sel, SGint i, SGint j);
SGbool SG_CALL sgeCaveGetCellPrev(SGECave* cave, SGint i, SGint j);
SGbool SG_CALL sgeCaveGetCellCurr(SGECave* cave, SGint i, SGint j);
SGuint SG_CALL sgeCaveGetSet(SGECave* cave, SGubyte sel, SGint i, SGint j);
SGuint SG_CALL sgeCaveGetSetCurr(SGECave* cave, SGint i, SGint j);
SGuint SG_CALL sgeCaveGetSetPrev(SGECave* cave, SGint i, SGint j);
SGubyte SG_CALL sgeCaveCount(SGECave* cave, SGubyte sel, SGint i, SGint j);

SGuint SG_CALL sgeCaveCalcSets(SGECave* cave, SGbool diag);
void SG_CALL sgeCaveConnect(SGECave* cave, SGbool diag); /* TODO */

void SG_CALL sgeCaveDrawDBG(SGECave* cave, float cell, SGbool border);
void SG_CALL sgeCaveDrawGrayDBG(SGECave* cave, float cell, SGbool border);
void SG_CALL sgeCaveDrawSetsDBG(SGECave* cave, SGColor base, float cell, SGbool border);

#endif /* SGE_CAVE_CAVE_H_ */

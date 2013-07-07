#ifndef SGE_CAVE_GEN_H_
#define SGE_CAVE_GEN_H_

#include "cave.h"
#include "rule.h"

SGuint SG_CALL sgeCaveGenStep(SGECave* cave, const SGERule* rule);
SGuint SG_CALL sgeCaveGenStepRough(SGECave* cave);
SGuint SG_CALL sgeCaveGenStepSmooth(SGECave* cave);

#endif /* SGE_CAVE_GEN_H_ */

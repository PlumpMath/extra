#ifndef SGE_POINT_H_
#define SGE_POINT_H_

#include <siege/siege.h>

typedef struct SGEPoint
{
    SGint x, y;
} SGEPoint;

SGEPoint SG_CALL sgePoint2i(SGint x, SGint y);

SGEPoint SG_CALL sgePointNearest(SGEPoint p, SGEPoint min, SGEPoint max);

#endif /* SGE_POINT_H_ */

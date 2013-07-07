#include <sge/point.h>

static void swap_SGint(SGint* a, SGint* b)
{
    SGint tmp = *a;
    *a = *b;
    *b = tmp;
}

SGEPoint SG_CALL sgePoint2i(SGint x, SGint y)
{
    SGEPoint p;
    p.x = x;
    p.y = y;
    return p;
}

SGEPoint SG_CALL sgePointNearest(SGEPoint p, SGEPoint min, SGEPoint max)
{
    if(max.x < min.x) swap_SGint(&min.x, &max.x);
    if(max.y < min.y) swap_SGint(&min.y, &max.y);

    if(p.x < min.x) p.x = min.x;
    if(p.x > max.x) p.x = max.x;
    if(p.y < min.y) p.y = min.y;
    if(p.y > max.y) p.y = max.y;

    return p;
}

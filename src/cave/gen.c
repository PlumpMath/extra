#include <sge/cave/gen.h>

#include <string.h>
#include <assert.h>
#include <math.h>

SGuint SG_CALL sgeCaveGenStep(SGECave* cave, const SGERule* rule)
{
    cave->curr = !cave->curr;
    SGubyte count;
    SGuint changed = 0;
    SGint i, j;
    for(j = 1; j < cave->height + 1; j++)
    {
        for(i = 1; i < cave->width + 1; i++)
        {
            count = sgeCaveCount(cave, !cave->curr, i, j);
            if(!cave->grid[!cave->curr][i][j] && rule->b[count])
            {
                changed++;
                cave->grid[cave->curr][i][j] = SG_TRUE;
            }
            else if(cave->grid[!cave->curr][i][j] && !rule->s[count])
            {
                changed++;
                cave->grid[cave->curr][i][j] = SG_FALSE;
            }
            else
            {
                changed = cave->grid[cave->curr][i][j] != cave->grid[!cave->curr][i][j];
                cave->grid[cave->curr][i][j] = cave->grid[!cave->curr][i][j];
            }
        }
    }
    return changed;
}
SGuint SG_CALL sgeCaveGenStepRough(SGECave* cave)
{
    static SGERule rule;
    static SGbool hasrule = SG_FALSE;
    if(!hasrule)
    {
        rule = sgeRule("678", "345678");
        hasrule = SG_TRUE;
    }
    return sgeCaveGenStep(cave, &rule);
}
SGuint SG_CALL sgeCaveGenStepSmooth(SGECave* cave)
{
    static SGERule rule;
    static SGbool hasrule = SG_FALSE;
    if(!hasrule)
    {
        rule = sgeRule("5678", "45678");
        hasrule = SG_TRUE;
    }
    return sgeCaveGenStep(cave, &rule);
}

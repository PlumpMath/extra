#ifndef SGE_CAVE_RULE_H_
#define SGE_CAVE_RULE_H_

#include <siege/siege.h>

typedef struct SGERule
{
    SGbool b[9];    // born
    SGbool s[9];    // survive
} SGERule;

SGERule SG_CALL sgeRule(const char* b, const char* s);

#endif /* SGE_CAVE_RULE_H_ */

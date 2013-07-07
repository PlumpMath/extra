#include <sge/cave/rule.h>

#include <string.h>

static void mkpart(SGubyte set[8], const char* str)
{
    memset(set, SG_FALSE, 8);
    while(*str)
    {
        if('0' <= *str && *str <= '8')
            set[*str - '0'] = SG_TRUE;
        str++;
    }
}

SGERule SG_CALL sgeRule(const char* b, const char* s)
{
    SGERule rule;
    mkpart(rule.b, b);
    mkpart(rule.s, s);
    return rule;
}

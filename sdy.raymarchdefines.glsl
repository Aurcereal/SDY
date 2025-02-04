#define MAXDIST 50.0
#define MAXSTEPS 100
#define SDEPS 0.0005
#define NORMEPS 0.0005

// This dist will be used for when something shouldn't exist, like a MAX node at -NULLDIST shouldn't exist but a MAX node at -MAXDIST should exist.
#define NULLMAXDIST (MAXDIST*2.0)
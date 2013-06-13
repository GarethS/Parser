/*
    A substitute file since malloc.h does not appear in the IAR distribution.
    The contents are there to get motor.tab.c compiling
*/

#if 0
Hm, maybe not needed after all
char* strdup(const char*);
#define BISON_IAR   1
//#define YY_NO_UNISTD_H
#define EINTR   (0)
#endif
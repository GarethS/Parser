/*************************************************
 *  Copyright (C) 2012 by Gareth Scott
 *  All Rights Reserved
 *************************************************/

#ifndef _COMPILERHELPER_H_
#define _COMPILERHELPER_H_

#ifndef FALSE
#define FALSE (0)
#endif /* FALSE */
#ifndef TRUE
#define TRUE  (!FALSE)
#endif /* TRUE */

#define NOT_FOUND   (-1)

typedef enum {nodeVar, nodeConst, nodeOperator, nodeInvalid} nodeType;
typedef enum {nodeLeft, nodeRight, nodeRoot} nodePosition;

#endif /* _COMPILERHELPER_H_ */
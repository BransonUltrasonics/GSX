/* rcc_log.h */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,20feb04,jws  added copyright and mod history.
*/


#ifndef __RCC_LOG_HEADER__
#define __RCC_LOG_HEADER__

#ifdef __cplusplus
extern "C" {
#endif

extern FILE *LogInit(char *fileName);
extern void  LogWrite(FILE *fd, char *text);
extern void  LogClose(FILE *fd);

extern void RCC_LOG_Login(sbyte *pLogin, sbyte *pMessage);
extern void RCC_LOG_Input(sbyte *pMessage);
extern void RCC_LOG_IO(sbyte *pPrompt, sbyte *pMessage);
extern void RCC_LOG_Output(sbyte *pMessage);

#ifdef __cplusplus
}
#endif

#endif /* __RCC_LOG_HEADER__ */

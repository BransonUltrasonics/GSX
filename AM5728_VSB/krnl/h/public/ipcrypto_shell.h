/*
 * Copyright (c) 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
09dec16,chm  written (F8092)
*/

#ifndef IPCRYPTO_SHELL_H
#define IPCRYPTO_SHELL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int (* Ipcrypto_shell_cmd_type) (int argc, char ** argv);

int ipcrypto_shell_add_cmd(const char *name, const char *usage,
                    const char *description, Ipcrypto_shell_cmd_type hook);

#ifdef __cplusplus
extern "C" {
#endif

#endif /*IPCRYPTO_SHELL_H*/

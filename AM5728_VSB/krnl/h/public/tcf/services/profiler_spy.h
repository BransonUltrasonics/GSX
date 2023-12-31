/*******************************************************************************
 * Copyright (c) 2013 Xilinx, Inc. and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 * The Eclipse Public License is available at
 * http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 * You may elect to redistribute this code under either of these licenses.
 *
 * Contributors:
 *     Xilinx - initial API and implementation
 *******************************************************************************/

/*
 * Generic sampling/stack-tracing profiler implementation.
 */

#ifndef D_profiler_spy
#define D_profiler_spy

#include <tcf/config.h>

#include <tcf/framework/link.h>
#include <tcf/framework/context.h>
#include <tcf/services/profiler.h>

/* Add profiling support for debug context 'ctx' */
extern void profiler_spy_add(Context * ctx);

/* Check if profiling is enabled for debug context 'ctx' */
extern int profiler_spy_is_enabled(Context * ctx);

/* Add a profiling sample */
extern void profiler_spy_sample(Context * ctx, ContextAddress pc);

/* Reset (clear) profilng data */
extern void profiler_spy_reset(Context * ctx);

extern void ini_profiler_spy(void);

#endif /* D_profiler_spy */

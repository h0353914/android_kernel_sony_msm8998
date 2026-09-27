/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Compat shim: this kernel predates the compiler.h/compiler_types.h split
 * (~Linux 4.11). Everything third-party code expects from
 * <linux/compiler_types.h> already lives in <linux/compiler.h> here.
 */
#ifndef _LINUX_COMPILER_TYPES_H
#define _LINUX_COMPILER_TYPES_H
#include <linux/compiler.h>
#endif

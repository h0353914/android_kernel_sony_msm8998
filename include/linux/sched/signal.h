/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Compat shim: this kernel predates the sched.h split (~Linux 4.11) into
 * sched/signal.h, sched/task.h, sched/user.h etc. Everything third-party
 * code expects from these headers already lives in <linux/sched.h> here.
 */
#ifndef _LINUX_SCHED_SIGNAL_H
#define _LINUX_SCHED_SIGNAL_H
#include <linux/sched.h>
#endif

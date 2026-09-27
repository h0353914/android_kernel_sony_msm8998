/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KERNELSU_LEGACY_COMPAT_H
#define _LINUX_KERNELSU_LEGACY_COMPAT_H

#include <linux/version.h>

/* KernelSU-Next legacy uses helpers introduced after Linux 4.4. */
#ifndef ALIGN_DOWN
#define ALIGN_DOWN(x, a) ((x) & ~((typeof(x))(a) - 1))
#endif

#ifndef __nocfi
#define __nocfi
#endif

#if defined(CONFIG_ARM64) && \
	LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
struct pt_regs;
typedef long (*syscall_fn_t)(const struct pt_regs *regs);
#endif

#endif /* _LINUX_KERNELSU_LEGACY_COMPAT_H */

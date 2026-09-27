/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KERNELSU_LEGACY_COMPAT_H
#define _LINUX_KERNELSU_LEGACY_COMPAT_H

#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>

/* KernelSU-Next legacy uses helpers introduced after Linux 4.4. */
#ifndef ALIGN_DOWN
#define ALIGN_DOWN(x, a) ((x) & ~((typeof(x))(a) - 1))
#endif

#ifndef __nocfi
#define __nocfi
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
#define ksys_close sys_close
#endif

#if defined(CONFIG_ARM64) && \
	LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
struct pt_regs;
typedef long (*syscall_fn_t)(const struct pt_regs *regs);
#endif

#ifndef strncpy_from_user_nofault
#define strncpy_from_user_nofault(dst, src, count) \
	strncpy_from_user((dst), (src), (count))
#endif

#ifndef copy_from_user_nofault
#define copy_from_user_nofault(dst, src, size) copy_from_user((dst), (src), (size))
#endif

#endif /* _LINUX_KERNELSU_LEGACY_COMPAT_H */

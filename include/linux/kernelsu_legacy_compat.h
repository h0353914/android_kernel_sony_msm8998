/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KERNELSU_LEGACY_COMPAT_H
#define _LINUX_KERNELSU_LEGACY_COMPAT_H

#include <linux/version.h>
#include <linux/compiler.h>

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

#if defined(CONFIG_ARM64) && !defined(untagged_addr)
#define untagged_addr(addr) sign_extend64(addr, 55)
#endif

extern long strncpy_from_user(char *dest, const char __user *src,
			      long count);

#ifndef copy_to_kernel_nofault
#define copy_to_kernel_nofault(dst, src, size) probe_kernel_write((dst), (src), (size))
#endif

#ifndef __flush_icache_range
#define __flush_icache_range(start, end) flush_icache_range((start), (end))
#endif

#endif /* _LINUX_KERNELSU_LEGACY_COMPAT_H */

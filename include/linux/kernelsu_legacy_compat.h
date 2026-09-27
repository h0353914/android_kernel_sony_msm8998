/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KERNELSU_LEGACY_COMPAT_H
#define _LINUX_KERNELSU_LEGACY_COMPAT_H

#include <linux/version.h>
#include <linux/compiler.h>

/* poll_mask was renamed to __poll_t after Linux 4.4. */
typedef unsigned int __poll_t;

#ifndef EPOLLIN
#define EPOLLIN 0x00000001
#endif
#ifndef EPOLLHUP
#define EPOLLHUP 0x00000010
#endif
#ifndef EPOLLRDNORM
#define EPOLLRDNORM 0x00000040
#endif

/* Linux 4.4 has no seccomp action cache; keep KernelSU's cache a no-op. */
#ifndef SECCOMP_ARCH_NATIVE_NR
#define SECCOMP_ARCH_NATIVE_NR 0
#endif
#ifndef refcount_t
#define refcount_t atomic_t
#endif

#ifndef REMAP_FILE_DEDUP
#define REMAP_FILE_DEDUP (1U << 0)
#endif

struct inode;
struct module;
struct qstr;

#ifdef CONFIG_MODULES
extern _Bool try_module_get(struct module *module);
extern void module_put(struct module *module);
#endif

/* Anonymous inodes had no dedicated LSM initialization hook in Linux 4.4. */
static inline int security_inode_init_security_anon(
	struct inode *inode, const struct qstr *name,
	const struct inode *context_inode)
{
	return 0;
}

/* KernelSU-Next legacy uses helpers introduced after Linux 4.4. */
#ifndef ALIGN_DOWN
#define ALIGN_DOWN(x, a) ((x) & ~((typeof(x))(a) - 1))
#endif

#ifndef __nocfi
#define __nocfi
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
#define ksys_close sys_close
#define ksys_unshare sys_unshare
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

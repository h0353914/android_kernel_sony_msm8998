/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_REFCOUNT_H
#define _LINUX_REFCOUNT_H

/*
 * Linux 4.4 has no refcount_t (added upstream in 4.11 as a saturating
 * wrapper around atomic_t with overflow protection). This plain atomic_t
 * based shim gives KernelSU's fsnotify backport the API it needs without
 * the overflow-detection fast path, which this kernel's exception-table
 * infrastructure was never updated to support.
 */

#include <linux/atomic.h>
#include <linux/spinlock.h>

typedef atomic_t refcount_t;

#define REFCOUNT_INIT(n) ATOMIC_INIT(n)

static inline void refcount_set(refcount_t *r, int n)
{
	atomic_set(r, n);
}

static inline unsigned int refcount_read(const refcount_t *r)
{
	return atomic_read(r);
}

static inline void refcount_inc(refcount_t *r)
{
	atomic_inc(r);
}

static inline bool refcount_inc_not_zero(refcount_t *r)
{
	return atomic_add_unless(r, 1, 0);
}

static inline bool refcount_dec_and_test(refcount_t *r)
{
	return atomic_dec_and_test(r);
}

static inline bool refcount_dec_and_lock(refcount_t *r, spinlock_t *lock)
{
	return atomic_dec_and_lock(r, lock);
}

#endif /* _LINUX_REFCOUNT_H */

// SPDX-License-Identifier: GPL-2.0
/*
 * Minimal backport of the generic per-(namespace,uid) resource counters
 * (kernel/ucount.c in later kernels) needed by the fsnotify backport for
 * KernelSU-Next. Real upstream keys a global hash table by (ns, uid) and
 * supports many resource types; this only implements the handful of
 * inotify/fanotify counters those subsystems actually call, but keeps the
 * same real limit-checking semantics (not a no-op) since these guard
 * against unbounded kernel memory use from a single uid.
 */
#include <linux/user_namespace.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/hash.h>
#include <linux/export.h>

#define UCOUNTS_HASH_BITS 7
static struct hlist_head ucounts_hash[1 << UCOUNTS_HASH_BITS];
static DEFINE_SPINLOCK(ucounts_lock);

struct ucounts_entry {
	struct hlist_node node;
	struct ucounts ucounts;
};

static struct ucounts *find_ucounts_locked(struct user_namespace *ns, kuid_t uid)
{
	struct ucounts_entry *entry;
	unsigned int hash = hash_32(__kuid_val(uid) ^ (unsigned long)ns,
				    UCOUNTS_HASH_BITS);

	hlist_for_each_entry(entry, &ucounts_hash[hash], node) {
		if (entry->ucounts.ns == ns && uid_eq(entry->ucounts.uid, uid))
			return &entry->ucounts;
	}
	return NULL;
}

static struct ucounts *get_or_create_ucounts(struct user_namespace *ns, kuid_t uid)
{
	struct ucounts_entry *entry;
	struct ucounts *ucounts;
	unsigned int hash;

	spin_lock(&ucounts_lock);
	ucounts = find_ucounts_locked(ns, uid);
	spin_unlock(&ucounts_lock);
	if (ucounts)
		return ucounts;

	entry = kzalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry)
		return NULL;
	entry->ucounts.ns = ns;
	entry->ucounts.uid = uid;

	spin_lock(&ucounts_lock);
	ucounts = find_ucounts_locked(ns, uid);
	if (ucounts) {
		spin_unlock(&ucounts_lock);
		kfree(entry);
		return ucounts;
	}
	hash = hash_32(__kuid_val(uid) ^ (unsigned long)ns, UCOUNTS_HASH_BITS);
	hlist_add_head(&entry->node, &ucounts_hash[hash]);
	spin_unlock(&ucounts_lock);
	return &entry->ucounts;
}

struct ucounts *inc_ucount(struct user_namespace *ns, kuid_t uid,
			   enum ucount_type type)
{
	struct ucounts *ucounts = get_or_create_ucounts(ns, uid);
	long max;

	if (!ucounts)
		return NULL;

	max = ns->ucount_max[type];
	if (max && atomic_long_inc_return(&ucounts->count[type]) > max) {
		atomic_long_dec(&ucounts->count[type]);
		return NULL;
	}
	return ucounts;
}
EXPORT_SYMBOL(inc_ucount);

void dec_ucount(struct ucounts *ucounts, enum ucount_type type)
{
	atomic_long_dec(&ucounts->count[type]);
}
EXPORT_SYMBOL(dec_ucount);

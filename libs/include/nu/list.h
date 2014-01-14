#ifndef _LINUX_LIST_H
#define _LINUX_LIST_H

#include "nu/compiler.h"
#include <stddef.h>

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct nu__ListHead {
    struct nu__ListHead *prev;
    struct nu__ListHead *next;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct nu__ListHead name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct nu__ListHead *list)
{
	list->next = list;
	list->prev = list;
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __nu__List__add(struct nu__ListHead *_new,
			      struct nu__ListHead *prev,
			      struct nu__ListHead *next)
{
	next->prev = _new;
	_new->next = next;
	_new->prev = prev;
	prev->next = _new;
}

/**
 * nu__List__add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void nu__List__add(struct nu__ListHead *_new, struct nu__ListHead *head)
{
	__nu__List__add(_new, head, head->next);
}


/**
 * nu__List__add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void nu__List__add_tail(struct nu__ListHead *_new, struct nu__ListHead *head)
{
	__nu__List__add(_new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __nu__List__del(struct nu__ListHead * prev, struct nu__ListHead * next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * nu__List__del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: nu__List__empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void __nu__List__del_entry(struct nu__ListHead *entry)
{
	__nu__List__del(entry->prev, entry->next);
}

static inline void nu__List__del(struct nu__ListHead *entry)
{
	__nu__List__del(entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}

/**
 * nu__List__replace - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
static inline void nu__List__replace(struct nu__ListHead *old,
				struct nu__ListHead *_new)
{
	_new->next = old->next;
	_new->next->prev = _new;
	_new->prev = old->prev;
	_new->prev->next = _new;
}

static inline void nu__List__replace_init(struct nu__ListHead *old,
					struct nu__ListHead *_new)
{
	nu__List__replace(old, _new);
	INIT_LIST_HEAD(old);
}

/**
 * nu__List__del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void nu__List__del_init(struct nu__ListHead *entry)
{
	__nu__List__del_entry(entry);
	INIT_LIST_HEAD(entry);
}

/**
 * nu__List__move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void nu__List__move(struct nu__ListHead *list, struct nu__ListHead *head)
{
	__nu__List__del_entry(list);
	nu__List__add(list, head);
}

/**
 * nu__List__move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void nu__List__move_tail(struct nu__ListHead *list,
				  struct nu__ListHead *head)
{
	__nu__List__del_entry(list);
	nu__List__add_tail(list, head);
}

/**
 * nu__List__is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static inline int nu__List__is_last(const struct nu__ListHead *list,
				const struct nu__ListHead *head)
{
	return list->next == head;
}

/**
 * nu__List__empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int nu__List__empty(const struct nu__ListHead *head)
{
	return head->next == head;
}

/**
 * nu__List__empty_careful - tests whether a list is empty and not being modified
 * @head: the list to test
 *
 * Description:
 * tests whether a list is empty _and_ checks that no other CPU might be
 * in the process of modifying either member (next or prev)
 *
 * NOTE: using nu__List__empty_careful() without synchronization
 * can only be safe if the only activity that can happen
 * to the list entry is nu__List__del_init(). Eg. it cannot be used
 * if another CPU could re-nu__List__add() it.
 */
static inline int nu__List__empty_careful(const struct nu__ListHead *head)
{
	struct nu__ListHead *next = head->next;
	return (next == head) && (next == head->prev);
}

/**
 * nu__List__rotate_left - rotate the list to the left
 * @head: the head of the list
 */
static inline void nu__List__rotate_left(struct nu__ListHead *head)
{
	struct nu__ListHead *first;

	if (!nu__List__empty(head)) {
		first = head->next;
		nu__List__move_tail(first, head);
	}
}

/**
 * nu__List__is_singular - tests whether a list has just one entry.
 * @head: the list to test.
 */
static inline int nu__List__is_singular(const struct nu__ListHead *head)
{
	return !nu__List__empty(head) && (head->next == head->prev);
}

static inline void __nu__List__cut_position(struct nu__ListHead *list,
		struct nu__ListHead *head, struct nu__ListHead *entry)
{
	struct nu__ListHead *new_first = entry->next;
	list->next = head->next;
	list->next->prev = list;
	list->prev = entry;
	entry->next = list;
	head->next = new_first;
	new_first->prev = head;
}

/**
 * nu__List__cut_position - cut a list into two
 * @list: a new list to add all removed entries
 * @head: a list with entries
 * @entry: an entry within head, could be the head itself
 *	and if so we won't cut the list
 *
 * This helper moves the initial part of @head, up to and
 * including @entry, from @head to @list. You should
 * pass on @entry an element you know is on @head. @list
 * should be an empty list or a list you do not care about
 * losing its data.
 *
 */
static inline void nu__List__cut_position(struct nu__ListHead *list,
		struct nu__ListHead *head, struct nu__ListHead *entry)
{
	if (nu__List__empty(head))
		return;
	if (nu__List__is_singular(head) &&
		(head->next != entry && head != entry))
		return;
	if (entry == head)
		INIT_LIST_HEAD(list);
	else
		__nu__List__cut_position(list, head, entry);
}

static inline void __nu__List__splice(const struct nu__ListHead *list,
				 struct nu__ListHead *prev,
				 struct nu__ListHead *next)
{
	struct nu__ListHead *first = list->next;
	struct nu__ListHead *last = list->prev;

	first->prev = prev;
	prev->next = first;

	last->next = next;
	next->prev = last;
}

/**
 * nu__List__splice - join two lists, this is designed for stacks
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void nu__List__splice(const struct nu__ListHead *list,
				struct nu__ListHead *head)
{
	if (!nu__List__empty(list))
		__nu__List__splice(list, head, head->next);
}

/**
 * nu__List__splice_tail - join two lists, each list being a queue
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void nu__List__splice_tail(struct nu__ListHead *list,
				struct nu__ListHead *head)
{
	if (!nu__List__empty(list))
		__nu__List__splice(list, head->prev, head);
}

/**
 * nu__List__splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void nu__List__splice_init(struct nu__ListHead *list,
				    struct nu__ListHead *head)
{
	if (!nu__List__empty(list)) {
		__nu__List__splice(list, head, head->next);
		INIT_LIST_HEAD(list);
	}
}

/**
 * nu__List__splice_tail_init - join two lists and reinitialise the emptied list
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * Each of the lists is a queue.
 * The list at @list is reinitialised
 */
static inline void nu__List__splice_tail_init(struct nu__ListHead *list,
					 struct nu__ListHead *head)
{
	if (!nu__List__empty(list)) {
		__nu__List__splice(list, head->prev, head);
		INIT_LIST_HEAD(list);
	}
}

/**
 * nu__List__entry - get the struct for this entry
 * @ptr:	the &struct nu__ListHead pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the nu__List__struct within the struct.
 */
#define nu__List__entry(ptr, type, member) \
	((type *)(void *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
 * nu__List__first_entry - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the nu__List__struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define nu__List__first_entry(ptr, type, member) \
	nu__List__entry((ptr)->next, type, member)

/**
 * nu__List__for_each	-	iterate over a list
 * @pos:	the &struct nu__ListHead to use as a loop cursor.
 * @head:	the head for your list.
 */
#define nu__List__for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * __nu__List__for_each	-	iterate over a list
 * @pos:	the &struct nu__ListHead to use as a loop cursor.
 * @head:	the head for your list.
 *
 * This variant doesn't differ from nu__List__for_each() any more.
 * We don't do prefetching in either case.
 */
#define __nu__List__for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * nu__List__for_each_prev	-	iterate over a list backwards
 * @pos:	the &struct nu__ListHead to use as a loop cursor.
 * @head:	the head for your list.
 */
#define nu__List__for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
 * nu__List__for_each_safe - iterate over a list safe against removal of list entry
 * @pos:	the &struct nu__ListHead to use as a loop cursor.
 * @n:		another &struct nu__ListHead to use as temporary storage
 * @head:	the head for your list.
 */
#define nu__List__for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * nu__List__for_each_prev_safe - iterate over a list backwards safe against removal of list entry
 * @pos:	the &struct nu__ListHead to use as a loop cursor.
 * @n:		another &struct nu__ListHead to use as temporary storage
 * @head:	the head for your list.
 */
#define nu__List__for_each_prev_safe(pos, n, head) \
	for (pos = (head)->prev, n = pos->prev; \
	     pos != (head); \
	     pos = n, n = pos->prev)

/**
 * nu__List__for_each_entry	-	iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the nu__List__struct within the struct.
 */
#define nu__List__for_each_entry(pos, head, member)				\
	for (pos = nu__List__entry((head)->next, typeof(*pos), member);	\
	     &pos->member != (head); 	\
	     pos = nu__List__entry(pos->member.next, typeof(*pos), member))

/**
 * nu__List__for_each_entry_reverse - iterate backwards over list of given type.
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the nu__List__struct within the struct.
 */
#define nu__List__for_each_entry_reverse(pos, head, member)			\
	for (pos = nu__List__entry((head)->prev, typeof(*pos), member);	\
	     &pos->member != (head); 	\
	     pos = nu__List__entry(pos->member.prev, typeof(*pos), member))

/**
 * nu__List__prepare_entry - prepare a pos entry for use in nu__List__for_each_entry_continue()
 * @pos:	the type * to use as a start point
 * @head:	the head of the list
 * @member:	the name of the nu__List__struct within the struct.
 *
 * Prepares a pos entry for use as a start point in nu__List__for_each_entry_continue().
 */
#define nu__List__prepare_entry(pos, head, member) \
	((pos) ? : nu__List__entry(head, typeof(*pos), member))

/**
 * nu__List__for_each_entry_continue - continue iteration over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the nu__List__struct within the struct.
 *
 * Continue to iterate over list of given type, continuing after
 * the current position.
 */
#define nu__List__for_each_entry_continue(pos, head, member) 		\
	for (pos = nu__List__entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head);	\
	     pos = nu__List__entry(pos->member.next, typeof(*pos), member))

/**
 * nu__List__for_each_entry_continue_reverse - iterate backwards from the given point
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the nu__List__struct within the struct.
 *
 * Start to iterate over list of given type backwards, continuing after
 * the current position.
 */
#define nu__List__for_each_entry_continue_reverse(pos, head, member)		\
	for (pos = nu__List__entry(pos->member.prev, typeof(*pos), member);	\
	     &pos->member != (head);	\
	     pos = nu__List__entry(pos->member.prev, typeof(*pos), member))

/**
 * nu__List__for_each_entry_from - iterate over list of given type from the current point
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the nu__List__struct within the struct.
 *
 * Iterate over list of given type, continuing from current position.
 */
#define nu__List__for_each_entry_from(pos, head, member) 			\
	for (; &pos->member != (head);	\
	     pos = nu__List__entry(pos->member.next, typeof(*pos), member))

/**
 * nu__List__for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the nu__List__struct within the struct.
 */
#define nu__List__for_each_entry_safe(pos, n, head, member)			\
	for (pos = nu__List__entry((head)->next, typeof(*pos), member),	\
		n = nu__List__entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = nu__List__entry(n->member.next, typeof(*n), member))

/**
 * nu__List__for_each_entry_safe_continue - continue list iteration safe against removal
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the nu__List__struct within the struct.
 *
 * Iterate over list of given type, continuing after current point,
 * safe against removal of list entry.
 */
#define nu__List__for_each_entry_safe_continue(pos, n, head, member) 		\
	for (pos = nu__List__entry(pos->member.next, typeof(*pos), member), 		\
		n = nu__List__entry(pos->member.next, typeof(*pos), member);		\
	     &pos->member != (head);						\
	     pos = n, n = nu__List__entry(n->member.next, typeof(*n), member))

/**
 * nu__List__for_each_entry_safe_from - iterate over list from current point safe against removal
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the nu__List__struct within the struct.
 *
 * Iterate over list of given type from current point, safe against
 * removal of list entry.
 */
#define nu__List__for_each_entry_safe_from(pos, n, head, member) 			\
	for (n = nu__List__entry(pos->member.next, typeof(*pos), member);		\
	     &pos->member != (head);						\
	     pos = n, n = nu__List__entry(n->member.next, typeof(*n), member))

/**
 * nu__List__for_each_entry_safe_reverse - iterate backwards over list safe against removal
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the nu__List__struct within the struct.
 *
 * Iterate backwards over list of given type, safe against removal
 * of list entry.
 */
#define nu__List__for_each_entry_safe_reverse(pos, n, head, member)		\
	for (pos = nu__List__entry((head)->prev, typeof(*pos), member),	\
		n = nu__List__entry(pos->member.prev, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = nu__List__entry(n->member.prev, typeof(*n), member))

/**
 * nu__List__safe_reset_next - reset a stale nu__List__for_each_entry_safe loop
 * @pos:	the loop cursor used in the nu__List__for_each_entry_safe loop
 * @n:		temporary storage used in nu__List__for_each_entry_safe
 * @member:	the name of the nu__List__struct within the struct.
 *
 * nu__List__safe_reset_next is not safe to use in general if the list may be
 * modified concurrently (eg. the lock is dropped in the loop body). An
 * exception to this is if the cursor element (pos) is pinned in the list,
 * and nu__List__safe_reset_next is called after re-taking the lock and before
 * completing the current iteration of the loop body.
 */
#define nu__List__safe_reset_next(pos, n, member)				\
	n = nu__List__entry(pos->member.next, typeof(*pos), member)

#endif

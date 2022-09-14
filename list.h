/* list.h - intrusive linked list for C
 *
 * NOT COMPLETE DON'T USE!!!
 *
 * sleepntsheep 2022
 *
 */

#ifndef SHEEP_LIST_H
#define SHEEP_LIST_H

typedef struct slist {
    struct slist *prev, *next;
} slist;

#define slist_init(ptr) { &ptr, &ptr }

void slist_ins(slist *prev, slist *next, slist *ptr);
void slist_add(slist *head, slist *ptr);

#endif /* SHEEP_LIST_H */

#ifdef SHEEP_LIST_IMPLEMENTATION

void slist_ins(slist *prev, slist *next, slist *ptr) {
    next->prev = ptr;
    prev->next = ptr;
    ptr->prev = prev;
    ptr->next = next;
}

void slist_add(slist *head, slist *ptr) {
    slist_ins(head, head->next, ptr);
}

#define slist_ishead(list, head) \
    ((list) == (head))

#define slist_offset(type, member) \
    (&(((type)*)0 - member))

#define slist_entry(list, type, member) \
    list - slist_offset(type, member)

#define slist_next(list) \
    list = list->next

#define slist_first_entry(list, type, member) \
    slist_entry((list)->next, type, member)

#define slist_entry_ishead(entry, head, member) \
    (&(entry)->member.next == (head))

#define slist_next_entry(entry, member) \
    slist_entry((entry)->member.next, typeof(*(entry)), member)

#define slist_foreach(iter, head) \
    for (slist *iter = (head)->next; !slist_is_head(iter, (head)); slist_next(iter))

#define slist_foreach_entry(iter, head, member) \
    for (slist *iter = slist_first_entry(head);
            !slist_entry_ishead(iter, head, member);
            iter = slist_next_entry(iter, member))


#endif /* SHEEP_LIST_IMPLEMENTATION */

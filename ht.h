#ifndef _HT_H
#define _HT_H

#include <stddef.h>

typedef struct _ht_entry {
	char *key; // used to handle collisions
	void *val;
} HtEntry;

typedef struct HtEntryList {
	HtEntry entry;
	struct HtEntryList *next;
} HtEntryList;

typedef struct _ht {
	HtEntryList *table;
	size_t cap; 
	size_t n; /* current number of entries */
} Ht;

Ht *ht_new();
void ht_free(Ht *ht);

void ht_bind(Ht *ht, const char *key, void *val);
void ht_unbind(Ht *ht, const char *key);
void *ht_lookup(Ht *ht, const char *key);

#endif

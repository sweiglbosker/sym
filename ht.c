#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "ht.h"

#define HT_CAPACITY 512

// TODO use linked list

// prime and offset recommended by http://isthe.com/chongo/tech/comp/fnv/
// for actual impl maybe look at siphash
#define FNV_PRIME 1099511628211UL
#define FNV_OFFSET_BASIS 14695981039346656037UL

#define EMPTY(e) (((HtEntry)e).key == NULL)
#define OCCUPIED(e) (!EMPTY(e))

static uint64_t hash(const char *s);
bool ht_expand(Ht *ht);

static inline bool occupied(HtEntry e) {
	return (e.key != NULL);
}

static uint64_t hash(const char *s) { // (fnv1a)
	uint64_t hash = FNV_OFFSET_BASIS; 
	for (char *c = (char *)s; *c != '\0'; c++) {
		hash ^= (uint64_t)(*c);		
		hash *= FNV_PRIME;
	}
	return hash;
}

Ht *ht_new(void) {
	Ht *ht = calloc(1, sizeof(Ht));

	if (ht == NULL)
		return NULL;

	ht->table = calloc(HT_CAPACITY, sizeof(HtEntryList));

	if (ht->table == NULL) {
		free(ht);
		return NULL;
	}

	ht->cap = HT_CAPACITY;
	ht->n = 0;

	return ht;
}

void ht_free(Ht *ht) {
	for (size_t i = 0; i < ht->cap; i++) {
		HtEntryList *next = &ht->table[i];
		while (next) { // free collisions
			HtEntryList *tmp = next;
			next = next->next;
			if (tmp->entry.key)
				free(tmp->entry.key);
		}
	}
	free(ht->table);
	free(ht);
}

bool ht_expand(Ht *ht) {
	size_t newcap = ht->cap * 2;
	HtEntryList *oldtable = ht->table;

	if (newcap <= ht->cap) // overflow
		return 0;


	void *newtable = calloc(newcap, sizeof(HtEntryList));
	if (!newtable)
		return false;

	ht->table = newtable;
	ht->cap = newcap;

	for (size_t i = 0; i < (ht->cap/2) - 1; i++) {
		HtEntryList el = oldtable[i];
		if (OCCUPIED(el.entry)) {
			ht_bind(ht, el.entry.key, el.entry.val);
		}
	}

	free(oldtable);
	return true;
}

void _ht_bind(Ht *ht, const char *key, void *val) {
	uint64_t hashed_key = hash(key) % (ht->cap - 1);
	size_t keylen = strlen(key);
	
	HtEntryList *i = &ht->table[hashed_key];
	while (i->entry.key) { 
		if (!strcmp(key, i->entry.key))
			break;
		if (i->next == NULL) {
			i->next = calloc(1, sizeof(HtEntryList));
		}
		i = i->next;
	}

	if (i->entry.key == NULL) {
		i->entry.key = calloc(keylen + 1, sizeof(char));
		strcpy(i->entry.key, key);
	}

	i->entry.val = val;
}

void ht_bind(Ht *ht, const char *key, void *val) {
	if (ht->n >= ht->cap / 2) 
		ht_expand(ht);

	_ht_bind(ht, key, val);
}

void ht_unbind(Ht *ht, const char *key) {
	uint64_t hashed_key = hash(key) % (ht->cap - 1);
	bool head = true; // we don't need to free from memory if node is not a collision/head of HtEntryList
	//
	HtEntryList *i = &ht->table[hashed_key];
	HtEntryList *op = i;
	

	while (strcmp(i->entry.key, key)) { // while i->entry.key != key
		head = false; // why does this run every iteration lol
		if (i->next == NULL)
			return; // key doesn't exist
		op = i;
		i = i->next;
	}
	
	i->entry.key = NULL; // why not?
	i->entry.val = NULL; 
	free(i->entry.key);
	op->next = i->next;
	if (!head) 
		free(i);
}

void *ht_lookup(Ht *ht, const char *key) {
	uint64_t hashed_key = hash(key) % (ht->cap - 1); 

	HtEntryList *i = &ht->table[hashed_key];

	while (i->entry.key) {
		if (!strcmp(i->entry.key, key))
			break;
		if (i->next == NULL)
			return NULL; 

		i = i->next;
	}

	return i->entry.val;
}

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

static uint64_t hash(char *s);
bool ht_expand(Ht *ht);

static inline bool occupied(HtEntry e) {
	return (e.key != NULL);
}

static uint64_t hash(char *s) { // (fnv1a)
	uint64_t hash = FNV_OFFSET_BASIS; 
	for (char *c = s; *c != '\0'; c++) {
		hash ^= (uint64_t)(*c);		
		hash *= FNV_PRIME;
	}
	return hash;
}

Ht *ht_new(void) {
	Ht *ht = calloc(1, sizeof(Ht));

	if (ht == NULL)
		return NULL;

	ht->table = calloc(HT_CAPACITY, sizeof(struct _ht_entry) * HT_CAPACITY);

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
		free(ht->table[i].key);
	}
	free(ht->table);
	free(ht);
}

bool ht_expand(Ht *ht) {
	size_t newcap = ht->cap * 2;
	HtEntry *oldtable = ht->table;

	if (newcap <= ht->cap) // overflow
		return 0;


	void *newtable = calloc(newcap, sizeof(struct _ht_entry));
	if (newtable)
		return false;

	ht->table = newtable;
	ht->cap = newcap;

	for (size_t i = 0; i < (ht->cap/2) - 1; i++) {
		HtEntry entry = oldtable[i];
		if (OCCUPIED(entry)) {
			ht_bind(ht, entry.key, entry.val);
			oldtable[i].key = 0;
			oldtable[i].val = 0;
		}
	}

	free(oldtable);
	return true;
}

void _ht_bind(Ht *ht, char *key, void *val) {
	uint64_t hashed_key = hash(key) % (ht->cap - 1);
	
	while (ht->table[hashed_key].key) {
		if (!strcmp(key, ht->table[hashed_key].key)) // key already exists in the table but this is the correct entry to store
			break;
		if (++hashed_key >= ht->cap) 
			hashed_key = 0;
	}
	ht->table[hashed_key].key = key;
	ht->table[hashed_key].val = val;
}

void ht_bind(Ht *ht, char *key, void *val) {
	if (ht->n >= ht->cap / 2) 
		ht_expand(ht);

	_ht_bind(ht, key, val);
}

void *ht_lookup(Ht *ht, char *key) {
	// s
	uint64_t hashed_key = hash(key) % (ht->cap - 1); 
	
	// TODO: this will hang if every entry in ht is occupied (i think) 
	while (ht->table[hashed_key].key) {
		if (!strcmp(key, ht->table[hashed_key].key)) 
			return ht->table[hashed_key].val;
		if (++hashed_key >= ht->cap)
			hashed_key = 0;
	}
	return NULL;
}

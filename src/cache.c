#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "cache.h"

/**
 * Allocate a cache entry
 */
struct cache_entry *alloc_entry(char *path, char *content_type, void *content, int content_length)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    // make some memory for new struct
    struct cache_entry *cache_entry = malloc(sizeof cache_entry);
    // take care of the path
    // let's just strdup ALL THE THINGS
    (*cache_entry).path = strdup(path);
    (*cache_entry).content_type = strdup(content_type);
    (*cache_entry).content = strdup(content);
    (*cache_entry).content_length = content_length;
}

/**
 * Deallocate a cache entry
 */
void free_entry(struct cache_entry *entry)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
}

/**
 * Insert a cache entry at the head of the linked list
 */
void dllist_insert_head(struct cache *cache, struct cache_entry *ce)
{
    // Insert at the head of the list
    if (cache->head == NULL)
    {
        cache->head = cache->tail = ce;
        ce->prev = ce->next = NULL;
    }
    else
    {
        cache->head->prev = ce;
        ce->next = cache->head;
        ce->prev = NULL;
        cache->head = ce;
    }
}

/**
 * Move a cache entry to the head of the list
 */
void dllist_move_to_head(struct cache *cache, struct cache_entry *ce)
{
    if (ce != cache->head)
    {
        if (ce == cache->tail)
        {
            // We're the tail
            cache->tail = ce->prev;
            cache->tail->next = NULL;
        }
        else
        {
            // We're neither the head nor the tail
            ce->prev->next = ce->next;
            ce->next->prev = ce->prev;
        }

        ce->next = cache->head;
        cache->head->prev = ce;
        ce->prev = NULL;
        cache->head = ce;
    }
}

/**
 * Removes the tail from the list and returns it
 * 
 * NOTE: does not deallocate the tail
 */
struct cache_entry *dllist_remove_tail(struct cache *cache)
{
    struct cache_entry *oldtail = cache->tail;

    cache->tail = oldtail->prev;
    cache->tail->next = NULL;

    cache->cur_size--;

    return oldtail;
}

/**
 * Create a new cache
 * 
 * max_size: maximum number of entries in the cache
 * hashsize: hashtable size (0 for default)
 */
struct cache *cache_create(int max_size, int hashsize)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // instantiate hash table
    // instantiate linked_lists
    // instantiate cache_size
}

void cache_free(struct cache *cache)
{
    struct cache_entry *cur_entry = cache->head;

    hashtable_destroy(cache->index);

    while (cur_entry != NULL)
    {
        struct cache_entry *next_entry = cur_entry->next;

        free_entry(cur_entry);

        cur_entry = next_entry;
    }

    free(cache);
}

/**
 * Store an entry in the cache
 *
 * This will also remove the least-recently-used items as necessary.
 * 
 * NOTE: doesn't check for duplicate cache entries
 */
void cache_put(struct cache *cache, char *path, char *content_type, void *content, int content_length)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
    // 1) Allocate a new cache entry with passed parameters
    // Meaning: I need space for the cache_entry, so we alloc space for it
    struct cache_entry *cache_entry = alloc_entry(path, content_type, content, content_length);
    // 2) Insert the entry at the head of the doubly-linked list
    // Meaning: dllist_insert_head(), add this cache_entry at its head
    dllist_insert_head(cache, cache_entry);
    // store entry in hashtable as well, indexed by the entry's path
    hashtable_put(cache->index, path, cache_entry);
    // Increment the current size of the cache.
    cache->cur_size++;

    // If the cache size is greater than the max size:
    if (cache->cur_size > cache->max_size)
    {
        // remove the tail, as that is literally the LRU
        dll_remove_tail(cache);
        // remove the same entry from the hash table
        hashtable_delete(cache->index, path);
        // free the cache entry
        free_entry(cache_entry);
    }
}

/**
 * Retrieve an entry from the cache
 */
struct cache_entry *cache_get(struct cache *cache, char *path)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // attempt to find the cache entry pointer by path in the hashtable
    if (hastable_get(cache->index, path) == NULL)
    {
        return NULL;
    }

    // move the cache entry to the head of the double-linked list
    // BECAUSE ITS NOW THE MOST RECENTLY ACCESSED ENTRY
    // so the key is literally an endpoint, like cat.jpg, and we look for it in the hashtable
    dllist_move_to_head(cache, hastable_get(cache->index, path));
    // return cache entry pointer
    // printf("the cache->head is: %s", cache->head);
    return cache->head;
}

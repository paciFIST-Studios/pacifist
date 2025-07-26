// paciFIST studios. 2025. MIT License

#ifndef PF_ALLOCATOR_H
#define PF_ALLOCATOR_H
 
/**
 * PFAllocator is meant to be a general purpose allocator, which
 * seems to replace the normal calls to malloc, realloc, and free
 * While acting similarly to those fns, PFAllocator has several
 * specific features which set it apart:
 *
 *    1. it uses memory for the application arena
 *    2. it supports memory compaction
 *    3. it allows access to memory via handles, not pointers
 *        a. the handles can return pointers
 *    4. it uses specific instrumentation, to deliver detailed
 *        information about existing allocations
 *    5. (GC?)
 *
 * it probably won't be a perfect match, but I'm going to try to have
 * these fns work the same
 * 
 * 
 * 
 * https://www.gingerbill.org/series/memory-allocation-strategies/ 
 * 
 * 
 * 
 * MemoryHandle
 *     void * memory
 *     GCHandle * gc_ptr
 *     int32_t flags 
 * 
 */

// sets memory to zero before returning it
// returns ptr to "new" memory, which is aligned correctly
// does zero out memory
// will accomodate several size regimes,
// will return null for out of memory error
// will set error message for out of memory error
void* pfmalloc(size_t const size);

// changes the size of this memory to the supplied size
// does zero out memory which is being released (but you don't know that)
// does zero out expanded memory
// returns pointer to new memory, which is aligned correctly
// if ptr is null, is like pfmalloc
// will accomodate several size regimes,
// will return null for out of memory error
// will set error message for out of memory error
void * pfrealloc(void* ptr, size_t const size);


// "frees" memory pointed to by this ptr.
// sets ptr to null 
void pffree(void* ptr);


void pf_compact_memory();


int32_t pf_requires_new_ptr(/* handle */);



#endif //PF_ALLOCATOR_H

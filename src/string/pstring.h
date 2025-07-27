// paciFIST studios. 2025. MIT License

#ifndef PSTRING_H
#define PSTRING_H

// stdlib
#include <stddef.h>
#include <stdint.h>
// framework
// engine

#define PSTR_LAST 0

// 120250727 - struct is 16 bytes right now
typedef struct PString_t {
    char * string;
    size_t length;
} PString_t;






void pstring_set_allocator(void* allocator);



/**
 * @brief Checks to see if A contains substring.  Does not check if substring contains A.
 *
 * @param a 
 * @param substring 
 * @param length 
 * @return
 *      1: A contains substring
 *      0: A does not contain substring
 */
int32_t pstring_contains_char_sub(PString_t const a, char const * substring, size_t const length);


/**
 * @brief Checks to see if A contains B.  Does not check if B contains A.
 *
 * @param a 
 * @param b 
 * @return
 *      1: A contains B
 *      0: A does not contain B
 */
int32_t pstring_contains_pstr_sub(PString_t const a, PString_t const b);


/**
 * @brief Gets a slice of the provided string, like in python.  You can use negative indices
 *
 * @param pstr 
 * @param begin 
 * @param end
 * @return 
 */
PString_t pstring_slice(PString_t const pstr, int32_t const begin, int32_t const end);


/**
 * @brief Stores copy of given string in allocated memory, returns owning PString_t
 * 
 * @param pstr 
 * @param i 
 * @param j 
 * @param count 
 * @return 
 */
PString_t pstring_copy_allocate(PString_t const pstr, int32_t const i, int32_t const j, int32_t const count);

/**
 * @brief reverses the supplied string, in the memory it already occupies
 *
 * @param pstr 
 */
void pstring_reverse_in_place(PString_t * pstr);


/**
 * @brief like strcmp, but uses PString_t
 *
 * @param a 
 * @param b 
 * @return 
 */
int32_t pstring_compare(PString_t const a, PString_t const b);

/**
 * @brief tokenizes the supplied PString_t, based on the delimiter.  Allocates space for PString_t to hold tokens, but does not copy or change memory:w
 * 
 * 
 * @param pstr 
 * @param delimiter 
 * @param length 
 * @param out_array 
 * @param out_array_length 
 */
void pstring_tokenize_allocate(PString_t const pstr, char const * delimiter, size_t const length, PString_t* out_array, size_t out_array_length);


void pstring_findall_allocate(PString_t const a, char  const * substring, size_t const length, int32_t* out_array, size_t out_array_length);

/**
 * @brief returns a pstring, containing the first patching substring found in pstr:w
 * 
 *
 * @param pstr 
 * @param substring 
 * @param length 
 * @return 
 */
PString_t pstring_find_first(PString_t const pstr, char const * substring, size_t const length);

/**
 * @brief returns a pstring, containing the last matching substring found in pstr
 *
 * @param pstr 
 * @param substring 
 * @param length 
 * @return 
 */
PString_t pstring_find_last(PString_t const pstr, char const * substring, size_t const length);



#endif //PSTRING_H

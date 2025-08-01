// paciFIST studios. 2025. MIT License

// include
#include "pstring.h"

// stdlib
#include <stdlib.h>
// framework
// engine
#include "../core/common.h"
#include "../core/error.h"
// game



int32_t pf_pstring_contains_char_sub(PString_t const a, char const * substring, size_t const length){
    if (length > a.length) {
        PF_LOG_ERROR(PF_STRING, "Searching for substring whose length is longer than the base string!");
        return FALSE;
    }

    int32_t failed_match_j_index = 0;

    int32_t const a_last_index = (int32_t)(a.length - 1);
    int32_t const b_last_index = (int32_t)(length - 1);

    // loop backwards through the base string
    int32_t i = 0;
    for (i = a_last_index; i >= 0; i--) {
        failed_match_j_index = 0;

        // loop backwards through the comparator string
        for (int32_t j = b_last_index; j >= 0; j--) {

            // the count_back_idx, allows us to search the base string
            // from right to left, from inside the comparator loop,
            // without disrupting the base string loop
            int32_t const count_back_idx = (b_last_index - j);

            // if these are different, break
            if (substring[j] != a.string[i-count_back_idx]) {
                failed_match_j_index = j;
                break;
            }

            // we've just compared every character of the comparator string
            // to some characters in the base string.  Upon reaching idx 0
            // in the comparator string without breaking, we know we've found
            // equal characters for each character in the comparator string
            // this is a matching substring.  Unclear if there are more
            if (j == 0) {
                return TRUE;
            }
        }

        // skip over these indices, so we don't re-check them
        int32_t const indices_to_skip = (b_last_index - failed_match_j_index);
        // if we need to skip 1 index, still don't, b/c the outer loop goes
        // by 1s anyway
        if (indices_to_skip > 1) {
            i -= indices_to_skip;
        }
    }

    return FALSE;
}



int32_t pf_pstring_contains_pstr_sub(PString_t const a, PString_t const b) {
    return pf_pstring_contains_char_sub(a, b.string, b.length);
}


PString_t pf_pstring_slice(PString_t const pstr, int32_t const begin, int32_t const end) {
    PString_t result = { .string = NULL, .length = 0 };

    if (pstr.string == NULL) {
        PF_LOG_ERROR(PF_STRING, "Null pointer passed to base string!");
        return result;
    }
    if (pstr.length == 0) {
        PF_LOG_ERROR(PF_STRING, "Invalid string length!");
        return result;
    }
    if (begin == end) {
        PF_LOG_ERROR(PF_STRING, "Cannot create a zero-length slice!");
        return result;
    }

    if (abs(begin) > pstr.length) {
        PF_LOG_ERROR(PF_STRING, "Begin parameter wants out of bounds access to string!");
        return result;
    }
    if (abs(end) > pstr.length) {
        PF_LOG_ERROR(PF_STRING, "End parameter wants out of bounds access to string!");
        return result;
    }

    // WARNING: Zero is a positive number in the "begin" parameter,
    //          but it's a negative number in the "end" parameter,
    //          and in the end parameter, it's used to mean "last idx"
    int32_t const bBeginIsPositive = begin >= 0;
    int32_t const bEndIsPositive = end > 0;

    char* pBegin = NULL;
    char* pEnd = NULL;

    if (bBeginIsPositive) {
        pBegin = (void*)(pstr.string + begin);
    } else {
        size_t const offset = pstr.length + begin;
        pBegin = (void*)(pstr.string + offset);
    }

    if (bEndIsPositive) {
         pEnd = (void*)(pstr.string + end);
     } else {
         size_t const offset = pstr.length + end;
         pEnd = (void*)(pstr.string + offset);
     }

    if (pBegin == NULL || pEnd == NULL) {
        char message[100];
        for (size_t i = 0; i < 100; i++) { message[i] = 0; }
        sprintf(message, "Somehow, attempted work on null ptr for pBegin=%p, pEnd=%p", pBegin, pEnd);
        PF_LOG_ERROR(PF_STRING, message);
        return result;
    }

    char * pFirst = NULL;
    char const * pLast = NULL;
    if (pBegin < pEnd) {
        pFirst = pBegin;
        pLast = pEnd;
    } else {
        pFirst = pEnd;
        pLast = pBegin;
    }

    size_t const length = pLast - pFirst;

    if (length == 0) {
        PF_LOG_ERROR(PF_STRING, "Somehow, attempted work on zero-length slice!");
        return result;
    }

    result.string = pFirst;
    result.length = length;

    return result;
}


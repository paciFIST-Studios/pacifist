// paciFIST studios. 2025. MIT License

// include
#include "pstring.h"

// stdlib
// framework
// engine
#include "../core/common.h"
// game





int32_t pstring_contains_substring(PString_t const a, PString_t const b) {
    if (b.length > a.length) {
        return FALSE;
    }    

    int32_t failed_match_j_index = 0;

    int32_t const a_last_index = (int32_t)(a.length - 1);
    int32_t const b_last_index = (int32_t)(b.length - 1);
    
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
            if (b.string[j] != a.string[i-count_back_idx]) {
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


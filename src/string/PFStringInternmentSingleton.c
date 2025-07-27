// paciFIST studios. 2025. MIT License.

// includes
#include "PFStringInternmentSingleton.h"

// stdlib
// framework
// engine
#include "../core/error.h"


int32_t pf_string_internment_initialize(PFStringInternmentSingleton_t * string_internment, void * memory_base, size_t const memory_size) {
    if (string_internment == NULL) {
        PF_LOG_ERROR(PF_STRING, "Null ptr to PFStringInternmentSingleton");
        return PFEC_ERROR_NULL_PTR;
    }
    if (memory_base == NULL) {
        PF_LOG_ERROR(PF_STRING, "Null ptr to PFStringInternmentSingleton memory base");
        return PFEC_ERROR_NULL_PTR;
    }
    if (memory_size == 0) {
        PF_LOG_ERROR(PF_STRING, "Invalid memory size for PFStringInternmentSingleton");
        return PFEC_ERROR_INVALID_LENGTH;
    }

    string_internment->usable_memory_base = memory_base;
    string_internment->owned_memory_size = memory_size;
    string_internment->used_memory_size = 0;
    string_internment->next_unused_idx = 0;

    for (size_t i = 0; i < PFSI_MAX_STRINGS; i++) {
        string_internment->strings[i].string = NULL;
        string_internment->strings[i].length = 0;
    }

    return PFEC_NO_ERROR;
}


int32_t pf_string_internment_emplace_cstring(char const * cstring, size_t const length) {
    if (cstring == NULL) {
        PF_LOG_ERROR(PF_STRING, "Null ptr to cstring!");
        return PFEC_ERROR_NULL_PTR;
    }
    return PFEC_NO_ERROR;
}


int32_t pf_string_internment_emplace_pstring(PString_t const pstring) {
    if (pstring.string == NULL) {
        PF_LOG_ERROR(PF_STRING, "Null ptr to base string!");
        return PFEC_ERROR_NULL_PTR;
    }
    
    return PFEC_NO_ERROR;
}



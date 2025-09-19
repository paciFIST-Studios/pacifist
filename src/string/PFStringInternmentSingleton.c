// paciFIST studios. 2025. MIT License.

// includes
#include "PFStringInternmentSingleton.h"

// stdlib
// framework
// engine

#include "core/error.h"


static PString_t error_pstr = { .string = NULL, .length = 0 };


int32_t pf_string_internment_initialize(
    PFStringInternmentSingleton_t * string_internment,
    void * memory_base,
    size_t const memory_size)
{
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


PString_t pf_string_internment_emplace_cstr(
    PFStringInternmentSingleton_t * string_internment,
    char * cstring,
    size_t const length)
{
    // PFStringInternmentSingleton_t* arg
    if (string_internment == NULL) {
        PF_LOG_ERROR(PF_STRING, "Null ptr to PFStringInternmentSingleton");
        error_pstr.length = PFEC_ERROR_NULL_PTR;
        return error_pstr;
    }
    if (string_internment->usable_memory_base == NULL) {
        PF_LOG_ERROR(PF_STRING, "Null ptr to PFStringInternmentSingleton usable memory base");
        error_pstr.length = PFEC_ERROR_NULL_PTR;
        return error_pstr;
    }
    if (string_internment->owned_memory_size == 0) {
        PF_LOG_ERROR(PF_STRING, "PFStringInternmentSingleton invalid owned memory size");
        error_pstr.length = PFEC_ERROR_INVALID_LENGTH;
        return error_pstr;
    }
    if (string_internment->used_memory_size > string_internment->owned_memory_size) {
        PF_LOG_CRITICAL(PF_STRING, "PFStringInternmentSingleton is using memory it doesn't own");
        error_pstr.length = PFEC_ERROR_OUT_OF_BOUNDS_MEMORY_USE;
        return error_pstr;
    }

    // char * cstr arg
    if (cstring == NULL) {
        PF_LOG_ERROR(PF_STRING, "Null ptr to cstring");
        error_pstr.length = PFEC_ERROR_NULL_PTR;
        return error_pstr;
    }
    // size_t const len arg
    if (length == 0) {
        PF_LOG_ERROR(PF_STRING, "Invalid string length");
        error_pstr.length = PFEC_ERROR_INVALID_LENGTH;
        return error_pstr;
    }

    size_t const remaining_bytes = string_internment->owned_memory_size - string_internment->used_memory_size;
    if (remaining_bytes <= length) {
        PF_LOG_CRITICAL(PF_STRING, "Ran out of memory for PFStringInternmentSingleton");
        error_pstr.length = PFEC_ERROR_OUT_OF_MEMORY;
        return error_pstr;
    }

    size_t const idx = string_internment->next_unused_idx;
    if (idx >= PFSI_MAX_STRINGS) {
        PF_LOG_CRITICAL(PF_STRING, "Ran out of tracking indices for PFStringInternmentSingleton");
        error_pstr.length = PFEC_ERROR_OUT_OF_MEMORY;
        return error_pstr;
    }

    int64_t const base = (int64_t)string_internment->usable_memory_base;
    int64_t const used = (int64_t)string_internment->used_memory_size;
    char* next_start_ptr = (void*)(base + used);

    strncpy(next_start_ptr, cstring, length);

    next_start_ptr[length] = 0;
    // add 1, b/c we also added a nul-terminator
    string_internment->used_memory_size += length + 1;

    PString_t* const next_pstring = &string_internment->strings[idx];
    next_pstring->string = next_start_ptr;
    next_pstring->length = length;

    string_internment->next_unused_idx++;

    return *next_pstring;
}


PString_t pf_string_internment_emplace_pstr(PFStringInternmentSingleton_t * string_internment, PString_t const pstring) {
    return pf_string_internment_emplace_cstr(string_internment, pstring.string, pstring.length);
}



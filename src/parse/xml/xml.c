// paciFIST studios. 2025. MIT License

// header
#include <parse/xml/xml.h>

// stdlib
// framework
// engine
#include <core/error.h>


int32_t pf_load_xml(PString_t const path, PFAllocator_FreeList_t const * allocator, PFXmlDocument_t* out_xml_doc) {
    if (path.string == NULL) {
        PF_LOG_ERROR(PF_PARSE, "Null ptr to path.string!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (path.length == 0) {
        PF_LOG_ERROR(PF_PARSE, "Zero length path.length!");
        return PFEC_ERROR_INVALID_LENGTH;
    }
    if (allocator == NULL) {
        PF_LOG_ERROR(PF_PARSE, "Null ptr to PFAllocator_FreeList_t!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (out_xml_doc == NULL) {
        PF_LOG_ERROR(PF_PARSE, "Null ptr to PFXmlDocument_t!");
        return PFEC_ERROR_NULL_PTR;
    }


    
    return PFEC_NO_ERROR;
}

// paciFIST studios. 2025. MIT License

// header
#include <parse/xml/xml.h>

// stdlib
// framework
// engine
#include <core/error.h>
#include <os/path.h>


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

    if (!pf_os_path_is_file_pstr_linux(path)) {
        PF_LOG_ERROR(PF_PARSE, "Got invalid path to xml file!");
        return PFEC_ERROR_FILE_DOES_NOT_EXIST;
    }

    
    return PFEC_NO_ERROR;
}

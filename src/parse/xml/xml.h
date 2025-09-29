// paciFIST studios. 2025. MIT License


#ifndef PF_XML_H
#define PF_XML_H

// stdlib
// framework
// engine
#include <memory/allocator/PFAllocator.h>
#include <string/pstring.h>




typedef struct PFXmlDocument_t{
    char* path;

} PFXmlDocument_t;


/*
 *
 **/
int32_t pf_load_xml(PString_t path, PFAllocator_FreeList_t const * allocator, PFXmlDocument_t* out_xml_doc);


//int32_t pf_free_xml_doc(PFAllocator_FreeList_t* allocator, PFXmlDocument_t* xml_doc);



#endif //PF_XML_H
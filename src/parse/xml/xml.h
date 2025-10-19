// paciFIST studios. 2025. MIT License

#ifndef PF_XML_H
#define PF_XML_H

// stdlib
// framework
// engine
#include <memory/allocator/PFAllocator_FreeList.h>
#include <parse/xml/pf_xml_structs.h>
#include <string/pstring.h>


/**
 * @brief 
 * 
 * @param allocator 
 * @param path 
 * @param out_xml_doc 
 * @return 
 */
int32_t pf_load_xml(
    PFAllocator_FreeList_t* allocator,
    PString_t path,
    PFXmlDocument_t* out_xml_doc);

/**
 * @brief
 * 
 * @param allocator 
 * @param xml_doc 
 * @return 
 */
int32_t pf_free_xml_doc(
    PFAllocator_FreeList_t* allocator,
    PFXmlDocument_t* xml_doc);

/**
 * @brief
 * 
 * @param allocator
 * @param parent
 * @return 
 */
PFXmlNode_t* pf_xml_node_create_with_memory(
    PFAllocator_FreeList_t* allocator,
    PFXmlNode_t* parent);

/**
 * @brief
 * 
 * @param allocator 
 * @param node 
 */
void pf_xml_node_free(
    PFAllocator_FreeList_t* allocator,
    PFXmlNode_t* node);



#endif //PF_XML_H
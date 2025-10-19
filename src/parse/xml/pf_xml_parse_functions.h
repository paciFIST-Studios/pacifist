// paciFIST studios. 2025. MIT License

#ifndef PF_XML_PARSE_H
#define PF_XML_PARSE_H

// stdlib
#include <stdint.h>
// framework
// engine
#include <memory/allocator/PFAllocator_FreeList.h>
#include <parse/xml/pf_xml_structs.h>
#include <string/pstring.h>

/**
 * @brief
 * 
 * @param allocator 
 * @param xml_pstr 
 * @param in_out_xml_doc - this should be set up w/ a root node 
 */
int32_t pf_parse_xml_string_v01(
    PFAllocator_FreeList_t* allocator,
    PString_t xml_pstr,
    PFXmlDocument_t* in_out_xml_doc);

int32_t pf_parse_xml_string_for_xml_header_v01(
    PFAllocator_FreeList_t* allocator,
    PString_t xml_header_pstr,
    void* xml_header_data);

int32_t pf_xml_parse_load_file_tag_v01(
    PFAllocator_FreeList_t* allocator,
    PString_t xml_pstr,
    PFXmlNode_LoadFile_t* out_load_file_node);

int32_t pf_xml_parse_load_unit_tag_v01(
    PFAllocator_FreeList_t* allocator,
    PString_t xml_pstr,
    PFXmlNode_LoadUnit_t* out_load_unit_node);

// load units manifest?

int32_t pf_xml_parse_project_file_manifest_tag_v01(
    PFAllocator_FreeList_t* allocator,
    PString_t xml_pstr,
    PFXmlNode_ProjectFileManifest_t* out_project_file_manifest_node);

int32_t pf_xml_parse_uses_editor_version_tag_v01(
    PFAllocator_FreeList_t* allocator,
    PString_t xml_pstr,
    PFXmlNode_EditorVersion_t* out_editor_version_node);

int32_t pf_xml_parse_uses_engine_version_tag_v01(
    PFAllocator_FreeList_t* allocator,
    PString_t xml_pstr,
    PFXmlNode_EngineVersion_t* out_engine_version_node);

int32_t pf_xml_parse_project_budgets_tag_v01(
    PFAllocator_FreeList_t* allocator,
    PString_t xml_pstr,
    PFXmlNode_ProjectBudgets_t* out_project_budgets_node);

int32_t pf_xml_parse_user_metadata_tag_v01(
    PFAllocator_FreeList_t* allocator,
    PString_t xml_pstr,
    PFXmlNode_UserMetaData_t* out_user_metadata_node);

int32_t pf_xml_parse_project_metadata_tag_v01(
    PFAllocator_FreeList_t* allocator,
    PString_t xml_pstr,
    PFXmlNode_ProjectMetaData_t* out_project_metadata_node);

int32_t pf_xml_parse_gay_tag_v01(
    PFAllocator_FreeList_t* allocator,
    PString_t xml_pstr,
    int32_t* out_bool_value);

int32_t pf_xml_parse_project_tag_v01(
    PFAllocator_FreeList_t* allocator,
    PString_t xml_pstr,
    void* out_project_node);

#endif // PF_XML_PARSE_H


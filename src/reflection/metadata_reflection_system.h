// paciFIST studios. 2025.

/** MetaData Reflection System
 * https://youtu.be/DFRtNy6wdYY
 *
 *  This system, based on the youtube link by John Jackson,
 *  to the video "Writing a Poor Man's Reflection System in C99",
 *  exists to look up and deliver:
 *      * contents of any given struct
 *         * types, names, and values for every property in a struct
 * 
 */

// stdlib
// framework
// project
#include "../core/common.h"


#ifndef METADATA_REFLECTION_SYSTEM_H
#define METADATA_REFLECTION_SYSTEM_H

// TODO: have a python script generate this
typedef enum EMetaDataPropertyType_t {

    MetaDataPropertyType__uint8 = 0,
    MetaDataPropertyType__uint16,
    MetaDataPropertyType__uint32,
    MetaDataPropertyType__uint64,
    MetaDataPropertyType__int8,
    MetaDataPropertyType__int16,
    MetaDataPropertyType__int32,
    MetaDataPropertyType__int64,
    MetaDataPropertyType__real32,
    MetaDataPropertyType__real64,
    MetaDataPropertyType__bool32,
    MetaDataPropertyType__sizet,
    MetaDataPropertyType__voidp,
    MetaDataPropertyType__int32p,
    MetaDataPropertyType__real32p,
    MetaDataPropertyType__charp,
    MetaDataPropertyType__COUNT
    
} EMetaDataPropertyType_t;

// holds information about the typing of a specific property,
// so it's possible to interpret
typedef struct MetaDataPropertyTypeInfo_t {
    char* type_name;
    EMetaDataPropertyType_t type_type;
} MetaDataPropertyTypeInfo_t;

// holds information about some specific property
typedef struct MetaDataProperty_t {
    char const * property_name;
    MemoryIndex_t property_offset; // used to get value
    MetaDataPropertyTypeInfo_t property_type; 
} MetaDataProperty_t;

// holds information about some specific type of struct
// it's not really a class, but the word feels helpful here
typedef struct MetaDataClass_t {
    char const * class_name;
    uint32 property_count;
    MetaDataPropertyTypeInfo_t* property_list;
} MetaDataClass_t;

// holds information about all the specific MetaDataClasses which
// are registered with the reflection system
typedef struct MetaDataRegistry_t {
    struct {uint64_t key; MetaDataClass_t value;}* registered_classes;
    
} MetaDataRegistry_t;









typedef enum EMetaDataReflectionTestEnum1 {
    UNDEFINED = 0,
    EImportantValue1,
    EImportantValue2,
    ENotSoImportantValue,
    COUNT,
    
} EMetaDataReflectionTestEnum1;

typedef struct MetaDataReflectionTestStruct2_t {
    uint8 r;
    uint8 g;
    uint8 b;
    uint8 a;
} MetaDataReflectionTestStruct2;

typedef struct MetaDataReflectionTestStruct1_t {
    float* position;
    float* rotation;
    float* scale;

    // holds a single pixel of color information
    struct MetaDataReflectionTestStruct2_t color;

    int32 speed;
    int32 health;
    int32 magic;
    
    char* significant_name;
    char* significant_description;
    EMetaDataReflectionTestEnum1 value;

    struct MetaDataReflectionTestStruct1_t* pNext;
    struct MetaDataReflectionTestStruct1_t* pLast;
} MetaDataReflectionTestStruct1;



/** MetaData Registry.  A dictionary (associative array)
 *      collection of metadata classes:
 *          collection of metadata properties:
 *              metadata_info_type
 *                  property_name
 *                  property_type  <-- EMetaDataPropertyType
 *                  property_value
 *                  property_size
 *                  property_address
 *
 *
 * 
 */








/**
 * @param object 
 */
void print_metadata_for_object(void* object) {
    // for property : properties
    //  print to console
    //  show in imgui
}


int32 serialize_metadata_to_json(void* object, char* out_buffer, size_t out_buffer_size) {
    
    
    // for property : properties
    //      write to string
    //
    // copy to out_string, if it's a valid ptr
    // return length of string
    
    
    return 0; 
}


#endif //METADATA_REFLECTION_SYSTEM_H

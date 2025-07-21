// paciFIST studios. 2025. MIT License

#ifndef CONFIG_FILE_PARSER_H
#define CONFIG_FILE_PARSER_H

#include "../core/WindowConfigStruct.h"

// fn: read text file as line

static bool32 parse_configuration_file(char const * filePath, struct PacifistWindowConfig_t * outConfig) {
    // nullify this ptr first off
    outConfig = NULL;

    struct PacifistWindowConfig_t result = {0};

    if (filePath == NULL) {
        return FALSE;
    }

    

    
    FILE* fd = fopen(filePath, "r");

    
    
    return TRUE;
}

// tests: parse_configuration_file
//      returns correct type
//      takes correct arguments
//      handles invalid args correctly
//      handles invalid path correctly
//      handles can't read, no rights correctly
//      handles no-config-data-found-in-file correctly
//      correctly parses correct config file
//      handles config-file-with-malformed-data correctly
//      handles config-file-with-only-some-data-but-its-valid-data correctly


#endif //CONFIG_FILE_PARSER_H

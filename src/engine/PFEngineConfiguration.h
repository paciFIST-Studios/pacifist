// paciFIST studios. 2025. MIT License

#ifndef PF_ENGINE_CONFIGURATION_H
#define PF_ENGINE_CONFIGURATION_H

// stdlib
// framework
// engine
#include <string/pstring.h>


typedef struct PFEngineConfiguration_t {
    PString_t program_execution_path;
    PString_t project_config_file_path;
    PString_t engine_base_path;


} PFEngineConfiguration_t;


#endif // PF_ENGINE_CONFIGURATION_H



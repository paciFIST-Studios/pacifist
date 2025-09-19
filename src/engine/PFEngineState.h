// paciFIST studios. 2025. MIT License

#ifndef PF_ENGINE_STATE_H
#define PF_ENGINE_STATE_H

// stdlib
// framework
// engine
#include "string/PFStringInternmentSingleton.h"
#include "core/error.h"


typedef struct PFEngineState_t {
    PFStringInternmentSingleton_t m_string_internment;

} PFEngineState_t;





int32_t pf_engine_state_initialize(PFEngineState_t * engine_state);

int32_t pf_engine_state_destroy(PFEngineState_t * engine_state);

int32_t pf_engine_state_save(PFEngineState_t const * engine_state);

int32_t pf_engine_state_load(PFEngineState_t * engine_state);

#endif //PF_ENGINE_STATE_H

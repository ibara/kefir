#ifndef KEFIR_CODEGEN_AMD64_SYSTEM_V_ABI_ALLOCATION_H_
#define KEFIR_CODEGEN_AMD64_SYSTEM_V_ABI_ALLOCATION_H_

#include <stdbool.h>
#include "kefir/core/basic-types.h"
#include "kefir/core/vector.h"
#include "kefir/core/mem.h"
#include "kefir/codegen/amd64/system-v/abi_data.h"
#include "kefir/codegen/amd64/system-v/abi_qwords.h"

typedef enum kefir_amd64_sysv_input_parameter_type {
    KEFIR_AMD64_SYSV_INPUT_PARAM_IMMEDIATE,
    KEFIR_AMD64_SYSV_INPUT_PARAM_NESTED,
    KEFIR_AMD64_SYSV_INPUT_PARAM_OWNING_CONTAINER,
    KEFIR_AMD64_SYSV_INPUT_PARAM_CONTAINER,
    KEFIR_AMD64_SYSV_INPUT_PARAM_SKIP
} kefir_amd64_sysv_input_parameter_type_t;

#define KEFIR_AMD64_SYSV_INPUT_PARAMETER_NONE KEFIR_SIZE_MAX

typedef struct kefir_amd64_sysv_input_parameter_location {
    kefir_size_t integer;
    kefir_size_t sse;
    kefir_size_t sseup;
    kefir_size_t memory;
} kefir_amd64_sysv_input_parameter_location_t;

typedef struct kefir_amd64_sysv_input_parameter_allocation {
    kefir_amd64_sysv_input_parameter_type_t type;
    kefir_amd64_sysv_data_class_t klass;
    kefir_size_t index;
    union {
        struct kefir_amd64_sysv_abi_qwords container;
        struct kefir_amd64_sysv_abi_qword_ref container_reference;
    };
    struct {
        kefir_size_t integer;
        struct {
            kefir_size_t sse;
            kefir_size_t sseup;
        };
        struct {
            kefir_size_t size;
            kefir_size_t alignment;
        } memory;
    } requirements;

    struct kefir_amd64_sysv_input_parameter_location location;
} kefir_amd64_sysv_input_parameter_allocation_t;

kefir_result_t kefir_amd64_sysv_input_parameter_classify(struct kefir_mem *,
                                                     const struct kefir_ir_type *,
                                                     const struct kefir_vector *,
                                                     struct kefir_vector *);

kefir_result_t kefir_amd64_sysv_input_parameter_free(struct kefir_mem *,
                                                 struct kefir_vector *);


kefir_result_t kefir_amd64_sysv_input_parameter_allocate(const struct kefir_ir_type *,
                                                     const struct kefir_vector *,
                                                     struct kefir_vector *);

#endif
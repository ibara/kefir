#ifndef KEFIR_CODEGEN_AMD64_SYSTEM_V_ABI_DATA_H_
#define KEFIR_CODEGEN_AMD64_SYSTEM_V_ABI_DATA_H_

#include "kefir/core/basic-types.h"
#include "kefir/core/util.h"
#include "kefir/core/vector.h"
#include "kefir/codegen/amd64-sysv.h"
#include "kefir/ir/type.h"

extern const char *KEFIR_AMD64_SYSV_INTEGER_REGISTERS[];
extern kefir_size_t KEFIR_AMD64_SYSV_INTEGER_REGISTER_COUNT;
extern const char *KEFIR_AMD64_SYSV_SSE_REGISTERS[];
extern kefir_size_t KEFIR_AMD64_SYSV_SSE_REGISTER_COUNT;

typedef enum kefir_amd64_sysv_data_class {
    KEFIR_AMD64_SYSV_PARAM_INTEGER = 0,
    KEFIR_AMD64_SYSV_PARAM_SSE,
    KEFIR_AMD64_SYSV_PARAM_SSEUP,
    KEFIR_AMD64_SYSV_PARAM_X87,
    KEFIR_AMD64_SYSV_PARAM_X87UP,
    KEFIR_AMD64_SYSV_PARAM_COMPLEX_X87,
    KEFIR_AMD64_SYSV_PARAM_NO_CLASS,
    KEFIR_AMD64_SYSV_PARAM_MEMORY
} kefir_amd64_sysv_data_class_t;

typedef struct kefir_amd64_sysv_data_allocation {
    kefir_amd64_sysv_data_class_t dataclass;
    union {
        kefir_uint32_t registers[2];
        kefir_uint64_t offset;
    } location;
    struct {
        kefir_size_t size;
        kefir_size_t alignment;
        kefir_size_t relative_offset;
    } layout;
} kefir_amd64_sysv_data_allocation_t;

kefir_result_t kefir_amd64_sysv_data_allocate(const struct kefir_ir_type *,
                                          struct kefir_mem *,
                                          struct kefir_vector *);

#endif
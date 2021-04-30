#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kefir/ir/function.h"
#include "kefir/ir/builder.h"
#include "kefir/ir/module.h"
#include "kefir/core/mem.h"
#include "kefir/core/util.h"
#include "kefir/codegen/amd64-sysv.h"

kefir_result_t kefir_int_test(struct kefir_mem *mem) {
    struct kefir_codegen_amd64 codegen;
    kefir_codegen_amd64_sysv_init(&codegen, stdout);

    struct kefir_ir_module module;
    REQUIRE_OK(kefir_ir_module_alloc(mem, &module));

    kefir_id_t int8type_id;
    struct kefir_ir_type *int8type = kefir_ir_module_new_type(mem, &module, 1, &int8type_id);
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, int8type, KEFIR_IR_TYPE_INT8, 0, 0));
    struct kefir_ir_data *int8data1 = kefir_ir_module_new_named_data(mem, &module, "int8_1", int8type_id);
    REQUIRE_OK(kefir_ir_data_set_integer(int8data1, 0, 104));
    REQUIRE_OK(kefir_ir_data_finalize(int8data1));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "int8_1"));
    REQUIRE_OK(kefir_ir_data_finalize(kefir_ir_module_new_named_data(mem, &module, "int8_2", int8type_id)));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "int8_2"));

    kefir_id_t int16type_id;
    struct kefir_ir_type *int16type = kefir_ir_module_new_type(mem, &module, 1, &int16type_id);
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, int16type, KEFIR_IR_TYPE_INT16, 0, 0));
    struct kefir_ir_data *int16data1 = kefir_ir_module_new_named_data(mem, &module, "int16_1", int16type_id);
    REQUIRE_OK(kefir_ir_data_set_integer(int16data1, 0, 2589));
    REQUIRE_OK(kefir_ir_data_finalize(int16data1));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "int16_1"));
    REQUIRE_OK(kefir_ir_data_finalize(kefir_ir_module_new_named_data(mem, &module, "int16_2", int16type_id)));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "int16_2"));

    kefir_id_t int32type_id;
    struct kefir_ir_type *int32type = kefir_ir_module_new_type(mem, &module, 1, &int32type_id);
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, int32type, KEFIR_IR_TYPE_INT32, 0, 0));
    struct kefir_ir_data *int32data1 = kefir_ir_module_new_named_data(mem, &module, "int32_1", int32type_id);
    REQUIRE_OK(kefir_ir_data_set_integer(int32data1, 0, 0x1ffff));
    REQUIRE_OK(kefir_ir_data_finalize(int32data1));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "int32_1"));
    REQUIRE_OK(kefir_ir_data_finalize(kefir_ir_module_new_named_data(mem, &module, "int32_2", int32type_id)));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "int32_2"));

    kefir_id_t int64type_id;
    struct kefir_ir_type *int64type = kefir_ir_module_new_type(mem, &module, 1, &int64type_id);
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, int64type, KEFIR_IR_TYPE_INT64, 0, 0));
    struct kefir_ir_data *int64data1 = kefir_ir_module_new_named_data(mem, &module, "int64_1", int64type_id);
    REQUIRE_OK(kefir_ir_data_set_integer(int64data1, 0, 0x1ffffffff));
    REQUIRE_OK(kefir_ir_data_finalize(int64data1));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "int64_1"));
    REQUIRE_OK(kefir_ir_data_finalize(kefir_ir_module_new_named_data(mem, &module, "int64_2", int64type_id)));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "int64_2"));

    kefir_id_t float32type_id;
    struct kefir_ir_type *float32type = kefir_ir_module_new_type(mem, &module, 1, &float32type_id);
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, float32type, KEFIR_IR_TYPE_FLOAT32, 0, 0));
    struct kefir_ir_data *float32data1 = kefir_ir_module_new_named_data(mem, &module, "float32_1", float32type_id);
    REQUIRE_OK(kefir_ir_data_set_float32(float32data1, 0, 3.14159));
    REQUIRE_OK(kefir_ir_data_finalize(float32data1));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "float32_1"));
    REQUIRE_OK(kefir_ir_data_finalize(kefir_ir_module_new_named_data(mem, &module, "float32_2", float32type_id)));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "float32_2"));

    kefir_id_t float64type_id;
    struct kefir_ir_type *float64type = kefir_ir_module_new_type(mem, &module, 1, &float64type_id);
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, float64type, KEFIR_IR_TYPE_FLOAT64, 0, 0));
    struct kefir_ir_data *float64data1 = kefir_ir_module_new_named_data(mem, &module, "float64_1", float64type_id);
    REQUIRE_OK(kefir_ir_data_set_float64(float64data1, 0, 2.718281828));
    REQUIRE_OK(kefir_ir_data_finalize(float64data1));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "float64_1"));
    REQUIRE_OK(kefir_ir_data_finalize(kefir_ir_module_new_named_data(mem, &module, "float64_2", float64type_id)));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "float64_2"));

    kefir_id_t struct1_type_id;
    struct kefir_ir_type *struct1_type = kefir_ir_module_new_type(mem, &module, 5, &struct1_type_id);
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, struct1_type, KEFIR_IR_TYPE_STRUCT, 0, 4));
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, struct1_type, KEFIR_IR_TYPE_INT8, 0, 0));
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, struct1_type, KEFIR_IR_TYPE_INT64, 0, 0));
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, struct1_type, KEFIR_IR_TYPE_INT16, 0, 0));
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, struct1_type, KEFIR_IR_TYPE_FLOAT32, 0, 0));
    struct kefir_ir_data *struct1_data1 = kefir_ir_module_new_named_data(mem, &module, "struct1_1", struct1_type_id);
    REQUIRE_OK(kefir_ir_data_set_integer(struct1_data1, 1, 127));
    REQUIRE_OK(kefir_ir_data_set_integer(struct1_data1, 2, 0x2ffffffff));
    REQUIRE_OK(kefir_ir_data_set_integer(struct1_data1, 3, 4096));
    REQUIRE_OK(kefir_ir_data_set_float32(struct1_data1, 4, 106.9994));
    REQUIRE_OK(kefir_ir_data_finalize(struct1_data1));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "struct1_1"));

    kefir_id_t array1_type_id;
    struct kefir_ir_type *array1_type = kefir_ir_module_new_type(mem, &module, 6, &array1_type_id);
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, array1_type, KEFIR_IR_TYPE_ARRAY, 0, 3));
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, array1_type, KEFIR_IR_TYPE_STRUCT, 0, 2));
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, array1_type, KEFIR_IR_TYPE_ARRAY, 0, 2));
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, array1_type, KEFIR_IR_TYPE_CHAR, 0, 0));
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, array1_type, KEFIR_IR_TYPE_ARRAY, 0, 2));
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, array1_type, KEFIR_IR_TYPE_FLOAT32, 0, 0));
    struct kefir_ir_data *array1_data = kefir_ir_module_new_named_data(mem, &module, "array1_1", array1_type_id);
    REQUIRE_OK(kefir_ir_data_set_integer(array1_data, 3, 10));
    REQUIRE_OK(kefir_ir_data_set_integer(array1_data, 4, 20));
    REQUIRE_OK(kefir_ir_data_set_float32(array1_data, 6, 3.4));
    REQUIRE_OK(kefir_ir_data_set_float32(array1_data, 7, 4.5));
    REQUIRE_OK(kefir_ir_data_set_integer(array1_data, 17, 110));
    REQUIRE_OK(kefir_ir_data_set_integer(array1_data, 18, 120));
    REQUIRE_OK(kefir_ir_data_set_float32(array1_data, 20, 40.56));
    REQUIRE_OK(kefir_ir_data_set_float32(array1_data, 21, 56.74));
    REQUIRE_OK(kefir_ir_data_finalize(array1_data));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "array1_1"));

    kefir_id_t uniont1_type_id;
    struct kefir_ir_type *union1_type = kefir_ir_module_new_type(mem, &module, 6, &uniont1_type_id);
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, union1_type, KEFIR_IR_TYPE_UNION, 0, 2));
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, union1_type, KEFIR_IR_TYPE_STRUCT, 0, 2));
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, union1_type, KEFIR_IR_TYPE_FLOAT32, 0, 0));
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, union1_type, KEFIR_IR_TYPE_INT32, 0, 0));
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, union1_type, KEFIR_IR_TYPE_ARRAY, 0, 8));
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, union1_type, KEFIR_IR_TYPE_CHAR, 0, 0));
    struct kefir_ir_data *union1_data = kefir_ir_module_new_named_data(mem, &module, "union1_1", uniont1_type_id);
    REQUIRE_OK(kefir_ir_data_set_float32(union1_data, 2, 3.14));
    REQUIRE_OK(kefir_ir_data_set_integer(union1_data, 3, 100500));
    REQUIRE_OK(kefir_ir_data_finalize(union1_data));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "union1_1"));

    const char *MSG = "Hello, cruel world!";
    kefir_id_t memory1_type_id;
    struct kefir_ir_type *memory1_type = kefir_ir_module_new_type(mem, &module, 1, &memory1_type_id);
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, memory1_type, KEFIR_IR_TYPE_MEMORY, 0, strlen(MSG) + 1));
    struct kefir_ir_data *memory1_data = kefir_ir_module_new_named_data(mem, &module, "memory1_1", memory1_type_id);
    REQUIRE_OK(kefir_ir_data_set_string(memory1_data, 0, MSG));
    REQUIRE_OK(kefir_ir_data_finalize(memory1_data));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "memory1_1"));

    kefir_id_t pad1_type_id;
    struct kefir_ir_type *pad1_type = kefir_ir_module_new_type(mem, &module, 1, &pad1_type_id);
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, pad1_type, KEFIR_IR_TYPE_PAD, 0, 10));
    struct kefir_ir_data *pad1_data = kefir_ir_module_new_named_data(mem, &module, "pad1_1", pad1_type_id);
    REQUIRE_OK(kefir_ir_data_set_raw(pad1_data, 0, MSG, strlen(MSG) + 1));
    REQUIRE_OK(kefir_ir_data_finalize(pad1_data));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "pad1_1"));

    kefir_id_t pointer1_type_id;
    struct kefir_ir_type *pointer1_type = kefir_ir_module_new_type(mem, &module, 1, &pointer1_type_id);
    REQUIRE_OK(kefir_irbuilder_type_append_v(mem, pointer1_type, KEFIR_IR_TYPE_WORD, 0, 0));
    struct kefir_ir_data *pointer1_data = kefir_ir_module_new_named_data(mem, &module, "pointer1_1", pointer1_type_id);
    REQUIRE_OK(kefir_ir_data_set_pointer(pointer1_data, 0, "memory1_1", 2));
    REQUIRE_OK(kefir_ir_data_finalize(pointer1_data));
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, "pointer1_1"));

    KEFIR_CODEGEN_TRANSLATE(mem, &codegen.iface, &module);
    KEFIR_CODEGEN_CLOSE(&codegen.iface);
    REQUIRE_OK(kefir_ir_module_free(mem, &module));
    return EXIT_SUCCESS;
}
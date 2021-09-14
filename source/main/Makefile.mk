KEFIR_MAIN_SOURCES := $(SOURCE_DIR)/main/main.c
KEFIR_MAIN_DEPENDENCIES := $(BIN_DIR)/main/main.d
KEFIR_MAIN_OBJECT_FILES := $(BIN_DIR)/main/main.o $(BIN_DIR)/main/help.o

KEFIR_MAIN_LINKED_LIBS=
ifeq ($(SANITIZE),undefined)
KEFIR_MAIN_LINKED_LIBS=-fsanitize=undefined
endif

$(BIN_DIR)/main/help.o: $(SOURCE_DIR)/main/help.txt $(SOURCE_DIR)/main/help.asm
	@mkdir -p $(shell dirname "$@")
	@echo "Building $@"
	@$(AS) -f elf64 -o $@ $(SOURCE_DIR)/main/help.asm

$(BIN_DIR)/kefir: $(KEFIR_MAIN_OBJECT_FILES) $(LIBKEFIR_SO)
	@mkdir -p $(shell dirname "$@")
	@echo "Linking $@"
	@$(CC) -o $@ $(KEFIR_MAIN_OBJECT_FILES) $(KEFIR_MAIN_LINKED_LIBS) -L $(LIB_DIR) -lkefir

DEPENDENCIES += $(KEFIR_MAIN_DEPENDENCIES)
OBJECT_FILES += $(KEFIR_MAIN_OBJECT_FILES)
BINARIES += $(BIN_DIR)/kefir

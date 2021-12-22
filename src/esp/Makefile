# Additional esp build rules

CROSS_PREFIX = ~/.espressif/tools/xtensa-$(CONFIG_MCU)-elf/esp-2021r2-8.4.0/xtensa-$(CONFIG_MCU)-elf/bin/xtensa-$(CONFIG_MCU)-elf-
target-y := build

dirs-y += src/esp src/generic src/esp/$(CONFIG_MCU)
src-y += $(foreach src_file,$(wildcard src/esp/*.c),../$(src_file))
src-y += $(foreach src_file,$(wildcard src/esp/$(CONFIG_MCU)/internal/*.c),../$(src_file))

$(info $$src-y is [${src-y}])

# Add all includes needed
CFLAGS += -Isrc/esp/ -Isrc/esp/$(CONFIG_MCU) -I$(OUT)board/ $(OUT)board-generic/

# Add linker scripts
CFLAGS_klipper.elf += -T $(OUT)board/$(CONFIG_MCU)/linker.ld

build: prepare_toolchain build_esp

build_esp: $(OUT)klipper.elf

prepare_toolchain:
	@echo "Preparing toolchain"
	./lib/esp/install.sh

flash: build
	@echo "  Flashing $< to $(FLASH_DEVICE) via idf"
	$(Q)if [ -z $(FLASH_DEVICE) ]; then echo "Please specify FLASH_DEVICE"; exit 1; fi
	$(Q)avrdude -p$(CONFIG_MCU) -c$(CONFIG_AVRDUDE_PROTOCOL) -P"$(FLASH_DEVICE)" -D -U"flash:w:$(<):i"

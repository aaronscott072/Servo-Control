##### Target ###################################################################
TARGET = Servo-Control

##### Build Variables ##########################################################
OPT = -O0 # @todo: consider -O2 etc.?
#DEBUG = 1

##### Paths ####################################################################
BUILD_DIR = build
DRIVERS_DIR = drivers
THIRD_PARTY_DIR = $(DRIVERS_DIR)/third-party
CMSIS_DIR = $(THIRD_PARTY_DIR)/CMSIS
FREERTOS_DIR = $(THIRD_PARTY_DIR)/FreeRTOS
STM32_HAL_DIR = $(THIRD_PARTY_DIR)/STM32L4xx_HAL_Driver

##### Sources ##################################################################
C_SOURCES = $(shell find src/ -type f -iname '*.c')
C_SOURCES += $(CMSIS_DIR)/Device/ST/STM32L4xx/Source/Templates/system_stm32l4xx.c
C_SOURCES += $(shell find $(STM32_HAL_DIR)/Src/ -type f \( -iname 'stm32l4xx_hal*.c' -not -iname '*_template.c' \)) # @note: ignoring LL and *_template.c source files
C_SOURCES += $(shell find $(FREERTOS_DIR)/Source/ -maxdepth 1 -type f -iname '*.c') # @note: only this directories source files, no sub-directories (-maxdepth 1)
C_SOURCES += $(FREERTOS_DIR)/Source/portable/MemMang/heap_4.c
C_SOURCES += $(FREERTOS_DIR)/Source/portable/GCC/ARM_CM4F/port.c

ASM_SOURCES = $(CMSIS_DIR)/Device/ST/STM32L4xx/Source/Templates/gcc/startup_stm32l433xx.s

##### Includes #################################################################
C_INCLUDES = -Iinc
C_INCLUDES += -I$(DRIVERS_DIR)
C_INCLUDES += -I$(STM32_HAL_DIR)/Inc
C_INCLUDES += -I$(STM32_HAL_DIR)/Inc/Legacy
C_INCLUDES += -I$(CMSIS_DIR)/Device/ST/STM32L4xx/Include
C_INCLUDES += -I$(CMSIS_DIR)/Include
C_INCLUDES += -I$(FREERTOS_DIR)/Source/include
C_INCLUDES += -I$(FREERTOS_DIR)/Source/portable/GCC/ARM_CM4F

AS_INCLUDES = 

##### Binaries #################################################################
PREFIX = arm-none-eabi-
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

##### CFLAGS ###################################################################
CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

AS_DEFS = 
C_DEFS = -DUSE_HAL_DRIVER -DSTM32L433xx

ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -fdata-sections -ffunction-sections
CFLAGS += -Werror -Wall -Wextra
CFLAGS += -Wcast-qual -Wpedantic -Wpointer-arith -Wshadow

ifeq ($(DEBUG), 1)
CFLAGS += -g3 -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

##### LDFLAGS ##################################################################
LDSCRIPT = STM32L433RCTxP_FLASH.ld
LIBDIR = 
LIBS = -lc -lm -lnosys 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

##### Builds ###################################################################
# Default action:
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# List of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# List of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@	

##### Clean-up #################################################################
clean:
	-rm -fR $(BUILD_DIR)

##### Phony Targets ############################################################
.PHONY: all clean

##### Dependencies #############################################################
-include $(wildcard $(BUILD_DIR)/*.d)
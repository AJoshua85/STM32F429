################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/Helpers/subdir.mk
-include Src/subdir.mk
-include Drivers/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
USBF4.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
USBF4.list \

OBJCOPY_BIN += \
USBF4.bin \


# All Target
all: USBF4.elf secondary-outputs

# Tool invocations
USBF4.elf: $(OBJS) $(USER_OBJS) C:\Users\Avinash\ J\STM32CubeIDE\USB_Prog\USBF4\STM32F429ZITX_FLASH.ld
	arm-none-eabi-gcc -o "USBF4.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Avinash J\STM32CubeIDE\USB_Prog\USBF4\STM32F429ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="USBF4.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

USBF4.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "USBF4.list"
	@echo 'Finished building: $@'
	@echo ' '

USBF4.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "USBF4.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

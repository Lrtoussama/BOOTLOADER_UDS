################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
bootloader_uds.lss \

FLASH_IMAGE += \
bootloader_uds.hex \

EEPROM_IMAGE += \
bootloader_uds.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: bootloader_uds.elf secondary-outputs

# Tool invocations
bootloader_uds.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,bootloader_uds.map -Ttext=0x7000 -mmcu=atmega328p -o "bootloader_uds.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

bootloader_uds.lss: bootloader_uds.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S bootloader_uds.elf  >"bootloader_uds.lss"
	@echo 'Finished building: $@'
	@echo ' '

bootloader_uds.hex: bootloader_uds.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex bootloader_uds.elf  "bootloader_uds.hex"
	@echo 'Finished building: $@'
	@echo ' '

bootloader_uds.eep: bootloader_uds.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex bootloader_uds.elf  "bootloader_uds.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: bootloader_uds.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p bootloader_uds.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) bootloader_uds.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

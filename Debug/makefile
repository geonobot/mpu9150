################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -f

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include example/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
libmpu9150.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: libmpu9150.a secondary-outputs

dependents:
	-cd /home/ldo/Pro/AXYN_Robotique/workspace/twi/Debug && $(MAKE) all

# Tool invocations
libmpu9150.a: $(OBJS) /home/ldo/Pro/AXYN_Robotique/workspace/twi/Debug/libtwi.a
	@echo 'Building target: $@'
	@echo 'Invoking: AVR Archiver'
	avr-ar -r  "libmpu9150.a" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

libmpu9150.lss: libmpu9150.a
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S libmpu9150.a  >"libmpu9150.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: libmpu9150.a
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega2560 libmpu9150.a
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(ARCHIVES)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) libmpu9150.a
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:
/home/ldo/Pro/AXYN_Robotique/workspace/twi/Debug/libtwi.a:

-include ../makefile.targets

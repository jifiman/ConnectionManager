################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ConnectionManager

# Tool invocations
ConnectionManager: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	arm-linux-gnueabihf-g++ -nostdinc -nostdlib -L/media/rca/myPart1/Beagle/Kernel/Debian/myLibs -L/mnt/beagle/usr/lib/gcc/arm-linux-gnueabihf/4.6 -L/mnt/beagle/usr/lib/gcc/arm-linux-gnueabihf/4.6/../../../arm-linux-gnueabihf -L/mnt/beagle/usr/lib/gcc/arm-linux-gnueabihf/4.6/../../.. -L/mnt/beagle/lib/arm-linux-gnueabihf -L/mnt/beagle/lib -L/mnt/beagle/usr/lib/arm-linux-gnueabihf -L /media/rca/myPart1/Beagle/Kernel/Debian/myLibs/    --sysroot=/mnt/beagle -o "ConnectionManager" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) ConnectionManager
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

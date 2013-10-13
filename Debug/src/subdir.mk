################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ConnectionManager.cpp \
../src/test.cpp 

OBJS += \
./src/ConnectionManager.o \
./src/test.o 

CPP_DEPS += \
./src/ConnectionManager.d \
./src/test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I/mnt/beagle/usr/include -I/mnt/beagle/usr/include/c++/4.6/backward -I/mnt/beagle/usr/lib/gcc/arm-linux-gnueabihf/4.6/include -I/mnt/beagle/usr/local/include -I/mnt/beagle/usr/lib/gcc/arm-linux-gnueabihf/4.6/include-fixed -I/mnt/beagle/usr/include/arm-linux-gnueabihf -I/mnt/beagle/usr/include/c++/4.6/arm-linux-gnueabihf -I/mnt/beagle/usr/include/c++/4.6 -I/media/rca/myPart1/Projects/Common -I/media/rca/myPart1/Beagle/Kernel/Debian/myIncludes -O0 -g3 -Wall -c -fmessage-length=0 -nostdinc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



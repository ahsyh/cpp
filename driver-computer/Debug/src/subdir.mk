################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_UPPER_SRCS += \
../src/TaskGroup.C \
../src/driver-compute.C \
../src/ppp.C \
../src/random.C 

OBJS += \
./src/TaskGroup.o \
./src/driver-compute.o \
./src/ppp.o \
./src/random.o 

C_UPPER_DEPS += \
./src/TaskGroup.d \
./src/driver-compute.d \
./src/ppp.d \
./src/random.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.C
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O1 -std=c++0x -g -D__x86_64__ -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



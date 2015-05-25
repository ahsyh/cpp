################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_UPPER_SRCS += \
../src/TaskGroup.C \
../src/driver-sort.C \
../src/ppp.C \
../src/random.C 

OBJS += \
./src/TaskGroup.o \
./src/driver-sort.o \
./src/ppp.o \
./src/random.o 

C_UPPER_DEPS += \
./src/TaskGroup.d \
./src/driver-sort.d \
./src/ppp.d \
./src/random.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.C
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -std=c++0x -g3 -D__x86_64__ -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



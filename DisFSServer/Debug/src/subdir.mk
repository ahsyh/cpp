################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CManager.cpp \
../src/CParameter.cpp \
../src/CSeqGenerator.cpp \
../src/CTasks.cpp \
../src/CWorker.cpp \
../src/DisFSServer.cpp \
../src/message.cpp \
../src/task_archive.cpp 

OBJS += \
./src/CManager.o \
./src/CParameter.o \
./src/CSeqGenerator.o \
./src/CTasks.o \
./src/CWorker.o \
./src/DisFSServer.o \
./src/message.o \
./src/task_archive.o 

CPP_DEPS += \
./src/CManager.d \
./src/CParameter.d \
./src/CSeqGenerator.d \
./src/CTasks.d \
./src/CWorker.d \
./src/DisFSServer.d \
./src/message.d \
./src/task_archive.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/syh/share/workspace/unplib/src -O0 -std=c++0x -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



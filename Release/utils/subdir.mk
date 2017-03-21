################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../utils/Buffer.cpp \
../utils/CodeConvert.cpp \
../utils/FileOp.cpp \
../utils/IThread.cpp \
../utils/TimeOp.cpp \
../utils/TimerThread.cpp 

OBJS += \
./utils/Buffer.o \
./utils/CodeConvert.o \
./utils/FileOp.o \
./utils/IThread.o \
./utils/TimeOp.o \
./utils/TimerThread.o 

CPP_DEPS += \
./utils/Buffer.d \
./utils/CodeConvert.d \
./utils/FileOp.d \
./utils/IThread.d \
./utils/TimeOp.d \
./utils/TimerThread.d 


# Each subdirectory must supply rules for building sources it contributes
utils/%.o: ../utils/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=gnu++11 -I/home/ruanbo/Codes/Ats -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



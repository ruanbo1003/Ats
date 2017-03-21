################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AtsMain/Ats.cpp 

OBJS += \
./AtsMain/Ats.o 

CPP_DEPS += \
./AtsMain/Ats.d 


# Each subdirectory must supply rules for building sources it contributes
AtsMain/%.o: ../AtsMain/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=gnu++11 -I/home/ruanbo/Codes/Ats -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



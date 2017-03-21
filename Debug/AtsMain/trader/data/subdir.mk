################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AtsMain/trader/data/OrderMngr.cpp 

OBJS += \
./AtsMain/trader/data/OrderMngr.o 

CPP_DEPS += \
./AtsMain/trader/data/OrderMngr.d 


# Each subdirectory must supply rules for building sources it contributes
AtsMain/trader/data/%.o: ../AtsMain/trader/data/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=gnu++11 -DDEBUG_MODE=1 -I/home/ruanbo/Codes/Ats -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



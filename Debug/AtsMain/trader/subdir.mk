################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AtsMain/trader/AtsTrader.cpp \
../AtsMain/trader/Helper.cpp \
../AtsMain/trader/Orders.cpp \
../AtsMain/trader/Settlement.cpp 

OBJS += \
./AtsMain/trader/AtsTrader.o \
./AtsMain/trader/Helper.o \
./AtsMain/trader/Orders.o \
./AtsMain/trader/Settlement.o 

CPP_DEPS += \
./AtsMain/trader/AtsTrader.d \
./AtsMain/trader/Helper.d \
./AtsMain/trader/Orders.d \
./AtsMain/trader/Settlement.d 


# Each subdirectory must supply rules for building sources it contributes
AtsMain/trader/%.o: ../AtsMain/trader/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=gnu++11 -DDEBUG_MODE=1 -I/home/ruanbo/Codes/Ats -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



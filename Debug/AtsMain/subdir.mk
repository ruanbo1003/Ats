################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AtsMain/Ats.cpp \
../AtsMain/AtsConfig.cpp 

OBJS += \
./AtsMain/Ats.o \
./AtsMain/AtsConfig.o 

CPP_DEPS += \
./AtsMain/Ats.d \
./AtsMain/AtsConfig.d 


# Each subdirectory must supply rules for building sources it contributes
AtsMain/%.o: ../AtsMain/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ruanbo/Codes/Ats" -I/usr/local/include/mysqlcppconn/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AtsMain/trader/AtsTrader.cpp \
../AtsMain/trader/Helper.cpp \
../AtsMain/trader/Instrument.cpp \
../AtsMain/trader/InvestorPotion.cpp \
../AtsMain/trader/Orders.cpp \
../AtsMain/trader/Settlement.cpp \
../AtsMain/trader/TradingAccount.cpp 

OBJS += \
./AtsMain/trader/AtsTrader.o \
./AtsMain/trader/Helper.o \
./AtsMain/trader/Instrument.o \
./AtsMain/trader/InvestorPotion.o \
./AtsMain/trader/Orders.o \
./AtsMain/trader/Settlement.o \
./AtsMain/trader/TradingAccount.o 

CPP_DEPS += \
./AtsMain/trader/AtsTrader.d \
./AtsMain/trader/Helper.d \
./AtsMain/trader/Instrument.d \
./AtsMain/trader/InvestorPotion.d \
./AtsMain/trader/Orders.d \
./AtsMain/trader/Settlement.d \
./AtsMain/trader/TradingAccount.d 


# Each subdirectory must supply rules for building sources it contributes
AtsMain/trader/%.o: ../AtsMain/trader/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ruanbo/Codes/Ats" -I/usr/local/include/mysqlcppconn/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



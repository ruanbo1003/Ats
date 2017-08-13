################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tests/BufferTest.cpp \
../tests/DbTest.cpp \
../tests/LogFile.cpp \
../tests/QuoteTest.cpp \
../tests/TraderTest.cpp 

OBJS += \
./tests/BufferTest.o \
./tests/DbTest.o \
./tests/LogFile.o \
./tests/QuoteTest.o \
./tests/TraderTest.o 

CPP_DEPS += \
./tests/BufferTest.d \
./tests/DbTest.d \
./tests/LogFile.d \
./tests/QuoteTest.d \
./tests/TraderTest.d 


# Each subdirectory must supply rules for building sources it contributes
tests/%.o: ../tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/ruanbo/Codes/Ats" -I/usr/local/include/mysqlcppconn/include -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



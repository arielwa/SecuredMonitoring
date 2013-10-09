################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../HelloWorld.o 

C_SRCS += \
../HelloWorld.c 

OBJS += \
./HelloWorld.o 

C_DEPS += \
./HelloWorld.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/tomer/Android/DroidScope/src/ -I/home/tomer/Android/DroidScope/src/DECAF_shared/DroidScope -I/home/tomer/Android/DroidScope/src/android/config/linux-x86 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../jni/attack_sim.c \
../jni/binder.c \
../jni/service_manager.c 

OBJS += \
./jni/attack_sim.o \
./jni/binder.o \
./jni/service_manager.o 

C_DEPS += \
./jni/attack_sim.d \
./jni/binder.d \
./jni/service_manager.d 


# Each subdirectory must supply rules for building sources it contributes
jni/%.o: ../jni/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/media/data/AndroidADT/ndk/platforms/android-14/arch-arm/usr/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



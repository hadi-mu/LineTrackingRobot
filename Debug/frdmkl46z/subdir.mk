################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../frdmkl46z/board.c \
../frdmkl46z/clock_config.c \
../frdmkl46z/pin_mux.c 

OBJS += \
./frdmkl46z/board.o \
./frdmkl46z/clock_config.o \
./frdmkl46z/pin_mux.o 

C_DEPS += \
./frdmkl46z/board.d \
./frdmkl46z/clock_config.d \
./frdmkl46z/pin_mux.d 


# Each subdirectory must supply rules for building sources it contributes
frdmkl46z/%.o: ../frdmkl46z/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DCPU_MKL46Z256VLL4 -DCPU_MKL46Z256VLL4_cm0plus -D__REDLIB__ -I"C:\work\Robot\projects\Robot\source" -I"C:\work\Robot\projects\Robot" -I"C:\work\Robot\projects\Robot\drivers" -I"C:\work\Robot\projects\Robot\CMSIS" -I"C:\work\Robot\projects\Robot\frdmkl46z" -I"C:\work\Robot\projects\Robot\startup" -I"C:\work\Robot\projects\Robot\utilities" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



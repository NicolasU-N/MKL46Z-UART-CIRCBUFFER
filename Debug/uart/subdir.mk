################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../uart/circbuff.c \
../uart/display.c \
../uart/uart.c 

OBJS += \
./uart/circbuff.o \
./uart/display.o \
./uart/uart.o 

C_DEPS += \
./uart/circbuff.d \
./uart/display.d \
./uart/uart.d 


# Each subdirectory must supply rules for building sources it contributes
uart/%.o: ../uart/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL46Z256VLL4 -DCPU_MKL46Z256VLL4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\nico3\Documents\MCUXpressoIDE_11.3.1_5262\workspace\MKL46Z4_UART_LCD_BUFF\board" -I"C:\Users\nico3\Documents\MCUXpressoIDE_11.3.1_5262\workspace\MKL46Z4_UART_LCD_BUFF\uart" -I"C:\Users\nico3\Documents\MCUXpressoIDE_11.3.1_5262\workspace\MKL46Z4_UART_LCD_BUFF\source" -I"C:\Users\nico3\Documents\MCUXpressoIDE_11.3.1_5262\workspace\MKL46Z4_UART_LCD_BUFF" -I"C:\Users\nico3\Documents\MCUXpressoIDE_11.3.1_5262\workspace\MKL46Z4_UART_LCD_BUFF\drivers" -I"C:\Users\nico3\Documents\MCUXpressoIDE_11.3.1_5262\workspace\MKL46Z4_UART_LCD_BUFF\utilities" -I"C:\Users\nico3\Documents\MCUXpressoIDE_11.3.1_5262\workspace\MKL46Z4_UART_LCD_BUFF\startup" -I"C:\Users\nico3\Documents\MCUXpressoIDE_11.3.1_5262\workspace\MKL46Z4_UART_LCD_BUFF\CMSIS" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



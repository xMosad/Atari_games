################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/BUTTON_program.c \
../src/BlockBuild_program.c \
../src/GPIO_program.c \
../src/Linked_List.c \
../src/OS_programe.c \
../src/RCC_program.c \
../src/SPI_program.c \
../src/STK_program.c \
../src/SnakeAPP_program.c \
../src/TFT_program.c \
../src/main.c 

OBJS += \
./src/BUTTON_program.o \
./src/BlockBuild_program.o \
./src/GPIO_program.o \
./src/Linked_List.o \
./src/OS_programe.o \
./src/RCC_program.o \
./src/SPI_program.o \
./src/STK_program.o \
./src/SnakeAPP_program.o \
./src/TFT_program.o \
./src/main.o 

C_DEPS += \
./src/BUTTON_program.d \
./src/BlockBuild_program.d \
./src/GPIO_program.d \
./src/Linked_List.d \
./src/OS_programe.d \
./src/RCC_program.d \
./src/SPI_program.d \
./src/STK_program.d \
./src/SnakeAPP_program.d \
./src/TFT_program.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



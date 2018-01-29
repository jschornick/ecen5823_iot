################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cmu.c \
../src/gpio.c 

OBJS += \
./src/cmu.o \
./src/gpio.o 

C_DEPS += \
./src/cmu.d \
./src/gpio.d 


# Each subdirectory must supply rules for building sources it contributes
src/cmu.o: ../src/cmu.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__HEAP_SIZE=0xD00' '-DHAL_CONFIG=1' '-D__STACK_SIZE=0x800' '-DEFR32BG1B232F256GM56=1' -I"/home/jeff/git/LED_BLINKY_SDK2_7" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/chip/efr32" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/gpiointerrupt/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/halconfig" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/bsp" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/CMSIS/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/drivers" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader/api" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/uartdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/halconfig/inc/hal-config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/common/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/app/bluetooth_2.7/common/stack_bridge" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/soc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/EFR32BG1_BRD4302A/config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source/GCC" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader" -I"//home/jeff/SimplicityStudio_v4/developer/toolchains/gnu_arm/4.9_2015q3//lib/gcc/arm-none-eabi/4.9.3/include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/src" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"src/cmu.d" -MT"src/cmu.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/gpio.o: ../src/gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__HEAP_SIZE=0xD00' '-DHAL_CONFIG=1' '-D__STACK_SIZE=0x800' '-DEFR32BG1B232F256GM56=1' -I"/home/jeff/git/LED_BLINKY_SDK2_7" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/chip/efr32" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/gpiointerrupt/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/halconfig" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/bsp" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/CMSIS/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/drivers" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader/api" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/uartdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/halconfig/inc/hal-config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/common/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/app/bluetooth_2.7/common/stack_bridge" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/soc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/EFR32BG1_BRD4302A/config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source/GCC" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader" -I"//home/jeff/SimplicityStudio_v4/developer/toolchains/gnu_arm/4.9_2015q3//lib/gcc/arm-none-eabi/4.9.3/include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/src" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"src/gpio.d" -MT"src/gpio.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



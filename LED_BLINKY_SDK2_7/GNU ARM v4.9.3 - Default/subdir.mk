################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../application_properties.c \
../gatt_db.c \
../init_board.c \
../init_mcu.c \
../main.c \
../pti.c 

OBJS += \
./application_properties.o \
./gatt_db.o \
./init_board.o \
./init_mcu.o \
./main.o \
./pti.o 

C_DEPS += \
./application_properties.d \
./gatt_db.d \
./init_board.d \
./init_mcu.d \
./main.d \
./pti.d 


# Each subdirectory must supply rules for building sources it contributes
application_properties.o: ../application_properties.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__HEAP_SIZE=0xD00' '-DHAL_CONFIG=1' '-D__STACK_SIZE=0x800' '-DEFR32BG1B232F256GM56=1' -I"/home/jeff/git/LED_BLINKY_SDK2_7" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/chip/efr32" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/gpiointerrupt/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/halconfig" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/bsp" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/CMSIS/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/drivers" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader/api" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/uartdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/halconfig/inc/hal-config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/common/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/app/bluetooth_2.7/common/stack_bridge" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/soc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/EFR32BG1_BRD4302A/config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source/GCC" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader" -I"//home/jeff/SimplicityStudio_v4/developer/toolchains/gnu_arm/4.9_2015q3//lib/gcc/arm-none-eabi/4.9.3/include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/src" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"application_properties.d" -MT"application_properties.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gatt_db.o: ../gatt_db.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__HEAP_SIZE=0xD00' '-DHAL_CONFIG=1' '-D__STACK_SIZE=0x800' '-DEFR32BG1B232F256GM56=1' -I"/home/jeff/git/LED_BLINKY_SDK2_7" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/chip/efr32" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/gpiointerrupt/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/halconfig" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/bsp" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/CMSIS/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/drivers" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader/api" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/uartdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/halconfig/inc/hal-config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/common/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/app/bluetooth_2.7/common/stack_bridge" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/soc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/EFR32BG1_BRD4302A/config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source/GCC" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader" -I"//home/jeff/SimplicityStudio_v4/developer/toolchains/gnu_arm/4.9_2015q3//lib/gcc/arm-none-eabi/4.9.3/include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/src" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"gatt_db.d" -MT"gatt_db.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

init_board.o: ../init_board.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__HEAP_SIZE=0xD00' '-DHAL_CONFIG=1' '-D__STACK_SIZE=0x800' '-DEFR32BG1B232F256GM56=1' -I"/home/jeff/git/LED_BLINKY_SDK2_7" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/chip/efr32" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/gpiointerrupt/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/halconfig" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/bsp" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/CMSIS/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/drivers" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader/api" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/uartdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/halconfig/inc/hal-config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/common/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/app/bluetooth_2.7/common/stack_bridge" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/soc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/EFR32BG1_BRD4302A/config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source/GCC" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader" -I"//home/jeff/SimplicityStudio_v4/developer/toolchains/gnu_arm/4.9_2015q3//lib/gcc/arm-none-eabi/4.9.3/include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/src" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"init_board.d" -MT"init_board.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

init_mcu.o: ../init_mcu.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__HEAP_SIZE=0xD00' '-DHAL_CONFIG=1' '-D__STACK_SIZE=0x800' '-DEFR32BG1B232F256GM56=1' -I"/home/jeff/git/LED_BLINKY_SDK2_7" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/chip/efr32" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/gpiointerrupt/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/halconfig" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/bsp" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/CMSIS/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/drivers" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader/api" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/uartdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/halconfig/inc/hal-config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/common/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/app/bluetooth_2.7/common/stack_bridge" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/soc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/EFR32BG1_BRD4302A/config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source/GCC" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader" -I"//home/jeff/SimplicityStudio_v4/developer/toolchains/gnu_arm/4.9_2015q3//lib/gcc/arm-none-eabi/4.9.3/include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/src" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"init_mcu.d" -MT"init_mcu.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.o: ../main.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__HEAP_SIZE=0xD00' '-DHAL_CONFIG=1' '-D__STACK_SIZE=0x800' '-DEFR32BG1B232F256GM56=1' -I"/home/jeff/git/LED_BLINKY_SDK2_7" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/chip/efr32" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/gpiointerrupt/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/halconfig" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/bsp" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/CMSIS/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/drivers" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader/api" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/uartdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/halconfig/inc/hal-config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/common/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/app/bluetooth_2.7/common/stack_bridge" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/soc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/EFR32BG1_BRD4302A/config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source/GCC" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader" -I"//home/jeff/SimplicityStudio_v4/developer/toolchains/gnu_arm/4.9_2015q3//lib/gcc/arm-none-eabi/4.9.3/include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/src" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"main.d" -MT"main.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

pti.o: ../pti.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__HEAP_SIZE=0xD00' '-DHAL_CONFIG=1' '-D__STACK_SIZE=0x800' '-DEFR32BG1B232F256GM56=1' -I"/home/jeff/git/LED_BLINKY_SDK2_7" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/chip/efr32" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/gpiointerrupt/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/halconfig" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/bsp" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/CMSIS/Include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/common/drivers" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader/api" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emlib/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/uartdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/halconfig/inc/hal-config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/radio/rail_lib/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/common" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/common/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/app/bluetooth_2.7/common/stack_bridge" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/tempdrv/inc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/protocol/bluetooth_2.7/ble_stack/inc/soc" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/emdrv/sleep/src" -I"/home/jeff/git/LED_BLINKY_SDK2_7/hardware/kit/EFR32BG1_BRD4302A/config" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source/GCC" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/Device/SiliconLabs/EFR32BG1B/Source" -I"/home/jeff/git/LED_BLINKY_SDK2_7/platform/bootloader" -I"//home/jeff/SimplicityStudio_v4/developer/toolchains/gnu_arm/4.9_2015q3//lib/gcc/arm-none-eabi/4.9.3/include" -I"/home/jeff/git/LED_BLINKY_SDK2_7/src" -O0 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"pti.d" -MT"pti.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



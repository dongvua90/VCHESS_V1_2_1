################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Src/Bluetooth.c \
../User/Src/ChessClock.c \
../User/Src/MB85RS256.c \
../User/Src/RtcRealTime.c \
../User/Src/SaveGame.c \
../User/Src/ScanPieces.c \
../User/Src/Status.c \
../User/Src/TaskScanPiece.c \
../User/Src/UsbHandling.c \
../User/Src/Utilities.c \
../User/Src/calibase.c \
../User/Src/interface.c 

C_DEPS += \
./User/Src/Bluetooth.d \
./User/Src/ChessClock.d \
./User/Src/MB85RS256.d \
./User/Src/RtcRealTime.d \
./User/Src/SaveGame.d \
./User/Src/ScanPieces.d \
./User/Src/Status.d \
./User/Src/TaskScanPiece.d \
./User/Src/UsbHandling.d \
./User/Src/Utilities.d \
./User/Src/calibase.d \
./User/Src/interface.d 

OBJS += \
./User/Src/Bluetooth.o \
./User/Src/ChessClock.o \
./User/Src/MB85RS256.o \
./User/Src/RtcRealTime.o \
./User/Src/SaveGame.o \
./User/Src/ScanPieces.o \
./User/Src/Status.o \
./User/Src/TaskScanPiece.o \
./User/Src/UsbHandling.o \
./User/Src/Utilities.o \
./User/Src/calibase.o \
./User/Src/interface.o 


# Each subdirectory must supply rules for building sources it contributes
User/Src/%.o User/Src/%.su: ../User/Src/%.c User/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/dongv/Documents/STM32CUBE_IDE/VCHESS_V1.2.1/User/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-User-2f-Src

clean-User-2f-Src:
	-$(RM) ./User/Src/Bluetooth.d ./User/Src/Bluetooth.o ./User/Src/Bluetooth.su ./User/Src/ChessClock.d ./User/Src/ChessClock.o ./User/Src/ChessClock.su ./User/Src/MB85RS256.d ./User/Src/MB85RS256.o ./User/Src/MB85RS256.su ./User/Src/RtcRealTime.d ./User/Src/RtcRealTime.o ./User/Src/RtcRealTime.su ./User/Src/SaveGame.d ./User/Src/SaveGame.o ./User/Src/SaveGame.su ./User/Src/ScanPieces.d ./User/Src/ScanPieces.o ./User/Src/ScanPieces.su ./User/Src/Status.d ./User/Src/Status.o ./User/Src/Status.su ./User/Src/TaskScanPiece.d ./User/Src/TaskScanPiece.o ./User/Src/TaskScanPiece.su ./User/Src/UsbHandling.d ./User/Src/UsbHandling.o ./User/Src/UsbHandling.su ./User/Src/Utilities.d ./User/Src/Utilities.o ./User/Src/Utilities.su ./User/Src/calibase.d ./User/Src/calibase.o ./User/Src/calibase.su ./User/Src/interface.d ./User/Src/interface.o ./User/Src/interface.su

.PHONY: clean-User-2f-Src


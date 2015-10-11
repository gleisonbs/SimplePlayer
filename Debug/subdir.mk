################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../main.o \
../moc_player_ui.o \
../moc_playlist_ui.o \
../player_ui.o \
../playlist_ui.o \
../utilities.o 

CPP_SRCS += \
../main.cpp \
../moc_player_ui.cpp \
../moc_playlist_ui.cpp 

OBJS += \
./main.o \
./moc_player_ui.o \
./moc_playlist_ui.o 

CPP_DEPS += \
./main.d \
./moc_player_ui.d \
./moc_playlist_ui.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -DDQT_CORE_LIB=1 -DDQT_GUI_LIB=1 -DDQT_NO_DEBUG=1 -DDQT_WIDGETS_LIB=1 -DDQT_MULTIMEDIA_LIB=1 -I/usr/include/qt -I/usr/include/qt/QtCore -I/usr/include/qt/QtGui -I/usr/include/qt/QtWidgets -I/usr/lib/qt/mkspecs/linux-g++ -I/usr/include/qt/QtMultimedia -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



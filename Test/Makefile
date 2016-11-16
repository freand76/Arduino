
PROGRAM=TestProgram

PORT=/dev/ttyACM0
ARDUINO_DIR=/usr/share/arduino
BOARD_TYPE=arduino
BAUD_RATE=115200

ARDUINO_CORE=$(ARDUINO_DIR)/hardware/arduino/cores/arduino
ARDUINO_VARIANT=$(ARDUINO_DIR)/hardware/arduino/variants/standard
AVR_INCLUDE=/usr/lib/avr/include
INCLUDE=-I. -I$(ARDUINO_CORE) -I$(ARDUINO_VARIANT) -I$(AVR_INCLUDE)
BUILD_DIR=build

MCU=atmega328p
DF_CPU=16000000

CC=/usr/bin/avr-gcc
CPP=/usr/bin/avr-g++
AVR_OBJCOPY=/usr/bin/avr-objcopy 
AVRDUDE=/usr/bin/avrdude

CC_FLAGS=-g -Os -w -Wall -ffunction-sections -fdata-sections -fno-exceptions \
	 -std=gnu99
CPP_FLAGS=-g -Os -w -Wall -ffunction-sections -fdata-sections -fno-exceptions

AVRDUDE_CONF=/etc/avrdude.conf

CORE_C_FILES= WInterrupts \
              wiring_analog \
              wiring \
              wiring_digital \
	      wiring_pulse \
              wiring_shift 

CORE_CPP_FILES= HardwareSerial \
                main \
                Print \
                Tone \
                WMath \
                WString

## FILES in /src
CPP_FILES = Test

all:		clean compile

clean:
		@echo '# *** Cleaning...'
		rm -rf "$(BUILD_DIR)"

compile:
		@echo '# *** Compiling...'

		mkdir $(BUILD_DIR)

		@#Compiling the Program:
		for cpp_file in ${CPP_FILES}; do \
                echo $$cpp_file; \
                $(CPP) -c -mmcu=$(MCU) -DF_CPU=$(DF_CPU) $(INCLUDE) \
		       $(CPP_FLAGS) src/$$cpp_file.cpp \
		       -o $(BUILD_DIR)/$$cpp_file.o; \
                done

                ##  $(SRC:.c=.o)

		@#Compiling Arduino core .c dependecies:
		for core_c_file in ${CORE_C_FILES}; do \
		    $(CC) -c -mmcu=$(MCU) -DF_CPU=$(DF_CPU) $(INCLUDE) \
		          $(CC_FLAGS) $(ARDUINO_CORE)/$$core_c_file.c \
			  -o $(BUILD_DIR)/$$core_c_file.o; \
		done

		@#Compiling Arduino core .cpp dependecies:
		for core_cpp_file in ${CORE_CPP_FILES}; do \
		    $(CPP) -c -mmcu=$(MCU) -DF_CPU=$(DF_CPU) $(INCLUDE) \
		           $(CPP_FLAGS) $(ARDUINO_CORE)/$$core_cpp_file.cpp \
			   -o $(BUILD_DIR)/$$core_cpp_file.o; \
		done

		@#TODO: compile external libraries here
		@#TODO: use .d files to track dependencies and compile them
		@#      change .c by -MM and use -MF to generate .d

		$(CC) -mmcu=$(MCU) -lm -Wl,--gc-sections -Os \
		      -o $(BUILD_DIR)/$(PROGRAM).elf $(BUILD_DIR)/*.o
		$(AVR_OBJCOPY) -O ihex -R .eeprom \
		               $(BUILD_DIR)/$(PROGRAM).elf \
			       $(BUILD_DIR)/$(PROGRAM).hex
		@echo '# *** Compiled successfully! \o/'


reset:
		@echo '# *** Resetting...'
		stty --file $(PORT) hupcl
		sleep 0.1
		stty --file $(PORT) -hupcl


upload:
		@echo '# *** Uploading...'
		$(AVRDUDE) -q -V -p $(MCU) -C $(AVRDUDE_CONF) -c $(BOARD_TYPE) \
		           -b $(BAUD_RATE) -P $(PORT) \
			   -U flash:w:$(BUILD_DIR)/$(PROGRAM).hex:i
		@echo '# *** Done - enjoy your sketch!'

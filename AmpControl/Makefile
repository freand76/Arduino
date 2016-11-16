PROGRAM=AmpControl

PORT=/dev/ttyACM0
ARDUINO_DIR=/usr/share/arduino
BOARD_TYPE=arduino
BAUD_RATE=115200

ARDUINO_CORE=$(ARDUINO_DIR)/hardware/arduino/cores/arduino
ARDUINO_VARIANT=$(ARDUINO_DIR)/hardware/arduino/variants/standard
AVR_INCLUDE=/usr/lib/avr/include

INCLUDE= \
	-I. \
	-I../NexaTx/src \
	-I$(ARDUINO_CORE) \
	-I$(ARDUINO_VARIANT) \
	-I$(AVR_INCLUDE)

BUILD_DIR=.build

MCU=atmega328p
DF_CPU=16000000

CC=/usr/bin/avr-gcc
CPP=/usr/bin/avr-g++
AVR_OBJCOPY=/usr/bin/avr-objcopy 
AVRDUDE=/usr/bin/avrdude

CC_FLAGS=-g -Os -w -Wall -ffunction-sections -fdata-sections -fno-exceptions -std=gnu99
CPP_FLAGS=-g -Os -w -Wall -ffunction-sections -fdata-sections -fno-exceptions

AVRDUDE_CONF=/etc/avrdude.conf

CORE_C_FILES= WInterrupts.c \
              wiring_analog.c \
              wiring.c \
              wiring_digital.c \
	      wiring_pulse.c \
              wiring_shift.c 

CORE_CPP_FILES= HardwareSerial.cpp \
                main.cpp \
                Print.cpp \
                Tone.cpp \
                WMath.cpp \
                WString.cpp

## FILES in /src
CPP_FILES = AmpControl.cpp \
	    NexaTx.cpp

OBJ_FILES =
OBJ_FILES += $(addprefix $(BUILD_DIR)/,$(CORE_C_FILES:.c=.o))
OBJ_FILES += $(addprefix $(BUILD_DIR)/,$(CORE_CPP_FILES:.cpp=.o))
OBJ_FILES += $(addprefix $(BUILD_DIR)/,$(CPP_FILES:.cpp=.o))

PROGRAM := $(addprefix $(BUILD_DIR)/,$(PROGRAM))

all:		clean compile

clean:
		@echo '# *** Cleaning...'
		rm -rf $(BUILD_DIR)

vpath %.cpp src/
vpath %.c  $(ARDUINO_CORE)/
vpath %.cpp $(ARDUINO_CORE)/
vpath %.cpp ../NexaTx/src

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

compile:  $(BUILD_DIR) $(PROGRAM).hex

$(PROGRAM).elf : $(OBJ_FILES)
	$(CC) -mmcu=$(MCU) -lm -Wl,--gc-sections -Os -o $@ $^

$(PROGRAM).hex : $(PROGRAM).elf
	$(AVR_OBJCOPY) -O ihex -R .eeprom $^ $@

$(BUILD_DIR)/%.o : %.cpp
	$(CPP) -c -mmcu=$(MCU) -DF_CPU=$(DF_CPU) $(INCLUDE) \
	$(CPP_FLAGS) $^ -o $@

$(BUILD_DIR)/%.o : %.c
	$(CC) -c -mmcu=$(MCU) -DF_CPU=$(DF_CPU) $(INCLUDE) \
	$(CC_FLAGS) $^ -o $@

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

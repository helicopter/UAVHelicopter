################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../drivers \
../radio


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../drivers/SerialDriverIntegrationTests.cpp \
../HelicopterIntegrationTests.cpp \
../radio/RadioInterfaceIntegrationTests.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
drivers/SerialDriverIntegrationTests.o \
HelicopterIntegrationTests.o \
radio/RadioInterfaceIntegrationTests.o

OBJS_AS_ARGS +=  \
drivers/SerialDriverIntegrationTests.o \
HelicopterIntegrationTests.o \
radio/RadioInterfaceIntegrationTests.o

C_DEPS +=  \
drivers/SerialDriverIntegrationTests.d \
HelicopterIntegrationTests.d \
radio/RadioInterfaceIntegrationTests.d

C_DEPS_AS_ARGS +=  \
drivers/SerialDriverIntegrationTests.d \
HelicopterIntegrationTests.d \
radio/RadioInterfaceIntegrationTests.d

OUTPUT_FILE_PATH +=HelicopterIntegrationTests.elf

OUTPUT_FILE_PATH_AS_ARGS +=HelicopterIntegrationTests.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler







drivers/%.o: ../drivers/%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DDEBUG  -I"../../HelicopterTestsCommon/util/testframework" -I"../../Helicopter/buffer" -I"../../Helicopter/drivers" -I"../../Helicopter/scheduler" -I"../../Helicopter/sensors" -I"../../Helicopter/util/common" -I"../../HelicopterUnitTests" -I"../radio" -I"../drivers" -I"../../Helicopter/interfaces" -I"../../HelicopterTestsCommon/mocks" -I"../../Helicopter/messages"  -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=atmega2560 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	

./%.o: .././%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DDEBUG  -I"../../HelicopterTestsCommon/util/testframework" -I"../../Helicopter/buffer" -I"../../Helicopter/drivers" -I"../../Helicopter/scheduler" -I"../../Helicopter/sensors" -I"../../Helicopter/util/common" -I"../../HelicopterUnitTests" -I"../radio" -I"../drivers" -I"../../Helicopter/interfaces" -I"../../HelicopterTestsCommon/mocks" -I"../../Helicopter/messages"  -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=atmega2560 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	

radio/%.o: ../radio/%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -DDEBUG  -I"../../HelicopterTestsCommon/util/testframework" -I"../../Helicopter/buffer" -I"../../Helicopter/drivers" -I"../../Helicopter/scheduler" -I"../../Helicopter/sensors" -I"../../Helicopter/util/common" -I"../../HelicopterUnitTests" -I"../radio" -I"../drivers" -I"../../Helicopter/interfaces" -I"../../HelicopterTestsCommon/mocks" -I"../../Helicopter/messages"  -O0 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=atmega2560 -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR8/GNU Linker : 3.4.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="HelicopterIntegrationTests.map" -Wl,--start-group -Wl,-lm -Wl,-lHelicopterTestsCommon -Wl,-lHelicopter  -Wl,--end-group -Wl,-L"../../HelicopterTestsCommon/Debug" -Wl,-L"../../Helicopter/Debug"  -Wl,--gc-sections -mrelax -mmcu=atmega2560 
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "HelicopterIntegrationTests.elf" "HelicopterIntegrationTests.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "HelicopterIntegrationTests.elf" "HelicopterIntegrationTests.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "HelicopterIntegrationTests.elf" > "HelicopterIntegrationTests.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "HelicopterIntegrationTests.elf" "HelicopterIntegrationTests.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.2.1002\avr8-gnu-toolchain\bin\avr-size.exe" "HelicopterIntegrationTests.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "HelicopterIntegrationTests.elf" "HelicopterIntegrationTests.a" "HelicopterIntegrationTests.hex" "HelicopterIntegrationTests.lss" "HelicopterIntegrationTests.eep" "HelicopterIntegrationTests.map" "HelicopterIntegrationTests.srec"
	
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

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../util \
../util/testframework


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../BufferTests.cpp \
../HelicopterUnitTests.cpp \
../SchedulerTest.cpp \
../util/testframework/TestSuite.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
BufferTests.o \
HelicopterUnitTests.o \
SchedulerTest.o \
util/testframework/TestSuite.o


OBJS_AS_ARGS +=  \
BufferTests.o \
HelicopterUnitTests.o \
SchedulerTest.o \
util/testframework/TestSuite.o


C_DEPS +=  \
BufferTests.d \
HelicopterUnitTests.d \
SchedulerTest.d \
util/testframework/TestSuite.d


C_DEPS_AS_ARGS +=  \
BufferTests.d \
HelicopterUnitTests.d \
SchedulerTest.d \
util/testframework/TestSuite.d


OUTPUT_FILE_PATH +=HelicopterUnitTests.elf

OUTPUT_FILE_PATH_AS_ARGS +=HelicopterUnitTests.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

# AVR32/GNU C Compiler









./%.o: .././%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C++ Compiler : (AVR_8_bit_GNU_Toolchain_3.4.0_663) 4.6.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Studio 6.0\extensions\Atmel\AVRGCC\3.4.0.65\AVRToolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -I"../../Helicopter/util/common" -I"../../Helicopter/buffer" -I"../../Helicopter/tasks" -I"../util/testframework" -I".." -I"../../Helicopter/scheduler"  -O0 -fpack-struct -fshort-enums -g2 -Wall -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -mmcu=atmega2560   -o"$@" "$<" 
	@echo Finished building: $<

util/testframework/%.o: ../util/testframework/%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C++ Compiler : (AVR_8_bit_GNU_Toolchain_3.4.0_663) 4.6.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Studio 6.0\extensions\Atmel\AVRGCC\3.4.0.65\AVRToolchain\bin\avr-g++.exe$(QUOTE) -funsigned-char -funsigned-bitfields -I"../../Helicopter/util/common" -I"../../Helicopter/buffer" -I"../../Helicopter/tasks" -I"../util/testframework" -I".." -I"../../Helicopter/scheduler"  -O0 -fpack-struct -fshort-enums -g2 -Wall -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -mmcu=atmega2560   -o"$@" "$<" 
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

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP)
	@echo Building target: $@
	@echo Invoking: AVR8/GNU Linker : (AVR_8_bit_GNU_Toolchain_3.4.0_663) 4.6.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Studio 6.0\extensions\Atmel\AVRGCC\3.4.0.65\AVRToolchain\bin\avr-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="HelicopterUnitTests.map" -Wl,--start-group -Wl,-lm -Wl,-lheli  -Wl,--end-group -Wl,-L"../../libs"   -mmcu=atmega2560  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Studio 6.0\extensions\Atmel\AVRGCC\3.4.0.65\AVRToolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "HelicopterUnitTests.elf" "HelicopterUnitTests.hex"
	"C:\Program Files (x86)\Atmel\Atmel Studio 6.0\extensions\Atmel\AVRGCC\3.4.0.65\AVRToolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "HelicopterUnitTests.elf" "HelicopterUnitTests.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Studio 6.0\extensions\Atmel\AVRGCC\3.4.0.65\AVRToolchain\bin\avr-objdump.exe" -h -S "HelicopterUnitTests.elf" > "HelicopterUnitTests.lss"
	"C:\Program Files (x86)\Atmel\Atmel Studio 6.0\extensions\Atmel\AVRGCC\3.4.0.65\AVRToolchain\bin\avr-size.exe" "HelicopterUnitTests.elf"
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS)$(C_DEPS_AS_ARGS) $(EXECUTABLES) 
	rm -rf "HelicopterUnitTests.elf" "HelicopterUnitTests.a" "HelicopterUnitTests.hex" "HelicopterUnitTests.lss" "HelicopterUnitTests.eep" "HelicopterUnitTests.map"
	
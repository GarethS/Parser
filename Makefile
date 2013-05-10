
# Need to run bison first to generate valve.tab.h file which is included in output of flex (lex.yy.c).
# Build in a GNU bash shell v4.1.10(4)-release (i686-pc-cygwin) from Cygwin running under Windows 7

# 1 = debug build, 0 = release
myDebug := 1

.PHONY: all clean run

ifeq ($(myDebug), 1)
FLEX_FLAGS = -d --nounistd
#FLEX_FLAGS = --noline --nounistd
BISON_FLAGS = --verbose --debug --report=state
GCC_FLAGS = -g -Wall
GPP_FLAGS = -g -Wall
DEBUG_FLAGS = -D CYGWIN=1 -D DUMP=1 -D REGRESS_1=1
else    # not Debug
FLEX_FLAGS =
BISON_FLAGS = --verbose --debug --report=state
GCC_FLAGS =
GPP_FLAGS = 
DEBUG_FLAGS = -D CYGWIN=1
endif 

includeRTOS = ../../../../dev/docs/rtos/freertos/FreeRTOS/Demo/Common/drivers/LuminaryMicro/
includeGPIO = ../../../../DriverLib/src

#all: lextest.exe parsetest.exe lex.yy.c valve.tab.c run
all: lextest.exe parsetest.exe motorparsetest.exe lex.yy.c motor.yy.c valve.tab.c motor.tab.c interpret.exe run

clean:
	rm lex.yy.c
	rm valve.tab.c
	#rm valve.dot

lextest.exe: lex.yy.c
	gcc $(GCC_FLAGS) lex.yy.c -lfl -o lextest.exe
	
parsetest.exe: valve.tab.c valve.tab.h lex.yy.c
	echo "BUILDING PARSER"
	gcc $(GCC_FLAGS) -D BISON_PHASE valve.tab.c -lfl -o parsetest.exe
	
motorparsetest.exe: motor.tab.c motor.tab.h motor.yy.c
	echo "BUILDING MOTOR PARSER"
	gcc $(GCC_FLAGS) -D BISON_PHASE motor.tab.c -lfl -o motorparsetest.exe
	
lex.yy.c: valve.l Makefile valve.tab.h
	flex $(FLEX_FLAGS) valve.l		# flex version 2.5.35
	
motor.yy.c: motor.l Makefile motor.tab.h
	flex --nounistd --outfile=motor.yy.c motor.l		# flex version 2.5.35
	
valve.tab.c valve.tab.h:	valve.y compiler.h compilerHelper.h
	bison $(BISON_FLAGS) valve.y	# bison version 2.4.2

motor.tab.c motor.tab.h:	motor.y
	bison --verbose motor.y	# bison version 2.4.2

interpret.exe: interpret.cpp interpret.h ../motor/accel.cpp ../motor/accel.h ../motor/lmi.cpp ../motor/stepper.cpp ../motor/stepper.h ../motor/log.cpp ../motor/log.h compilerHelper.h parseTreeEntry.cpp parseTreeEntry.h symbolTableEntry.cpp symbolTableEntry.h tinyQueue.h valve.tab.h
	echo "BUILDING INTERPRETER"
	g++ $(GPP_FLAGS) $(DEBUG_FLAGS) -I. -I../motor -I$(includeRTOS) -I$(includeGPIO) interpret.cpp parseTreeEntry.cpp symbolTableEntry.cpp ../motor/accel.cpp ../motor/lmi.cpp ../motor/stepper.cpp ../motor/log.cpp -o interpret.exe 
    
run:
	./parsetest >parse.txt 2>bison.txt
	#mv log1.txt log2.txt
	#mv log.txt log1.txt
	rm log.txt  # Delete log file, otherwise results will be concatenated to the end of the file
	./interpret
	./motorparsetest
    
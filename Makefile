
# Need to run bison first to generate valve.tab.h file which is included in output of flex (lex.yy.c).
# Build in a GNU bash shell v4.1.10(4)-release (i686-pc-cygwin) from Cygwin running under Windows 7

# 1 = debug build, 0 = release
myDebug := 1

.PHONY: all clean

ifeq ($(myDebug), 1)
FLEX_FLAGS = -d
#FLEX_FLAGS =
BISON_FLAGS = --verbose --debug --report=state
GCC_FLAGS = -g
GPP_FLAGS = -Wall
DEBUG_FLAGS = -D CYGWIN=1 -D DUMP=1 -D REGRESS_1=1
else    # not Debug
FLEX_FLAGS =
BISON_FLAGS = --verbose --debug --report=state
GCC_FLAGS =
GPP_FLAGS = 
DEBUG_FLAGS = -D CYGWIN=1
endif 


all: lextest.exe parsetest.exe lex.yy.c valve.tab.c interpret.exe

clean:
	rm lex.yy.c
	rm valve.tab.c
	#rm valve.dot

lextest.exe: lex.yy.c
	gcc $(GCC_FLAGS) lex.yy.c -lfl -o lextest.exe
	
parsetest.exe: valve.tab.c valve.tab.h lex.yy.c
	gcc $(GCC_FLAGS) -D BISON_PHASE valve.tab.c -lfl -o parsetest.exe
	
lex.yy.c: valve.l Makefile valve.tab.h
	flex $(FLEX_FLAGS) valve.l		# flex version 2.5.35
	
valve.tab.c valve.tab.h:	valve.y compiler.h compilerHelper.h
	bison $(BISON_FLAGS) valve.y	# bison version 2.4.2

interpret.exe: interpret.cpp interpret.h ../motor/log.cpp ../motor/log.h compilerHelper.h parseTreeEntry.cpp parseTreeEntry.h
	g++ $(GPP_FLAGS) $(DEBUG_FLAGS) -I. -I../motor interpret.cpp parseTreeEntry.cpp ../motor/log.cpp -o interpret.exe 
    

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
else
FLEX_FLAGS =
BISON_FLAGS = --verbose --debug --report=state
GCC_FLAGS =
endif 


all: lextest.exe parsetest.exe lex.yy.c valve.tab.c

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
	
valve.tab.c valve.tab.h:	valve.y compiler.h
	bison $(BISON_FLAGS) valve.y	# bison version 2.4.2
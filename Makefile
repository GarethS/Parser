
# Need to run bison first to generate valve.tab.h file which is included in output of flex (lex.yy.c).
# Build in a GNU bash shell v4.1.10(4)-release (i686-pc-cygwin) from Cygwin running under Windows 7

.PHONY: all clean

# -v = verbose
# -l = no #line directives
#BISON_FLAGS = --verbose --debug --report=all
BISON_FLAGS = --verbose --debug --report=state

#FLEX_FLAGS = -d
FLEX_FLAGS =

all: lextest.exe parsetest.exe lex.yy.c valve.tab.c

clean:
	rm lex.yy.c
	rm valve.tab.c
	#rm valve.dot

lextest.exe: lex.yy.c
	gcc lex.yy.c -lfl -o lextest.exe
	
parsetest.exe: valve.tab.c valve.tab.h lex.yy.c
	gcc -D BISON_PHASE valve.tab.c -lfl -o parsetest.exe
	
lex.yy.c: valve.l Makefile
	flex $(FLEX_FLAGS) valve.l		# flex version 2.5.35
	
valve.tab.c valve.tab.h:	valve.y
	bison $(BISON_FLAGS) valve.y	# bison version 2.4.2
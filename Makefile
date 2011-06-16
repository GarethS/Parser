
# Need to run bison first to generate valve.tab.h file which is included in output of flex (lex.yy.c).

.PHONY: all clean

all: lextest.exe lex.yy.c valve.tab.c

clean:
	rm lex.yy.c
	rm valve.tab.c
	rm valve.dot

lextest.exe: lex.yy.c valve.tab.h
	gcc lex.yy.c -lfl -o lextest.exe
	
lex.yy.c: valve.l valve.tab.h
	flex -d valve.l
	
valve.tab.c valve.tab.h:	valve.y valve.l
	bison --verbose --debug --report=all valve.y
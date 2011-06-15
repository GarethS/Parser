
.PHONY: all clean

all: lex.yy.c valve.tab.c

clean:
	rm lex.yy.c
	rm valve.tab.c

lex.yy.c: valve.l
	flex -d valve.l
	
valve.tab.c:	valve.y
	bison -t --report=all valve.y
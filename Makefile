OBJECTS = main.o parse.o parseBF.o parseQQQ.o
bf : $(OBJECTS)
	gcc -O2 $(OBJECTS) -o bf

main.o : common.h parse.h
parse.o : common.h parse.h
parseBF.o : common.h parse.h
parseQQQ.o : common.h parse.h

.PHONY : clean
clean :
	rm bf $(OBJECTS)
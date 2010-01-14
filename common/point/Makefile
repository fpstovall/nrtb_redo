build:	common_test Makefile
	@echo build complete
	
common_test:	common_test.cpp triad.h Makefile
	@rm -vf triad.o
	g++ -O3 common_test.cpp  -idirafter . -o common_test
#	g++ -g common_test.cpp  -idirafter . -o common_test

clean:
	@rm -vf *.o common_test ../../include3/triad.h

lib:	common_test ../../include3/triad.h

../../include3/triad.h:	triad.h
	@cp -fv triad.h ../../include3



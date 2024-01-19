run:
	g++ -o num5.o num5.cpp -lgsl -lgslcblas -lm -I/path/to/boost/include -L/path/to/boost/lib -lboost_iostreams
	g++ -o check.o check.cpp -lgsl -lgslcblas -lm
	@echo
	@echo NUM5.CPP
	@./num5.o
	@echo 
	@echo CHECK.CPP
	@./check.o
	@echo 
	@make clean

clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
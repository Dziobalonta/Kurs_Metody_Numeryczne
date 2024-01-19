run:
	g++ -o num7.o num7.cpp -lgsl -lgslcblas -lm -I/path/to/boost/include -L/path/to/boost/lib -lboost_iostreams
	@./num7.o
	@echo 
	@make clean

clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
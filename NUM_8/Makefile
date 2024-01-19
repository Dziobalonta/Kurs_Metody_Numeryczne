runA:
	g++ -o num8_a.o num8_a.cpp -lgsl -lgslcblas -lm -I/path/to/boost/include -L/path/to/boost/lib -lboost_iostreams
	@./num8_a.o
	@echo 
	@make clean

runB:
	g++ -o num8_b.o num8_b.cpp -lgsl -lgslcblas -lm -I/path/to/boost/include -L/path/to/boost/lib -lboost_iostreams
	@./num8_b.o
	@echo 
	@make clean

clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
runA:
	g++ -c metody.cpp -o metody.o
	g++ -o num9_a.o num9_a.cpp -lm -I/path/to/boost/include -L/path/to/boost/lib -lboost_iostreams
	@./num9_a.o
	@echo 
	@make clean

runB:
	g++ -c metody.cpp -o metody.o
	g++ -o num9_b.o num9_b.cpp -lm -I/path/to/boost/include -L/path/to/boost/lib -lboost_iostreams
	@./num9_b.o
	@echo 
	@make clean

clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
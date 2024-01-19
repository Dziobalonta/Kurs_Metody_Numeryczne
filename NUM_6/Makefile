runA:
	g++ -o num6_a.o num6_a.cpp -lgsl -lgslcblas -lm -I/path/to/boost/include -L/path/to/boost/lib -lboost_iostreams
	g++ -o check_a.o check_a.cpp -lgsl -lgslcblas -lm
	@echo
	@echo NUM6_A.CPP
	@./num6_a.o
	@echo 
	@echo CHECK_A.CPP
	@./check_a.o
	@echo 
	@make clean

runB:
	g++ -o num6_b.o num6_b.cpp -lgsl -lgslcblas -lm -I/path/to/boost/include -L/path/to/boost/lib -lboost_iostreams
	g++ -o check_b.o check_b.cpp -lgsl -lgslcblas -lm
	@echo
	@echo NUM6_B.CPP
	@./num6_b.o
	@echo 
	@echo CHECK_B.CPP
	@./check_b.o
	@echo 
	@make clean

clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
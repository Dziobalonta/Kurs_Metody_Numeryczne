run:
	g++ -o num4.o num4.cpp -lgsl -lgslcblas -lm
	g++ -o check.o check.cpp -lgsl -lgslcblas -lm
	@echo
	@echo NUM4.CPP
	@./num4.o
	@echo 
	@echo CHECK.CPP
	@./check.o
	@echo 
	@make clean

runplot:
	g++ -o plot.o plot.cpp -lgsl -lgslcblas -lm -I/path/to/boost/include -L/path/to/boost/lib -lboost_iostreams
	@./plot.o 200 20000
	@echo data ready!
	@echo 
	@make clean

clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
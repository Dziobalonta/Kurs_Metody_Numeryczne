run:
	g++ -o num3.o num3.cpp
	g++ -o check.o check.cpp -lgsl -lgslcblas -lm
	@echo
	@echo NUM3.CPP
	@./num3.o
	@echo 
	@echo CHECK.CPP
	@./check.o
	@make clean

runplot:
	g++ -o plot.o plot.cpp -I/path/to/boost/include -L/path/to/boost/lib -lboost_iostreams
	@echo plotting data...
	@./plot.o $(DATA_AMOUNT)
	@echo data ready!
	@echo 
	@make clean

clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
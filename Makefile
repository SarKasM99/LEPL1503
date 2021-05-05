#Auteurs : Alicia & Alexandre 

CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=-lcunit -lpthread 
INCLUDE_HEADERS_DIRECTORY=-Iheaders -I${HOME}/local/include -L${HOME}/local/lib

kmeans: main.c src/distance.o src/file_functions.o src/Lloyd.o src/combination.o # add your other object files needed to compile your program here. !! The ordering is important !!  
	@echo Creating kmeans:
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LIBS) 
	@echo Compiling unit tests
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o  test/test test/test.c src/*.o $(LIBS)

%.o: %.c                  # if for example you want to compute example.c this will create an object file called example.o in the same directory as example.c. Don't forget to clean it in your "make clean"
	@echo Compiling $<:
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $<

clean:
	rm -f src/*.o
	rm -f kmeans
	rm -f test/test
tests: kmeans 
	@echo Launching unit tests:
	./test/test

memcheck: kmeans 
	@echo Valgrind test1
	valgrind ./kmeans -n 1 -p 10 -k 2 -f example.csv test/example.bin
	@echo
	@echo Valgrind test2
	valgrind ./kmeans -n 2 -p 10 -k 2 -f example.csv test/example2.bin
	@echo 
	@echo Valgrind test3
	valgrind ./kmeans -n 3 -p 10 -k 2 -f example.csv test/example_dim1.bin
	@echo 
	@echo Valgrind test4
	valgrind ./kmeans -n 4 -p 10 -k 2 -f example.csv test/example_dim3.bin
	@echo Valgrind test5
	valgrind ./test/test

# a .PHONY target forces make to execute the command even if the target already exists
.PHONY: clean tests

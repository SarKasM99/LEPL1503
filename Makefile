#Auteurs : Alicia & Alexandre 

CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=-lcunit -lpthread
INCLUDE_HEADERS_DIRECTORY=-Iheaders

kmeans: main.c src/distance.o src/file_functions.o src/Lloyd.o    # add your other object files needed to compile your program here. !! The ordering is important !!  
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LIBS)    # this will run the following command: gcc -Wall -Werror -g -o kmeans src/distance.o other_object_filespresent_above.o ... -lcunit -lpthread

%.o: %.c                  # if for example you want to compute example.c this will create an object file called example.o in the same directory as example.c. Don't forget to clean it in your "make clean"
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $<

clean:
	rm -f src/*.o
	rm -f kmeans
	rm -f test/test
tests: kmeans 
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o  test/test test/test.c src/*.o $(LIBS)
	./test/test

# a .PHONY target forces make to execute the command even if the target already exists
.PHONY: clean tests

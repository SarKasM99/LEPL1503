LEPL1503 - k-means problem

This project solves a clustering problem, more precisely it solves the k-means problem. 

How it works?

The program first reads the data from a binary file, where: \
-The first number is an unsigned 32 bits int that represents the dimension of the points used \
-The second number is an unsigned 64 bits int that represents the number of points in the data \
-The rest is a succession of signed 64 bits int that gives the coordinates of each point \
N.B.: The data in the binary file needs to be encoded in big endian order. The program will later convert to host byte order. 

Then the program applies the Lloyd's algorithm testing every combinations of initialisation of clusters possible among the n initialisation of points that is specified by the user (more details below). 

After solving the clustering problem with Lloyd's algorithm, the program stores the result in a csv file of each initialisation. Note that the program does not store in any particular order between the lines since the program uses threads in order to solve the problem faster. Here is an example of how an initialisation is stored: \
-First line is always: \
initialization centroids,distortion,centroids,clusters \
-And the rest are the results, where the each quotation represents (respectively) initialization centroids, distortion, centroids and data in each cluster (respectively to the order of centroids) \
"[(1, 1), (2, 2)]",11,"[(1, 1), (4, 5)]","[[(1, 1), (2, 2)], [(3, 4), (5, 7), (3, 5), (5, 5), (4, 5)]]" 

How to use? \
Use our makefile in order to simplify the compilation process: \
make kmeans - compiles and creates an executable file to solve the clustering problem \
make tests - does kmeans, then compiles our units tests and executes them \
make clean - removes all executables and object files 

Here are the parameters that can be used with the executable file (kmeans): \
-q if used, then the data in each cluster (the last column) in the csv file will not be shown \
-k n_clusters (default: 2) specifies the numbers of clusters  \
-p n_combinations (default: same as n_clusters) specifies the first number of points that needs to be tested for the clustering problem. \ For example, if n_clusters = 2 and n_combinations = 7, then the program will test C(n_combinations,n_clusters) combinations.  \
-n threads (default: 4) specifies the numbers of threads that needs to be used \
-d distance_metric (default: "manhattan") it's either "manhattan" or "euclidean". Specifies the distance formula needs to be used \
-f output_file (default: stdout) the path of the csv file where the results needs to be written \
input_filename (default: stdin) the path of the binary file that contains the data that needs to be processed. 

NB: The code is well documented, so we recommend you use vscode or doxygen, it will allow you to see easily how each structure and function works. The strategy and the use of threads is explained in the main function (in main.c). We will add some graphs in order to show the difference in performance.

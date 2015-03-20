# combn-thrust
ECS 158 Final Project. Parallelize the R combn function through Thrust

1. Run in CSIF @pc43
2. If running thrustcombn.cpp, change extension to *.cu
2. nvcc thrustcombn.cu . Use the -arch=sm_20 flag to invoke printf
3. Run a.out

To connect R to the CUDA backend, refer to http://heather.cs.ucdavis.edu/~matloff/rth.html#build

In the first stage, one runs a straight nvcc compile (since I've used .cpp suffixes throughout, you need to tell nvcc these are really CUDA files, via -x cu):

   export CPLUS_INCLUDE_PATH=/home/matloff/R/Rcpp/include
   nvcc -x cu -c rthsort.cpp -Xcompiler "-fpic" -I/usr/include/R
   
This produces rthsort.o, which you input to R CMD SHLIB. For that purpose, you must first indicate where the CUDA files are:

   export PKG_LIBS="-L/usr/local/cuda/lib64 -lcudart"
   R CMD SHLIB rthsort.o -o rthsort.so
   
Again, this produces rthsort.so.


To get Rcpp to work, use 
bash
export R_LIBS_USER=/home/<csif-login>/Rcpp
export PKG_LIBS="-lgomp"
export PKG_CXXFLAGS="-std=c++11 -fopenmp -I/home/<csif-login>/Rcpp/inst/include"
R CMD SHLIB thrustcombn.cpp


TODO: 3/19
1. Pass retMat = array of size pos[len[pos]] * k to device function -> This may need to be bigger 
2. In device function, find_comb, pass pos[i]*k as argument to be the size of the return array. 
3. As each thread goes through the function find_comb, it updates the respective section (should be ordered then)
4. return retMat to check if output printd. 

Revision to run program: 
1 Install or load rcpp using bash
2. Open R session and type library(Rcpp) to confirm installation 


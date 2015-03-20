1. Start a bash session
[skinamda@pc43 project]$ bash
export CPLUS_INCLUDE_PATH=/home/skinamda/R/x86_64-redhat-linux-gnu-library/3.1/Rcpp/include/
nvcc -x cu -c -arch=sm_20 thrustcombn.cpp -Xcompiler "-fpic"  -I/usr/include/R
export PKG_LIBS="-L/usr/local/cuda/lib64 -lcudart"
R CMD SHLIB thrustcombn.o -o thrustcombn.so
gcc -m64 -std=gnu99 -shared -L/usr/lib64/R/lib -Wl,-z,relro -o thrustcombn.so thrustcombn.o -L/usr/local/cuda/lib64 -lcudart -L/usr/lib64/R/lib -lR
 export LD_LIBRARY_PATH=/usr/local/cuda/lib64

Then start R session 
>library(Rcpp)
source('/path/to/thrustcombn.R')
combn(x,m)



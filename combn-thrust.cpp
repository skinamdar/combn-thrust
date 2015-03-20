// .cu
#include <iostream>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/random.h>
#include <stdlib.h>
#include <thrust/transform.h>
#include <stdio.h>
#include <boost/math/special_functions/binomial.hpp>

#include <Rcpp.h>

using namespace std; 
using namespace Rcpp;


struct comb {
	
	const thrust::device_vector<int>::iterator x;
	const thrust::device_vector<int>::iterator pos;
	const thrust::device_vector<int>::iterator retmat;
	int n;
	int m;
	int *x_arr, *position, *ret;


	comb(thrust::device_vector<int>::iterator _x_arr, thrust::device_vector<int>::iterator _pos, int _n, int _m, thrust::device_vector<int>::iterator _retmat):
		x(_x_arr),
		pos(_pos),
		n(_n),
		m(_m),
		retmat(_retmat)
	{
		x_arr = thrust::raw_pointer_cast(&x[0]);		
		position = thrust::raw_pointer_cast(&pos[0]);
		ret = thrust::raw_pointer_cast(&retmat[0]);
	}
	
	__device__
	void operator()(int i)
	{
//		printf("pos: ");
//		for(int i = 0; i < m; i++){
	//		printf("operator: %d\n", i);
//		}

        if(i <= n - m) {
      //      printf("operatorinside: %d\n", i);
            find_comb(i, x_arr, m, n, position, ret);
        }
	}
	__device__
	void store(int *pos, int *output, int idx, int m, int *x, int *comb, int &outidx){
        printf("Store idx: %d and outidx is: %d\n",idx, outidx);
		for(int i = 0; i < m; i++){
			output[outidx++] = x[comb[i]+idx];
		}

	}

	__device__
	void find_comb(int idx, int *x, int m, int n, int *pos, int *output){
		//printf("%d, %d\n", i, x[i]);
		//printf("m = %d\n", m);
		int *comb = new int[m];
		for(int i = 0; i < m; i++){
			comb[i] = i;
		}
		int new_n= n - idx;
        
        int outidx = pos[idx];
		
	//	printf("pos = ");
	//	for(int i = 0; i < m; i++){
	//		printf("%d ", pos[i]);
	//	}
	//	printf("\n");


	/*
		printf("the x inside = ");
		for(int i = 0; i < n; i++){
			printf("%d ", x[i]);
		}
		printf("\n");	
	*/
	//	printf("	%d %d %d\n", comb[0], comb[1], comb[2]);		
	//	printf("index %d has n = %d\n", idx, new_n);
	
		printf("ANSWER! %d %d %d wtth index: %d\n", x[comb[0] + idx], x[comb[1]+idx], x[comb[2]+idx], idx);
        
		store(pos, output, idx, m, x, comb, outidx);
        
        printf("outidx! is: %d \n", outidx);

	
		// store into 


	//	printf("after 1st one %d %d %d\n", comb[0], comb[1], comb[2]);		
	
	//	while(next_comb(comb, m, new_n)){
	//		printf("inside whiel?");
	//		printf("%d %d %d\n", x[comb[0]], x[comb[1]], x[comb[2]]);
	//		printf("		%d %d %d\n", comb[0], comb[1], comb[2]);		
	//	}

		while(true){
	//		printf(" inside comb is: %d %d %d, i = nothing\n", comb[0], comb[1], comb[2]);
			int i = m - 1;
			++comb[i];
	
	//		printf("	after ++comb, comb is %d %d %d, i = %d\n", comb[0], comb[1], comb[2], i);
			while((i >= 0) && (comb[i] >= new_n - m + 1 + i)){
				--i;
				++comb[i];
			}
	//		printf("	after while, comb is %d %d %d, i = %d\n", comb[0], comb[1], comb[2], i);
			if(comb[0] == 1){
				break;
			}
	//		printf("	after if, comb is %d %d %d, i = %d\n", comb[0], comb[1], comb[2], i);
			for(i = i + 1; i < m; ++i){
				comb[i] = comb[i-1] + 1;
			}
	//		printf("	after for, comb is %d %d %d, i = %d\n", comb[0], comb[1], comb[2], i);
			//return 1;
		
			printf("ANSWER %d %d %d wtih index %d \n", x[comb[0]+idx], x[comb[1]+idx], x[comb[2]+idx],idx);
			//store(pos, output, idx, m, x, comb);
            store(pos, output, idx, m, x, comb, outidx);
            printf("outidx2 is: %d \n", outidx);
		}
	}

};

RcppExport SEXP combn(SEXP x_, SEXP m_, SEXP n_, SEXP nCm_, SEXP out){
	NumericVector x(x_);
	int m = as<int>(m_), n = as<int>(n_), nCm = as<int>(nCm_);
	NumericMatrix retmat(m, nCm);

    // keeps track of the position in output
    int *pos = new int[n-m+1];
    
    // Calculate combination possibilities for each element in the list that
    // start with the element in the 0th index
    int tmp_n = n; // Why do we need a tmp_n ??
    int k = 0;
    for(int i = 0; i < (n-m+1); i++){
        pos[i] = boost::math::binomial_coefficient<double>(tmp_n - i - 1, m-1);
        k++;
    }
    
    int temp = pos[0]*m;
    int temp2;
    pos[0]=0;
    for (int j=1; j<(n-m+1); j++){
        temp2 = pos[j];
        pos[j]=temp;
        temp=pos[j]+(m*temp2);
    }
    
    /* temp <- pos[1]
    pos[1] <- 0
    for (i in 2:length(pos)) {
        temp2 <- pos[i]
        pos[i] <- temp
        temp <- pos[i] + temp2
    }*/
    
    for (int i=0; i<(n-m+1); i++){
        printf("pos [%d] = %d \n:",i,pos[i]);
    }
	
	thrust::device_vector<int> d_x(x.begin(), x.end());
	//thrust::device_vector<int> d_pos(pos.begin(), pos.end());
    thrust::device_vector<int> d_pos(pos, pos + (n-m+1));
	thrust::device_vector<int> d_mat(retmat.begin(), retmat.end());
    
   
    
    
   


//	thrust::device_vector<int> d_c(comb_arr, comb_arr + m);
//	for(int i = 0; i < m; i++){
//		printf("%d %d %d", comb_arr[0], comb_arr[1], comb_arr[2]);
//	}

	thrust::counting_iterator<int> begin(0);
	thrust::counting_iterator<int> end = begin + n;

	
	thrust::for_each(begin, end, comb(d_x.begin(), d_pos.begin(), n, m, d_mat.begin()));

	thrust::copy(d_mat.begin(), d_mat.end(), retmat.begin());
	
	return retmat;
    
}

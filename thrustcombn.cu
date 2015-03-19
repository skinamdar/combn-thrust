// .cu
#include <iostream>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/random.h>
#include <stdlib.h>
#include <thrust/transform.h>
#include <stdio.h>
#include <boost/math/special_functions/binomial.hpp>

using namespace std;

__device__
int next_comb(int *comb, int m, int n){
    printf("Inside next_comb\n");

    int i = m - 1;
    ++comb[i];
    
    while((i >= 0) && (comb[i] >= n - m + 1 - i)){
        --i;
        ++comb[i];
    }
    if(comb[0] == 1){
        return 0;
    }
    for(i = i + 1; i < m; ++i){
        comb[i] = comb[i-1] + 1;
    }
    return 1;
}

__device__
void find_comb(int idx, int *x, int m, int n, int*pos){
    printf("Inside find_comb");

    // test pos passed correctly
     for(int i = 0; i < n-m+1; i++){
        printf("Dpos [%d] = %d \n",i,pos[i]);
    } 

    //printf("%d, %d\n", i, x[i]);
    //printf("m = %d\n", m);
    int *comb = new int[m];
    for(int i = 0; i < m; i++){
        comb[i] = i;
    }
    int new_n= n - idx;
    
    /*
     printf("the x inside = ");
     for(int i = 0; i < n; i++){
     printf("%d ", x[i]);
     }
     printf("\n");
     */
    //	printf("	%d %d %d\n", comb[0], comb[1], comb[2]);
    printf("index %d has n = %d\n", idx, new_n);
    
    printf("ANSWER %d %d %d\n", x[comb[0] + idx], x[comb[1]+idx], x[comb[2]+idx]);
    printf("after 1st one %d %d %d\n", comb[0], comb[1], comb[2]);
    
    //	while(next_comb(comb, m, new_n)){
    //		printf("inside whiel?");
    //		printf("%d %d %d\n", x[comb[0]], x[comb[1]], x[comb[2]]);
    //		printf("		%d %d %d\n", comb[0], comb[1], comb[2]);
    //	}
    
    while(true){
        printf(" inside comb is: %d %d %d, i = nothing\n", comb[0], comb[1], comb[2]);
        int i = m - 1;
        ++comb[i];
        
        printf("	after ++comb, comb is %d %d %d, i = %d\n", comb[0], comb[1], comb[2], i);
        while((i >= 0) && (comb[i] >= new_n - m + 1 + i)){
            --i;
            ++comb[i];
        }
        printf("	after while, comb is %d %d %d, i = %d\n", comb[0], comb[1], comb[2], i);
        if(comb[0] == 1){
            break;
        }
        printf("	after if, comb is %d %d %d, i = %d\n", comb[0], comb[1], comb[2], i);
        for(i = i + 1; i < m; ++i){
            comb[i] = comb[i-1] + 1;
        }
        printf("	after for, comb is %d %d %d, i = %d\n", comb[0], comb[1], comb[2], i);
        //return 1;
        
        printf("ANSWER %d %d %d\n", x[comb[0]+idx], x[comb[1]+idx], x[comb[2]+idx]);
    }
}


struct comb {
    
    const thrust::device_vector<int>::iterator x;
    const thrust::device_vector<int>::iterator r;//might use this to store result?
    const thrust::device_vector<int>::iterator pos; 
    int n;
    int m;
    int *x_ptr, *r_ptr, *pos_ptr;
    int *comb_arr;
    
    comb(thrust::device_vector<int>::iterator _x, thrust::device_vector<int>::iterator _r, int _n, int _m,thrust::device_vector<int>::iterator _pos):
    x(_x),
    r(_r),
    n(_n),
    m(_m),
    pos(_pos)
    {
        x_ptr = thrust::raw_pointer_cast(&x[0]);
        r_ptr = thrust::raw_pointer_cast(&r[0]);
        pos_ptr = thrust::raw_pointer_cast(&pos[0]);
    }
    
    __device__
    void operator()(int i)
    {
        if(i <= n - m)
            //	printf("%d ", i);
            find_comb(i, x_ptr, m, n,pos_ptr);
    }
};

void combn(int*x, int n, int m, int *comb_arr, int *result, int nCm, int*pos ){
    //void combn(int *x, int n, int m, vector<int> result){
    
    thrust::device_vector<int> d_x(x, x+n);
    thrust::device_vector<int> d_r(result, result + (nCm * m));
    thrust::device_vector<int> d_pos(pos, pos + (n-m+1));
    
    /*
     thrust::device_vector<int> d_c(comb_arr, comb_arr + m);
     for(int i = 0; i < m; i++){
     printf("%d %d %d", comb_arr[0], comb_arr[1], comb_arr[2]);
     }
     */
    
    thrust::counting_iterator<int> begin(0);
    thrust::counting_iterator<int> end = begin + n;
    
    
    //	thrust::transform(begin, end, d_r.begin(), comb(d_x.begin(), d_r.begin(), n, m));
    thrust::for_each(begin, end, comb(d_x.begin(), d_r.begin(), n, m, d_pos.begin()));
    
    //thrust::copy(d_r.begin(), d_r.end(), result);
}



int main(){
    int n = 5;
    int m = 3;
    
    int x[n];
    int *result;
    
    // Count the number of possible combinations 
    int nCm = boost::math::binomial_coefficient<double>(n, m);

    // Size of output array 
    result = new int[nCm * m];
    
    // keeps track of the position in output
    int *pos = new int[n-m+1];	
    
    // test input array 
    cout << "x = ";
    for(int i=0; i<n; i++)
    {
        //x[i] = rand() % 5;
        x[i] = i; 
        cout << x[i] << " ";
    }
    cout << endl;
    
    int *comb_arr = new int[m];
    for(int i = 0; i < m; i++){
        comb_arr[i] = i;
    }
    

    // Calculate combination possibilities for each element in the list that 
    // start with the element in the 0th index 
    int tmp_n = n; // Why do we need a tmp_n ??
    int k = 0;
    for(int i = 0; i < (n-m+1); i++){
        pos[i] = boost::math::binomial_coefficient<double>(tmp_n - i - 1, m-1);
        k++;
    }
     
    // Print for testing purposees 
    cout << "nCm = " << nCm << endl;
    cout << "k = " << k << endl;
    cout << "n-m+1 = " << n-m+1 << endl;
    for(int i = 0; i < n-m+1; i++){
        cout << "pos[" << i << "] = " << pos[i] << endl;
    } 
    
  
    //combn(x, n, m, comb_arr, result, nCm);
    combn(x, n, m, comb_arr, result, nCm,pos);
    
    cout << "result = ";
    for(int i = 0; i < n; i++){
        cout << result[i] << " ";
    }
    cout << endl;
    return 0;
    
    
}

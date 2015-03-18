// .cu
// goal is to send numbers 1 to 5 to device.
// mulitply by 2
// send back to get basic program working

#include <iostream>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/generate.h>
#include <thrust/random.h>

//#include <stdio.h>
using namespace std; 
using namespace thrust;

/*
int randnum() { 
	return rand() % 20;
}

struct combination{

};
void combn(float *x, int n, int m, float *result){
	//copy host to device
	host_vector<float> hx(x, x+n);

	device_vector<float> dx = hx;
	device_vector<float> dr = (result, result + (n * m));

	copy(dr.begin(), dr.end(), result);

} */

struct mult
{
    
    
    const thrust::device_vector<int>::iterator x;
    const thrust::device_vector<int>::iterator m;
    int *x1,*m1;
    int n;
    
    mult(thrust::device_vector<int>::iterator _x,
         thrust::device_vector<int>::iterator _m,
         int _n):
        x(_x),m(_m),n(_n)
    {
        x1 = thrust::raw_pointer_cast(&x[0]);
        m1 = thrust::raw_pointer_cast(&m[0]);
    }
    
    // each thread operates on a different ith element of the x array
    __device__ void operator() (const int i)
    {
        //for (int j=0; j<n; j++)
        {
            
        }
        
        m1[i]=2*x[i];
    }
};

void test(int *x, int *m, int n){
    
    // allocate host vector
    thrust::host_vector<int>hx(x,x+n);
    thrust::host_vector<int>hm(m,m+n);
    
    // allocate device vector
    thrust::device_vector<int>dx=hx;
    thrust::device_vector<int>dm(m,m+n);
    
  /*   thrust::counting_iterator<int>begin(0);
    thrust::counting_iterator<int>end=begin+n;
    thrust::for_each(begin,end,mult(dx.begin(),dm.begin(),n));
    
    // transfer data back to host
    thrust::copy(dm.begin(),dm.end(),hm.begin());
    
    
    // For testing purposes, printing out the different arrays:
    
     cout << "HX: " << endl;
     // check what's in hx
     for(int i = 0; i < hx.size(); i++)
     cout << hx[i] << ", ";
     
    cout << endl << "HM: " << endl << endl;
     // check output vector
     for(int i = 0; i < hm.size(); i++)
     cout << hm[i] << ", "; */
     

    
    
}

int main(){
	int n = 5;
	int m = 3;

    int *hx = (int *) malloc(n*sizeof(int));
    int *hm = (int *) malloc(n*sizeof(int));
    
    for(int i=0; i<n; i++)
    {
        hx[i] = i;
		cout << hx[i] << " ";
	}
	cout << endl;

	//combn(x, n, m);
    
    test(hx,hm,n);

	return 0;


}

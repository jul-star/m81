#include <iostream>
#include <omp.h>
#include <math.h>

void vars(int A, int B, int C, int i, int n);
void loops();
void barriers();

int main(int c , char **v)
{
  omp_set_num_threads(4);
  #pragma omp parallel //num_threads(4)
  {
  std::cout << omp_get_thread_num()  << " " << "Hello world!" << std::endl;
  }
  std::cout << omp_get_thread_num() << std::endl;
  vars(1,2,3,4,5);
  loops();
  return 0;
}


void vars(int A, int B, int C, int i, int n)
{
  std::cout << "***VARS***"<<std::endl;
    std::cout<< omp_get_thread_num()<<"A=" <<A <<", B=" << B << ", C="<<C<<", i=" <<i << ", n=" << n << std::endl;
  omp_set_num_threads(2);
#pragma omp parallel shared(A,B,C) private(i,n)
  {
    ++A; ++B; ++C; ++i;++n;
    std::cout<< omp_get_thread_num()<<"A=" <<A <<", B=" << B << ", C="<<C<<", i=" <<i << ", n=" << n << std::endl;
  }
}

void loops()
{
  std::cout << "***LOOPS***"<<std::endl;
  double sum=0;
  long int i;
  long int edge = 1e9;
  double cnv = 3.141527/180;
// pragma omp parallel for reduction(+:sum) num_threads(10)
  for (i=0; i < edge;  i++)
  {
    sum += sin(i*cnv);    
  }
  
  std::cout<<"Sum=" <<sum<<std::endl;
}

void barriers()
{
#pragma omp parallel 
  {
    std::cout<< 1 << std::endl;
    std::cout <<2 << std::endl;
#pragma omp barrier
    std::cout <<3 << std::endl;
  }
}

#include <iostream>
#include <omp.h>
#include <math.h>

void vars(int A, int B, int C, int i, int n);
void loops();
void barriers();
void single();
void order();
void critical();
void atomic();
void threads();
void task();

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
  barriers();
  single();
  order();
  critical();
  atomic();
  threads();
  task();

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
  long int edge = 1e5; // 1e9;
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
  std::cout << "***BARRIER***"<<std::endl;
#pragma omp parallel 
  {
    std::cout<< 1 << std::endl;
    std::cout <<2 << std::endl;
#pragma omp barrier
    std::cout <<3 << std::endl;
  }
}
void single()
{
  std::cout << "***SINGLE***"<<std::endl;
#pragma omp parallel num_threads(4)
  {
std::cout <<omp_get_thread_num()<<": " << 1 << std::endl; 
#pragma omp single
    {
      std::cout<<omp_get_thread_num()<<": " << 2 << std::endl;
    } // hidden barrier

    std::cout <<omp_get_thread_num()<<": " << 3  << std::endl; 
  }
}
void order()
{
  std::cout << "***ORDER***"<<std::endl;
  int edge = 1e1;
  int i;
  #pragma omp parallel for ordered num_threads(4)
  for(i=0; i<edge;++i)
  {
    std::cout << omp_get_thread_num()<< ": " << "out" << std::endl; 
    #pragma omp ordered
    {
      std::cout << omp_get_thread_num()<< ": " << i <<std::endl;
    }
  }
}
void critical()
{
  std::cout << "***CRITICAL***"<<std::endl;
    int i=0;
   #pragma omp parallel num_threads(4)
  {
    ++i;
    #pragma omp critical
    {
      std::cout << omp_get_thread_num()<< ": " << i <<std::endl;
    }
  }
}

void atomic()
{
  std::cout << "***ATOMIC***"<<std::endl;
    int i=0, j=0;
   #pragma omp parallel num_threads(4)
  {
    ++i;
    #pragma omp atomic
      ++j;
    std::cout << omp_get_thread_num()<< ": out:" << i<<"in: "<< j <<std::endl;
  }
}

void threads()
{
  std::cout << "***THREAD/SECTION***"<<std::endl;
    int i;
#pragma omp sections 
  {
    #pragma omp section 
    {
      for(i=0;i<10;++i)
        std::cout << omp_get_thread_num() << ": "<< i <<std::endl;
    }
  #pragma omp section
    {
      for(i=0;i<10;++i)
        std::cout << omp_get_thread_num() << ": "<< i <<std::endl;
    }
  }
}

void task()
{
  std::cout << "***TASK***"<<std::endl;
  int i;
  #pragma omp task
  {
      for(i=0;i<10;++i)
        std::cout << omp_get_thread_num() << ": "<< i <<std::endl;
  }
  std::cout << "end of task 1" << std::endl;
  #pragma omp task
  {

      for(i=0;i<10;++i)
        std::cout << omp_get_thread_num() << ": "<< i <<std::endl;
  }
  // barrier
  #pragma omp taskwait
  std::cout << "behind barrier" << std::endl;
}

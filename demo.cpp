#include<iostream>
#include <ctime>
using namespace std;

void f() {
  clock_t begin = clock();
  for(int i=0;i<1e6;i++) cout<<i<<endl;
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  cout<<elapsed_secs<<endl;
}
int main()
{
  f();
  return 0;
}

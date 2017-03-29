#include <vector>
#include <algorithm>
#include <set>
#include <streambuf>
#include <ostream>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <CkSpider.h>
#include "scheduler.hpp"
#include "crawler.hpp"
using namespace std;
int main()
{
	srand(time(NULL));
  Crawler c;
  vector<string> v;
  c.SetFolder("Collection/");
  
  ifstream ifs("seeds.txt",ifstream::in);

  while(ifs.good()){
    string s;
    getline(ifs,s);
    v.push_back(s);
  }
  ifs.close();

  c.PoeNaRoda(v);
  
  
  cout<<"Starting to crawl"<<endl;
  c.Start(21);
	cout<<"ACABOU JOVEM!"<<endl;
}

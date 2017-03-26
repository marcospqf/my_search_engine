#include <vector>
#include <algorithm>
#include <set>
#include <streambuf>
#include <ostream>
#include <fstream>
#include<iostream>
#include <cstdio>
#include <cstdlib>
#include<CkSpider.h>
#include "scheduler.hpp"
#include "crawler.hpp"
using namespace std;
int main()
{
	srand(time(NULL));
	Crawler c;
	c.SetFolder("./Collection/");
	cout<<"Starting to crawl"<<endl;
	c.Start(1);
	cout<<"ACABOU JOVEM!"<<endl;
}

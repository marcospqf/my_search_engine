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
	Crawler c;
	c.Start(10);
}

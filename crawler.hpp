#ifndef _crawler_hpp
#define _crawler_hpp

#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <streambuf>
#include <iostream>
#include <CkSpider.h>
#include <sstream>
#include "scheduler.hpp"
using namespace std;


class Crawler
{
	public:

		Crawler();
		// Starts to crawl
		void Start(int nthreads);

	private:

		// Name of the folder that will keep the pages crawled
		static string folderName;

		static mutex scheduler_mutex;
		// Number of threads the crawler will create in order to collect the pages
		int nthreads;
		
		void Crawl();

};



#endif

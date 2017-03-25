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
#include "FileWriter.hpp"
using namespace std;


class Crawler
{
	public:

		Crawler();
		
		void Start(int nthreads);

		void SetFolder(string s);

		static int ncraw;

	private:

		// Name of the folder that will keep the pages crawled
		static string folderName;

		static mutex scheduler_mutex;
		
		static mutex crawlado;
		
		int nthreads;
		
		static string foldername;

		void Crawl();

};



#endif

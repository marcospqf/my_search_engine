#ifndef _crawler_hpp
#define _crawler_hpp

#include <iomanip>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <streambuf>
#include <iostream>
#include <CkSpider.h>
#include <sstream>
#include <time.h>
#include "scheduler.hpp"
#include "FileWriter.hpp"
using namespace std;


class Crawler
{
	public:

		Crawler();
		
		//Start crawnling..
		void Start(int nthreads);

		//Set the folder the Html and Urls will be located
		void SetFolder(string s);

		//Set the initial time to measure how 
		//fast we are crowling;

    void PoeNaRoda(vector<string> &v);
		// number of Url crawlead
		static int ncraw;

	private:
		
		//lock of tread when we access the queue		
		static mutex scheduler_mutex;
		
		// lock of ncraw
		static mutex crawlado;
		
		int nthreads;
		
		// Name of the folder that will keep the pages crawled
		static string foldername;
		
		void Crawl();

};



#endif

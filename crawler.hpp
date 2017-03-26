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
		
		//Start crawnling..
		void Start(int nthreads);

		//Set the folder the Html and Urls will be located
		void SetFolder(string s);

		// number of Url crawlead
		static int ncraw;

	private:

		// Name of the folder that will keep the pages crawled
		static string folderName;
		
		//lock of tread when we access the queue		
		static mutex scheduler_mutex;
		
		// lock of ncraw
		static mutex crawlado;
		
		int nthreads;
		
		static string foldername;

		void Crawl();

};



#endif

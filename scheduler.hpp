#ifndef _scheduler_hpp
#define _scheduler_hpp
#include <vector>
#include <algorithm>
#include <set>
#include <streambuf>
#include <ostream>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <unordered_map>
#include <queue>
using namespace std;

class Scheduler
{
	public:
		
		Scheduler();

		//push na priorityqueue, 
		//inner=true, push insideurl, else outsideurl
		static void PushUrl(const string &s,const string &domain, bool inside);
		
		//remove the topmost Url, if the queue is empty, nothing happens
		static void PopUrl(bool inside);
		
		// return if the queue is empty
		static bool IsEmpty(bool inside);
		
		//return the next query to be processed, if the queue is empty, return ""
		static string TopUrl();

    static bool thistime(double x);

	private:
		struct url {
			string name,domain;
			int priority;
			int sizeurl;
			url(const string &s, const string &d);
			bool operator<(const url &a) const;
		};
		
		static long long peso[100000];

		static set<url> inside_url;
		static set<url> outside_url;
		
    //just a simple polinomial hash!
    static unordered_set<long long> visited;
    //last time that occur a requisition on a domain x
    static unordered_map<long long , double > last_time;
};
#endif

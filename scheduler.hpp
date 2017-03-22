#ifndef _scheduler_hpp
#define _scheduler_hpp
#include<bits/stdc++.h>
#include<unordered_set>
using namespace std;

class Scheduler
{
	public:
		
		Scheduler();

		//push na priorityqueue, 
		//inner=true, push insideurl, else outsideurl
		static void PushUrl(const string &s, bool inside);
		
		//remove the topmost Url, if the queue is empty, nothing happens
		static void PopUrl(bool inside);
		
		// return if the queue is empty
		static bool IsEmpty(bool inside);
		
		//return the next query to be processed, if the queue is empty, return ""
		static string TopUrl();


	private:
		struct url {
			string name;
			int priority;
			url(const string &s);
			bool operator<(const url &a) const;
		};
		static set<url> inside_url;
		static set<url> outside_url;
		static unordered_set<long long> visited;
};
#endif

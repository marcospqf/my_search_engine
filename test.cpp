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
using namespace std;
string html,url;
Scheduler Queue;
void ChilkatSample(ostream &os)
{
	CkSpider spider;

	//  The spider object crawls a single web site at a time.  As you'll see
	//  in later examples, you can collect outbound links and use them to
	//  crawl the web.  For now, we'll simply spider 10 pages of chilkatsoft.com
	string s="globo.com";
	Queue.PushUrl(s.c_str(),false);
	//  Begin crawling the site by calling CrawlNext repeatedly.
	int cont=0;
	while(true){
		string next=Queue.TopUrl();
		cout<<next<<endl;
		spider.Initialize(next.c_str());
		spider.AddUnspidered(next.c_str());

		bool success = spider.CrawlNext();
		cont++;
		//if(cont>100) break;
		if (success) {
			//  Show the URL of the page just spidered.
			os << spider.lastUrl() << "\n";
			//os << spider.lastHtml() << "\n";
			//  The HTML is available in the LastHtml property
			int size=spider.get_NumOutboundLinks();
			set<int> jafoi;
			while(jafoi.size()<min(size,5)){
				int x=rand()%size;
				if(!jafoi.count(x)) jafoi.insert(x),Queue.PushUrl(spider.getOutboundLink(x),false);
			}
			spider.ClearOutboundLinks();
			size=spider.get_NumUnspidered();
			while(jafoi.size()<min(size,5)){
				int x=rand()%size;
				if(!jafoi.count(x)) {
					jafoi.insert(x);
					CkString josias;
					spider.GetUnspideredUrl(x,josias);
					Queue.PushUrl(josias.getString(),true);
				}
			}
		}
	}
}

int main()
{
	filebuf fb;
	fb.open ("test.txt",std::ios::out);
	ostream os(&fb);
	os << "Test sentence\n";
	ChilkatSample(os);
	fb.close();
	return 0;
}

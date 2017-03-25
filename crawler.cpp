#include "crawler.hpp"
using namespace std;

std::mutex Crawler::scheduler_mutex;
Crawler::Crawler() 
{
	int x=rand()%3;
	Crawler::scheduler_mutex.lock();
	
	if(x==0)
	Scheduler::PushUrl("www.globo.com",false);
	if(x==1)
	Scheduler::PushUrl("www.uol.com",false);
	if(x==2)
	Scheduler::PushUrl("www.terra.com",false);
	
	Crawler::scheduler_mutex.unlock();
}

void Crawler::Start(int nthreads)
{
	cout<<"Starting to crawl.."<<endl;
	vector<thread> running;
	for(int i=0;i<nthreads;i++){
		running.push_back(thread(&Crawler::Crawl,this));
	}

	for(auto it=running.begin();it!=running.end();it++){
		it->join();
	}
	this->nthreads=nthreads;
}

void Crawler::Crawl()
{
	CkSpider spider;
	CkString collectedUrl,collectedHtml;
	
	stringstream ss;
	ss << this_thread::get_id();
	string filename ="Html"+ss.str();
	filebuf fb;
	fb.open(filename,ios::out);
	ostream os(&fb);
	
	while(true){
		scheduler_mutex.lock();
		string nextUrl= Scheduler::TopUrl();
		Crawler::scheduler_mutex.unlock();
		if(nextUrl.size()==0) continue;

		spider.Initialize(nextUrl.c_str());
		spider.AddUnspidered(nextUrl.c_str());
		
		if(spider.CrawlNext())
		{
			spider.get_LastUrl(collectedUrl);
			spider.get_LastHtml(collectedHtml);
			os<<collectedUrl.getString()<< " | "<<collectedHtml.getString()  << "|||" ;
			cout<<"COLETANDO: "<<endl;
			cout<<collectedUrl.getString()<<endl;
			int size=spider.get_NumOutboundLinks();
			set<int> jafoi;
			while(jafoi.size()<min(size,15)){
				int x=rand()%size;
				if(!jafoi.count(x)) {
					jafoi.insert(x);
					Crawler::scheduler_mutex.lock();
					Scheduler::PushUrl(spider.getOutboundLink(x),false);
					Crawler::scheduler_mutex.unlock();
				}

			}
			jafoi.clear();
			spider.ClearOutboundLinks();
			size=spider.get_NumUnspidered();
			
			while(jafoi.size()<min(size,5)){
				int x=rand()%size;
				if(!jafoi.count(x)){
					jafoi.insert(x);
					CkString Next;
					spider.GetUnspideredUrl(x,Next);

					Crawler::scheduler_mutex.lock();
					Scheduler::PushUrl(Next.getString(),true);
					Crawler::scheduler_mutex.unlock();

				}
			}

			int unspidered = spider.get_NumUnspidered();
			for(int i = 0; i < unspidered; i++)
			{
				spider.SkipUnspidered(0);
			}
		}
		spider.ClearOutboundLinks();
	   spider.ClearSpideredUrls();
	   spider.ClearFailedUrls();

	}
}

#include "crawler.hpp"
using namespace std;

mutex Crawler::scheduler_mutex;
mutex Crawler::crawlado;
int Crawler::ncraw=0;
string Crawler::foldername;

Crawler::Crawler() 
{
	Crawler::scheduler_mutex.lock();
	Scheduler::PushUrl("http://uol.com",false);
	Scheduler::PushUrl("http://g1.com.br",false);
	Scheduler::PushUrl("http://r7.com.br",false);
	Scheduler::PushUrl("http://ig.com.br",false);
	Scheduler::PushUrl("http://br.leagueoflegends.com",false);
	Scheduler::PushUrl("http://pt.wikipedia.org",false);
	Crawler::scheduler_mutex.unlock();
}

void Crawler::SetFolder(string s){
	Crawler::foldername=s;
}

void Crawler::Start(int nthreads)
{
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
	FileWriter fw;
	CkSpider spider;
	CkString collectedUrl,collectedHtml;

	stringstream ss;
	ss << this_thread::get_id();
	string filename =this->foldername+"mp"+ss.str()+".txt";
	fw.SetFilename(filename);
	fw.OpenStream();

	while(true){
		if(ncraw>1e6) break;

		Crawler::scheduler_mutex.lock();
		string nextUrl= Scheduler::TopUrl();
		Crawler::scheduler_mutex.unlock();

		if(nextUrl.size()==0) 
			continue;
		CkString x;
		if(!spider.GetUrlDomain(nextUrl.c_str(),x)) continue;

		spider.Initialize(x);
		spider.AddUnspidered(nextUrl.c_str());

		if(spider.CrawlNext())
		{
			spider.get_LastUrl(collectedUrl);
			spider.get_LastHtml(collectedHtml);
			fw.print(collectedUrl,collectedHtml);

			Crawler::crawlado.lock();
			ncraw++;
			std::cout << ncraw << ": "<< collectedUrl.getString() << std::endl;
			Crawler::crawlado.unlock();

			int sz=spider.get_NumOutboundLinks();
			for(int x=0;x<sz;x++){
				string Url=spider.getOutboundLink(x);
//				Url=spider.canonicalizeUrl(Url.c_str());
				Crawler::scheduler_mutex.lock();
				Scheduler::PushUrl(Url,false);
				Crawler::scheduler_mutex.unlock();
			}

			sz=spider.get_NumUnspidered();
			for(int x=0;x<sz;x++){
				CkString Next;
				spider.GetUnspideredUrl(0,Next);
				spider.SkipUnspidered(0);

				string Url=spider.canonicalizeUrl(Next.getString());
				//string Url=Next.getString();
				Crawler::scheduler_mutex.lock();
				Scheduler::PushUrl(Url,true);
				Crawler::scheduler_mutex.unlock();
			}

/*			int unspidered = spider.get_NumUnspidered();
			for(int i = 0; i < unspidered; i++)
			{
				spider.SkipUnspidered(0);
			}*/
			spider.ClearOutboundLinks();
			spider.ClearSpideredUrls();
			spider.ClearFailedUrls();
		}
		else{
			Crawler::crawlado.lock();
			CkString x;
			spider.get_LastErrorText(x);
//			cout<<x.getString()<<endl;
			Crawler::crawlado.unlock();
		}
	}
}

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
	Scheduler::PushUrl("http://https://pt.wikipedia.org",false);
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
	int cont=0;

	while(true){
		if(cont>1e4) break;
		Crawler::scheduler_mutex.lock();
		string nextUrl= Scheduler::TopUrl();
		Crawler::scheduler_mutex.unlock();
		if(nextUrl.size()==0) {
			cont++;
			continue;
		}

		spider.Initialize(nextUrl.c_str());
		spider.AddUnspidered(nextUrl.c_str());

		if(spider.CrawlNext())
		{
			spider.get_LastUrl(collectedUrl);
			spider.get_LastHtml(collectedHtml);
			fw.print(collectedUrl,collectedHtml);

			crawlado.lock();
			ncraw++;
			cout<<"numero de site ja coletados: "<<ncraw<<endl;
			crawlado.unlock();

			int sz=spider.get_NumOutboundLinks();
			for(int x=0;x<sz;x++){
				string Url=spider.getOutboundLink(x);
				Url=spider.canonicalizeUrl(Url.c_str());

				Crawler::scheduler_mutex.lock();
				Scheduler::PushUrl(Url,false);
				Crawler::scheduler_mutex.unlock();
			}

			set<int> jafoi;

			sz=spider.get_NumUnspidered();

			while(jafoi.size()<min(sz,20)){
				int x=rand()%sz;
				if(!jafoi.count(x)){
					jafoi.insert(x);
					CkString Next;
					spider.GetUnspideredUrl(x,Next);
					string Url=spider.canonicalizeUrl(Next.getString());
			
					Crawler::scheduler_mutex.lock();
					Scheduler::PushUrl(Url,true);
					Crawler::scheduler_mutex.unlock();

				}
			}
			int unspidered = spider.get_NumUnspidered();
			for(int i = 0; i < unspidered; i++)
			{
				spider.SkipUnspidered(0);
			}
			spider.ClearOutboundLinks();
			spider.ClearSpideredUrls();
			spider.ClearFailedUrls();
		}
	}
}

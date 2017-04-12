#include "crawler.hpp"
using namespace std;
mutex Crawler::scheduler_mutex;
mutex Crawler::crawlado;
int Crawler::ncraw=0;
string Crawler::foldername;

Crawler::Crawler() 
{
}
void Crawler::PoeNaRoda(vector<string> &v){
	for(string s: v){
		string x;
		for(int j=0;j<s.size();j++) x.push_back(s[j]);
		Scheduler::PushUrl(s,x,false);
	}
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
		string nextUrl;
		Crawler::scheduler_mutex.lock();
		nextUrl=Scheduler::TopUrl();
		int cont=0;
		while(nextUrl.size()==0){
			nextUrl=Scheduler::TopUrl();
			cont++;
			if(cont>2000) break;
		}
		Crawler::scheduler_mutex.unlock();
		if(nextUrl.size()==0) continue;
		CkString x;
	
    if(!spider.GetUrlDomain(nextUrl.c_str(),x)) continue;

		spider.Initialize(x.getString());
		spider.AddUnspidered(nextUrl.c_str());

		if(spider.CrawlNext())
		{
			spider.get_LastUrl(collectedUrl);
			spider.get_LastHtml(collectedHtml);
			fw.print(collectedUrl,collectedHtml);

			Crawler::crawlado.lock();
			ncraw++;
			cout << ncraw << ": "<< collectedUrl.getString() << std::endl;
			Crawler::crawlado.unlock();
					


			int sz=spider.get_NumOutboundLinks();
			CkString Next,aux;
			for(int x=0;x<sz;x++){
				string Url=spider.getOutboundLink(x);
      //  if(spider.CanonicalizeUrl(Url.c_str(),aux)) Url=aux.getString();
      //  else continue;
        if(spider.GetBaseDomain(Url.c_str(),Next)){
					Crawler::scheduler_mutex.lock();
					Scheduler::PushUrl(Url,Next.getString(),false);
					Crawler::scheduler_mutex.unlock();
				}

			}


			sz=spider.get_NumUnspidered();

			for(int x=0;x<sz%15;x++){
				CkString Next;
				spider.GetUnspideredUrl(0,Next);
				spider.SkipUnspidered(0);
				string Url=Next.getString();
   //     if(spider.CanonicalizeUrl(Url.c_str(),aux)) Url=aux.getString();
    //    else continue;
				if(spider.GetBaseDomain(Url.c_str(),Next)){
					Crawler::scheduler_mutex.lock();
					Scheduler::PushUrl(Url,Next.getString(),true);
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

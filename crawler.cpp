#include "crawler.hpp"
using namespace std;

mutex Crawler::scheduler_mutex;
mutex Crawler::crawlado;
clock_t Crawler::tbegin;
int Crawler::ncraw=0;
string Crawler::foldername;

Crawler::Crawler() 
{
	Crawler::scheduler_mutex.lock();
	int x=rand()%4;
	if(x==0)
	Scheduler::PushUrl("http://uol.com",false);
	if(x==1)
	Scheduler::PushUrl("http://g1.com.br",false);
	if(x==2)	
	Scheduler::PushUrl("http://r7.com.br",false);
	if(x==3)	
	Scheduler::PushUrl("http://folha.uol.com.br",false);



	Crawler::scheduler_mutex.unlock();
}

void Crawler::SetTime(){
	Crawler::tbegin=clock();
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
		if(cont>1000000) break;
		scheduler_mutex.lock();
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
			int sz=spider.get_NumOutboundLinks();
			unordered_set<int> jafoi;
			for(int x=0;x<sz;x++){
					Crawler::scheduler_mutex.lock();
					string Url=spider.getOutboundLink(x);
					Url=spider.canonicalizeUrl(Url.c_str());
					Scheduler::PushUrl(Url,false);
					Crawler::scheduler_mutex.unlock();
				}
			spider.ClearOutboundLinks();
			sz=spider.get_NumUnspidered();
			
			while(jafoi.size()<min(sz,35)){
				int x=rand()%sz;
				//cout<<"NO WHILE"<<endl;
				if(!jafoi.count(x)){
					jafoi.insert(x);
					CkString Next;
					spider.GetUnspideredUrl(x,Next);
					
					Crawler::scheduler_mutex.lock();
					
					string Url=spider.canonicalizeUrl(Next.getString());
					Scheduler::PushUrl(Url,true);
					Crawler::scheduler_mutex.unlock();

				}
			}

			int unspidered = spider.get_NumUnspidered();
			//cout<<"tamanho2 "<<unspidered<<endl;
			for(int i = 0; i < unspidered; i++)
			{
				spider.SkipUnspidered(0);
			}
			spider.ClearOutboundLinks();
	   	spider.ClearSpideredUrls();
	   	spider.ClearFailedUrls();
		}
		else{

			cout<<"PAU"<<endl;
		}
	}
	for(int i=0;i<1000;i++) cout<<"#";
	cout<<endl;
}

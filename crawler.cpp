#include "crawler.hpp"
using namespace std;

mutex Crawler::scheduler_mutex;
mutex Crawler::crawlado;
int Crawler::ncraw=0;
string Crawler::foldername;

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
		if(ncraw>1000) return;		
		scheduler_mutex.lock();
		string nextUrl= Scheduler::TopUrl();
		Crawler::scheduler_mutex.unlock();
		if(nextUrl.size()==0) continue;
		if(Scheduler::IsEmpty(false) and Scheduler::IsEmpty(true)) cont++;
		if(cont>100) break;
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
		else{
			cout<<"PAU"<<endl;
		}
		spider.ClearOutboundLinks();
	   spider.ClearSpideredUrls();
	   spider.ClearFailedUrls();
	}
	for(int i=0;i<100000;i++) cout<<"#";
	cout<<endl;
}

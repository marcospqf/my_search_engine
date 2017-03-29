#include "scheduler.hpp"
using namespace std;

set<Scheduler::url> Scheduler::inside_url;
set<Scheduler::url> Scheduler::outside_url;
long long Scheduler::peso[100000];
unordered_set<long long> Scheduler::visited;
unordered_map<long long, double> Scheduler::last_time;
const long long  B=193;
const int MAX_HEAP=1e4;
double step=2.0;

Scheduler::Scheduler() {}

bool Scheduler::url::operator<(const url &cur) const {
	if(priority != cur.priority )
		return priority < cur.priority;
	if(name.size() != cur.name.size()) return name.size()< cur.name.size();	
	return name < cur.name;
}

Scheduler::url::url(const string &s, const string &d){
	name=s;
	domain=d;
	int x=0;
	for(char c: d){
		if(c>='a' and c<='z') x+=c-'a';
		else x+=5;
	}
	priority=(++peso[x])*1000+x;
}

void Scheduler::PushUrl(const string &s,const string &domain, bool inside){
	if(s.size()>80) return;

	url cur(s,domain);
	long long hp=0;

	if(inside){
		if(inside_url.size()>MAX_HEAP) return;
		for(char c: s) hp=hp*B+c;
		if(!visited.count(hp)) {
			visited.insert(hp);
			Scheduler::inside_url.insert(cur);
		}
	}
	else{
		if(outside_url.size()>MAX_HEAP) return;
		for(char c: s) hp=hp*B+c;
		if(!visited.count(hp)) {
			visited.insert(hp);

			Scheduler::outside_url.insert(cur);
		}
	}
}

void Scheduler::PopUrl(bool inside){
	if(inside){
		if(!inside_url.empty()) inside_url.erase(inside_url.begin());
	}
	else{
		if(!outside_url.empty()) outside_url.erase(inside_url.begin());
	}
}

bool Scheduler::IsEmpty(bool inside){
	if(inside) return inside_url.empty();
	return outside_url.empty();
}

bool Scheduler::thistime(double x){
	return ( (clock()-x)/ CLOCKS_PER_SEC > step);
}

string Scheduler::TopUrl(){
	//cout<<inside_url.size()+outside_url.size()<<endl;
	int q=rand();
	if(q%5==0) q=1;
	else q=0;
	if(q){
		if(inside_url.empty()) q=!q;
	}
	else{
		if(outside_url.empty()) q=!q;
	}
	if(q){
		if(!inside_url.empty()) {

			url s=(*inside_url.begin());
			inside_url.erase(inside_url.begin());
			string rosklin=s.domain;
			long long hp=0;
			for(char c: rosklin) hp=hp*B+c;
			if(!last_time.count(hp) or thistime(last_time[hp])){
				last_time[hp]=clock();
				return s.name;
			}
			else{
				s.priority++;
				inside_url.insert(s);
			}
		}
	}
	else{
		if(!outside_url.empty()) {
			url s=(*outside_url.begin());
			outside_url.erase(outside_url.begin());
			string rosklin=s.domain;
			long long hp=0;
			for(char c: rosklin) hp=hp*B+c;
			if(!last_time.count(hp) or thistime(last_time[hp]) ){
				last_time[hp]=clock();
				return s.name;
			}
			else{
				//cout<<s.name<<endl;
				//s.priority++;
				//outside_url.insert(s);
			}
		}
	}
	return "";
}

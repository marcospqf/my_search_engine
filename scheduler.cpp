#include "scheduler.hpp"
using namespace std;

priority_queue<Scheduler::url> Scheduler::inside_url;
priority_queue<Scheduler::url> Scheduler::outside_url;
unordered_set<long long> Scheduler::visited;
const long long B=271;
const long long MAX_SIZE_HEAP=5e4;
Scheduler::Scheduler() {}

bool Scheduler::url::operator<(const url &cur) const {
	if(priority != cur.priority )
		return priority > cur.priority;
	
	return name > cur.name;
}

Scheduler::url::url(const string &s){
	name=s;
	priority=0;
	bool last=true;
	for(char c: s) {
		if(c!='/') last=false;
		else if(last) priority+=2;
		else priority++,last=true;
	}
}

void Scheduler::PushUrl(const string &s, bool inside){
	
	if(s.size()>70) return;
	if(s.size()<6) return;
	//cout<<" tamanho "<<" ";
//	cout<<(Scheduler::inside_url.size())<<" ";
//	cout<<(Scheduler::outside_url.size())<<endl;
	url cur(s);
	long long hp=0;
	if(inside){
		if(inside_url.size()>MAX_SIZE_HEAP) return;
		for(char c: s) hp=hp*B+c;
		if(!visited.count(hp)) visited.insert(hp),
			inside_url.push(cur);
	}
	else{
		if(outside_url.size()>MAX_SIZE_HEAP) return;
		for(char c: s) hp=hp*B+c;
		if(!visited.count(hp)) visited.insert(hp),
			outside_url.push(cur);
	}
}

void Scheduler::PopUrl(bool inside){
	if(inside){
		if(!inside_url.empty()) inside_url.pop();
	}
	else{
		if(!outside_url.empty()) outside_url.pop();
	}
}

bool Scheduler::IsEmpty(bool inside){
	if(inside) return inside_url.empty();
	return outside_url.empty();
}


string Scheduler::TopUrl(){
	int q=rand()&1;
	if(q){
		if(inside_url.empty()) q=!q;
	}
	else{
		if(outside_url.empty()) q=!q;
	}
	if(q){
		if(!inside_url.empty()) {
			string s=(inside_url.top()).name;
			inside_url.pop();
			return s;
		}
	}
	else{
		if(!outside_url.empty()) {
			string s=(outside_url.top()).name;
			outside_url.pop();
			return s;
		}
	}
	return "";
}

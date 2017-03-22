#include "scheduler.hpp"
using namespace std;

set<Scheduler::url> Scheduler::inside_url;
set<Scheduler::url> Scheduler::outside_url;
unordered_set<long long> Scheduler::visited;
const long long B=31;
Scheduler::Scheduler() {}

bool Scheduler::url::operator<(const url &cur) const {
	if(priority != cur.priority )
		return priority < cur.priority;
	
	return name<cur.name;
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
	url cur(s);
	long long hp=0;
	if(inside){
		if(inside_url.size()>1e5) return;
		for(char c: s) hp=hp*B+c;
		if(!visited.count(hp)) visited.insert(hp),
			inside_url.insert(cur);
	}
	else{
		if(outside_url.size()>1e5) return;
		for(char c: s) hp=hp*B+c;
		if(!visited.count(hp)) visited.insert(hp),
			outside_url.insert(cur);
	}
}

void Scheduler::PopUrl(bool inside){
	if(inside){
		if(!inside_url.empty()) inside_url.erase(inside_url.begin() );
	}
	else{
		if(!outside_url.empty()) outside_url.erase(outside_url.begin() );
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
			string s=(*inside_url.begin()).name;
			inside_url.erase(inside_url.begin() );
			return s;
		}
	}
	else{
		if(!outside_url.empty()) {
			string s=(*outside_url.begin()).name;
			outside_url.erase(outside_url.begin() );
			return s;
		}
	}
	return "";
}

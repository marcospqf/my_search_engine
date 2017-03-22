all: 
	g++ -std=c++11 -I./chilkat/include -L./chilkat/lib test.cpp scheduler.cpp -o exe -l chilkat-9.5.0 -lresolv

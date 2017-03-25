all: mac
mac: 
	g++ -std=c++11 -I./chilkat-osx/include -L./chilkat-osx/libStatic test2.cpp crawler.cpp scheduler.cpp -o exe -lchilkat -lresolv -pthread
linux:
	g++ -std=c++11 -I./chilkat-linux/include -L./chilkat-linux/lib test.cpp scheduler.cpp -o exe -l chilkat-9.5.0 -lresolv
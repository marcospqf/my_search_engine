all: mac
mac: 
	g++ -std=c++11 -I./chilkat-osx/include -L./chilkat-osx/libStatic main.cpp crawler.cpp scheduler.cpp FileWriter.cpp -o exe -lchilkat -lresolv -pthread -O2
linux:
	g++ -std=c++11 -I./chilkat-linux/include -L./chilkat-linux/lib main.cpp crawler.cpp scheduler.cpp FileWriter.cpp -o exe -l chilkat-9.5.0 -lresolv -pthread -O2

#ifndef _fileWriter_hpp
#define _fileWriter_hpp

#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "CkString.h"
using namespace std;
class FileWriter {
	public:

		FileWriter();
		//set the name of file of a data
		void SetFilename(string filename);
		
		//Put the string on the format given:
		//All | must be replaced by ' '
		void Filter(CkString &s);

		void OpenStream();

		void print(CkString &url, CkString &html);

		void CloseStream();

	private:
		string _filename;
		filebuf _fb;
		ostream *out;

		//how many Url will be at a file
		int qnts;
};

#endif

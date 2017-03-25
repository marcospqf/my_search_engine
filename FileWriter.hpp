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
		void SetFilename(string filename);

		void OpenStream();

		void print(CkString &url, CkString &html);

		void CloseStream();

	private:
		string _filename;
		filebuf _fb;
		ostream *out;
		int qnts;
};

#endif

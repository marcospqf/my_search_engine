#include "FileWriter.hpp"
using namespace std;

FileWriter::FileWriter(){};

void FileWriter::SetFilename(string filename)
{
	this->_filename = filename;
}

void FileWriter::OpenStream()
{
	this->_fb.open(this->_filename, ios::out);
	this->out = new ostream(&this->_fb);
	this->qnts=0;
}

void FileWriter::print(CkString &ckurl, CkString &ckhtml)
{
	this->qnts++;
	(*this->out) << "|||\n" << ckurl.getString() << "\n|\n";
	(*this->out) << ckhtml.getString();

	if(this->qnts > 100)
	{
		(*this->out) << endl;
		this->out->flush();
		this->qnts=0;
	}
	else (*this->out) << "\n";
}

void FileWriter::CloseStream()
{
	delete this->out;
	this->_fb.close();
}

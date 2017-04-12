#include "FileWriter.hpp"
using namespace std;

FileWriter::FileWriter(){};
void FileWriter::SetFilename(string &filename)
{
  this->_filename = filename;
}

void FileWriter::OpenStream()
{
  this->_fb.open(this->_filename, ios::out);
  this->out = new ostream(&this->_fb);
  this->qnts=0;
}

void FileWriter::Filter(string &s)
{
  for(int i=0;i<s.size();i++) 
    if(s[i]=='|') s[i]=' ';

}
void FileWriter::print(CkString &ckurl, CkString &ckhtml)
{
  this->qnts++;
  string s1=ckurl.getString();
  string s2=ckhtml.getString();
  Filter(s1);
  Filter(s2);
  (*this->out) << "||| " << s1 << " | ";
  (*this->out) << s2;

  if(this->qnts > 200)
  {
    this->out->flush();
    this->qnts=0;
  }
}

void FileWriter::CloseStream()
{
  delete this->out;
  this->_fb.close();
}

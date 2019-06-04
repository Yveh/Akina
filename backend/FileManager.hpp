#ifndef AKINA_FILEMANAGER_HPP
#define AKINA_FILEMANAGER_HPP
#include <fstream>
#include <stdio.h>
#include <cstring>
template <class T,size_t Bufsiz>
class FileManager
{
private:
	std::string FileName;
	std::fstream File;
	T *bf;
	size_t blen;
public:
    size_t siz;
	FileManager()
    {
	    bf=new T[Bufsiz];
	    blen=0;
    }
	void SetName(const std::string FName)
	{
		FileName=FName;
		File.open(FileName,std::fstream::in);
		if (!File) File.open(FileName,std::ios::binary|std::ios_base::out);
		File.close();
		File.open(FileName,std::ios::binary|std::ios::in|std::ios::out);

        File.seekg(0,std::ios::end);
        siz=File.tellg()/sizeof(T);

	}
	~FileManager()
	{
	    Flush_Back();
	    delete[] bf;
	    File.close();
	}

	void Read(T* TPointer,long long pos)
	{
	    if (pos>siz) Flush_Back();
		File.seekg(sizeof(T)*(pos-1),std::ios::beg);
		File.read((reinterpret_cast<char * > (TPointer)), sizeof(T));
	}
	void AllRead(T* TPointer,long long pos,size_t len)
	{
        if (pos+len>siz) Flush_Back();
		File.seekg(sizeof(T)*(pos-1),std::ios::beg);
		File.read((reinterpret_cast<char * > (TPointer)), sizeof(T)*len);
	}
	void Write(const T* TPointer,long long pos)
	{
        if (pos>siz) Flush_Back();
		File.seekp(sizeof(T)*(pos-1),std::ios::beg);
		File.write((reinterpret_cast<const char * > (TPointer)), sizeof(T));
	}
	void AlWrite(const T* TPointer,long long pos,size_t len)
	{
        if (pos+len>siz) Flush_Back();
		File.seekp(sizeof(T)*(pos-1),std::ios::beg);
		File.write((reinterpret_cast<const char * > (TPointer)), sizeof(T)*len);
	}

	long Push_Back(const T* TPointer,size_t len)
	{
	    /*
		File.seekp(0,std::ios::end);
		File.write((reinterpret_cast<const char * > (TPointer)), sizeof(T)*len);
		siz+=len;
		return siz;
		*/
        if ((blen+siz)>=Bufsiz) Flush_Back();
	    memcpy(bf+blen,TPointer,sizeof(T)*len);
	    blen+=len;
	    return siz+blen;
	}
	void Flush_Back()
    {
	    if (blen==0)return;
        File.seekp(0,std::ios::end);
        File.write((reinterpret_cast<const char * > (bf)), sizeof(T)*blen);
        siz+=blen;
        blen=0;
    }
    size_t size(){return siz+blen;}
	void Clear()
	{
	    Flush_Back();
		File.close();
		File.open(FileName,std::ios::binary|std::ios::in|std::ios::out|std::ios::trunc);
	    siz=0;
	}
};
#endif //AKINA_FILEMANAGER_HPP

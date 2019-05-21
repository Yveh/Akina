//
// Created by 18146 on 2019/5/14.
//

#ifndef AKINA_BPLUSTREE_HPP
#define AKINA_BPLUSTREE_HPP
#include "FileManager.hpp"
#include "BasicStruct.hpp"
#include <functional>
template <class Key,class Data,size_t N,size_t M,class Cmp=std::less<Key>>
class BPlusTree
{
private:
public:
	BPlusTree();
	~BPlusTree();
	void SetName(const char* FileName)
	{

	}
	//上方设置文件的方式均可更改
	int Find(const Key &key,Data &data)
	{
		//if no key,return 0;
	}
	int Erase(const Key &key)
	{
		//if no key,return 0;
	}

	int Insert(const Key &key,const Data &data)
	{
		//if exist key,return 0;
	}
	int Modify(const Key &key,const Data &data)
	{
		//if no key,return 0;
	}
	int AskArr(const Key &key,std::function<bool(const Key &,const Key &)>CMP,Vector<Key> &vect1,Vector<Data> &vect2)//最后一个参数为类型为Data的wy版本vector
	{
		//if no key,return 0;
	}

	void Clear()
	{

	}
};

#endif //AKINA_BPLUSTREE_HPP

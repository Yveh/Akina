//
// Created by 18146 on InitId/5/4.
//

#ifndef AKINA_USERMANANGER_HPP
#define AKINA_USERMANANGER_HPP
#include "FileManager.hpp"
#include "BasicStruct.hpp"
class UserManager
{
private:
	FileManager<USER,10240> File;
	const int InitId=2019;
	const int cLimit=1024;
	int ModLimit=1024;
	USER* x;bool AMode=false;
    void ModeOn()
    {
        x=new USER[Siz];
        if (Siz>ModLimit) ModLimit=Siz+13;
        File.AllRead(x,1,Siz);
        AMode=true;
    }
    void ModeOff()
    {
        if (AMode)
        {
            File.AlWrite(x,1,Siz);
            delete[] x;
            AMode=false;
        }
    }
    void DoubleSpace()
    {
        ModLimit*=2;
        x=(USER*)(realloc(x,ModLimit*sizeof(USER)));
    }
public:
	size_t Siz;
	UserManager()
	{
	    File.SetName("UserRecord.bin");
	    Siz=File.size();
	    x=nullptr;
	    AMode=false;
	}
	~UserManager(){ModeOff();}
	size_t Register(USER &User)
	{
		User.Priv=1+(Siz==0);
		if (AMode)
        {
		    x[Siz]=User;
		    if (Siz>ModLimit-10) DoubleSpace();
        }
		else File.Push_Back(&User,1);
		++Siz;
		return Siz+InitId-1;
	}
	int Login(const USER &User,size_t Id)
	{
        if (Siz>cLimit&&!AMode&&OnKey) ModeOn();
		if (Id<=(InitId-1)||Id>(InitId-1)+Siz) return 0;
		if (AMode)return x[Id-InitId].Passwd==User.Passwd;
		USER X;
		File.Read(&X,Id-(InitId-1));
		return (X.Passwd==User.Passwd);
	}
	int Query(USER &User,size_t Id)
	{
        if (Siz>cLimit&&!AMode) ModeOn();
		if (Id<=(InitId-1)||Id>(InitId-1)+Siz) return 0;
		if (AMode) User=x[Id-InitId];
		else File.Read(&User,Id-(InitId-1));
		return 1;
	}
	int Modify_Profile(USER &User,size_t Id)
	{
        if (Siz>cLimit&&!AMode) ModeOn();
		if (Id<=(InitId-1)||Id>(InitId-1)+Siz) return 0;
		if (AMode)
        {
		    User.Priv=x[Id-InitId].Priv;
		    x[Id-InitId]=User;
		    return 1;
        }
		USER X;
		File.Read(&X,Id-(InitId-1));
		User.Priv=X.Priv;
		File.Write(&User,Id-(InitId-1));
		return 1;
	}
	int Modify_Privilege(size_t Id1,size_t Id2,int Priv)
	{
        if (Siz>cLimit&&!AMode) ModeOn();
		if (Id1<=(InitId-1)||Id1>(InitId-1)+Siz) return 0;
		if (Id2<=(InitId-1)||Id2>(InitId-1)+Siz) return 0;
		if (AMode)
        {
		    if (x[Id1-InitId].Priv<2||x[Id2-InitId].Priv>Priv) return 0;
            x[Id2-InitId].Priv=Priv;
            return 1;
        }
        USER x1,x2;
		File.Read(&x1,Id1-(InitId-1));
		File.Read(&x2,Id2-(InitId-1));
		//std::cout<<x1.Priv<<x2.Priv;
		if (x1.Priv<2||x2.Priv>Priv) return 0;
		x2.Priv=Priv;
		File.Write(&x2,Id2-(InitId-1));
		return 1;
	}
	void Clear()
	{
	    File.Clear();
		Siz=0;
	}
};


#endif //AKINA_USERMANANGER_HPP

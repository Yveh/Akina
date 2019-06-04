#pragma once
#ifndef AKINA_BASICSTRUCT_HPP
#define AKINA_BASICSTRUCT_HPP
#include <fstream>
#include <cstdio>
#include <ostream>
#include "BasicString.hpp"
#include "vector.hpp"
#include "FileManager.hpp"

const bool OnKey=false;
const bool OnKey2=false;

struct USER
{
	short Priv=0;
	String<40> Name;
	String<20> Passwd,Email,Phone;
};
struct StationKey
{
	String<20> Loc,TrainId;
	char Catalog;
};//stationvalue==pos(short)
bool operator<(const StationKey &l,const StationKey &r)
{
	int x=l.Loc.cmp(r.Loc);
	if (x<0) return true;
	if (x>0) return false;
	return l.TrainId<r.TrainId;
}
struct station
{
    String<20> Loc;
    unsigned short Time1,Time2;
    double Price[5];
};
struct TrainValue
{
	String<40> Name;
	char Catalog;
	String<20> TicketKind[5];
	short KindNum,LocNum;
	long Leftpos[31],StPos;
		TrainValue()=default;
	TrainValue(const TrainValue &o)
	{
		Catalog=o.Catalog;
		Name=o.Name;
		KindNum=o.KindNum;
		LocNum=o.LocNum;
		StPos=o.StPos;
        for (size_t i=0;i<5;++i) TicketKind[i]=o.TicketKind[i];
        memcpy(Leftpos,o.Leftpos,248);
    }
	TrainValue &operator=(const TrainValue &o)
	{
		if (&o==this) return *this;
		Catalog=o.Catalog;
		Name=o.Name;
		KindNum=o.KindNum;
		LocNum=o.LocNum;
        StPos=o.StPos;
        for (size_t i=0;i<5;++i) TicketKind[i]=o.TicketKind[i];
        memcpy(Leftpos,o.Leftpos,248);
        return *this;
	}
	void WriteTrain(std::ostream &os,FileManager<station,1024> &f)
	{
		os<<Name<<Catalog<<' '<<LocNum<<' '<<KindNum<<' ';
		for (int i=0;i<KindNum;i++) os<<TicketKind[i];
		os<<'\n';
		station *a=new station[LocNum];
		f.AllRead(a,StPos,LocNum);
		os<<a[0].Loc<<"xx:xx "<<StrTime(a[0].Time1)<<"xx:xx ";
		for (int j=0;j<KindNum;j++) os<<"￥"<<a[0].Price[j]<<' ';
		os<<'\n';
		for (int i=1;i<LocNum-1;i++)
		{
			os<<a[i].Loc<<StrTime(a[i].Time1)<<StrTime(a[i].Time2)<<StrTime(a[i].Time2-a[i].Time1);
			for (int j=0;j<KindNum;j++) os<<"￥"<<a[i].Price[j]-a[i-1].Price[j]<<' ';
			os<<'\n';
		}
		os<<a[LocNum-1].Loc<<StrTime(a[LocNum-1].Time1)<<"xx:xx xx:xx ";
		for (int j=0;j<KindNum;j++) os<<"￥"<<a[LocNum-1].Price[j]-a[LocNum-2].Price[j]<<' ';
		os<<'\n';
		delete[] a;
	}
	void ReadTrain(std::istream &is,FileManager<station,1024> &f,station* &a)
	{
		String<20> s1,s2,sp;
		Leftpos[0]=-1;
		is>>Name>>Catalog>>LocNum>>KindNum;
		a=new station[LocNum];
		for (int i=0;i<KindNum;i++) is>>TicketKind[i];
		is>>a[0].Loc>>sp>>s1>>sp;a[0].Time2=a[0].Time1=s1.ToTime();
		for (int j=0;j<KindNum;j++) is>>sp,a[0].Price[j]=sp.ToPrice();
		for (int i=1;i<LocNum-1;i++)
		{
			is>>a[i].Loc>>s1>>s2>>sp;
            a[i].Time1=s1.ToTime();a[i].Time2=s2.ToTime();
            while (a[i].Time1<a[i-1].Time2) a[i].Time1+=1440;
            while (a[i].Time2<a[i].Time1) a[i].Time2+=1440;
			for (int j=0;j<KindNum;j++)
			{
			    is>>sp;
			    a[i].Price[j]=a[i-1].Price[j]+sp.ToPrice();
			}

		}
		is>>a[LocNum-1].Loc>>s2>>sp>>sp;
		a[LocNum-1].Time1=s2.ToTime();
        while (a[LocNum-1].Time1<a[LocNum-2].Time2) a[LocNum-1].Time1+=1440;
        a[LocNum-1].Time2=a[LocNum-1].Time1;
		for (int j=0;j<KindNum;j++)
		{
		    is>>sp;
		    a[LocNum-1].Price[j]=a[LocNum-2].Price[j]+sp.ToPrice();
		}

	}

};//trainkey:string<20>=TrainId
struct UTicketKey
{
	String<20> TrainId;
	char Catalog;
	short Date,l1,l2,Kind;
	size_t UserId;
};//date for starting date.Uticketvalue=short.
bool operator<(const UTicketKey &t,const UTicketKey &o)
{
	if (t.UserId<o.UserId) return true;
	if (t.UserId>o.UserId) return false;
	if (t.Date<o.Date) return true;
	if (t.Date>o.Date) return false;
	if (t.Catalog<o.Catalog) return true;
	if (t.Catalog>o.Catalog) return false;
	//
	return (t.l1*240+t.l2*4+t.Kind<o.l1*240+o.l2*4+o.Kind);
}
bool Cmp_UT(const UTicketKey &x,const UTicketKey &y)
{
	if (x.UserId<y.UserId) return true;
	if (x.UserId>y.UserId) return false;
	if (x.Date<y.Date) return true;
	if (x.Date>y.Date) return false;
	return false;
}
bool Cmp_SK(const StationKey &x,const StationKey &y)
{
	return x.Loc<y.Loc;
}
struct Iticket
{
	String<20> TrainId,Loc1,Loc2;
	String<20> Kind;
	short Date,Num;
	size_t UserId;
};
struct Trans
{
	String<20> TrainId1,TrainId2;
	short l11,l12,l21,l22,Date;

};
#endif //AKINA_BASICSTRUCT_HPP

//
// Created by 18146 on 2019/5/3.
//
#pragma once
#ifndef AKINA_BASICSTRUCT_HPP
#define AKINA_BASICSTRUCT_HPP
#include <fstream>
#include <cstdio>
#include <ostream>
#include "BasicString.hpp"
#include "vector.hpp"
struct USER
{
	size_t Id=0;
	int Priv=0;
	String<40> Name;
	String<20> Passwd,Email,Phone;
};//todo:remove ID in struct
struct StationKey
{
	String<20> Loc,TrainId;
	bool operator<(const StationKey &o)
	{
		int x=Loc.cmp(o.Loc);
		if (x<0) return true;
		if (x>0) return false;
		return TrainId<o.TrainId;
	}
};//stationvalue==pos(short)

struct TrainValue
{
	String<20> Name;
	char Catalog;
	String<20> Loc[60];
	String<10> TicketKind[5];
	short KindNum,LocNum,Time1[60],Time2[60];
	double Price[300];
	long LeftPos;
	void ReadTrain(std::istream &is)
	{
		String<10> s1,s2,sp;
		LeftPos=-1;
		is>>Name>>Catalog>>LocNum>>KindNum;
		for (int i=0;i<KindNum;i++) is>>TicketKind[i];
		is>>Loc[0]>>s1>>sp>>sp;Time2[0]=Time1[0]=s1.ToTime();
		for (int j=0;j<KindNum;j++) is>>sp,Price[j]=sp.ToPrice();
		for (int i=1;i<LocNum-1;i++)
		{
			is>>Loc[i]>>s1>>s2>>sp;
			Time1[i]=s1.ToTime();Time2[i]=s2.ToTime();
			for (int j=0;j<KindNum;j++) is>>sp,Price[i*5+j]=sp.ToPrice();
		}
		is>>Loc[LocNum-1]>>sp>>s2>>sp;Time2[0]=Time1[0]=s2.ToTime();
	}
	void WriteTrain(std::ostream &os)
	{
		os<<Name<<Catalog<<LocNum<<KindNum;
		for (int i=0;i<KindNum;i++) os<<TicketKind[i];
		os<<'\n';
		os<<Loc[0]<<"xx:xx "<<StrTime(Time1[0])<<"00:00 ";
		for (int j=0;j<KindNum;j++) os<<"￥"<<Price[j]<<' ';
		os<<'\n';
		for (int i=1;i<LocNum-1;i++)
		{
			os<<Loc[i]<<StrTime(Time1[i])<<StrTime(Time2[i])<<StrTime(Time2[i]-Time1[i]);
			for (int j=0;j<KindNum;j++) os<<"￥"<<Price[i*5+j]<<' ';
			os<<'\n';
		}
		os<<Loc[LocNum-1]<<StrTime(Time1[LocNum-1])<<"xx:xx "<<"00:00 ";
		for (int j=0;j<KindNum;j++) os<<"￥"<<Price[(LocNum-1)*5+j]<<' ';
		os<<'\n';
	}
};//trainkey:string<20>=TrainId
struct UTicketKey
{
	String<20> TrainId;
	char Catalog;
	short Date,l1,l2,Kind;
	size_t UserId;
	bool operator<(const UTicketKey &o)
	{
		if (UserId<o.UserId) return true;
		if (UserId>o.UserId) return false;
		if (Date<o.Date) return true;
		if (Date>o.Date) return false;
		if (Catalog<o.Catalog) return true;
		if (Catalog>o.Catalog) return false;
		//
		return (l1*137+l2*13+Kind<o.l1*137+o.l2*13+o.Kind);
	}
};//date for starting date.Uticketvalue=short.
bool Cmp_UT(const UTicketKey &x,const UTicketKey &y)
{
	if (x.UserId<y.UserId) return true;
	if (x.UserId>y.UserId) return false;
	if (x.Date<y.Date) return true;
	if (x.Date>y.Date) return false;
	return false;
	//todo: ==?
}
struct Iticket
{
	String<20> TrainId,Loc1,Loc2;
	String<10> Kind;
	short Date,Num;
	size_t UserId;
};
#endif //AKINA_BASICSTRUCT_HPP

//
// Created by 18146 on 2019/5/14.
//

#ifndef AKINA_TRAINMANAGER_HPP
#define AKINA_TRAINMANAGER_HPP
#include "FileManager.hpp"
#include "BPlusTree.hpp"
class TrainManager
{
private:
	BPlusTree<String<20>,TrainValue,30,30> AlTrain;
	BPlusTree<StationKey,short,100,1000> LocToId;
	BPlusTree<UTicketKey,short,30,30> UserTicket;
	FileManager<short> LeftTicket;
	long LSiz;


public:
	TrainManager()
	{
		LSiz=LeftTicket.size();
	}

	~TrainManager() = default;
	int AddTrain(const String<20> &Id,TrainValue &Train)
	{
		Train.LeftPos=-1;
		return AlTrain.Insert(Id,Train);
	}
	int ModTrain(const String<20> &Id,TrainValue &Train)
	{
		Train.LeftPos=-1;
		return AlTrain.Modify(Id,Train);
	}
	int QueryTrain(const String<20> &Id,TrainValue &Train)
	{
		return AlTrain.Find(Id,Train);
	}
	int DelTrain(const String<20> &Id)
	{
		TrainValue Train;
		if (AlTrain.Find(Id,Train)==0)  return 0;
		if (Train.LeftPos>=0) return 0;
		AlTrain.Erase(Id);
		return 1;
		//todo:find in tree 2 times for the same train?
	}
	int SaleTrain(const String<20> &Id)
	{
		TrainValue Train;
		if (AlTrain.Find(Id,Train)==0)  return 0;
		if (Train.LeftPos>=0) return 0;
	//leftpos+kindpos*locnum=this kind of ticket left.
		short a[Train.LocNum*Train.KindNum];
		for (int i=0;i<Train.LocNum*Train.KindNum;i++) a[i]=2000;
		Train.LeftPos=LSiz+1;
		LSiz=LeftTicket.Push_Back(a,Train.LocNum*Train.KindNum);

		StationKey x;x.TrainId=Id;
		for (short i=0;i<Train.LocNum;i++)
		{
			x.Loc=Train.Loc[i];
			LocToId.Insert(x,i);
		}

		AlTrain.Modify(Id,Train);

		return 1;
		//todo:find in tree 2 times for the same train?
	}
	int BuyTicket(Iticket &x)
	{
		TrainValue Train;
		if (AlTrain.Find(x.TrainId,Train)==0)  return 0;
		if (Train.LeftPos<0) return 0;
		UTicketKey key;
		short i=0;
		for (i=0;i<Train.KindNum;i++) if (Train.TicketKind[i]==x.Kind) {key.Kind=i;break;}
		if (i==Train.KindNum) return 0;
		for (i=0;i<Train.LocNum;i++)
		{
			if (Train.Loc[i]==x.Loc1) key.l1=i;
			else if (Train.Loc[i]==x.Loc2) key.l2=i;
		}//todo:probably faster but n=60 so not important.
		if (key.l1>=key.l2) return 0;
		//check:enough tickets left
		short a[Train.LocNum];
		LeftTicket.AllRead(a,Train.LeftPos+key.Kind*Train.LocNum,Train.LocNum);
		for (int i=key.l1;i<=key.l2;++i) if (a[i]<x.Num) return 0;
		for (int i=key.l1;i<=key.l2;++i) a[i]-=x.Num;
		LeftTicket.AlWrite(a,Train.LeftPos+key.Kind*Train.LocNum,Train.LocNum);
		//set values in UTicket
		key.UserId=x.UserId;key.Date=x.Date;key.TrainId=x.TrainId;key.Catalog=Train.Catalog;
		UserTicket.Insert(key,x.Num);
		return 1;
	}
	int RefundTicket(Iticket &x)
	{
		TrainValue Train;
		if (AlTrain.Find(x.TrainId,Train)==0)  return 0;
		if (Train.LeftPos<0) return 0;
		UTicketKey key;
		short i=0;
		for (i=0;i<Train.KindNum;i++) if (Train.TicketKind[i]==x.Kind) {key.Kind=i;break;}
		if (i==Train.KindNum) return 0;
		for (i=0;i<Train.LocNum;i++)
		{
			if (Train.Loc[i]==x.Loc1) key.l1=i;
			else if (Train.Loc[i]==x.Loc2) key.l2=i;
		}//todo:probably faster but n=60 so not important.
		if (key.l1>=key.l2) return 0;
		key.UserId=x.UserId;key.Date=x.Date;key.TrainId=x.TrainId;key.Catalog=Train.Catalog;
		short y=-1;
		UserTicket.Find(key,y);if (y<x.Num) return 0;
		//refund in LeftTicket
		short a[Train.LocNum];
		LeftTicket.AllRead(a,Train.LeftPos+key.Kind*Train.LocNum,Train.LocNum);
		for (int i=key.l1;i<=key.l2;++i) a[i]+=x.Num;
		LeftTicket.AlWrite(a,Train.LeftPos+key.Kind*Train.LocNum,Train.LocNum);
		//set values in UTicket
		if (y==x.Num)UserTicket.Erase(key);
		else UserTicket.Modify(key,x.Num-y);
		return 1;
	}
	void QueryOrder(size_t UserId,short Date,String<20> ct,std::ostream &os)
	{
		bool ch[26]={0};
		for (int i=0;ct[i]!='\0';++i) ch[ct[i]-'A']=true;
		UTicketKey x;x.UserId=UserId;x.Date=Date;
		TrainValue t;
		Vector<UTicketKey> Vkey;
		Vector<short> Vdata;
		size_t cnt=0;
		UserTicket.AskArr(x,Cmp_UT,Vkey,Vdata);
		for (int i=0;i<Vkey.size();++i)
			if (ch[Vkey[i].Catalog-'A']) ++cnt;
		if (cnt==0) {os<<"-1\n";return;}
		os<<cnt<<'\n';
		for (int i=0;i<Vkey.size();++i)
			if (ch[Vkey[i].Catalog-'A'])
				ShowTicket_Bought(Vkey[i],Vdata[i],os);
	}
	void ShowTicket_Bought(const UTicketKey &k,short num,std::ostream &os)
	{
		TrainValue t;
		AlTrain.Find(k.TrainId,t);
		os<<k.TrainId<<t.Loc[k.l1]<<StrDate(k.Date)<<StrTime(t.Time2[k.l1]);
		double pri[5]={0};
		short plus=0,mk=t.Time2[k.l1];
		for (short i=k.l1+1;i<=k.l2;++i)
		{
			for (short j=0;j<t.KindNum;++j) pri[j]+=t.Price[i*5+j];
			if (t.Time1[i]<mk) ++plus;mk=t.Time1[i];
			if (t.Time2[i]<mk) ++plus;mk=t.Time2[i];
		}
		os<<k.TrainId<<t.Loc[k.l2]<<StrDate(k.Date+plus)<<StrTime(t.Time1[k.l2]);
	}
};




#endif //AKINA_TRAINMANAGER_HPP

//
// Created by ReFraction on 2019/5/23.
//

#include "BPlusTree.hpp"
#include "vector.hpp"
#include <iostream>
#include <cassert>
#include <string>

#define DEBUG

const size_t Nodesize = 4096;

class Integer {
public:
    static int counter;
    int val;

    Integer(int val = 0) : val(val) {
        counter++;
    }

    Integer(const Integer &rhs) {
        val = rhs.val;
        counter++;
    }

    Integer& operator = (const Integer &rhs) = default;

    ~Integer() {
        counter--;
    }
};

int Integer::counter = 0;

class Compare {
public:
    bool operator () (const std::pair<int,long long> &lhs, const std::pair<int,long long> &rhs) const {
        return lhs.first < rhs.first;
    }
};
const int keysize =2000;
//int used[keysize][20];
void tester(void) {
    //	test: constructor
    BPlusTree< std::pair<int,long long>, int,Nodesize, std::less< std::pair<int ,long long> > > bptree("./BPlusTreeMaintaining/database.txt","./BPlusTreeMaintaining/ex_memory.txt");
//#ifdef DEBUG
//    bptree.printinfo();
//#endif
    //remember to reverse and random;
    srand(time(0));
//    memset(used,0,sizeof(used));
    bptree.Insert(std::make_pair(rand() % 10,rand() % 14),rand() % 15);
    std::cout << bptree.Size() << "\n";
    for (int i = 0; i <= 5000; ++i) {
        int tmp = rand() % keysize;
        int tmp1 = rand() % 20;
        int tmp2 = (tmp) % 97 + tmp1 % 7;
        int res = bptree.Insert(std::make_pair(tmp,tmp1),tmp2 + 1);
//        if (used[tmp][tmp1])
//            assert(!res);
//        else
//        {
//            assert(res);
//            used[tmp][tmp1] = 1 + tmp2;
//        }
    }
    std::cout << "size_after_insert: " << bptree.Size() << "\n";
    for (int i = 0; i < 5000; ++i)
    {
        int data;
        int tmp = rand() % keysize;
        int tmp1 = rand() % 20;
        int ans = bptree.Find(std::make_pair(tmp,tmp1),data);
//        assert((ans == 0 && used[tmp][tmp1] == 0) || (data == used[tmp][tmp1]));
    }
    std::cout << "size_after_find: " << bptree.Size() << "\n";
//    for (int i = 0; i < 10000; ++i)
//    {
//        int data;
//        int tmp = rand() % keysize;
//        int tmp1 = rand() % 20;
//        int ans = bptree.Find(std::make_pair(tmp,tmp1),data);
//        if (ans)
//        {
//            std::cout << "i,ans,data " << i << " " << ans << " " << data << "\n";
//            int ans = bptree.Modify(std::make_pair(tmp,tmp1),data * 2);
//            assert(ans);
//            int data2;
//            int res = bptree.Find(std::make_pair(tmp,tmp1),data2);
//            std::cout << "i,res,data2 " << i << " " << res << " " << data2 << "\n";
//            assert(res && data2 == used[tmp][tmp1] * 2);
//            used[tmp][tmp1] = data2;
//        }
//        else
//        {
//            int ans = bptree.Modify(std::make_pair(tmp,tmp1),data * 2);
//            assert(!ans);
//        }
//    }
    std::cout << "size_after_modify: " << bptree.Size() << "\n";
    for (int i = 0; i < 5000; ++i)
    {
        sjtu::vector<std::pair<int ,long long>> vect1;
        sjtu::vector<int> vect2;
        Compare CMP = Compare();
        int tmp = rand() % keysize;
        int tmp1 = rand() % 20;
        auto result = bptree.AskArr(std::make_pair(tmp,tmp1),CMP,vect1,vect2);
        int flag = 0;
//        for (int i = 0;i < 20; ++i)
//            if (used[tmp][i]){
//                flag = 1;
//                break;
//            }
//        if (flag)
//        {
//            assert(result);
//            int cnt = 0;
//            for (long long i = 0;i < 20; ++i)
//                if (used[tmp][i])
//                {
////#ifdef DEBUG
////                    std::cout << "vect1[cnt],vect2[cnt],make_pair(tmp,i),used[tmp][i]   tmp i:\n" << "(" << vect1[cnt].first << "," << vect1[cnt].second << ")," << vect2[cnt] << ",(" << tmp << "," << i << ")," << used[tmp][i] << "    " << tmp << " " << i << "\n";
////                    #endif
//                    assert(vect1[cnt] == std::make_pair(tmp,i) && used[tmp][i] == vect2[cnt]);
//                    ++cnt;
//                }
//        }
//        else
//            assert(!result);
        vect1.clear();
        vect2.clear();
    }
    std::cout << "size_after_askarr: " << bptree.Size() << "\n";
//    bptree.Clear();
    std::cout << "size_after_clear: " << bptree.Size() << "\n";
}

int main(void) {
    tester();
    std::cout << Integer::counter << std::endl;
}

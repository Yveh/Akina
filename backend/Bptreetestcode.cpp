//
// Created by ReFraction on 2019/5/23.
//

#include "BPlusTree.hpp"
#include "./vector/vector.hpp"
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

    Integer& operator = (const Integer &rhs) {
        assert(false);
    }

    ~Integer() {
        counter--;
    }
};

int Integer::counter = 0;

class Compare {
public:
    bool operator () (const Integer &lhs, const Integer &rhs) const {
        return lhs.val < rhs.val;
    }
};

void tester(void) {
    //	test: constructor
    BPlusTree<Integer, std::string,Nodesize, Compare> bptree("./BPlusTreeMaintaining/database.txt","./BPlusTreeMaintaining/ex_memory.txt");
    bptree.printinfo();
    for (int i = 0; i < 100000; ++i) {
        std::string string = "";
        for (int number = i; number; number /= 10) {
            char digit = '0' + number % 10;
            string = digit + string;
        }
        if (i & 1) {
             bptree.Insert(Integer(i),string);
            auto result = bptree.Insert(Integer(i),string);
            assert(!result);
        } else {
            auto result = bptree.Insert(Integer(i),string);
            assert(result);
        }
    }
#ifdef DEBUG
    std::cout << "????\n";
#endif
    for (int i = 0; i < 100000; i += 2) {
        bptree.Erase(Integer(i));
    }
    for (int i = 0; i < 100000; ++i) {
        auto result = bptree.Erase(Integer(i));
        if (i & 1)
            assert(result);
        else
            assert(!result);
    }
    for (int i = 0; i < 100000; ++i) {
        std::string string = "";
        for (int number = i; number; number /= 10) {
            char digit = '0' + number % 10;
            string = digit + string;
        }
        if (i & 1) {
            bptree.Insert(Integer(i),string);
            auto result = bptree.Insert(Integer(i),string);
            assert(!result);
        } else {
            auto result = bptree.Insert(Integer(i),string);
            assert(result);
        }
    }
    for (int i = 0; i < 100000; ++i) {
        std::string string = "";
        for (int number = i; number; number /= 16) {
            char digit = '0' + number % 16;
            string = digit + string;
        }
        if (i & 1) {
            bptree.Insert(Integer(i),string);
            auto result = bptree.Modify(Integer(i),string);
            assert(!result);
        } else {
            auto result = bptree.Modify(Integer(i),string);
            assert(result);
        }
    }
    for (int i = 0; i < 100000; ++i) {
        std::string data;
        auto result = bptree.Find(Integer(i),data);
        assert(result);
    }
    for (int i = 0; i < 100000; ++i)
    {
        sjtu::vector<Integer> vect1;
        sjtu::vector<std::string> vect2;
        Compare CMP = Compare();
        auto result = bptree.AskArr(Integer(i),CMP,vect1,vect2);
        assert(result);
    }
}

int main(void) {
    tester();
    std::cout << Integer::counter << std::endl;
}

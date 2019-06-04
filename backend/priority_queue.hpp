//
// Created by ReFraction on 2019/6/2.
//

#ifndef AKINA_PRIORITY_QUEUE_HPP
#define AKINA_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 * it should be based on the vector written by yourself.
 */
    template<typename T, class Compare = std::less<T>>
    class priority_queue {
    private:
        class node
        {
        public:
            T key;
            size_t degree;
            node *left, *right, *child;

            node(const T &_key, const size_t &_degree = 0) : degree(_degree), key(_key), left(this), right(this), child(nullptr) {}
            ~node() {}
        };

        size_t keyNum;
        node *min;
        node **A;
        Compare cc;

        void del(node *cur)
        {
            if (cur == nullptr)
                return;
            node *tmp = cur, *_right;
            do
            {
                del(tmp->child);
                _right = tmp->right;
                delete tmp;
                tmp = _right;
            } while(tmp != cur);
        }
        void cra(node **cur, node *other)
        {
            if (other == nullptr)
                return;
            node *tail = new node(other->key, other->degree);
            node *head = tail;
            cra(&(tail->child), other->child);
            node *tmp = other->right;
            while (tmp != other)
            {
                tail->right = new node(tmp->key, tmp->degree);
                tail->right->left = tail;
                tail = tail->right;

                cra(&(tail->child), tmp->child);

                tmp = tmp->right;
            }
            (*cur) = head;
            tail->right = head;
            head->left = tail;
        }
    public:
        /**
         * TODO constructors
         */
        priority_queue()
        {
            keyNum = 0;
            min = nullptr;
            A = new node*[64];
            for (int i = 0; i < 64; i++)
                A[i] = nullptr;
        }
        priority_queue(const priority_queue &other)
        {
            min = nullptr;
            cra(&min, other.min);
            keyNum = other.keyNum;
            A = new node*[64];
            for (int i = 0; i < 64; i++)
                A[i] = nullptr;
        }
        /**
         * TODO deconstructor
         */
        ~priority_queue()
        {
            del(min);
            delete []A;
        }
        /**
         * TODO Assignment operator
         */
        priority_queue &operator=(const priority_queue &other)
        {
            if (this == &other)
                return *this;
            del(min);
            cra(&min, other.min);
            keyNum = other.keyNum;
            return *this;
        }
        /**
         * get the top of the queue.
         * @return a reference of the top element.
         * throw container_is_empty if empty() returns true;
         */
        const T & top() const
        {
            if (empty())
                throw container_is_empty();
            return min->key;
        }
        /**
         * TODO
         * push new element to the priority queue.
         */
        void addToList(node *a, node *list)
        {
            list->left->right = a;
            a->left->right = list;
            std::swap(a->left, list->left);
        }
        void push(const T &e)
        {
            node *nd = new node(e);
            if (empty())
                min = nd;
            else
            {
                addToList(nd, min);
                if (cc(min->key, e))
                    min = nd;
            }
            ++keyNum;
        }
        /**
         * TODO
         * delete the top element.
         * throw container_is_empty if empty() returns true;
         */
        void consolidate()
        {
            int lgn = 1;
            for (int i = 1; i <= keyNum + 1; i *= 2, ++lgn);

            node *tmp = min, *_right = min->right, *x, *y;
            do
            {
                tmp = _right;
                _right = tmp->right;
                x = tmp;
                size_t d = tmp->degree;

                while (A[d] != nullptr)
                {
                    y = A[d];
                    if (cc(x->key, y->key))
                        std::swap(x, y);
                    y->left->right = y->right;
                    y->right->left = y->left;
                    y->left = y;
                    y->right = y;
                    if (x->child == nullptr)
                        x->child = y;
                    else
                        addToList(y, x->child);
                    ++x->degree;
                    A[d] = nullptr;
                    ++d;
                }
                A[d] = x;
            } while(tmp != min);

            min = nullptr;
            for (int i = 0; i < lgn; i++)
                if (A[i] != nullptr)
                {
                    A[i]->left->right = A[i]->right;
                    A[i]->right->left = A[i]->left;
                    A[i]->left = A[i];
                    A[i]->right = A[i];
                    if (min == nullptr)
                        min = A[i];
                    else
                    {
                        addToList(A[i], min);
                        if (cc(min->key, A[i]->key))
                            min = A[i];
                    }
                    A[i] = nullptr;
                }
        }
        void pop()
        {
            if (empty())
                throw container_is_empty();
            node *tmp = min->child;
            if (tmp != nullptr)
                addToList(tmp, min);
            --keyNum;
            if (empty())
            {
                delete min;
                min = nullptr;
            }
            else
            {
                min->left->right = min->right;
                min->right->left = min->left;
                tmp = min->right;
                delete min;
                min = tmp;
                consolidate();
            }
        }
        /**
         * return the number of the elements.
         */
        size_t size() const
        {
            return keyNum;
        }
        /**
         * check if the container has at least an element.
         * @return true if it is empty, false if it has at least an element.
         */
        bool empty() const
        {
            return keyNum == 0;
        }
        /**
         * return a merged priority_queue with at least O(logn) complexity.
         */
        void merge(priority_queue &other)
        {
            if (other.empty()) return;
            if (empty())
            {
                min = other.min;
                keyNum = other.keyNum;
            }
            else
            {
                addToList(other.min, min);
                if (cc(min->key, other.min->key))
                    min = other.min;
                keyNum += other.keyNum;
            }
            other.min = nullptr;
            other.keyNum = 0;
        }
    };

}

#endif //AKINA_PRIORITY_QUEUE_HPP

/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include <iostream>
#include "utility.hpp"
#include "exceptions.hpp"
//#define DEBUG
namespace sjtu {

    template<
            class Key,
            class T,
            class Compare = std::less<Key>
    > class map {
        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::map as value_type by typedef.
         */
    public:
        typedef pair<const Key, T> value_type;
    private:
        struct node{
            value_type data;
            node *l_son,*r_son,*pre,*nex;
            int height;
            map *mp;
            node(map *mp,value_type data,node *l_son = nullptr,node *r_son = nullptr,int height = 1,node *pre = nullptr,node *nex = nullptr):mp(mp),data(data),l_son(l_son),r_son(r_son),height(height),pre(pre),nex(nex){}
        };
        node *rt;
        node *tail;
        node **to_tail;
        size_t sz;
        // return a new create subtree whose pre and nex are not maintained.
        node *find_first()
        {
            node *nownode = rt;
            if (nownode == nullptr)
                return nullptr;
            while (nownode->l_son != nullptr)
                nownode = nownode->l_son;
            return nownode;
        }
        node *find_last()
        {
            node *nownode = rt;
            if (nownode == nullptr)
                return nullptr;
            while (nownode->r_son != nullptr)
                nownode = nownode->r_son;
            return nownode;
        }
        void calc_tail()
        {
            node *nd = rt;
            if (rt == nullptr)
            {
                tail = rt;
                return;
            }
            while (nd -> r_son != nullptr)
                nd = nd->r_son;
            tail = nd;
        }
        node *copy_subtree(node *root)
        {
            if (root == nullptr)
                return nullptr;
            node *nownode = new node(this,root->data,copy_subtree(root->l_son),copy_subtree(root->r_son),root->height);
            return nownode;
        }
        // maintain the pre and nex information after copy. remember to set nowpre nullptr before using.
        node *nowpre = nullptr;
        void find_pre_and_nex(node *root)
        {
            if (root == nullptr)
                return;
            find_pre_and_nex(root->l_son);
            if (nowpre != nullptr)
                nowpre->nex = root;
            root->pre = nowpre;
            if (root->pre != nullptr)
                root->pre->nex = root;
            nowpre = root;
            find_pre_and_nex(root->r_son);
        }
        // find the key node in subtree. return nullptr if there's not such a point, else return the node.
        node *find_subtree(node *root,node *place) const
        {
            if (root == nullptr)
                return nullptr;
            const Key &nowkey = root->data.first;
            if (!Compare()(place->data.first,nowkey) && !Compare()(nowkey,place->data.first))
            {
                if (root == place)
                    return root;
                else
                    return nullptr;
            }
            if (Compare()(place->data.first,nowkey))
                return find_subtree(root->l_son,place);
            else
                return find_subtree(root->r_son,place);
        }
        node *find_subtree(node *root,const Key &key) const
        {
            if (root == nullptr)
                return nullptr;
            const Key &nowkey = root->data.first;
            if (!Compare()(key,nowkey) && !Compare()(nowkey,key))
                return root;
            if (Compare()(key,nowkey))
                return find_subtree(root->l_son,key);
            else
                return find_subtree(root->r_son,key);
        }
        // to clear all the element in the subtree.
        void clear_subtree(node *root)
        {
            if (root == nullptr)
                return;
            clear_subtree(root->l_son);
            clear_subtree(root->r_son);
            delete root;
        }
        // to calc height for the subtree after insert or rotate.
        int calc_height(node *root)
        {
            if (root == nullptr)
                return 0;
            int ans = 0;
            if (root->l_son != nullptr)
                ans = std::max(ans,root->l_son->height);
            if (root->r_son != nullptr)
                ans = std::max(ans,root->r_son->height);
            return root->height = ans + 1;
        }
        //rotate and maintain the height information.
        node *rotate_L(node *root)
        {
            node *nowson = root->l_son;
            root->l_son = nowson->r_son;
            nowson->r_son = root;
            root->height = calc_height(root);
            nowson->height = calc_height(nowson);
            return nowson;
        }
        node *rotate_R(node *root)
        {
            node *nowson = root->r_son;
            root->r_son = nowson->l_son;
            nowson->l_son = root;
            root->height = calc_height(root);
            nowson->height = calc_height(nowson);
            return nowson;
        }
        //to insert a key which has been find non-existent.(use after a find and need another find to provide iterator.)
        node *insert_subtree(node* root,const value_type &data,node *father)
        {
            if (root == nullptr)
            {
                node *new_node = new node(this,data, nullptr, nullptr,1, nullptr, nullptr);
                if (father != nullptr)
                {
                    if (Compare()(data.first,father->data.first))
                    {
                        if (father->pre != nullptr)
                            father->pre->nex = new_node;
                        new_node->pre = father->pre;
                        new_node->nex = father;
                        father->pre = new_node;
                    }
                    else
                    {
                        if (father->nex != nullptr)
                            father->nex->pre = new_node;
                        new_node->nex = father->nex;
                        new_node->pre = father;
                        father->nex = new_node;
                    }
                }
                return new_node;
            }
            if (Compare()(data.first,root->data.first))
            {
                root->l_son = insert_subtree(root->l_son,data,root);
                root->height = 	calc_height(root);
                if (calc_height(root->l_son) - calc_height(root->r_son) > 1)
                {
                    if (calc_height(root->l_son->l_son) < calc_height(root->l_son->r_son))
                        root->l_son = rotate_R(root->l_son);
                    return rotate_L(root);
                }
                return root;
            }
            else
            {
                root->r_son = insert_subtree(root->r_son,data,root);
                root->height = calc_height(root);
                if (calc_height(root->r_son) - calc_height(root->l_son) > 1)
                {
                    if (calc_height(root->r_son->r_son) < calc_height(root->r_son->l_son))
                        root->r_son = rotate_L(root->r_son);
                    return rotate_R(root);
                }
                return root;
            }
        }
        //to adjust the subtree after delete, whose son's subtree are balanced.
        node *adjust(node *root)
        {
            int lh = calc_height(root->l_son),rh = calc_height(root->r_son);
            if (abs(lh - rh) < 2)
                return root;
            if (lh > rh)
            {
                int llh = calc_height(root->l_son->l_son),lrh = calc_height(root->l_son->r_son);
                if (llh < lrh)
                    root->l_son = rotate_R(root->l_son);
                return rotate_L(root);
            }
            else
            {
                int rlh = calc_height(root->r_son->l_son),rrh = calc_height(root->r_son->r_son);
                if (rrh < rlh)
                    root->r_son = rotate_L(root->r_son);
                return rotate_R(root);
            }
        }
        //to delete a vertex,return the deleted subtree.If there's not such a point ,throw.
        node* delete_vertex(node *place,node *root)
        {
            if (root == nullptr)
                throw(index_out_of_bound());
            if (root == place)
            {
                if (root->l_son != nullptr && root->r_son != nullptr)
                {
                    node *cur_fa = root,*cur = root->r_son;
                    while (cur->l_son != nullptr)
                        cur_fa = cur,cur = cur->l_son;
                    if(cur_fa == root)
                    {
                        std::swap(cur->l_son,root->l_son);
                        std::swap(cur->r_son,root->r_son);
                        if (root->pre != nullptr)
                            root->pre->nex = cur;
                        if (cur->nex != nullptr)
                            cur->nex->pre = root;
                        cur->r_son = root;
                        cur->pre = root->pre;
                        root->nex = cur->nex;
                        cur->nex = root;
                        root->pre = cur;
                        std::swap(cur->height,root->height);
                        cur->r_son = delete_vertex(root,root);
                        return adjust(cur);
                    }
                    else
                    {
                        std::swap(cur->l_son,root->l_son);
                        std::swap(cur->r_son,root->r_son);
                        if (root->pre != nullptr)
                            root->pre->nex = cur;
                        if (cur->nex != nullptr)
                            cur->nex->pre = root;
                        cur->pre = root->pre;
                        root->nex = cur->nex;
                        cur->nex = root;
                        root->pre = cur;
                        std::swap(cur->height,root->height);
                        cur_fa->l_son = root;
                        cur->r_son  = delete_vertex(root,cur->r_son);
                        return adjust(cur);
                    }
                }
                else
                {
                    if (root->pre != nullptr)
                        root->pre->nex = root->nex;
                    if (root->nex != nullptr)
                        root->nex->pre = root->pre;
                    if (root->l_son == nullptr && root->r_son == nullptr)
                    {
                        delete root;
                        return nullptr;
                    }
                    if (root->l_son == nullptr)
                    {
                        node *tmp = root->r_son;
                        delete root;
                        return tmp;
                    }
                    if (root->r_son == nullptr)
                    {
                        node *tmp = root->l_son;
                        delete root;
                        return tmp;
                    }
                }
            }
            else
            {
                if (Compare()(place->data.first,root->data.first))
                    root->l_son = delete_vertex(place,root->l_son);
                else
                    root->r_son = delete_vertex(place,root->r_son);
            }
            node *tmp = adjust(root);
            return tmp;
        }
    public:
        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = map.begin(); --it;
         *       or it = map.end(); ++end();
         */
        class const_iterator;
        class iterator {
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            friend map;
            node **pre, **nex;
        public:
            node *nd;
            iterator(node *nd = nullptr,node **pre = nullptr,node **nex = nullptr) : nd(nd),pre(pre),nex(nex) {}
            iterator(const iterator &other) : nd(other.nd),pre(other.pre),nex(other.nex){}
            iterator(const const_iterator &other) : nd(other.nd),pre(other.pre),nex(other.nex){}
            /**
             * return a new iterator which pointer n-next elements
             *   even if there are not enough elements, just return the answer.
             * as well as operator-
             */
            iterator operator + (const int &n) const
            {
                int cnt = n;
                if (nd == nullptr && n == 0)
                    return *this;
                node *now_node = nd;
                if (now_node == nullptr)
                    throw(invalid_iterator());
                map *tmp = nd->mp;
                while (cnt--)
                {
                    if (now_node == nullptr)
                        throw(invalid_iterator());
                    now_node = now_node->nex;
                }
                if (now_node != nullptr)
                    return iterator(now_node,&(now_node->pre),&(now_node->pre));
                else
                    return tmp->end();
            }
            iterator operator - (const int &n) const
            {
                if (n == 0)
                    return *this;
                int cnt = n;node *now_node = nd;
                while (cnt--)
                {
                    if (now_node == nullptr && pre == nullptr)
                        throw(invalid_iterator());
                    if (now_node == nullptr)
                        now_node = *pre;
                    else
                        now_node = now_node->pre;
                }
                if (now_node != nullptr)
                    return iterator(now_node,(now_node->pre == nullptr) ? nullptr : &(now_node->pre),(now_node->nex == nullptr) ? nullptr : &(now_node->nex));
                else
                    throw(invalid_iterator());
            }

            /**
             * TODO iter++
             */
            iterator operator++(int)
            {
                auto tmp = *this;
                *this = *this + 1;
                return tmp;
            }
            /**
             * TODO ++iter
             */
            iterator & operator++() {
                return *this = *this + 1;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                auto tmp = *this;
                *this = *this - 1;
                return tmp;
            }
            /**
             * TODO --iter
             */
            iterator & operator--() {
                return *this = *this - 1;
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type & operator*() const { return nd->data; }
            bool operator==(const iterator &rhs) const { return nd == rhs.nd && pre == rhs.pre && nex == rhs.nex;}
            bool operator==(const const_iterator &rhs) const {return nd == rhs.nd && pre == rhs.pre && nex == rhs.nex;}
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return nd != rhs.nd || pre != rhs.pre || nex != rhs.nex;}
            bool operator!=(const const_iterator &rhs) const { return nd != rhs.nd || pre != rhs.pre || nex != rhs.nex;}

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type* operator->() const noexcept {
                return &(nd->data);}
        };
        class const_iterator {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        private:
            // data members.
            friend map;
            node *nd,**pre,**nex;
        public:
            const_iterator(node *nd = nullptr,node **pre = nullptr,node **nex = nullptr) : nd(nd),pre(pre),nex(nex) {}
            const_iterator(const const_iterator &other) : nd(other.nd),pre(other.pre),nex(other.nex){}
            const_iterator(const iterator &other) : nd(other.nd),pre(other.pre),nex(other.nex){}
            // And other methods in iterator.
            const_iterator operator + (const int &n) const
            {
                int cnt = n;
                if (nd == nullptr && n == 0)
                    return *this;
                node *now_node = nd;
                if (now_node == nullptr)
                    throw(invalid_iterator());
                map *tmp = nd->mp;
                while (cnt--)
                {
                    if (now_node == nullptr)
                        throw(invalid_iterator());
                    now_node = now_node->nex;
                }
                if (now_node != nullptr)
                    return const_iterator(now_node,&(now_node->pre),&(now_node->pre));
                else
                    return tmp->cend();
            }
            const_iterator operator - (const int &n) const
            {
                if (n == 0)
                    return *this;
                int cnt = n;node *now_node = nd;
                while (cnt--)
                {
                    if (now_node == nullptr && pre == nullptr)
                        throw(invalid_iterator());
                    if (now_node == nullptr)
                        now_node = *pre;
                    else
                        now_node = now_node->pre;
                }
                if (now_node != nullptr)
                    return const_iterator(now_node,now_node->pre == nullptr ? nullptr : &(now_node->nex),&(now_node->nex));
                else
                    throw(invalid_iterator());
            }
            const_iterator operator++(int)
            {
                auto tmp = *this;
                *this = *this + 1;
                return tmp;
            }
            const_iterator & operator++() {
                return *this = *this + 1;
            }
            const_iterator operator--(int) {
                auto tmp = *this;
                *this = *this - 1;
                return tmp;
            }
            const_iterator & operator--() {
                return *this = *this - 1;
            }
            value_type & operator*() const { return nd->data; }
            bool operator==(const iterator &rhs) const {return nd == rhs.nd && pre == rhs.pre && nex == rhs.nex;}
            bool operator==(const const_iterator &rhs) const {return nd == rhs.nd && pre == rhs.pre && nex == rhs.nex;}
            bool operator!=(const iterator &rhs) const {return nd != rhs.nd || pre != rhs.pre || nex != rhs.nex;}
            bool operator!=(const const_iterator &rhs) const { return nd != rhs.nd || pre != rhs.pre || nex != rhs.nex;}
            value_type* operator->() const noexcept {return &(nd->data);}
            // And other methods in iterator.
        };
        /**
         * TODO two constructors
         */
        map():rt(nullptr),tail(nullptr),to_tail(&tail),sz(0){}
        map(const map &other)
        {
            rt = copy_subtree(other.rt);
            sz = other.sz;
            nowpre = nullptr;
            find_pre_and_nex(rt);
            calc_tail();
            to_tail = &tail;
        }
        /**
         * TODO assignment operator
         */
        map & operator=(const map &other)
        {
            if (this == &other)
                return *this;
            clear();
            rt = copy_subtree(other.rt);
            sz = other.sz;
            nowpre = nullptr;
            find_pre_and_nex(rt);
            calc_tail();
            to_tail = &tail;
            return *this;
        }
        /**
         * TODO Destructors
         */
        ~map()
        {
            clear();
            rt = tail = nullptr;
        }
        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */
        T & at(const Key &key)
        {
            iterator element = find(key);
            if (element == end())
                throw(index_out_of_bound());
            return element->second;
        }
        const T & at(const Key &key) const
        {
            iterator element = find(key);
            if (element == end())
                throw(index_out_of_bound());
            return element->second;
        }
        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T & operator[](const Key &key)
        {
            iterator element = find(key);
            if (element != end())
                return at(key);
            return insert(pair<Key,T>(key,T())).first->second;
        }
        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T & operator[](const Key &key) const
        {
            return at(key);
        }
        /**
         * return a iterator to the beginning
         */
        iterator begin()
        {
            node *nd = find_first();
            if (nd != nullptr)
                return iterator(nd, nullptr,&(nd->nex));
            return end();
        }
        const_iterator cbegin() const
        {
            node *nd = rt;
            if (rt != nullptr)
                for (;nd->l_son != nullptr;nd = nd->l_son);
            if (nd != nullptr)
                return const_iterator(nd, nullptr,&(nd->nex));
            return cend();
        }
        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end() const
        {
            return iterator(nullptr,to_tail, nullptr);
        }
        const_iterator cend() const
        {
            return const_iterator(nullptr,to_tail, nullptr);
        }
        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const {return rt == nullptr;}
        /**
         * returns the number of elements.
         */
        size_t size() const {return sz;}
        /**
         * clears the contents
         */
        void clear()
        {
            clear_subtree(rt);
            rt = tail = nullptr;
            sz = 0;
        }
        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type &value)
        {
            map::iterator it = find(value.first);
            if (it != end())
                return pair<iterator, bool>(it,false);
            rt = insert_subtree(rt,value, nullptr);
            sz++;
            calc_tail();
            return pair<iterator,bool>(find(value.first),true);
        }
        /**
         * erase the element at pos.
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos)
        {
            if (pos.nd == nullptr||find_subtree(rt,pos.nd) == nullptr)
                throw(runtime_error());
            rt = delete_vertex(pos.nd,rt);
            calc_tail();
            --sz;
        }
        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key &key) const
        {
            return find(key) == end() ? 0 : 1;
        }
        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key &key)
        {
            node *nd = find_subtree(rt,key);
            if (nd == nullptr)
                return end();
            else
                return iterator(nd,(nd->pre == nullptr) ? nullptr : &(nd->pre),(nd->nex == nullptr) ? nullptr : &(nd->nex));
        }
        const_iterator find(const Key &key) const
        {
            node *nd = find_subtree(rt,key);
            if (nd == nullptr)
                return cend();
            else
                return const_iterator(nd,(nd->pre == nullptr) ? nullptr : &(nd->pre),(nd->nex == nullptr) ? nullptr : &(nd->nex));
        }
    };

}

#endif
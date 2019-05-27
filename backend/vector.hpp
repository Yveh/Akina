#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
	template<typename T>
	class vector
	{
	private:
		T *storage;
		size_t elementnumber;
		size_t capa;

		void double_the_capacity()
		{
			T *tmp = storage;
			storage = new T[capa *= 2];
			for (size_t i = 0; i < elementnumber; ++i)
				storage[i] = tmp[i];
		}

	public:
		/**
		 * TODO
		 * a type for actions of the elements of a vector, and you should write
		 *   a class named const_iterator with same interfaces.
		 */
		/**
		 * you can see RandomAccessIterator at CppReference for help.
		 */
		class const_iterator;

		class iterator
		{
			friend class vector<T>;

		private:
			/**
			 * TODO add data members
			 *   just add whatever you want.
			 */
			T *element;
			size_t pos;

			iterator(T *ele, size_t pos) : element(ele), pos(pos)
			{}

		public:
			/**
			 * return a new iterator which pointer n-next elements
			 *   even if there are not enough elements, just return the answer.
			 * as well as operator-
			 */
			iterator operator+(const int &n) const
			{
				return iterator(element + n, pos + n);
			}

			iterator operator-(const int &n) const
			{
				return iterator(element - n, pos - n);
			}

			// return th distance between two iterator,
			// if these two iterators points to different vectors, throw invaild_iterator.
			int operator-(const iterator &rhs) const
			{
				if (element - rhs.element != pos - rhs.pos)
					throw (invalid_iterator());
				return element - rhs.element;
			}

			iterator operator+=(const int &n)
			{
				return *this = *this + n;
			}

			iterator operator-=(const int &n)
			{
				return *this = *this - n;
			}

			/**
			 * TODO iter++
			 */
			iterator operator++(int)
			{
				iterator tmp = *this;
				*this += 1;
				return tmp;
			}

			/**
			 * TODO ++iter
			 */
			iterator &operator++()
			{
				return *this += 1;
			}

			/**
			 * TODO iter--
			 */
			iterator operator--(int)
			{
				iterator tmp = *this;
				*this -= 1;
				return tmp;
			}

			/**
			 * TODO --iter
			 */
			iterator &operator--()
			{
				return *this -= 1;
			}

			/**
			 * TODO *it
			 */
			T &operator*() const
			{
				return *element;
			}

			/**
			 * a operator to check whether two iterators are same (pointing to the same memory).
			 */
			bool operator==(const iterator &rhs) const
			{
				return element == rhs.element;
			}

			bool operator==(const const_iterator &rhs) const
			{
				return element == rhs.element;
			}

			/**
			 * some other operator for iterator.
			 */
			bool operator!=(const iterator &rhs) const
			{
				return element != rhs.element;
			}

			bool operator!=(const const_iterator &rhs) const
			{
				return element != rhs.element;
			}
		};

		/**
		 * TODO
		 * has same function as iterator, just for a const object.
		 */
		class const_iterator
		{
		private:
			T *element;
			size_t pos;

			const_iterator(T ele, size_t pos) : element(ele), pos(pos)
			{}

		public:
			const_iterator operator+(const int &n) const
			{
				return iterator(element + n, pos + n);
			}

			const_iterator operator-(const int &n) const
			{
				return iterator(element - n, pos - n);
			}

			int operator-(const iterator &rhs) const
			{
				if (element - rhs.element != pos - rhs.pos)
					throw (invalid_iterator());
				return element - rhs.element;
			}

			const_iterator operator+=(const int &n)
			{
				return *this = *this + n;
			}

			const_iterator operator-=(const int &n)
			{
				return *this = *this - n;
			}

			const_iterator operator++(int)
			{
				iterator tmp = *this;
				*this += 1;
				return tmp;
			}

			const_iterator &operator++()
			{
				return *this += 1;
			}

			const_iterator operator--(int)
			{
				iterator tmp = *this;
				*this -= 1;
				return tmp;
			}

			const_iterator &operator--()
			{
				return *this -= 1;
			}

			const T operator*() const
			{
				return *element;
			}

			bool operator==(const iterator &rhs) const
			{
				return element == rhs.element;
			}

			bool operator==(const const_iterator &rhs) const
			{
				return element == rhs.element;
			}

			bool operator!=(const iterator &rhs) const
			{
				return element != rhs.element;
			}

			bool operator!=(const const_iterator &rhs) const
			{
				return element != rhs.element;
			}
		};

		/**
		 * TODO Constructs
		 * Atleast three: default constructor, copy constructor and a constructor for std::vector
		 */
		vector(size_t n = 10)
		{
			storage = new T[n];
			elementnumber = 0;
			capa = n;
		}

		vector(const vector &other)
		{
			elementnumber = other.size();
			storage = new T[capa = other.capacity()];
			for (size_t i = 0; i < elementnumber; ++i)
				storage[i] = other[i];
		}

		/**
		 * TODO Destructor
		 */
		~vector()
		{
			delete storage;
		}

		/**
		 * TODO Assignment operator
		 */
		vector &operator=(const vector &other)
		{
			delete storage;
			storage = new T[capa = other.capacity()];
			elementnumber = other.size();
			for (size_t i = 0; i < elementnumber; ++i)
				storage[i] = other[i];
		}

		/**
		 * assigns specified element with bounds checking
		 * throw index_out_of_bound if pos is not in [0, size)
		 */
		T &at(const size_t &pos)
		{
			if (pos < 0 || pos >= elementnumber)
				throw (index_out_of_bound());
			return storage[pos];
		}

		const T &at(const size_t &pos) const
		{
			if (pos < 0 || pos >= elementnumber)
				throw (index_out_of_bound());
			return storage[pos];
		}

		/**
		 * assigns specified element with bounds checking
		 * throw index_out_of_bound if pos is not in [0, size)
		 * !!! Pay attentions
		 *   In STL this operator does not check the boundary but I want you to do.
		 */
		T &operator[](const size_t &pos)
		{
			return at(pos);
		}

		const T &operator[](const size_t &pos) const
		{
			return at(pos);
		}

		/**
		 * access the first element.
		 * throw container_is_empty if size == 0
		 */
		const T &front() const
		{
			if (empty())
				throw (container_is_empty());
			return storage[0];
		}

		/**
		 * access the last element.
		 * throw container_is_empty if size == 0
		 */
		const T &back() const
		{
			if (empty())
				throw (container_is_empty());
			return storage[elementnumber];
		}

		/**
		 * returns an iterator to the beginning.
		 */
		iterator begin()
		{
			return iterator(storage, 0);
		}

		const_iterator cbegin() const
		{
			return const_iterator(storage, 0);
		}

		/**
		 * returns an iterator to the end.
		 */
		iterator end()
		{
			return iterator(storage + elementnumber, elementnumber);
		}

		const_iterator cend() const
		{
			return const_iterator(storage + elementnumber, elementnumber);
		}

		/**
		 * checks whether the container is empty
		 */
		bool empty() const
		{
			return elementnumber == 0;
		}

		/**
		 * returns the number of elements
		 */
		size_t size() const
		{
			return elementnumber;
		}

		size_t capacity() const
		{
			return capa;
		}

		void clear()
		{
			elementnumber = 0;
		}

		iterator insert(iterator pos, const T &value)
		{
			if (elementnumber == capa)
				double_the_capacity();
			++elementnumber;
			for (size_t i = elementnumber; i > pos.pos(); --i)
				storage[i] = storage[i - 1];
			size_t insert_pos = pos.pos();
			storage[insert_pos] = value;
			return iterator(&storage[insert_pos], insert_pos);
		}

		iterator insert(const size_t &ind, const T &value)
		{
			if (ind > elementnumber)
				throw (index_out_of_bound());
			return insert(iterator(begin() + ind, ind), value);
		}

		iterator erase(iterator pos)
		{
			size_t erase_pos = pos.pos();
			for (size_t i = erase_pos + 1; i < elementnumber; ++i)
				storage[i - 1] = storage[i];
			--elementnumber;
			return iterator(&storage[erase_pos], erase_pos);
		}

		iterator erase(const size_t &ind)
		{
			if (ind >= elementnumber)
				throw (index_out_of_bound());
			return erase(storage[ind], ind);
		}

		void push_back(const T &value)
		{
			storage[elementnumber++] = value;
		}

		void pop_back()
		{
			if (empty())
				throw (container_is_empty());
			else
			{
				elementnumber--;
			}

		}
	};

}


#endif

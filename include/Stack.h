#pragma once

#include <iostream>

template<typename T>
class Vector
{
protected:
	size_t sz;
	size_t capacity;
	T* pMem;
public:
	Vector(size_t size = 0) : sz(size), capacity(sz * 2 + 2)
	{
		pMem = new T[capacity];
		if (pMem == nullptr)
			throw std::bad_alloc();
	}
	Vector(T* arr, size_t s) : sz(s), capacity(sz * 2 + 2)
	{
		if (arr == nullptr)
			throw std::logic_error("Vector ctor requires non - nullptr arg");
		pMem = new T[capacity];
		if (pMem == nullptr)
			throw std::bad_alloc();
		std::copy(arr, arr + sz, pMem);
	}
	Vector(const Vector& v) : sz(v.sz), capacity(v.capacity)
	{
		pMem = new T[capacity];
		if (pMem == nullptr)
			throw std::bad_alloc();
		std::copy(v.pMem, v.pMem + sz, pMem);
	}
	Vector(Vector&& v) noexcept
	{
		sz = 0;
		capacity = 0;
		pMem = nullptr;
		swap(*this, v);
	}
	~Vector()
	{
		delete[] pMem;
		pMem = nullptr;
	}
	Vector& operator=(const Vector& v)
	{
		if (this != &v)
		{
			if (sz != v.sz)
			{
				sz = v.sz;
				delete[] pMem;
				T* p = new T[sz];
				if (p == nullptr)
					throw std::bad_alloc();
				pMem = p;
				p = nullptr;
			}
			std::copy(v.pMem, v.pMem + sz, pMem);
		}
		return (*this);
	}
	Vector& operator=(Vector&& v) noexcept
	{
		sz = 0;
		capacity = 0;
		delete[] pMem;
		pMem = nullptr;
		swap(*this, v);
		return (*this);
	}

	size_t size() const noexcept { return sz; }
	size_t getCapacity() const noexcept { return capacity; }
	bool isFull() const noexcept { return (sz == capacity); }
	bool isEmpty() const noexcept { return (sz == 0); }

	T& operator[] (size_t ind)
	{
		return pMem[ind];
	}
	const T& operator[] (size_t ind) const
	{
		return pMem[ind];
	}

	bool operator==(const Vector& v) const noexcept
	{
		if (sz == v.sz)
		{
			for (size_t i = 0; i < sz; i++)
				if (pMem[i] != v.pMem[i])
					return 0;
			return 1;
		}
		else return 0;
	}
	bool operator!=(const Vector& v) const noexcept
	{
		return !(*this == v);
	}

	friend void swap(Vector& lhs, Vector& rhs) noexcept
	{
		std::swap(lhs.sz, rhs.sz);
		std::swap(lhs.capacity, rhs.capacity);
		std::swap(lhs.pMem, rhs.pMem);
	}

	void resize(size_t newsize)
	{
		T* p = new T[newsize * 2 + 2];
		if (p == nullptr)
			throw std::bad_alloc();
		std::copy(pMem, pMem + sz, p);
		delete[] pMem;
		pMem = p;
		p = nullptr;
		capacity = newsize * 2 + 2;
	}
	void push_back(T elem)
	{
		if (isFull())
			resize(sz + 1);
		pMem[sz++] = elem;
	}
	void pop_back()
	{
		if (sz != 0)
			sz--;
		else throw std::logic_error("Vector is empty");
	}
	void push_front(T elem)
	{
		insert(elem, 0);
	}
	void pop_front()
	{
		erase(0);
	}
	void insert(T elem, size_t ind)
	{
		if (ind < 0 || ind >= capacity)
			throw std::out_of_range("Index should be greater than zero and less than vector size");
		if (sz == capacity)
			resize(sz + 1);
		for (size_t i = sz; i > ind; i--)
			pMem[i] = pMem[i - 1];
		pMem[ind] = elem;
		sz++;
	}
	void erase(size_t ind)
	{
		if (ind < 0 || ind >= capacity)
			throw std::out_of_range("Index should be greater than zero and less than vector size");
		if (sz == 0)
			throw std::logic_error("Vector is empty");
		for (size_t i = ind; i < sz; i++)
			pMem[i] = pMem[i + 1];
		sz--;
	}
};

template<typename T>
class Stack : private Vector<T>
{
	using Vector<T>::sz;
	using Vector<T>::capacity;
	using Vector<T>::pMem;
	using Vector<T>::push_back;
	using Vector<T>::pop_back;
	using Vector<T>::resize;
public:
	using Vector<T>::isFull;
	using Vector<T>::isEmpty;
	using Vector<T>::size;
	using Vector<T>::getCapacity;
	Stack() : Vector<T>(0) { };
	void push(T elem)
	{
		push_back(elem);
	}
	void pop()
	{
		pop_back();
	}
	T top()
	{
		if (isEmpty())
			throw std::logic_error("Stack is empty");
		return pMem[sz - 1];
	}
};

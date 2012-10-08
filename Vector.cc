// Implementation of the templated Vector class
// ECE4893/8893 lab 3
// Phillip Johnston

#include <iostream> // debugging\
#include <cstdio>
#include "Vector.h"

#include <stdint.h>
#include <cstring>

// Your implementation here
// Fill in all the necessary functions below
using namespace std;

/*********************
* Vector Definitions *
*********************/

// Default constructor
template <typename T>
Vector<T>::Vector()
{
	//Construct an empty vector as default
	count = 0;
	reserved = 0;
	elements = NULL;
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs)
{
	count = rhs.count;
	reserved = rhs.reserved;
	elements = (T *) calloc(count, sizeof(T));
	for(int i = 0; i < count; i++)
	{
		elements[i]  = rhs.elements[i];
		//new (&elements[i]) T(rhs.elements[i]);
	}
}


// Destructor
template <typename T>
Vector<T>::~Vector()
{
	T * curr_obj;
	
	for(int32_t i = count - 1; i >= 0; i--)
	{	
		elements[i].~T(); //Call the object's destructor
	}
	
	free(elements); //Destroy
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
	if(count < reserved)
	{
		//If we have reserved the space already, just copy the new obj in
		new (&elements[count++]) T(rhs);
	}
	else
	{
		count++;
		//Need to realloc for contiguity
		T * temp = (T *) realloc(elements, count * sizeof(T));
		if(temp == NULL)
		{
			cout << "Could not allocate contiguous block of size " << count * sizeof(T) << endl;
			fflush(stdout);
		}
		else
		{
			elements = temp;
			//elements[count - 1] = rhs;
			new (&elements[count - 1]) T(rhs);
			reserved = count;
		}
	}
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
	if(count < reserved)
	{
		
		//Move the memory over
		for(uint32_t i = count - 1; i > 0; i--)
		{
			elements[i] = elements[i - 1];
		}
		
		new (&elements[0]) T(rhs); //Place the first element on the list
	}
	else
	{
		//Allocate a new buffer and copy the old data into it
		count++;
		//Need to realloc for contiguity
		T * temp = (T *) realloc(elements, count * sizeof(T));
		
		if(temp == NULL)
		{
			cout << "Could not allocate contiguous block of size " << count * sizeof(T) << endl;
			fflush(stdout);
		}
		else
		{
			elements = temp;
			
			for(uint32_t i = count - 1; i > 0; i--)
			{
				new (&elements[i]) T(elements[i - 1]);
				elements[i - 1].~T();
				//elements[i] = elements[i - 1];
			}
			
			new (elements) T(rhs);
			
			reserved = count;
		}
	}
}

template <typename T>
void Vector<T>::Pop_Back()
{ // Remove last element
	count--; //Decrement the count
	
	elements[count].~T(); //Destruct the element
	
	//We keep reserved at the same count just to ease the burden when adding stuff in later
	//NO FREEDOM EARNED HERE BRO
}

template <typename T>
void Vector<T>::Pop_Front()
{ // Remove first element
	elements[0].~T(); //Call the destructor on the first element
	count--;
	for(uint32_t i = 1; i <= count; i++)
	{
		new (&elements[i - 1]) T(elements[i]);
		elements[i].~T();
	}
	
	//We keep reserved at the same count just to ease the burden when adding stuff in later
}

// Element Access
template <typename T>
T& Vector<T>::Front() const
{	
	return elements[0];
}

// Element Access
template <typename T>
T& Vector<T>::Back() const
{
	return elements[count - 1];
}

template <typename T>
const T& Vector<T>::operator[](size_t i) const
{

	return elements[i];
}

template <typename T>
T& Vector<T>::operator[](size_t i)
{		
	return elements[i];
}

template <typename T>
size_t Vector<T>::Size() const
{
	return count;
}

template <typename T>
bool Vector<T>::Empty() const
{
	return count == 0;
}

// Implement clear
template <typename T>
void Vector<T>::Clear()
{
	//Call destructors on each element. Keep the memory for later use.
	
	for(uint32_t i = 0; i < count; i++)
	{
		elements[i].~T();	
	}
	
	//Reset the count, but keep the reserve.
	count = 0;
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
	return VectorIterator<T>(&elements[0]);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
	return VectorIterator<T>(&elements[count]);
}


/*****************************
* VectorIterator Definitions *
*****************************/

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
{
	current = NULL; //Init to empty iterator
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
{
	current = c;
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
{
	current = rhs.current;
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
	return *current;
}

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
	current++;
	return *this;
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
	VectorIterator<T> copy(current);
	current++;
	return copy;
}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
	return (current != rhs.current);
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
	return (current == (rhs.current));
}


/************************
* Grad Student Stuff *
************************/
#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved)
{ // Initialize with reserved memory
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t)
{ // Initialize with "n" copies of "t"
}
#endif


#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
}
#endif
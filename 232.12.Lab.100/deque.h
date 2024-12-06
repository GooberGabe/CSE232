/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a deque
 *        deque::iterator       : An iterator through a deque
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

// Debug stuff
#include <cassert>
#include <algorithm>

class TestDeque;    // forward declaration for TestDeque unit test class

namespace custom
{

/******************************************************
 * DEQUE
 *         iaFront
 * ia =   0   1   2   3   4
 *      +---+---+---+---+---+
 *      |   | A | B | C |   |
 *      +---+---+---+---+---+
 * id =       0   1   2
 * iaFront = 1
 * numElements = 3
 * numCapacity = 5
 *****************************************************/
template <class T>
class deque
{
   friend class ::TestDeque; // give unit tests access to the privates
public:

   // 
   // Construct
   //
   deque()
   { 
      iaFront = 0;
      numElements = 0;
   }
   deque(int newCapacity);
   deque(const deque <T> & rhs);
   ~deque()
   { 
   }

   //
   // Assign
   //
   deque<T> & operator = (const deque <T> & rhs);

   //
   // Iterator
   //
   class iterator;
   iterator begin()
   {
      return iterator();
   }
   iterator end()
   {
      return iterator();
   }

   //
   // Access
   //
   T& front();
   T& back();
   const T & front() const;
   const T & back()  const;
   const T & operator[](size_t index) const;
   T& operator[](size_t index);

   // 
   // Insert
   //
   void push_front(const T& t);
   void push_back(const T& t);

   //
   // Remove
   //
   void clear()
   { 
   }
   void pop_front();
   void pop_back();

   // 
   // Status
   //
   size_t size() const 
   { 
      return numElements; 
   }
   bool empty() const 
   { 
      return numElements == 0; 
   }

   
private:
   
   // fetch array index from the deque index
   int iaFromID(int id) const
   {
      assert(0 <= id);
      assert(0 <= iaFront < numCapacity);
      int ia = (id + iaFront) % numCapacity;
      assert(0 <= ia < numCapacity);
      return ia;
   }
   void resize(int newCapacity = 0);

   // member variables
   T * data;           // dynamically allocated data for the deque
   size_t numCapacity; // the size of the data array
   size_t numElements; // number of elements in the deque
   int iaFront;        // the index of the first item in the array
};



/**********************************************************
 * DEQUE ITERATOR
 * Forward and reverse iterator through a deque, just call
 *********************************************************/
template <typename T>
class deque <T> ::iterator
{
   friend class ::TestDeque; // give unit tests access to the privates
public:
   //
   // Construct
   //
   iterator()
   {
   }
   iterator(custom::deque<T> *pDeque, int id)
   {
   }
   iterator(const iterator& rhs)
   {
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      return *this;
   }

   //
   // Compare
   //
   bool operator == (const iterator& rhs) const { return true; }
   bool operator != (const iterator& rhs) const { return true; }

   // 
   // Access
   //
   const T & operator * () const
   {
      return *(new T);
   }
   T& operator * () 
   {
      return *(new T);
   }

   // 
   // Arithmetic
   //
   int operator - (iterator it) const
   {
      return 99;
   }
   iterator& operator += (int offset)
   {
      return *this;
   }
   iterator& operator ++ ()
   {
      return *this;
   }
   iterator operator ++ (int postfix)
   {
      return *this;
   }
   iterator& operator -- ()
   {
      return *this;
   }
   iterator  operator -- (int postfix)
   {
      return *this;
   }

private:

   // Member variables
   int id;             // deque index
   deque<T> *pDeque;
};


/****************************************************
 * DEQUE : CONSTRUCTOR - non-default
 ***************************************************/
template <class T>
deque <T> :: deque(int newCapacity)
{
   resize(newCapacity);
}

/****************************************************
 * DEQUE : CONSTRUCTOR - copy
 ***************************************************/
template <class T>
deque <T> :: deque(const deque <T> & rhs)
{
   data = new T[rhs.numCapacity];
   numElements = rhs.numElements;
   numCapacity = rhs.numCapacity;

   for (int i = 0; i < numElements; ++i) {
      data[i] = rhs.data[i];
   }
}


/****************************************************
 * DEQUE : ASSIGNMENT OPERATOR
 ***************************************************/
template <class T>
deque <T> & deque <T> :: operator = (const deque <T> & rhs)
{
   if (rhs.numCapacity > this->numCapacity) {
      delete[] this->data;
      this->data = new T[rhs.numCapacity];
      this->numCapacity = rhs.numCapacity;
   }

   this->numElements = rhs.numElements;

   for (int i = 0; i < this->numElements; ++i) {
      this->data[i] = rhs.data[i];
   }

   return *this;
}


/**************************************************
 * DEQUE :: FRONT
 * Fetch the item that is at the beginning of the deque
 *************************************************/
template <class T>
const T & deque <T> :: front() const 
{
   return *(new T);
}
template <class T>
T& deque <T> ::front()
{
   return *(new T);
}

/**************************************************
 * DEQUE :: BACK
 * Fetch the item that is at the end of the deque
 *************************************************/
template <class T>
const T & deque <T> :: back() const 
{
   return *(new T);
}
template <class T>
T& deque <T> ::back()
{
   return *(new T);
}

/**************************************************
 * DEQUE :: SUBSCRIPT
 * Fetch the item in the deque
 *************************************************/
template <class T>
const T& deque <T> ::operator[](size_t index) const
{
   return *(new T);
}
template <class T>
T& deque <T> ::operator[](size_t index)
{
   return *(new T);
}

/*****************************************************
 * DEQUE : POP_BACK
 *****************************************************/
template <class T>
void deque <T> :: pop_back()
{
}

/*****************************************************
 * DEQUE : POP_FRONT
 *****************************************************/
template <class T>
void deque <T> :: pop_front()
{
}

/******************************************************
 * DEQUE : PUSH_BACK
 ******************************************************/
template <class T>
void deque <T> :: push_back(const T & t) 
{
   if (numCapacity == 0)
   {
      resize(1);
   }
   else if (numElements == numCapacity) {
      resize(2 * numCapacity);
   }

   int ia;

   if (numElements == 0)
      data[0] = t;
   else
   {
      ia = iaFromID(numElements);
      data[ia] = t;
   }

   numElements++;
}

/******************************************************
 * DEQUE : PUSH_FRONT
 ******************************************************/
template <class T>
void deque <T> :: push_front(const T & t) 
{
   /*
   if (numCapacity == 0)
   {
      resize(1);
   }
   else if (numElements == numCapacity) {
      resize(2 * numCapacity);
   }


   if (numElements == 0)
      data[0] = t;
   else
   {
      T* newData = new T[numCapacity];
      newData[0] = t;
      for (size_t i = 0; i < numElements; i++)
      {
         newData[i + 1] = data[i];
      }
      delete data;
      data = newData;
   }

   numElements++;*/
}

/****************************************************
 * DEQUE :: RESIZE
 * Resize the deque so the numCapacity matches the newCapacity
 ***************************************************/
template <class T>
void deque <T> :: resize(int newCapacity) 
{

   T* newData = new T[newCapacity];

   if (numElements > 0) {
      size_t numCopy;
      if (numElements < newCapacity)
         numCopy = numElements;
      else
         numCopy = newCapacity;

      for (size_t i = 0; i < numCopy; ++i) {
         newData[i] = data[iaFromID(i)];
      }

      delete[] data;
   }

   data = newData;
   numCapacity = newCapacity;

}

} // namespace custom

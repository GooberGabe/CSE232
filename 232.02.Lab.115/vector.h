/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An iterator through Vector
 * Author
 *    M. Scott O'Connor & Gabe Lytle
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include <new>      // std::bad_alloc
#include <memory>   // for std::allocator

class TestVector; // forward declaration for unit tests
class TestStack;
class TestPQueue;
class TestHash;

namespace custom
{

/*****************************************
 * VECTOR
 * Just like the std :: vector <T> class
 ****************************************/
template <typename T, typename A = std::allocator<T>>
class vector
{
   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;
public:
   
   //
   // Construct
   //
   vector(const A& a = A());
   vector(size_t numElements,                const A & a = A());
   vector(size_t numElements, const T & t,   const A & a = A());
   vector(const std::initializer_list<T>& l, const A & a = A());
   vector(const vector &  rhs);
   vector(      vector && rhs);
  ~vector();

   //
   // Assign
   //
   void swap(vector& rhs)
   {
      T* swapData = data;
      int swapNumElements = numElements;
      int swapNumCapacity = numCapacity;

      numElements = rhs.numElements;
      numCapacity = rhs.numCapacity;
      data = rhs.data;

      rhs.data = swapData;
      rhs.numElements = swapNumElements;
      rhs.numCapacity = swapNumCapacity;

      swapData = nullptr;
   }
   vector & operator = (const vector & rhs);
   vector & operator = (vector&& rhs);

   //
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 
      return iterator(data); 
   }
   iterator end() 
   { 
      return iterator(data + numElements); 
   }

   //
   // Access
   //
         T& operator [] (size_t index);
   const T& operator [] (size_t index) const;
         T& front();
   const T& front() const;
         T& back();
   const T& back() const;

   //
   // Insert
   //
   void push_back(const T& t);
   void push_back(T&& t);
   void reserve(size_t newCapacity);
   void resize(size_t newElements);
   void resize(size_t newElements, const T& t);

   //
   // Remove
   //
   void clear()
   {
      for (int i = 0; i < numElements; i++)
      {
         alloc.destroy(data + i);
      }
      numElements = 0;
   }
   void pop_back()
   {
      if (numElements > 0)
         alloc.destroy(data + --numElements);
   }
   void shrink_to_fit();

   //
   // Status
   //
   size_t  size()          const { return numElements;}
   size_t  capacity()      const { return numCapacity;}
   bool empty()            const { return !numElements;}
  
private:
   
   A    alloc;                // use allocator for memory allocation
   T *  data;                 // user data, a dynamically-allocated array
   size_t  numCapacity;       // the capacity of the array
   size_t  numElements;       // the number of items currently used
};

/**************************************************
 * VECTOR ITERATOR
 * An iterator through vector.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
template <typename T, typename A>
class vector <T, A> ::iterator
{
   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;
public:
   // constructors, destructors, and assignment operator
   iterator()                           {  }
   iterator(T* p)                       { this->p = p; }
   iterator(const iterator& rhs)        {  }
   iterator(size_t index, vector<T>& v) { this->p = &v[index]; }
   iterator& operator = (const iterator& rhs)
   {
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator& rhs) const { return !(this->p == rhs.p);}
   bool operator == (const iterator& rhs) const { return this->p == rhs.p; }

   // dereference operator
   T& operator * ()
   {
      return *p;
   }

   // prefix increment
   iterator& operator ++ ()
   {
      ++p;
      return *this;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      return *this;
   }

   // prefix decrement
   iterator& operator -- ()
   {
      return *this;
   }

   // postfix decrement
   iterator operator -- (int postfix)
   {
      return *this;
   }

private:
   T* p;
};


/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(const A & a)
{
   data = nullptr;
   numElements = 0;
   numCapacity = 0;
}


/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(size_t num, const T & t, const A & a) 
{
   alloc = a;
   data = alloc.allocate(num);

   for (int i = 0; i < num; i++) {
      alloc.construct(data + i, t);
   }
   numElements = num;
   numCapacity = num;
}

/*****************************************
 * VECTOR :: INITIALIZATION LIST constructors
 * Create a vector with an initialization list.
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(const std::initializer_list<T> & l, const A & a) 
{
   data = alloc.allocate(l.size());
   for (int i = 0; i < l.size(); i++)
   {
      alloc.construct(data + i, *(l.begin() + i));
   }
   numElements = l.size();
   numCapacity = l.size();
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector(size_t num, const A & a) 
{
   alloc = a;
   data = alloc.allocate(num);
   
   for (int i = 0; i < num; i++) {
      alloc.construct(data + i);
   }
   numElements = num;
   numCapacity = num;

   //std::cout << "Test!" << std::endl;
}

/*****************************************
 * VECTOR :: COPY CONSTRUCTOR
 * Allocate the space for numElements and
 * call the copy constructor on each element
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector (const vector & rhs) 
{
   data = alloc.allocate(rhs.size());

   for (int i = 0; i < rhs.size(); i++) {
     
      alloc.construct(data + i, rhs[i]);
      
   }
   numElements = rhs.size();
   numCapacity = rhs.size();
}
   
/*****************************************
 * VECTOR :: MOVE CONSTRUCTOR
 * Steal the values from the RHS and set it to zero.
 ****************************************/
template <typename T, typename A>
vector <T, A> :: vector (vector && rhs) 
{
   data = rhs.data;
   numElements = rhs.numElements;
   numCapacity = rhs.numCapacity;
   rhs.data = nullptr;
   rhs.numCapacity = 0;
   rhs.numElements = 0;
}

/*****************************************
 * VECTOR :: DESTRUCTOR
 * Call the destructor for each element from 0..numElements
 * and then free the memory
 ****************************************/
template <typename T, typename A>
vector <T, A> :: ~vector()
{
   for (int i = 0; i < numElements; i++)
   {
      alloc.destroy(data + i);
   }

   alloc.deallocate(data, numCapacity);
}

/***************************************
 * VECTOR :: RESIZE
 * This method will adjust the size to newElements.
 * This will either grow or shrink newElements.
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T, typename A>
void vector <T, A> :: resize(size_t newElements)
{
   if (numCapacity == 0 && newElements != 0)
   {
      data = alloc.allocate(newElements);
      numCapacity = newElements;
   }
   else if (numCapacity < newElements)
   {
      T* newData = alloc.allocate(newElements);

      for (int i = 0; i < numElements; i++)
      {
         alloc.construct(newData + i, std::move(data[i]));
         alloc.destroy(data + i);
      }

      alloc.deallocate(data, numCapacity);

      data = newData;
      numCapacity = newElements;
   }
   
   if (numElements > newElements)
   {
      for (int i = newElements; i < numElements; i++)
      {
         alloc.destroy(data + i);
      }
   }
   else if (numElements < newElements)
   {
      for (int i = numElements; i < newElements; i++)
      {
         alloc.construct(data + i);
      }
   }

   numElements = newElements;

}

template <typename T, typename A>
void vector <T, A> :: resize(size_t newElements, const T & t)
{
   if (numCapacity == 0 && newElements != 0)
   {
      data = alloc.allocate(newElements);
      numCapacity = newElements;
   }
   else if (numCapacity < newElements)
   {
      T* newData = alloc.allocate(newElements);

      for (int i = 0; i < numElements; i++)
      {
         alloc.construct(newData + i, std::move(data[i]));
         alloc.destroy(data + i);
      }

      alloc.deallocate(data, numCapacity);

      data = newData;
      numCapacity = newElements;
   }

   if (numElements > newElements)
   {
      for (int i = newElements; i < numElements; i++)
      {
         alloc.destroy(data + i);
      }
   }
   else if (numElements < newElements)
   {
      for (int i = numElements; i < newElements; i++)
      {
         alloc.construct(data + i, t);
      }
   }

   numElements = newElements;

}

/***************************************
 * VECTOR :: RESERVE
 * This method will grow the current buffer
 * to newCapacity.  It will also copy all
 * the data from the old buffer into the new
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T, typename A>
void vector <T, A> :: reserve(size_t newCapacity)
{
   if (newCapacity > numCapacity)
   {
      T* newData = alloc.allocate(newCapacity);

      for (int i = 0; i < numElements; i++)
      {
         alloc.construct(newData + i, std::move(data[i]));
         alloc.destroy(data + i);
      }

      alloc.deallocate(data, numCapacity);

      data = newData;
      numCapacity = newCapacity;
   }
}

/***************************************
 * VECTOR :: SHRINK TO FIT
 * Get rid of any extra capacity
 *     INPUT  :
 *     OUTPUT :
 **************************************/
template <typename T, typename A>
void vector <T, A> :: shrink_to_fit()
{
   if (numCapacity > numElements) {
      if (numElements > 0) {

         T* newData = alloc.allocate(numElements);

         for (int i = 0; i < numElements; i++)
         {
            alloc.construct(newData + i, data[i]);
            alloc.destroy(data + i);
         }
         alloc.deallocate(data, numCapacity);

         data = newData;
      }
      else if (data)
      {
         alloc.deallocate(data, numCapacity);
         data = nullptr;
      }

      numCapacity = numElements;
   }
}



/*****************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 ****************************************/
template <typename T, typename A>
T & vector <T, A> :: operator [] (size_t index)
{
   return *(data + index);
    
}

/******************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 *****************************************/
template <typename T, typename A>
const T & vector <T, A> :: operator [] (size_t index) const
{
   return *(data + index);
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T, typename A>
T & vector <T, A> :: front ()
{
   return *(data);
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T, typename A>
const T & vector <T, A> :: front () const
{
   return *(data);
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T, typename A>
T & vector <T, A> :: back()
{
   return *(data + numElements - 1);
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T, typename A>
const T & vector <T, A> :: back() const
{
   return *(data + numElements - 1);
}

/***************************************
 * VECTOR :: PUSH BACK
 * This method will add the element 't' to the
 * end of the current buffer.  It will also grow
 * the buffer as needed to accomodate the new element
 *     INPUT  : 't' the new element to be added
 *     OUTPUT : *this
 **************************************/
template <typename T, typename A>
void vector <T, A> :: push_back (const T & t)
{
   if (numCapacity == 0) 
   {
      data = alloc.allocate(1);
      numCapacity = 1;
   }
   else if (numCapacity == numElements)
   {
      int newCapacity = numCapacity * 2;
      T* newData = alloc.allocate(newCapacity);

      for (int i = 0; i < numElements; i++)
      {
         alloc.construct(newData + i, std::move(data[i]));
         alloc.destroy(data + i);
      }

      alloc.deallocate(data, numCapacity);

      data = newData;
      numCapacity = newCapacity;
   }

   alloc.construct(data + numElements, t);
   numElements++;
}

template <typename T, typename A>
void vector <T, A> ::push_back(T && t)
{
   if (numCapacity == 0)
   {
      data = alloc.allocate(1);
      numCapacity = 1;
   }
   else if (numCapacity == numElements)
   {
      int newCapacity = numCapacity * 2;
      T* newData = alloc.allocate(newCapacity);

      for (int i = 0; i < numElements; i++)
      {
         alloc.construct(newData + i, std::move(data[i]));
         alloc.destroy(data + i);
      }

      alloc.deallocate(data, numCapacity);

      data = newData;
      numCapacity = newCapacity;
   }

   alloc.construct(data + numElements, std::move(t));
   numElements++;
}

/***************************************
 * VECTOR :: ASSIGNMENT
 * This operator will copy the contents of the
 * rhs onto *this, growing the buffer as needed
 *     INPUT  : rhs the vector to copy from
 *     OUTPUT : *this
 **************************************/
template <typename T, typename A>
vector <T, A> & vector <T, A> :: operator = (const vector & rhs)
{
   if (rhs.size() > numCapacity)
   {
      for (int i = 0; i < numElements; i++)
      {
         alloc.destroy(data + i);
      }
      numElements = 0;
      data = alloc.allocate(rhs.capacity());
      numCapacity = rhs.capacity();
      
   }
   else if (numElements > rhs.size())
   {
      for (int i = rhs.size(); i <= numElements; i++)
      {
         alloc.destroy(data + i);
         numElements--;
      }
   }

   for (int i = 0; i < rhs.size(); i++)
   {
      if (numElements <= i) {
         alloc.construct(data + i, rhs[i]);
         numElements++;
      }
      else
         data[i] = rhs[i];
   }

   return *this;
}
template <typename T, typename A>
vector <T, A>& vector <T, A> :: operator = (vector&& rhs)
{
   for (int i = 0; i < numElements; i++)
   {
      alloc.destroy(data + i);
   }
   alloc.deallocate(data, numCapacity);
   numElements = rhs.numElements;
   numCapacity = rhs.numCapacity;
   data = rhs.data;

   rhs.data = nullptr;
   rhs.numElements = 0;
   rhs.numCapacity = 0;

   return *this;
}




} // namespace custom


/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator

class TestList;        // forward declaration for unit tests
class TestHash;        // to be used later

namespace custom
{

   /**************************************************
    * LIST
    * Just like std::list
    **************************************************/
   template <typename T>
   class list
   {
      friend class ::TestList; // give unit tests access to the privates
      friend class ::TestHash;
      friend void swap(list& lhs, list& rhs);
   public:
      // 
      // Construct
      //

      list();
      list(list <T>& rhs);
      list(list <T>&& rhs);
      list(size_t num, const T& t);
      list(size_t num);
      list(const std::initializer_list<T>& il);
      template <class Iterator>
      list(Iterator first, Iterator last);
      ~list()
      {
         if (pHead == nullptr)
            return;

         for (auto p = pHead; p->pNext != nullptr; p = p->pNext)
         {
            delete p->pPrev;
         }

         delete pTail;
         pHead = nullptr;
         numElements = 0;
      }

      // 
      // Assign
      //

      list <T>& operator = (list& rhs);
      list <T>& operator = (list&& rhs);
      list <T>& operator = (const std::initializer_list<T>& il);
      void swap(list <T>& rhs);

      //
      // Iterator
      //

      class  iterator;
      iterator begin() { return iterator(pHead); }
      iterator rbegin() { return iterator(pTail); }
      iterator end() { return iterator(nullptr); }

      //
      // Access
      //

      T& front();
      T& back();

      //
      // Insert
      //

      void push_front(const T& data);
      void push_front(T&& data);
      void push_back(const T& data);
      void push_back(T&& data);
      iterator insert(iterator it, const T& data);
      iterator insert(iterator it, T&& data);

      //
      // Remove
      //

      void pop_back();
      void pop_front();
      void clear();
      iterator erase(const iterator& it);

      // 
      // Status
      //

      bool empty()  const { return numElements == 0; }
      size_t size() const { return numElements; }


   private:
      // nested linked list class
      class Node;

      // member variables
      size_t numElements; // though we could count, it is faster to keep a variable
      Node* pHead;    // pointer to the beginning of the list
      Node* pTail;    // pointer to the ending of the list
   };

   /*************************************************
    * NODE
    * the node class.  Since we do not validate any
    * of the setters, there is no point in making them
    * private.  This is the case because only the
    * List class can make validation decisions
    *************************************************/
   template <typename T>
   class list <T> ::Node
   {
   public:
      //
      // Construct
      //
      Node()
      {
         pNext = pPrev = nullptr;
      }
      Node(const T& data) : data(data)
      {
         pNext = pPrev = nullptr;
      }
      Node(T&& data)
      {
         pNext = pPrev = nullptr;
         this->data = std::move(data);
      }

      //
      // Data
      //

      T data;                 // user data
      Node* pNext;       // pointer to next node
      Node* pPrev;       // pointer to previous node
   };

   /*************************************************
    * LIST ITERATOR
    * Iterate through a List, non-constant version
    ************************************************/
   template <typename T>
   class list <T> ::iterator
   {
      friend class ::TestList; // give unit tests access to the privates
      friend class ::TestHash;
      template <typename TT>
      friend class custom::list;
   public:
      // constructors, destructors, and assignment operator
      iterator() : p(nullptr) {}

      iterator(Node* p) : p(p) {}

      iterator(const list <T> ::iterator& rhs) : p(rhs.p) {}

      iterator& operator = (const iterator& rhs)
      {
         if (this != &rhs)
         {
            p = rhs.p;
         }
         return *this;
      }

      // equals, not equals operator
      bool operator == (const iterator& rhs) const { return p == rhs.p; }
      bool operator != (const iterator& rhs) const { return p != rhs.p; }

      // dereference operator, fetch a node
      T& operator * ()
      {
         if (p == nullptr)
         {
            throw "Accessing null iterator";
         }
         return p->data;
      }

      // postfix increment
      iterator operator ++ (int)
      {
         iterator temp = *this;
         if (p != nullptr)
         {
            p = p->pNext;
         }
         return temp;
      }

      // prefix increment
      iterator& operator ++ ()
      {
         if (p != nullptr)
         {
            p = p->pNext;
         }
         return *this;
      }

      // postfix decrement
      iterator operator -- (int)
      {
         iterator temp = *this;
         if (p != nullptr)
         {
            p = p->pPrev;
         }
         return temp;
      }

      // prefix decrement
      iterator& operator -- ()
      {
         if (p != nullptr)
         {
            p = p->pPrev;
         }
         return *this;
      }

      // two friends who need to access p directly
      friend iterator list <T> ::insert(iterator it, const T& data);
      friend iterator list <T> ::insert(iterator it, T&& data);
      friend iterator list <T> ::erase(const iterator& it);

   private:

      typename list <T> ::Node* p;
   };

   /*****************************************
    * LIST :: NON-DEFAULT constructors
    * Create a list initialized to a value
    ****************************************/
   template <typename T>
   list <T> ::list(size_t num, const T& t)
   {
      if (num == 0)
      {
         pHead = nullptr;
         pTail = nullptr;
         numElements = 0;
         return;
      }

      numElements = 1;
      this->pHead = new list <T> ::Node(t);

      for (auto p = pHead; numElements < num; p = p->pNext)
      {
         p->pNext = new list <T> ::Node(t);
         p->pNext->pPrev = p;
         numElements++;
      }
   }

   /*****************************************
    * LIST :: ITERATOR constructors
    * Create a list initialized to a set of values
    ****************************************/
   template <typename T>
   template <class Iterator>
   list <T> ::list(Iterator first, Iterator last)
   {
      if (first == last)
      {
         pHead = nullptr;
         pTail = nullptr;
         numElements = 0;
         return;
      }

      numElements = 0;//il.size();
      this->pHead = new list <T> ::Node();
      auto* p = pHead;

      for (auto it = first; it != last; it++)
      {
         p->pNext = new list <T> ::Node();
         p->pNext->pPrev = p;
         p->data = *it;
         pTail = p;
         p = p->pNext;
         numElements++;
      }

      pTail->pNext = nullptr;
   }

   /*****************************************
    * LIST :: INITIALIZER constructors
    * Create a list initialized to a set of values
    ****************************************/
   template <typename T>
   list <T> ::list(const std::initializer_list<T>& il)
   {
      if (il.size() == 0)
      {
         pHead = nullptr;
         pTail = nullptr;
         numElements = 0;
         return;
      }

      numElements = il.size();
      this->pHead = new list <T> ::Node();
      auto* p = pHead;

      for (auto it = il.begin(); it != il.end(); it++)
      {
         p->pNext = new list <T> ::Node();
         p->pNext->pPrev = p;
         p->data = *it;
         pTail = p;
         p = p->pNext;
      }

      pTail->pNext = nullptr;
   }

   /*****************************************
    * LIST :: NON-DEFAULT constructors
    * Create a list initialized to a value
    ****************************************/
   template <typename T>
   list <T> ::list(size_t num)
   {
      if (num == 0)
      {
         pHead = nullptr;
         pTail = nullptr;
         numElements = 0;
         return;
      }

      numElements = 1;
      this->pHead = new list <T> ::Node();

      for (auto p = pHead; numElements < num; p = p->pNext)
      {
         p->pNext = new list <T> ::Node();
         p->pNext->pPrev = p;
         numElements++;
      }
   }

   /*****************************************
    * LIST :: DEFAULT constructors
    ****************************************/
   template <typename T>
   list <T> ::list()
   {
      numElements = 0;
      pHead = pTail = nullptr;
   }

   /*****************************************
    * LIST :: COPY constructors
    ****************************************/
   template <typename T>
   list <T> ::list(list& rhs)
   {
      if (rhs.pHead == nullptr)
         return;

      this->numElements = rhs.size();

      this->pHead = new list <T> ::Node(rhs.pHead->data);
      list <T> ::Node* pAssign = this->pHead; // Pointer to assign new nodes.

      // Traverse the source list and copy each node.
      list <T> ::Node* pSourceNext = rhs.pHead->pNext;
      while (pSourceNext != nullptr)
      {
         pAssign->pNext = new list <T> ::Node(pSourceNext->data);
         pAssign->pNext->pPrev = pAssign;
         pAssign = pAssign->pNext;
         pSourceNext = pSourceNext->pNext;

         this->pTail = pAssign;
      }

   }

   /*****************************************
    * LIST :: MOVE constructors
    * Steal the values from the RHS
    ****************************************/
   template <typename T>
   list <T> ::list(list <T>&& rhs)
   {
      pHead = rhs.pHead;
      pTail = rhs.pTail;
      numElements = rhs.size();

      rhs.pHead = nullptr;
      rhs.pTail = nullptr;
      rhs.numElements = 0;
   }

   /**********************************************
    * LIST :: assignment operator - MOVE
    * Copy one list onto another
    *     INPUT  : a list to be moved
    *     OUTPUT :
    *     COST   : O(n) with respect to the size of the LHS
    *********************************************/
   template <typename T>
   list <T>& list <T> :: operator = (list <T>&& rhs)
   {
      pHead = rhs.pHead;
      pTail = rhs.pTail;
      numElements = rhs.size();

      rhs.pHead = nullptr;
      rhs.pTail = nullptr;
      rhs.numElements = 0;
      return *this;
   }

   /**********************************************
    * LIST :: assignment operator
    * Copy one list onto another
    *     INPUT  : a list to be copied
    *     OUTPUT :
    *     COST   : O(n) with respect to the number of nodes
    *********************************************/
   template <typename T>
   list <T>& list <T> :: operator = (list <T>& rhs)
   {
      if (rhs.pHead == nullptr)
      {
         clear();
         return *this;
      }

      this->numElements = rhs.size();

      this->pHead = new list <T> ::Node(rhs.pHead->data);
      list <T> ::Node* pAssign = this->pHead; // Pointer to assign new nodes.

      list <T> ::Node* pSourceNext = rhs.pHead->pNext;
      while (pSourceNext != nullptr)
      {
         pAssign->pNext = new list <T> ::Node(pSourceNext->data);
         pAssign->pNext->pPrev = pAssign;
         pAssign = pAssign->pNext;
         pSourceNext = pSourceNext->pNext;

         this->pTail = pAssign;
      }

      return *this;
   }

   /**********************************************
    * LIST :: assignment operator
    * Copy one list onto another
    *     INPUT  : a list to be copied
    *     OUTPUT :
    *     COST   : O(n) with respect to the number of nodes
    *********************************************/
   template <typename T>
   list <T>& list <T> :: operator = (const std::initializer_list<T>& rhs)
   {
      if (rhs.size() == 0)
      {
         clear();
         return *this;
      }

      numElements = rhs.size();
      this->pHead = new list <T> ::Node();
      auto* p = pHead;

      for (auto it = rhs.begin(); it != rhs.end(); it++)
      {
         p->pNext = new list <T> ::Node();
         p->pNext->pPrev = p;
         p->data = *it;
         pTail = p;
         p = p->pNext;
      }

      pTail->pNext = nullptr;
      return *this;
   }

   /**********************************************
    * LIST :: CLEAR
    * Remove all the items currently in the linked list
    *     INPUT  :
    *     OUTPUT :
    *     COST   : O(n) with respect to the number of nodes
    *********************************************/
   template <typename T>
   void list <T> ::clear()
   {
      if (pHead == nullptr)
         return;

      for (auto p = pHead; p->pNext != nullptr; p = p->pNext)
      {
         delete p->pPrev;
      }

      pTail = nullptr;
      pHead = nullptr;
      numElements = 0;
   }

   /*********************************************
    * LIST :: PUSH BACK
    * add an item to the end of the list
    *    INPUT  : data to be added to the list
    *    OUTPUT :
    *    COST   : O(1)
    *********************************************/
   template <typename T>
   void list <T> ::push_back(const T& data)
   {
      auto pNew = new list<T>::Node(data);
      numElements++;
      if (pHead == nullptr)
      {
         pHead = pTail = pNew;
         return;
      }
      pTail->pNext = pNew;
      pNew->pPrev = pTail;
      pTail = pNew;
   }

   template <typename T>
   void list <T> ::push_back(T&& data)
   {
      auto pNew = new list<T>::Node(std::move(data));
      numElements++;
      if (pHead == nullptr)
      {
         pHead = pTail = pNew;
         return;
      }
      pTail->pNext = pNew;
      pNew->pPrev = pTail;
      pTail = pNew;
   }

   /*********************************************
    * LIST :: PUSH FRONT
    * add an item to the head of the list
    *     INPUT  : data to be added to the list
    *     OUTPUT :
    *     COST   : O(1)
    *********************************************/
   template <typename T>
   void list <T> ::push_front(const T& data)
   {
      auto pNew = new list<T>::Node(data);
      numElements++;
      if (pHead == nullptr)
      {
         pHead = pTail = pNew;
         return;
      }
      pHead->pPrev = pNew;
      pNew->pNext = pHead;
      pHead = pNew;
   }

   template <typename T>
   void list <T> ::push_front(T&& data)
   {
      auto pNew = new list<T>::Node(std::move(data));
      numElements++;
      if (pHead == nullptr)
      {
         pHead = pTail = pNew;
         return;
      }
      pHead->pPrev = pNew;
      pNew->pNext = pHead;
      pHead = pNew;
   }


   /*********************************************
    * LIST :: POP BACK
    * remove an item from the end of the list
    *    INPUT  :
    *    OUTPUT :
    *    COST   : O(1)
    *********************************************/
   template <typename T>
   void list <T> ::pop_back()
   {
      if (pTail == nullptr)
         return;

      numElements--;
      if (pTail->pPrev == nullptr)
      {
         pTail = nullptr;
         pHead = nullptr;
         return;
      }

      pTail->pPrev->pNext = nullptr;
      pTail = pTail->pPrev;
   }

   /*********************************************
    * LIST :: POP FRONT
    * remove an item from the front of the list
    *    INPUT  :
    *    OUTPUT :
    *    COST   : O(1)
    *********************************************/
   template <typename T>
   void list <T> ::pop_front()
   {
      if (pHead == nullptr)
         return;

      numElements--;
      if (pHead->pNext == nullptr)
      {
         pTail = nullptr;
         pHead = nullptr;
         return;
      }

      pHead->pNext->pPrev = nullptr;
      pHead = pHead->pNext;
   }

   /*********************************************
    * LIST :: FRONT
    * retrieves the first element in the list
    *     INPUT  :
    *     OUTPUT : data to be displayed
    *     COST   : O(1)
    *********************************************/
   template <typename T>
   T& list <T> ::front()
   {
      if (pHead == nullptr)
      {
         throw "ERROR: unable to access data from an empty list";
      }
      return pHead->data;
   }

   /*********************************************
    * LIST :: BACK
    * retrieves the last element in the list
    *     INPUT  :
    *     OUTPUT : data to be displayed
    *     COST   : O(1)
    *********************************************/
   template <typename T>
   T& list <T> ::back()
   {
      if (pTail == nullptr)
      {
         throw "ERROR: unable to access data from an empty list";
      }
      return pTail->data;
   }

   /******************************************
    * LIST :: REMOVE
    * remove an item from the middle of the list
    *     INPUT  : an iterator to the item being removed
    *     OUTPUT : iterator to the new location
    *     COST   : O(1)
    ******************************************/
   template <typename T>
   typename list <T> ::iterator  list <T> ::erase(const list <T> ::iterator& it)
   {
      if (it.p == nullptr)
         return end();

      list<T>::Node* nodeToRemove = it.p;
      list<T>::Node* nextNode = nodeToRemove->pNext;

      if (nodeToRemove == pHead)
      {
         pHead = nextNode;
         if (pHead)
            pHead->pPrev = nullptr;
         else
            pTail = nullptr;
      }
      else if (nodeToRemove == pTail)
      {
         pTail = nodeToRemove->pPrev;
         if (pTail)
            pTail->pNext = nullptr;
         else
            pHead = nullptr;
      }
      else
      {
         nodeToRemove->pPrev->pNext = nextNode;
         nextNode->pPrev = nodeToRemove->pPrev;
      }

      delete nodeToRemove;

      numElements--;

      return iterator(nextNode);
   }

   /******************************************
    * LIST :: INSERT
    * add an item to the middle of the list
    *     INPUT  : data to be added to the list
    *              an iterator to the location where it is to be inserted
    *     OUTPUT : iterator to the new item
    *     COST   : O(1)
    ******************************************/
   template <typename T>
   typename list <T> ::iterator list <T> ::insert(list <T> ::iterator it,
      const T& data)
   {
      auto pNew = new list<T>::Node(data);
      if (it.p == nullptr)
      {
         pNew->pPrev = pTail;
         pNew->pNext = nullptr;

         if (pTail != nullptr)
            pTail->pNext = pNew;
         if (pHead == nullptr)
            pHead = pNew;
         pTail = pNew;
      }
      else if (it.p == pHead)
      {
         pNew->pNext = pHead;
         pHead->pPrev = pNew;
         pHead = pNew;
      }
      else
      {

         Node* prevNode = it.p->pPrev;
         pNew->pPrev = prevNode;
         pNew->pNext = it.p;

         prevNode->pNext = pNew;
         it.p->pPrev = pNew;

         if (it.p == pTail)
            pTail = pNew;
      }

      numElements++;
      return iterator(pNew);
   }

   template <typename T>
   typename list <T> ::iterator list <T> ::insert(list <T> ::iterator it,
      T&& data)
   {
      auto pNew = new list<T>::Node(std::move(data));
      if (it.p == nullptr)
      {
         pNew->pPrev = pTail;
         pNew->pNext = nullptr;

         if (pTail != nullptr)
            pTail->pNext = pNew;
         if (pHead == nullptr)
            pHead = pNew;
         pTail = pNew;
      }
      else if (it.p == pHead)
      {
         pNew->pNext = pHead;
         pHead->pPrev = pNew;
         pHead = pNew;
      }
      else
      {

         Node* prevNode = it.p->pPrev;
         pNew->pPrev = prevNode;
         pNew->pNext = it.p;

         prevNode->pNext = pNew;
         it.p->pPrev = pNew;

         if (it.p == pTail)
            pTail = pNew;
      }

      numElements++;
      return iterator(pNew);
   }

   /**********************************************
    * LIST :: assignment operator - MOVE
    * Copy one list onto another
    *     INPUT  : a list to be moved
    *     OUTPUT :
    *     COST   : O(n) with respect to the size of the LHS
    *********************************************/
   template <typename T>
   void swap(list <T>& lhs, list <T>& rhs)
   {
      lhs.swap(rhs);
   }

   template <typename T>
   void list<T>::swap(list <T>& rhs)
   {
      std::swap(this->pHead, rhs.pHead);
      std::swap(this->numElements, rhs.numElements);
      std::swap(this->pTail, rhs.pTail);
   }

   //#endif
}; // namespace custom

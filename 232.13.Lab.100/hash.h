/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    M. Scott O'Connor & Gabe Lytle
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil
   

class TestHash;             // forward declaration for Hash unit tests

namespace custom
{
/************************************************
 * UNORDERED SET
 * A set implemented as a hash
 ************************************************/
template <typename T>
class unordered_set
{
   friend class ::TestHash;   // give unit tests access to the privates
public:
   //
   // Construct
   //
   unordered_set()
   {
      numElements = 0;
   }
   unordered_set(unordered_set& rhs)
   {
      for (int i = 0; i < std::size(rhs.buckets); i++)
      {
         buckets[i] = rhs.buckets[i];
      }
      numElements = rhs.numElements;

   }
   unordered_set(unordered_set&& rhs) 
   {
      for (int i = 0; i < std::size(rhs.buckets); i++)
      {
         buckets[i] = std::move(rhs.buckets[i]);
      }
      numElements = rhs.numElements;
      rhs.numElements = 0;
   }
   template <class Iterator>
   unordered_set(Iterator first, Iterator last)
   {
      numElements = 0;
      for (Iterator it = first; it != last; it++)
      {
         buckets[(*it) % std::size(buckets)].push_back((*it));
         numElements++;
      }
   }

   //
   // Assign
   //
   unordered_set& operator=(unordered_set& rhs)
   {
      for (int i = 0; i < std::size(rhs.buckets); i++)
      {
         this->buckets[i] = rhs.buckets[i];
      }
      this->numElements = rhs.numElements;
      return *this;
   }
   unordered_set& operator=(unordered_set&& rhs)
   {
      for (int i = 0; i < std::size(rhs.buckets); i++)
      {
         this->buckets[i] = std::move(rhs.buckets[i]);
      }
      this->numElements = rhs.numElements;
      rhs.numElements = 0;
      return *this;
   }
   unordered_set& operator=(const std::initializer_list<T>& il)
   {
      return *this;
   }
   void swap(unordered_set& rhs)
   {
      for (int i = 0; i < std::size(rhs.buckets); i++)
      {
         std::swap(rhs.buckets[i], buckets[i]);
      }
      std::swap(rhs.numElements, numElements);
   }

   // 
   // Iterator
   //
   class iterator;
   class local_iterator;
   iterator begin()
   {
      if (empty())
         return end();

      for (int i = 0; i < std::size(buckets); i++)
      {
         if (!buckets[i].empty())
            return iterator(&buckets[1], &buckets[10], buckets[i].begin());
      }
      return iterator();
   }
   iterator end()
   {
      return iterator(buckets + 10, buckets + 10,
             typename custom::list<T>::iterator());
   }
   local_iterator begin(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].begin());
   }
   local_iterator end(size_t iBucket)
   {
      return local_iterator();
   }

   //
   // Access
   //
   size_t bucket(const T& t)
   {
      return t % 10;
   }
   iterator find(const T& t);

   //   
   // Insert
   //
   custom::pair<iterator, bool> insert(const T& t);
   void insert(const std::initializer_list<T> & il);


   // 
   // Remove
   //
   void clear() noexcept
   {
      for (list<T>* it = &buckets[0]; it != &buckets[10]; it++)
      {
         it->clear();
      }
      numElements = 0;
   }
   iterator erase(const T& t);

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
   size_t bucket_count() const 
   { 
      return 100;
   }
   size_t bucket_size(size_t i) const
   {
      return buckets[i].size();
   }

private:

   custom::list<T> buckets [10];   // exactly 10 buckets
   int numElements;                // number of elements in the Hash
};


/************************************************
 * UNORDERED SET ITERATOR
 * Iterator for an unordered set
 ************************************************/
template <typename T>
class unordered_set <T> ::iterator
{
   friend class ::TestHash;   // give unit tests access to the privates
   template <class TT>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   //
   iterator()  
   {  
   }
   iterator(typename custom::list<T>* pBucket,
            typename custom::list<T>* pBucketEnd,
            typename custom::list<T>::iterator itList)
   {
      this->pBucket = pBucket;
      this->pBucketEnd = pBucketEnd;
      this->itList = itList;
   }
   iterator(const iterator& rhs) = default;

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs) = default;

   //
   // Compare
   //
   bool operator != (const iterator& rhs) const 
   { 
      return !(*this == rhs);
   }
   bool operator == (const iterator& rhs) const 
   { 
      return pBucket == rhs.pBucket && pBucketEnd == rhs.pBucketEnd
             && itList == rhs.itList;
   }

   // 
   // Access
   //
   T& operator * ()
   {
      if (itList != nullptr) {
         return *itList;
      }
      else {
         throw std::runtime_error("Iterator is not initialized");
      }
   }

   //
   // Arithmetic
   //
   iterator& operator ++ ();
   iterator operator ++ (int postfix)
   {
      return *this;
   }

private:
   custom::list<T> *pBucket;
   custom::list<T> *pBucketEnd;
   typename list<T>::iterator itList;
};


/************************************************
 * UNORDERED SET LOCAL ITERATOR
 * Iterator for a single bucket in an unordered set
 ************************************************/
template <typename T>
class unordered_set <T> ::local_iterator
{
   friend class ::TestHash;   // give unit tests access to the privates

   template <class TT>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   //
   local_iterator() : itList(nullptr)
   {
   }
   local_iterator(const typename custom::list<T>::iterator& itList) : itList(itList)
   {
   }
   local_iterator(const local_iterator& rhs) = default;

   //
   // Assign
   //
   local_iterator& operator = (const local_iterator& rhs) = default;

   // 
   // Compare
   //
   bool operator != (const local_iterator& rhs) const
   {
      return !(*this == rhs);
   }
   bool operator == (const local_iterator& rhs) const
   {
      return itList == rhs.itList;
   }

   // 
   // Access
   //
   T& operator * ()
   {
      if (itList != nullptr) {
         return *itList;
      }
      else {
         throw std::runtime_error("Iterator is not initialized");
      }
   }

   // 
   // Arithmetic
   //
   local_iterator& operator ++ ()
   {
      ++itList;
      return *this;
   }
   local_iterator operator ++ (int postfix)
   {
      return *this;
   }

private:
   typename list<T>::iterator itList;
};


/*****************************************
 * UNORDERED SET :: ERASE
 * Remove one element from the unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator unordered_set<T>::erase(const T& t)
{
   if (find(t) != end()) 
   {
      iterator itNext = ++find(t);
      auto bt = bucket(t);
      for (auto it = buckets[bt].begin(); it != buckets[bt].end(); it++)
      {
         if (*it == t) 
         {
            buckets[bt].erase(it);
            numElements--;
            return itNext;
         }
      }
   }

   return end();
}

/*****************************************
 * UNORDERED SET :: INSERT
 * Insert one element into the hash
 ****************************************/
template <typename T>
custom::pair<typename custom::unordered_set<T>::iterator, bool> unordered_set<T>::insert(const T& t)
{
   bool original = false;
   if (find(t) == end())
   {
      auto bt = bucket(t);
      buckets[bt].push_back(t);
      numElements++;
      original = true;
   }
   return custom::pair<custom::unordered_set<T>::iterator, bool>(find(t), original);
}
template <typename T>
void unordered_set<T>::insert(const std::initializer_list<T> & il)
{
}

/*****************************************
 * UNORDERED SET :: FIND
 * Find an element in an unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator unordered_set<T>::find(const T& t)
{
   auto bt = bucket(t);
   for (auto it = buckets[bt].begin(); it != buckets[bt].end(); it++)
   {
      if (*it == t)
         return iterator(&buckets[bt], &buckets[10], it);
   }

   return end();
}

/*****************************************
 * UNORDERED SET :: ITERATOR :: INCREMENT
 * Advance by one element in an unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator & unordered_set<T>::iterator::operator ++ ()
{
   //not sure if this works
   if (itList != custom::list<T>::iterator()) 
   {
      ++itList;
      if (itList == nullptr)
      {
         for (auto it = pBucket + 1; it < pBucketEnd; it++)
         {
            if (std::size(*it) > 0)
            {
               pBucket = it;
               itList = pBucket->begin();
               return *this;
            }
         }

         pBucket = pBucketEnd;
         itList = custom::list<T>::iterator();
      }
   }
   return *this;
}

/*****************************************
 * SWAP
 * Stand-alone unordered set swap
 ****************************************/
template <typename T>
void swap(unordered_set<T>& lhs, unordered_set<T>& rhs)
{
   lhs.swap(rhs);
}

}

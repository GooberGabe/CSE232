/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    M Scott O'Connor & Gabe Lytle
 ************************************************************************/

#pragma once

#include <cassert>     // for ASSERT
#include <iostream>    // for NULL
#include <algorithm>

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <class T>
class Node
{
public:

   //
   // Construct
   //

   Node()
   {
      pPrev = nullptr;
      pNext = nullptr;
   }
   Node(const T & data) : data(data)
   {
      pPrev = nullptr;
      pNext = nullptr;
   }
   Node(      T && data)
   {
      pPrev = nullptr;
      pNext = nullptr;
      this->data = std::move(data);
   }

   //
   // Member variables
   //

   T data;                 // user data
   Node <T> * pNext;       // pointer to next node
   Node <T> * pPrev;       // pointer to previous node
};

/***********************************************
 * COPY
 * Copy the list from the pSource and return
 * the new list
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline Node <T> * copy(const Node <T> * pSource) 
{
   if (pSource == nullptr)
      return nullptr; 

   // Create a new node and copy the data from pSource.
   Node<T>* pCopy = new Node<T>(pSource->data);
   Node<T>* pAssign = pCopy; // Pointer to assign new nodes.

   // Traverse the source list and copy each node.
   const Node<T>* pSourceNext = pSource->pNext;
   while (pSourceNext != nullptr)
   {
      pAssign->pNext = new Node<T>(pSourceNext->data);
      pAssign->pNext->pPrev = pAssign;
      pAssign = pAssign->pNext;
      pSourceNext = pSourceNext->pNext;
   }

   return pCopy; // Return the head of the copied list.
}

/***********************************************
 * Assign
 * Copy the values from pSource into pDestination
 * reusing the nodes already created in pDestination if possible.
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline void assign(Node <T> * & pDestination, const Node <T> * pSource)
{
   if (pSource == nullptr)
   {
      if (pDestination == nullptr)
         return;
      else 
      {
         clear(pDestination);
         return;
      }
   }
   else if (pDestination == nullptr)
   {
      pDestination = copy(pSource);
      return;
   }

   const Node <T>* pS = pSource;
   Node <T>* pD = pDestination;
   Node <T>* pSave = pDestination;

   while (pD != nullptr && pS != nullptr)
   {
      pD->data = pS->data;
      if (pD->pNext == nullptr)
         pSave = pD;
      pD = pD->pNext;
      pS = pS->pNext;
   }
   if (pS == nullptr && pD != nullptr)
   {
      if (pD->pNext != nullptr)
         clear(pD->pNext);

      if (pD->pPrev != nullptr)
         pD->pPrev->pNext = nullptr;

      delete pD;
   }
   else if (pS != nullptr && pD == nullptr)
   {
      pD = copy(pS);
      pD->pPrev = pSave;
      pSave->pNext = pD;
   }

}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{
   std::swap(pLHS, pRHS);
   //Node <T>* pSwap = pLHS;
   //pLHS = pRHS;
   //pRHS = pSwap;
}

/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node <T> * remove(const Node <T> * pRemove) 
{
   Node <T>* pReturn = nullptr;
   if (pRemove == nullptr)
      return nullptr;
   else if (pRemove->pNext != nullptr && pRemove->pPrev != nullptr)
   {
      pRemove->pNext->pPrev = pRemove->pPrev;
      pRemove->pPrev->pNext = pRemove->pNext;

      pReturn = pRemove->pPrev;
      delete pRemove;
   }
   else if (pRemove->pNext != nullptr) {
      pRemove->pNext->pPrev = nullptr;

      pReturn = pRemove->pNext;
      delete pRemove;
   }
   else if (pRemove->pPrev != nullptr)
   {
      pRemove->pPrev->pNext = nullptr;

      pReturn = pRemove->pPrev;
      delete pRemove;
   }
   
   return pReturn;
}

/**********************************************
 * INSERT 
 * Insert a new node the the value in "t" into a linked
 * list immediately before the current position.
 *   INPUT   : t - the value to be used for the new node
 *             pCurrent - a pointer to the node before which
 *                we will be inserting the new node
 *             after - whether we will be inserting after
 *   OUTPUT  : return the newly inserted item
 *   COST    : O(1)
 **********************************************/
template <class T>
inline Node <T> * insert(Node <T> * pCurrent,
                  const T & t,
                  bool after = false)
{
   if (pCurrent == nullptr)
      return new Node <T>(t);

   Node <T>* pFar;
   Node <T>* newNode = new Node <T>(t);
   if (after)
   {
      pFar = pCurrent->pNext;
      newNode->pPrev = pCurrent;
      newNode->pNext = pFar;
      pCurrent->pNext = newNode;
      if (pFar != nullptr)
      {
         pFar->pPrev = newNode;
      }
   }
   else
   {
      pFar = pCurrent->pPrev;
      newNode->pNext = pCurrent;
      newNode->pPrev = pFar;
      pCurrent->pPrev = newNode;
      if (pFar != nullptr)
      {
         pFar->pNext = newNode;
      }
   }
   return newNode;
}

/******************************************************
 * SIZE
 * Find the size an unsorted linked list.  
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : number of nodes
 *  COST    : O(n)
 ********************************************************/
template <class T>
inline size_t size(const Node <T> * pHead)
{
   if (pHead == nullptr)
      return 0;

   int size = 1;

   for (auto p = pHead->pNext; p != nullptr; p = p->pNext)
   {
      size++;
   }
   return size;
}

/***********************************************
 * DISPLAY
 * Display all the items in the linked list from here on back
 *    INPUT  : the output stream
 *             pointer to the linked list
 *    OUTPUT : the data from the linked list on the screen
 *    COST   : O(n)
 **********************************************/
template <class T>
inline std::ostream & operator << (std::ostream & out, const Node <T> * pHead)
{
   return out;
}

/*****************************************************
 * CLEAR
 * Free all the data currently in the linked list
 *   INPUT   : pointer to the head of the linked list
 *   OUTPUT  : pHead set to NULL
 *   COST    : O(n)
 ****************************************************/
template <class T>
inline void clear(Node <T> * & pHead)
{
   if (pHead == nullptr)
      return;

   if (pHead->pNext != nullptr)
   {
      pHead->pNext->pPrev = nullptr;
      clear(pHead->pNext);
   }

   if (pHead->pPrev != nullptr) 
   {
      pHead->pPrev->pNext = nullptr;
      clear(pHead->pPrev);
   }

   delete pHead;
   pHead = nullptr;
}



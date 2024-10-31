/***********************************************************************
 * Header:
 *    BINARY NODE
 * Summary:
 *    One node in a binary tree (and the functions to support them).
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        BNode         : A class representing a BNode
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <iostream>  // for OFSTREAM
#include <cassert>

/*****************************************************************
 * BNODE
 * A single node in a binary tree.  Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <class T>
class BNode
{
public:
   // 
   // Construct
   //
   BNode()
   {
      pLeft = nullptr;
      pRight = nullptr;
      pParent = nullptr;
      data = NULL;
   }
   BNode(const T& t)
   {
      pLeft = nullptr;
      pRight = nullptr;
      pParent = nullptr;
      data = t;
   }
   BNode(T&& t)
   {
      pLeft = nullptr;
      pRight = nullptr;
      pParent = nullptr;
      data = std::move(t);
   }

   //
   // Data
   //
   BNode <T>* pLeft;
   BNode <T>* pRight;
   BNode <T>* pParent;
   T data;
};

/*******************************************************************
 * SIZE BTREE
 * Return the size of a b-tree under the current node
 *******************************************************************/
template <class T>
inline size_t size(const BNode <T> * p)
{
   if (p == nullptr)
      return 0;
   
   int n = 1;

   n += size(p->pLeft);
   n += size(p->pRight);

   return n;
}


/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft(BNode <T> * pNode, BNode <T> * pAdd)
{
   if (pNode->pLeft != nullptr)
      addLeft(pNode->pLeft, pAdd);
   else {
      pNode->pLeft = pAdd;
      pNode->pLeft->pParent = pNode;
   }
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
inline void addRight (BNode <T> * pNode, BNode <T> * pAdd)
{
   if (pNode->pRight != nullptr)
      addRight(pNode->pRight, pAdd);
   else {
      pNode->pRight = pAdd;
      pNode->pRight->pParent = pNode;
   }
}

/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft (BNode <T> * pNode, const T & t)
{
   if (pNode->pLeft != nullptr)
      addLeft(pNode->pLeft, t);
   else {
      pNode->pLeft = new BNode<T>(t);
      pNode->pLeft->pParent = pNode;
   }
}

template <class T>
inline void addLeft(BNode <T>* pNode, T && t)
{
   if (pNode->pLeft != nullptr)
      addLeft(pNode->pLeft, std::move(t));
   else {
      pNode->pLeft = new BNode<T>(std::move(t));
      pNode->pLeft->pParent = pNode;
   }
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
void addRight (BNode <T> * pNode, const T & t)
{
   if (pNode->pRight != nullptr)
      addRight(pNode->pRight, t);
   else {
      pNode->pRight = new BNode<T>(t);
      pNode->pRight->pParent = pNode;
   }
}

template <class T>
void addRight(BNode <T>* pNode, T && t)
{
   if (pNode->pRight != nullptr)
      addRight(pNode->pRight, std::move(t));
   else {
      pNode->pRight = new BNode<T>(std::move(t));
      pNode->pRight->pParent = pNode;
   }
}

/*****************************************************
 * DELETE BINARY TREE
 * Delete all the nodes below pThis including pThis
 * using postfix traverse: LRV
 ****************************************************/
template <class T>
void clear(BNode <T> * & pThis)
{
   if (pThis == nullptr)
      return;

   if (pThis->pLeft != nullptr)
      clear(pThis->pLeft);

   if (pThis->pRight != nullptr)
      clear(pThis->pRight);

   delete pThis;
   pThis = nullptr;
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(BNode <T>*& pLHS, BNode <T>*& pRHS)
{
   std::swap(pLHS, pRHS);
}

/**********************************************
 * COPY BINARY TREE
 * Copy pSrc->pRight to pDest->pRight and
 * pSrc->pLeft onto pDest->pLeft
 *********************************************/
template <class T>
BNode <T> * copy(const BNode <T> * pSrc)
{
   BNode<T>* dest = new BNode<T>;

   if (pSrc == nullptr)
      return nullptr;

   dest->data = pSrc->data;

   if (pSrc->pLeft != nullptr) {
      dest->pLeft = copy(pSrc->pLeft);
      dest->pLeft->pParent = dest;
   }
   
   if (pSrc->pRight != nullptr) {
      dest->pRight = copy(pSrc->pRight);
      dest->pRight->pParent = dest;
   }

   return dest;
}

/**********************************************
 * assign
 * copy the values from pSrc onto pDest preserving
 * as many of the nodes as possible.
 *********************************************/
template <class T>
void assign(BNode <T> * & pDest, const BNode <T>* pSrc)
{
   if (pSrc == nullptr) {
      clear(pDest);
      return;
   }

   if (pDest == nullptr) {
      pDest = copy(pSrc);
   }

   pDest->data = pSrc->data;

   assign(pDest->pLeft, pSrc->pLeft);
   assign(pDest->pRight, pSrc->pRight);
   
   if (pDest->pLeft != nullptr)
      pDest->pLeft->pParent = pDest;

   if (pDest->pRight != nullptr)
      pDest->pRight->pParent = pDest;
}

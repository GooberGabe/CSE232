/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair
#include <stack>

class TestBST; // forward declaration for unit tests
class TestMap;
class TestSet;

namespace custom
{

   template <class TT>
   class set;
   template <class KK, class VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set;

   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return nullptr; }

   //
   // Access
   //

   iterator find(const T& t);

   // 
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   // 

   iterator erase(iterator& it);
   void   clear() noexcept;

   // 
   // Status
   //

   bool   empty() const noexcept { return true; }
   size_t size()  const noexcept { return numElements;   }
   

private:

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //
   BNode()
   {
      pLeft = pRight = nullptr;
   }
   BNode(const T &  t) : data(t)
   {
      pLeft = pRight = nullptr; 
   }
   BNode(T&& t) : data(std::move(t))
   {
      pLeft = pRight = nullptr;
   }

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);

   // 
   // Status
   //
   bool isRightChild(BNode * pNode) const { return true; }
   bool isLeftChild( BNode * pNode) const { return true; }

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set; 
public:
   // constructors and assignment
   iterator(BNode* p = nullptr) : pNode(p) {}
   
   iterator(const iterator& rhs) : pNode(rhs.pNode) {}

   iterator & operator = (const iterator & rhs)
   {
      if (this != &rhs)
      {
         pNode = rhs.pNode;
      }
      return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
      return pNode == rhs.pNode;
   }
   bool operator != (const iterator & rhs) const
   {
      return pNode != rhs.pNode;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const 
   {
      if (pNode == nullptr)
      {
         throw "Accessing null iterator";
      }
      return pNode->data;
   }

   // increment and decrement
   iterator& operator ++ ();
   iterator   operator ++ (int postfix)
   {
      return *this;
   }
   iterator & operator -- ();

   iterator   operator -- (int postfix)
   {
      return *this;;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> ::BST()
{
   numElements = 0;
   root = nullptr;
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST ( const BST<T>& rhs) 
{
   numElements = rhs.numElements;

   if (rhs.root == nullptr)
   {
      this->root = nullptr;
      return;
   }

   std::stack<BNode*> rhsNodeStack;
   std::stack<BNode*> newNodeStack;

   auto newRoot = new BNode(rhs.root->data);
   rhsNodeStack.push(rhs.root);
   newNodeStack.push(newRoot);

   this->root = newRoot;

   while (newNodeStack.size() > 0)
   {
      auto rhsTop = rhsNodeStack.top();
      auto newTop = newNodeStack.top();
      rhsNodeStack.pop();
      newNodeStack.pop();

      if (rhsTop->pRight != nullptr)
      {
         auto newNode = new BNode(rhsTop->pRight->data);
         newNode->pParent = newTop;
         newTop->pRight = newNode;
         rhsNodeStack.push(rhsTop->pRight);
         newNodeStack.push(newNode);
      }

      if (rhsTop->pLeft != nullptr)
      {
         auto newNode = new BNode(rhsTop->pLeft->data);
         newNode->pParent = newTop;
         newTop->pLeft = newNode;
         rhsNodeStack.push(rhsTop->pLeft);
         newNodeStack.push(newNode);
      }
   }
   
}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs) 
{
   numElements = rhs.numElements;

   if (rhs.root == nullptr)
   {
      this->root = nullptr;
      return;
   }
   
   this->root = rhs.root;
   rhs.root = nullptr;
   rhs.numElements = 0;
   
}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/
template <typename T>
BST <T> ::BST(const std::initializer_list<T>& il)
{
   numElements = 0;
   if (il.size() == 0)
   {
      this->root = nullptr;
      return;
   }

   for (auto it = il.begin(); it != il.end(); it++) 
      insert(*it);
   
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
   if (root == nullptr)
      return;

   
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
   return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
   return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
   return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{

}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
   if (root == nullptr)
   {
      root = new BNode(t);
      auto it = BST<T>::iterator(root);
      std::pair<iterator, bool> pairReturn(it, true);
      numElements++;
      return pairReturn;
   }

   auto* pCurrent = root;

   bool exit = false;
   bool isLeft;
   
   while (!exit)
   {
      if (keepUnique)
      {
         if (pCurrent->data == t)
         {
            auto it = BST<T>::iterator(pCurrent);
            std::pair<iterator, bool> pairReturn(it, false);
            return pairReturn;
         }
      }

      if (t < pCurrent->data)
      {
         if (pCurrent->pLeft == nullptr)
         {
            isLeft = true;
            exit = true;
         }
         else
            pCurrent = pCurrent->pLeft;
      }
      else
      {
         if (pCurrent->pRight == nullptr)
         {
            
            isLeft = false;
            exit = true;
         }
         else
            pCurrent = pCurrent->pRight;
      }
   }

   auto* newNode = new BNode(t);

   if (isLeft)
      pCurrent->pLeft = newNode;
   else
      pCurrent->pRight = newNode;


   newNode->pParent = pCurrent;
   auto it = BST<T>::iterator(newNode);
   std::pair<iterator, bool> pairReturn(it, true);
   numElements++;
   return pairReturn;
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T && t, bool keepUnique)
{
   if (root == nullptr)
   {
      root = new BNode(t);
      auto it = BST<T>::iterator(root);
      std::pair<iterator, bool> pairReturn(it, true);
      numElements++;
      return pairReturn;
   }
      
   auto* pCurrent = root;

   bool exit = false;
   bool isLeft;

   while (!exit)
   {
      if (keepUnique)
      {
         if (pCurrent->data == t)
         {
            auto it = BST<T>::iterator(pCurrent);
            std::pair<iterator, bool> pairReturn(it, false);
            return pairReturn;
         }
      }

      if (t < pCurrent->data)
      {
         if (pCurrent->pLeft == nullptr)
         {
            isLeft = true;
            exit = true;
         }
         else
            pCurrent = pCurrent->pLeft;
      }
      else
      {
         if (pCurrent->pRight == nullptr)
         {

            isLeft = false;
            exit = true;
         }
         else
            pCurrent = pCurrent->pRight;
      }
   }

   auto* newNode = new BNode(std::move(t));

   if (isLeft)
      pCurrent->pLeft = newNode;
   else
      pCurrent->pRight = newNode;

   newNode->pParent = pCurrent;
   auto it = BST<T>::iterator(newNode);
   std::pair<iterator, bool> pairReturn(it, true);
   numElements++;
   return pairReturn;
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST <T> ::iterator BST <T> :: erase(iterator & it)
{  
   if (it == end()) {
      return it;
   }

   iterator next = it;
   ++next;

   BNode* node = it.pNode;
   BNode* parent = node->pParent;

   if (!node->pLeft && !node->pRight) {
      if (parent) {
         if (parent->pLeft == node) parent->pLeft = nullptr;
         else parent->pRight = nullptr;
      }
      else {
         root = nullptr;
      }
      delete node;
   }
   else if (!node->pLeft) {
      if (parent) {
         if (parent->pLeft == node) parent->pLeft = node->pRight;
         else parent->pRight = node->pRight;
      }
      else {
         root = node->pRight;
      }
      node->pRight->pParent = parent;
      delete node;
   }
   else if (!node->pRight) {
      if (parent) {
         if (parent->pLeft == node) parent->pLeft = node->pLeft;
         else parent->pRight = node->pLeft;
      }
      else {
         root = node->pLeft;
      }
      node->pLeft->pParent = parent;
      delete node;
   }
   else {
      BNode* successor = node->pRight;
      while (successor->pLeft) {
         successor = successor->pLeft;
      }

      if (successor->pParent != node) {
         successor->pParent->pLeft = successor->pRight;
         if (successor->pRight)
            successor->pRight->pParent = successor->pParent;

         successor->pRight = node->pRight;
         node->pRight->pParent = successor;
      }

      successor->pLeft = node->pLeft;
      node->pLeft->pParent = successor;

      successor->pParent = node->pParent;
      if (parent) {
         if (parent->pLeft == node)
            parent->pLeft = successor;
         else
            parent->pRight = successor;
      }
      else {
         root = successor;
      }

      delete node;
   }

   numElements--;
   return next;

}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{

}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
   if (root == nullptr)
      return iterator(nullptr);

   BNode* tempNode = root;
   while (tempNode->pLeft != nullptr)
   {
      tempNode = tempNode->pLeft;
   }

   return iterator(tempNode);

}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
   return end();
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{

}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{

}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{

}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{

}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{

}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{

}



/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
   if (pNode == nullptr)
      return *this;

   if (pNode->pRight != nullptr)
   {
      BNode* tempNode = pNode->pRight;
      while (tempNode->pLeft != nullptr)
      {
         tempNode = tempNode->pLeft;
      }
      pNode = tempNode;
      return *this;
   }
   else
   {
      BNode* tempNode = pNode;
      bool exit = false;
      while (exit == false)
      {
         if (tempNode->pParent == nullptr)
         {
            pNode = nullptr;
            return *this;
         }
         
         if (tempNode == tempNode->pParent->pLeft)
            exit = true;

         tempNode = tempNode->pParent;
      }
      pNode = tempNode;
      return *this;
   }
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
   if (pNode == nullptr)
      return *this;

   if (pNode->pLeft != nullptr)
   {
      BNode* tempNode = pNode->pLeft;
      while (tempNode->pRight != nullptr)
      {
         tempNode = tempNode->pRight;
      }
      pNode = tempNode;
      return *this;
   }
   else
   {
      BNode* tempNode = pNode;
      bool exit = false;
      while (exit == false)
      {
         if (tempNode->pParent == nullptr)
         {
            pNode = nullptr;
            return *this;
         }

         if (tempNode == tempNode->pParent->pRight)
            exit = true;

         tempNode = tempNode->pParent;
      }
      pNode = tempNode;
      return *this;
   }

}


} // namespace custom



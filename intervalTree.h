/**
 * @file intervalTree.h
 * @author Liam Gomez
 * @brief Class Spec for interval tree and all other necassary classes
 * @date 4/29/2015
 */

// HEADER FILES ////////////////////////////////////////////////////////////////
#ifndef INTERVAL_TREE
#define INTERVAL_TREE
   
#include <iostream>
#include <string>

using namespace std;

// GLOBAL CONSTANTS ////////////////////////////////////////////////////////////
const int BLACK = 0;
const int RED = 1;

// simple max help function
int findMax(int a, int b);

// INTERVAL CLASS //////////////////////////////////////////////////////////////
class interval
  {
   public:
      // constructor
      interval();

      // print functions
      void printInterval();
      void printOverlap();

      // assignment operator
      interval& operator =(const interval& copy);

   // Maybe make private or protected? doesnt really matter though
   int low, high;
   
  };

// INTERVAL NODE CLASS /////////////////////////////////////////////////////////

/** @note uses a template becuase i initially started with my BST imp */
template <class T>
class iNode
  {
   public:
      // constructors
      iNode();
      iNode( interval copy );

      // TODO: Make private?

      /**
       * @note I left these as public data members because of all the pointer 
       *       logic/operations that where needed.
       */

      // NODE POINTERS
      iNode<T> *left;
      iNode<T> *right;
      iNode<T> *parent;
      
      // OTHER DATA MEMBERS
      int color;
      int max;
      interval i;

  };

// INTERVAL TREE CLASS /////////////////////////////////////////////////////////

/** @note uses a template becuase i initially started with my BST imp */
template <class T>
class intervalTree
  {
   public:
      intervalTree();
      ~intervalTree();
      void destroyTree(iNode<T> *sub);

      void insert(const interval input);

      // interval deletion
      iNode<T>* getLeftMostNode(iNode<T> *sub);
      iNode<T>* search(iNode<T>* sub, const interval search);
      iNode<T>* searchHelper(const interval search);
      void deleteInterval(iNode<T>* target);
      void fixDelete(iNode<T>* x);
      void swap(iNode<T> *a, iNode<T>* b);

      // insertion helpers and fixers 
      void rotateLeft(iNode<T> *x);
      void rotateRight(iNode<T> *x);
      void fixInsertion(iNode<T> *fix);
      void fixMaxValues(iNode<T> *x);

      // overlap detection
      bool overlaps(interval a, interval b);
      void findOverlap(iNode<T> *sub, const interval& target);
      void overlapHelper(const interval& target);

      // pre order
      void preorder(iNode<T> *sub);

      // inorder print
      void inorderPrint(iNode<T> *sub);

      // prints both inorder then pre order for now.
      void showTree();

   private:

      // private methods
      iNode<T> *root;
      iNode<T> *NIL;

  };

#include "intervalTree.cpp"
#endif

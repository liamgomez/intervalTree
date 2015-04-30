// Header Files
#ifndef INTERVAL_TREE
#define INTERVAL_TREE
   
   // debug
   #include <iostream>
   #include <string>
   using namespace std;

// set colors black and red to integer values
const int BLACK = 0;
const int RED = 1;

// simple max help function
int findMax(int a, int b);

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
       *       logic and operations that where needed.
       */
      int color;
      int max;
      interval i;
      iNode<T> *left;
      iNode<T> *right;
      iNode<T> *parent;

  };

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

// Header Files
#ifndef INTERVAL_TREE
#define INTERVAL_TREE
   
   // debug
   #include <iostream>
   #include <string>
   using namespace std;

const int BLACK = 0;
const int RED = 1;

// simple max help function
int findMax(int a, int b);

class interval
  {
   public:
      interval();
      void printInterval();
      void printOverlap();
      interval& operator =(const interval& copy);

   // Maybe make private or protected? doesnt really matter though
   int low, high;
   
  };

template <class T>
class iNode
  {


   public:
      iNode();
      iNode( interval copy );
      int getLeftColor();
      int getRightColor();

      void setLeftColor(int nodeColor);
      void setRightColor(int nodeColor);


      int color;
      int max;
      interval i;
      iNode<T> *left;
      iNode<T> *right;
      iNode<T> *parent;

  };

template <class T>
class intervalTree
  {
   public:
      intervalTree();
      ~intervalTree();
      void destroyTree(iNode<T> *sub);

      void insert(const interval input);
      iNode<T>* search(const interval search);
      
      // delete, overlap

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
      iNode<T> *root;
      iNode<T> *NIL;

  };
#include "intervalTree.cpp"
#endif

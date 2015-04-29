#include "intervalTree.h"

/**
 * @brief Finds the max number between two numbers and returns it
 * @details simple comparison to find the max of two inputs, used throughout
 *          the interval tree.
 * 
 * @param a The first number to compare
 * @param b The second number to compare
 * 
 * @return An integer, the maximum of the two. If equal b will be returned
 *         intead.
 */
int findMax(int a, int b)
  {
   if(a > b)
      return a;

   else 
      return b;
  }

// INTERVAL CLASS //////////////////////////////////////////////////////////////
  /**
   * @brief default constructor for interval class
   * @details sets the low and high values to a negative flag value of -1
   */
interval::interval()
  {
   // set default values
   low = -1;
   high = -1;
  }

/**
 * @brief simple print function to assist in the printing of intervals
 */
void interval::printInterval()
  {
   cout << "[" << low << ", " << high << "] ";
  }

/**
 * @brief A simple print function to assist with printing the overlap message
 *        and corrosponding interval
 */   
void interval::printOverlap()
  {
   cout << "Overlap Found for time interval :  ";
   printInterval();
   cout << endl << endl;
  }

/**
 * @brief overloaded assignment operator for interval class
 * @details Simply copies the low and high values of the left side to
 *          the current interval. Not really used but thought it might
 *          come in handy.
 * 
 * @param copy The interval to copy
 * @return *this interval to finalize assingment
 */
interval& interval::operator =(const interval& copy)
  {
   low = copy.low;
   high = copy.high;

   return *this;
  }

// INTERVAL NODE CLASS /////////////////////////////////////////////////////////
/**
 * @brief Default constructor for interval node class sets the max value of the
 *        node to -1
 *        
 * @return none
 */
template <class T>
iNode<T>::iNode()
  {
   max = -1;
  }

/**
 * @brief copy constructor for interval node class
 * @details accepts one interval and copies it to the nodes interval, used
 *          throughout program.
 * 
 * @param copy The interval to copy has low and high values/
 * @return none
 */
template <class T>
iNode<T>::iNode( interval copy )
  {
   i.low = copy.low;
   i.high = copy.high;
  }

template <class T>
intervalTree<T>::intervalTree()
  {
   NIL = new iNode<T>;

      NIL->left = NIL;
      NIL->right = NIL;
      NIL->parent = NIL;

      NIL->max = -10000000;
      NIL->color = BLACK;
      NIL->i.low = -10000000;
      NIL->i.high = -10000000;

   root = new iNode<T>;
      root = NIL;
      //root->left = NIL;
      //root->right = NIL;
      //root->parent = NIL;
      //root->max = 10000000;
      //root->i.low = 10000000;
      //root->i.high = 10000000;
  }

// INTERVAL TREE IMPLEMENTATION ////////////////////////////////////////////////
template <class T>
intervalTree<T>::~intervalTree()
  {
   // destroy the tree starting at the root
   destroyTree(root);
  }

template <class T>
void intervalTree<T>::destroyTree(iNode<T> *sub)
  {
   if (sub != NIL)
     {
      destroyTree(sub->left);
      destroyTree(sub->right);
      delete sub;
     }   
  }

template <class T>
void intervalTree<T>::insert( const interval input )
  {   
   iNode<T> *x = root;
   iNode<T> *y = NIL;

   iNode<T> *temp;
   temp = new iNode<T>(input);

   while( x != NIL)
     {
      y = x;
      if (temp->i.low < x->i.low)
        {
         x = x->left;
        }
      else
        {
         x = x->right;
        }
     }

   temp->parent = y;

   if( y == NIL)
     {
      // the tree is empty so set new element as root
      root = temp;
      //cout << "Root node : " << endl;
      //cout << root->i.low << "  " << root->i.high << endl;
     }

   else if(temp->i.low < y->i.low)
     {
      y->left = temp;
      //cout << "Inserting Left Node : " << endl;
      //cout << y->left->i.low << "  " << y->left->i.high << endl;
     }

   else
     {
      y->right = temp;
      //cout << "Inserting Right Node : " << endl;
      //cout << y->right->i.low << "  " << y->right->i.high << endl;
     }

   // set left and right ptrs and color
   temp->left = NIL;
   temp->right = NIL;
   temp->color = RED;

   fixMaxValues(temp);
   fixInsertion(temp);
  }

template <class T>
iNode<T>* intervalTree<T>::search(const interval search)
  {


  }

template <class T>
void intervalTree<T>::rotateLeft(iNode<T> *x)
  {

   iNode<T> *y = x->right;
   y = x->right;
   x->right = y->left;

   if (y->left != this->NIL)
     {
      y->left->parent = x;
     }

   y->parent = x->parent;

   if (x->parent == this->NIL)
     {
      this->root = y;
     }

   else if (x == x->parent->left)
     {
      x->parent->left = y;
     }

   else
     {
      x->parent->right = y;
     }

   y->left = x;
   x->parent = y;

   // update the max data members
   y->max = x->max;
   x->max = findMax(x->i.high, findMax(x->left->max, x->right->max));
  }

template <class T>
void intervalTree<T>::rotateRight(iNode<T> *x)
  {

   iNode<T> *y = x->left;
   x->left = y->right;

   if (y->right != this->NIL)
     {
      y->right->parent = x;
     }

   y->parent = x->parent;

   if (x->parent == this->NIL)
     {
      this->root = y;
     }

   else if (x == x->parent->left)
     {
      x->parent->left = y;
     }

   else
     {
      x->parent->right = y;
     }

   y->right = x;
   x->parent = y;

   // update the max data members
   x->max = y->max;
   y->max = findMax(y->i.high, findMax(y->left->max, y->right->max));
  }

template <class T>
void intervalTree<T>::fixInsertion(iNode<T> *fix)
  {
   while(fix->parent->color == RED)
     {
      if(fix->parent == fix->parent->parent->left)
        {
         iNode<T> *temp = fix->parent->parent->right;

         if(temp->color == RED)
           {
            fix->parent->color = BLACK;
            temp->color = BLACK;
            fix->parent->parent->color = RED;
            fix = fix->parent->parent;
           }

         else
           {
            if(temp == temp->parent->right)
              {
               fix = fix->parent;
               rotateLeft(fix);  
              }

            temp->parent->color = BLACK;
            temp->parent->parent->color = RED;
            rotateRight(fix->parent->parent);
           }
        }

      else
        {
         iNode<T> *temp = fix->parent->parent->right;
         if (temp->color == RED)
           {
            fix->parent->color = BLACK;
            temp->color = BLACK;
            fix->parent->parent->color = RED;
            fix = fix->parent->parent;
           }
         else
           {
            if (fix == fix->parent->left)
              {
               fix = fix->parent;
               rotateRight(fix);
              }
            fix->parent->color = BLACK;
            fix->parent->parent->color = RED;
            rotateLeft(fix->parent->parent);
           }
        }
     }

   // set the root color to black
   root->color = BLACK;
  }

template <class T>
void intervalTree<T>::fixMaxValues(iNode<T> *x)
  {
   while( x != NIL )
     {
      x->max = findMax( x->i.high, findMax(x->left->max, x->right->max));
      x = x->parent;
     }
  }

template <class T>
bool intervalTree<T>::overlaps(interval a, interval b)
  {
   if ((b.high >= a.low) && (a.high >= b.low))
      return true;
   else
      return false;
  }

template <class T>
void intervalTree<T>::findOverlap(iNode<T> *sub, const interval& target)
  {
   bool overlapFound = false;

   if (sub != NIL)
     {
      // search left
      findOverlap(sub->left, target);
      
      // detect overlap
      overlapFound = overlaps(sub->i, target);
      if (overlapFound)
        {
         sub->i.printOverlap();
        }

      // search right
      findOverlap(sub->right, target);
     }

  }

template <class T>
void intervalTree<T>::overlapHelper(const interval& target)
  {
   findOverlap(root, target);
  }

template <class T>
void intervalTree<T>::preorder(iNode<T> *sub)
  {
   if (sub != NIL)
     {
      preorder(sub->left);

      sub->i.printInterval();
      cout << "max = " << sub->max << "  Color : ";
      if (sub->color == RED)
        {
         cout << "RED" << endl;
        }

      else
         cout << "BLACK" << endl;
      preorder(sub->right);
     }

  }

template <class T>
void intervalTree<T>::inorderPrint(iNode<T> *sub)
  {
   if (sub != NIL)
     {
      sub->i.printInterval();
      cout << "max = " << sub->max << "  Color : ";

      if (sub->color == RED)
        {
         cout << "RED" << endl;
        }

      else
         cout << "BLACK" << endl;
      inorderPrint(sub->left);
      inorderPrint(sub->right);
     }

  }

template <class T>
void intervalTree<T>::showTree()
  {
   cout << "CURRENT ROOT DATA: " << '[' << root->i.low << ", ";
   cout << root->i.high << ']' << "  MAX : " << root->max;
   cout << "   Color : ";

      if (root->color == RED)
        {
         cout << "RED" << endl;
        }

      else
         cout << "BLACK" << endl;

   cout << endl << endl;

   inorderPrint(root);
   cout << endl << endl;

   preorder(root);
  }
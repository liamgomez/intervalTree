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
   // set max to integer flag
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

////////////////////////////////////////////////////////////////////////////////
// INTERVAL TREE IMPLEMENTATION ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief default constructor for interval tree class
 * @details This constructor allocates two nodes, one for the NIL pointer and
 *          the other for the root pointer. It sets all the interval and max
 *          variables to a very low number to avoid any conflicts. Sets root to
 *          NIL to make the tree empty. To maintain RB tree properties the
 *          NIL and root nodes both start with the color black.
 * 
 * @return none
 */
template <class T>
intervalTree<T>::intervalTree()
  {
   // allocate and set NIL values
      NIL = new iNode<T>;

      NIL->left = NIL;
      NIL->right = NIL;
      NIL->parent = NIL;

      NIL->max = -999999;
      NIL->color = BLACK;
      NIL->i.low = -999999;
      NIL->i.high = -999999;

   // allocate root node, and set to NIL
      root = new iNode<T>;
      root = NIL;
  }

/**
 * @brief default destructor for interval tree class
 * @details uses the method destroy tree, on the root node to preform
 *          a post order deletion of the nodes in the tree.
 * 
 * @pre the program must end
 * @post the tree will be deleted and the memory returned to the operating
 *       system.
 * 
 * @return none
 */
template <class T>
intervalTree<T>::~intervalTree()
  {
   // destroy the tree starting at the root
   destroyTree(root);

   // set root and NIL to null
   root = nullptr;
   NIL = nullptr;
  }

/**
 * @brief destroys the tree by preforming a post order deletion
 * @details This function is used in the default destructor for the interval
 *          tree class. 
 * 
 * @param sub The root pointer of the tree
 * @return none
 */
template <class T>
void intervalTree<T>::destroyTree(iNode<T> *sub)
  {
   if (sub != NIL)
     {
      // post order delete

         // destroy left subtree
         destroyTree(sub->left);

         // destroy right subtree
         destroyTree(sub->right);

      // delete sub tree
      delete sub;
     }   
  }

/**
 * @brief Inserts the input interval into new node and puts it into the tree
 * @details This insertion function first finds the appropriate place for the
 *          new node, using the interval low value (i.low) as the key. It then
 *          checks the various cases, and prepares the node for insertion fix.
 * 
 * @pre The interval should not already exist in the tree, and should probably
 *      not be negative although i havent tested it.
 * 
 * @post The new interval will be placed into the tree with no children, and
 *       be the color RED to prepare it for insertion fix.
 * 
 * @param input The interval to be inputed into the interval tree
 * @return none
 */
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
     }

   else if(temp->i.low < y->i.low)
     {
      // put to the left
      y->left = temp;
     }

   else
     {
      // put to the right
      y->right = temp;
     }

   // set the new node as being NIL and color RED to prepare for fix
   temp->left = NIL;
   temp->right = NIL;
   temp->color = RED;

   // fix the max data member values in the tree
   fixMaxValues(temp);

   // fix up the tree
   fixInsertion(temp);
  }

/**
 * @brief Obtains the left most node for a given subtree
 * @details Gets the farthest node to the left and returns it, the subtree
 *          is ussually the root node, but it doesnt matter either way.
 * 
 * @param sub The subtree in which to obtain the left most node
 * @return The interval node that is the farthest left for the given subtree.
 */
template <class T>
iNode<T>* intervalTree<T>::getLeftMostNode(iNode<T> *sub)
  {
   iNode<T> *search = sub;

   while( search->left != NIL)
     {
      search = search->left;
     }

   return search;
  }

/**
 * @brief Searches the tree for a given interval
 * @details This function recursively searches the tree, going either left or 
 *          right based on the key value. 
 * 
 * @pre the subtree to search for the interval should be the root initially,
 *      Should only be called using the searchHelper function or bad things
 *      will happen.
 * 
 * @post The given interval will either be found, or a null pointer will be
 *       returned.
 * 
 * @param sub The subtree to find the interval in (Should be root!)
 * @param find The interval to find in the tree
 * 
 * @return The interval node that contains the target interval
 */
template <class T>
iNode<T>* intervalTree<T>::search(iNode<T>* sub, const interval find)
  {
   // if the pointer is NIL
   if( sub == NIL)
     {
      // The interval can not be found
      return nullptr;
     }

   // is this the node that contains our interval?
   else if (sub->i.low == find.low)
     {
      // yes, return the interval node
      return sub;
     }

   // if the key is less than our current subtree key
   else if (find.low < sub->i.low)
     {
      // search left
      search(sub->left, find);
     }

   // otherwise key is greater than current sub tree
   else
     {
      // search right portion of subtree
      search(sub->right, find);
     }
  }

/**
 * @brief The recursive helper for the search function
 * @details Simply calls the search function on the root node and returns
 *          the found node to caller.
 *
 * @pre the interval should exist in the tree, if not the caller should force
 *      conditions for no run on nullptr return
 *
 * @param find The interval to find
 * @return The interval node that contains the specified interval, nullptr if
 *         the interval could not be found.
 */
template <class T>
iNode<T>* intervalTree<T>::searchHelper(const interval find)
  {
   iNode<T>* found = search(root, find);

   return found;
  }

template <class T>
void intervalTree<T>::deleteInterval(iNode<T>* target)
  {
   iNode<T>* xNode;
   iNode<T>* yNode = target;

   int yColor = yNode->color;
   if (target->left == this->NIL){
      xNode = target->right;
      swap(target, target->right);
   }
   else if (target->right == this->NIL){
      xNode = target->left;
      swap(target, target->left);
   }
   else
     {
      yNode = getLeftMostNode(target->right);

      yColor = yNode->color;
      xNode = yNode->right;

      if (yNode->parent == target)
        {
         xNode->parent = yNode;
        }

      else
        {
         swap(yNode, yNode->right);
         yNode->right = target->right;
         yNode->right->parent = yNode;
        }

      swap(target, yNode);
      yNode->left = target->left;
      yNode->left->parent = yNode;
      yNode->color = target->color;
      yNode->max = findMax(yNode->i.high, findMax(yNode->left->max, yNode->right->max));
     }

   // fix the max data member for affected tree
   fixMaxValues(target->parent);

   // if we deleted a black node
   if (yColor == BLACK)
     {
      // preform fix operation on tree
      fixDelete(xNode);
     }

   // delete the target node
   delete target;
  }

template <class T>
void intervalTree<T>::fixDelete(iNode<T>* x)
  {
   while(x != root && x->color == BLACK) 
     {
      if(x == x->parent->left) 
        {
         iNode<T>* w = x->parent->right;
         if(w->color == RED) 
           { //case 1
            w->color = BLACK;
            x->parent->color = RED;
            rotateLeft(x->parent);
            w = x->parent->right;
           }
         
         if(w->left->color == BLACK && w->right->color == BLACK) 
           { // case 2
            w->color = RED;
            x = x->parent;
           }

         else 
           {
            if(w->right->color == BLACK)
              { // case 3
               w->left->color = BLACK;
               w->color = RED;
               rotateRight(w);
               w = x->parent->right;
              }

            w->color = x->parent->color; // case 4
            x->parent->color = BLACK;
            w->right->color = BLACK;
            rotateLeft(x->parent);
            x = root;
           }
        }

       else 
         {
         iNode<T>* w = x->parent->left;
         if(w->color == RED) 
           { //case 1
            w->color = BLACK;
            x->parent->color = RED;
            rotateRight(x->parent);
            w = x->parent->left;
           }
         
         if(w->left->color == BLACK && w->right->color == BLACK)
           { // case 2
            w->color = RED;
            x = x->parent;
           }

         else 
           {
            if(w->left->color == BLACK)
              { // case 3
               w->right->color = BLACK;
               w->color = RED;
               rotateLeft(w);
               w = x->parent->left;
              }

            w->color = x->parent->color; // case 4
            x->parent->color = BLACK;
            w->left->color = BLACK;
            rotateRight(x->parent);
            x = root;
           }
        }
     }
   x->color = BLACK;
  }

/**
 * @brief Simple function thats swaps the two nodes a and b
 * 
 * @param a First interval node to swap
 * @param b Second interval node to swa[]
 * @return none
 */
template <class T>
void intervalTree<T>::swap(iNode<T> *a, iNode<T>* b)
  {
   if (a->parent == NIL)
     {
      root = b;
     }

   else if (a == a->parent->left)
     {
      a->parent->left = b;
     }

   else
     {
      a->parent->right = b;
     }

   b->parent = a->parent;
  }

/**
 * @brief Rotates the tree left around the specified x node
 * @details This function will rotate the tree around the interval node x,
 *          does not change the order of the elements.
 *       
 * @pre should only be used by fix functions
 * @param x The pivot node for which to rotate on
 * @return none
 */
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

/**
 * @brief Rotates the tree right around the specified x node
 * @details This function will rotate the tree around the interval node x,
 *          does not change the order of the elements.
 *       
 * @pre should only be used by fix functions
 * @param x The pivot node for which to rotate on
 * @return none
 */
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

/**
 * @brief Maintains the Red/Black properties of the tree after an insertion.
 * @details This function uses five different cases to maintain the Red/black
 *          tree properties; Property 3 - All leaves must be black, Property 4
 *          - Both children of a red node are always black, and Property 5 -
 *          All paths from a node to its leaf contain the same number of black nodes
 * 
 * @pre Should only be called after the insertion operation is preformed on the
 *      tree.
 * @post The tree will be fixed, and its RB properties maintained.
 * @param fix The pointer to the subtree / tree to fix
 * @return none
 */
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

/**
 * @brief Finds the largest max values in a portion of the tree
 * @details This function finds the highest max value from a given subtree,
 *          it is used to maintain the max data values throughout insertion /
 *          deletion operations. It works by simply looping through the tree
 *          comparing max values at each stop.
 * 
 * @param x The interval node to find the max value in.
 * @return none
 */
template <class T>
void intervalTree<T>::fixMaxValues(iNode<T> *x)
  {
   while( x != NIL )
     {
      x->max = findMax( x->i.high, findMax(x->left->max, x->right->max));
      x = x->parent;
     }
  }

/**
 * @brief Determines if two intervals (a) and (b) overlap in some form
 * @details Does a simple comparison to determine if two intervals overlap
 *          used to find overlapping intervals in the overlapHelper
 * 
 * @param a First interval for overlap testing
 * @param b Second interval for overlap testing
 * 
 * @return Boolean result of the test, states if they are overlapping or not.
 */
template <class T>
bool intervalTree<T>::overlaps(interval a, interval b)
  {
   if ((b.high >= a.low) && (a.high >= b.low))
     {
      return true;
     }

   else
     {
      return false;
     }
  }

/**
 * @brief Finds all the overlaps in a given subtree (root)
 * @details This function recursively goes through the tree finding overlapping
 *          intervals, once one is found it is printed using the interval
 *          class methods.
 * 
 * @param sub The subtree to search for overlaps in (initially should be root)
 * @param target The target interval to search for overlap conflicts with
 * 
 * @return none
 */
template <class T>
void intervalTree<T>::findOverlap(iNode<T> *sub, const interval& target)
  {
   // variables
   bool overlapFound = false;

   // recursively loop until subtree is NIL
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

/**
 * @brief Recursive helper for the findOverlap function, just uses the root node
 *        as the subtree to find all overlap conflicts
 * 
 * @param target The interval to search for overlaps in the tree with
 * @return none
 */
template <class T>
void intervalTree<T>::overlapHelper(const interval& target)
  {
   findOverlap(root, target);
  }

/**
 * @brief Preorder print function for the interval tree
 * 
 * @param sub The subtree to print
 * @return none
 */
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

/**
 * @brief inorder print function for interval tree
 * 
 * @param sub The subtree to print
 * @return none
 */
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

/**
 * @brief Prints the tree inorder then in preorder.
 * @details calls both functions on the root node to display entire tree, also
 *          prints the current interval at the root node and color of each node.   
 * 
 * @pre the tree should not be empty
 * @post the interval tree contents will be displayed to the user in both
 *       orders.
 * 
 * @return none
 */   
template <class T>
void intervalTree<T>::showTree()
  {
   cout << endl << endl;
   cout << "CURRENT ROOT DATA: " << '[' << root->i.low << ", ";
   cout << root->i.high << ']' << "  MAX : " << root->max;
   cout << "   Color : ";

      if (root->color == RED)
        {
         cout << "RED" << endl;
        }

      else
        {
         cout << "BLACK" << endl;
        }

   cout << endl << endl;
   cout << "INORDER PRINT FOR INTERVAL TREE : " << endl;

   inorderPrint(root);
   cout << endl << endl;

   cout << "PREORDER PRINT FOR INTERVAL TREE : " << endl;
   preorder(root);
   cout << endl << endl;
  }



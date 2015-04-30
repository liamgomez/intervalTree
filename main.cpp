// Header Files ////////////////////////////////////////////////////////////////
#include "intervalTree.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


// Global Constants ////////////////////////////////////////////////////////////



// Function Spec ///////////////////////////////////////////////////////////////

   // todo list
      // make delete and delete fixxer functions
      // get user input
      // comment code
      // get the node depth stats for preorder printng.
      // make some data members of classes private mainly node.

// Main Program ////////////////////////////////////////////////////////////////
int main()
  {
   // variables
      intervalTree<interval> testing;

      // testing
      interval i1, i2, i3;
      ifstream fin;
      int fLow, fHigh;
      bool done = false;
      char response;

      fin.open("intervalFile.txt");


   fin >> fLow >> fHigh;
   for (int i = 1; fin.good(); ++i)
     {
      i1.low = fLow;
      i1.high = fHigh;
      testing.insert(i1);
      fin >> fLow >> fHigh;
     }

   do
     {
      testing.showTree();

      cout << "Please enter the low value of an interval to find overlaps : ";
      cin >> i1.low;

      cout << endl << "Enter high value of interval to find overlaps : ";
      cin >> i1.high;

      testing.overlapHelper(i1);

      cout << endl << "Enter low value of interval you wish to remove : ";
      cin >> i1.low;
      cout << endl << "Enter high value of interval you wish to remove : ";
      cin >> i1.high;

      // search for interval
      iNode<interval> *search = testing.searchHelper(i1);

      // check if the innterval was found

      if (search != nullptr)
        {
         testing.deleteInterval(search);
         testing.showTree();
        }

      else
        {
         cout << endl << "The interval could not be found, select an interval";
         cout << "From the current content of the tree..." << endl << endl;
        }

      cout << "Continue demo??? (y/n) : ";
      cin >> response;

      if (response == 'n' || response == 'N')
        {
         // end demo
         done = true;  
        }
     }
     while(!done);

   // end demo
   return 0;
  }

// Header Files ////////////////////////////////////////////////////////////////
#include "intervalTree.h"
#include <iostream>
#include <fstream>

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
   intervalTree<interval> testing;

      // testing
      interval i1, i2, i3;
      ifstream fin;

      fin.open("intervalFile.txt");
      int fLow, fHigh;

      fin >> fLow >> fHigh;
      for (int i = 1; fin.good(); ++i)
        {
         i1.low = fLow;
         i1.high = fHigh;
         testing.insert(i1);
         fin >> fLow >> fHigh;
        }

   i1.low = 16;
   i1.high = 20;

   testing.overlapHelper(i1);
   testing.showTree();
   return 0;
  }


// Function Imp ////////////////////////////////////////////////////////////////




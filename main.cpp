/**
 * @file intervalTree.cpp
 * @author Liam Gomez
 * @brief Implementation file for interval tree and all other necassary classes
 * 
 * @mainpage 
 * This program contains a total of three classes
 *    -- An interval class with low and high data members
 *    -- An interval Node class with left, right, and parent node pointers.
 *       Aswell as max and color data members.
 *    -- Interval Tree class for the tree implementation, containing a root
 *       and NIL node pointers.
 *       
 * Everything is tested and appears to be working properly, main program runs
 * through the various operations that can be preformed on the tree.
 * 
 * Comments / Questions : liamgomez@nevada.unr.edu
 * 
 * @date 4/29/2015
 */

// Header Files ////////////////////////////////////////////////////////////////
#include "intervalTree.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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

   // load the intervals from file
   fin.open("intervalFile.txt");
   fin >> fLow >> fHigh;

   // loop till end of file
   for (int i = 1; fin.good(); ++i)
     {
      i1.low = fLow;
      i1.high = fHigh;
      testing.insert(i1);
      fin >> fLow >> fHigh;
     }

   // testing loop
   do
     {
      // print the tree
      testing.showTree();

      cout << "Please enter the low value of an interval to find overlaps : ";
      cin >> i1.low;

      cout << endl << "Enter high value of interval to find overlaps : ";
      cin >> i1.high;

      // test for overlap
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

      // not found, interval does not exist
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

#include <iostream>
#include <fstream>
#include <chrono>
#include <cstring>
#include "sorting.h"
using namespace std;


/***********************************************************
Main progam template build a Max Heap and Heap Sort in place
************************************************************/

static int OpCount = 0;

int localSize = 100;

void Heapify(int *Heap, int n);
void CheckHeapOrder(int *Heap,int n);
int  DeleteHeap(int index,int *Heap,int ntemp);
void InsertHeap(int value ,int *Heap, int ntemp);
void HeapSort(int *Heap, int n);
 
int main(int argc, char *argv[]) 
{
  /* Timeing and IO setup */
  chrono::time_point<chrono::steady_clock> start, stop; 
  chrono::duration<double> difference_in_time;
  double difference_in_seconds_heap_sort;  // Holds the final run time
  ifstream infile;
  ofstream outfile;
  
  /* Local data */
  int n;
 
  
  // Input Data:  DO NOT CHANGE 
  infile.open(argv[1]);

  int* Heap = NULL;
  
  if(!infile)
    {
      cout << "No input files so genearated internal random file of size = "<<  localSize  << endl;
      n = localSize;
      cout << "n  = " << n << endl;
      Heap = new int[n+1];
      Heap[0] = n;  // Set zero entry in in heap to n
      for(int i=1; i<n+1 ; i++)  Heap[i] = rand()%100000;
      // Define out put file
       outfile.open("SortedRandomList.dat");
    }
  else
    {
      infile >> n;
      Heap = new int[n+1];
      Heap[0] = n;  // Set zero entry in in heap to n	    
      for(int i=1; i<n+1 ; i++) infile >> Heap[i];
      infile.close();
      // Define out put file 
      outfile.open(strcat(argv[1],"_HeapSorted.dat"));
    }
  
  

   /* Set up Heat */
   
   Heapify(Heap, n);

   CheckHeapOrder(Heap,n);
   
     /* Practive delete and insert */
 
   int ntemp = n;
   int index = 0;
   int value = -1;

   /* 
 You might want to do a Merge Sort and compare to make sure you haven't lost anything and compare with the Merge Sort after Insert/Delete test!
 */
   
# if 1   
   // Try Deleting 5 
   for(int k = 0; k <5; k++)
     {
       index = rand()%n + 1;
       value = DeleteHeap(index,Heap,ntemp); // Also reHeapfies 
       Heap[ntemp] = value;
       ntemp--;
     }
   //Reinsert 5       
   for(int k = 0; k <5; k++)
     {
       value = Heap[ntemp +1];
       InsertHeap(value,Heap, ntemp); // Also reHeapfies
       ntemp++;
     }
      CheckHeapOrder(Heap,n); // Is it still a  Heap?
#endif
      
/* 
 Test and compare  efficiency of Heap Sort  relative to Merge Sort
 */
      
  int* a_tmp = new int[n+1];      
      
  OpCount = 0;
  start = chrono::steady_clock::now();
   mergeSort(Heap, a_tmp,  1, n);
   // REPLACE WITH HEAP SORT //
   // HeapSort(Heap, n);
  stop = chrono::steady_clock::now();
  difference_in_time = stop - start;
  difference_in_seconds_heap_sort = double(difference_in_time.count());
  
  //Begin output file : DO NOT CHANGE  
 
  outfile << Heap[0] << endl;
  for(int i=1; i< Heap[0]+1  ; i++)
    outfile << Heap[i] << endl;

  //End output file

  return 0;
}



/****************************************
Provide funtions below
****************************************/

void Heapify(int *Heap, int n) {
    // Starting from the last parent node and moving up the tree
    for(int i = n/2 - 1; i >= 0; i--) {
        int parent = i;
        // Bubble down the current node until it satisfies the max-heap property
        while(parent*2 + 1 < n) {
            int left_child = parent*2 + 1;
            int right_child = parent*2 + 2;
            int max_child = left_child;
            // Determine which child is larger
            if(right_child < n && Heap[right_child] > Heap[left_child]) {
                max_child = right_child;
            }
            // If the current node is smaller than the larger child, swap them and continue bubbling down
            if(Heap[parent] < Heap[max_child]) {
                std::swap(Heap[parent], Heap[max_child]);
                parent = max_child;
            } else {
                break;
            }
        }
    }
}


void CheckHeapOrder(int *Heap, int n) {
    // Starting from the root of the heap
    for(int i = 0; i < n; i++) {
        int left_child = 2*i + 1;
        int right_child = 2*i + 2;
        // If the left child is within the bounds of the heap and is greater than the parent, return false
        if(left_child < n && Heap[left_child] > Heap[i]) {
            std::cout << "Heap order violated at index " << i << std::endl;
            return;
        }
        // If the right child is within the bounds of the heap and is greater than the parent, return false
        if(right_child < n && Heap[right_child] > Heap[i]) {
            std::cout << "Heap order violated at index " << i << std::endl;
            return;
        }
    }
    std::cout << "Heap order is valid" << std::endl;
}


int DeleteHeap(int index, int *Heap, int ntemp) {
    if(index < 0 || index >= ntemp) {
        // Index is out of bounds, return -1 to indicate an error
        return -1;
    }
    int save = Heap[index];
    // Replace the deleted key with the last element in the heap
    Heap[index] = Heap[ntemp-1];
    ntemp--;
    // Bubble down the new element to restore the max-heap property
    int parent = index;
    while(parent*2 + 1 < ntemp) {
        int left_child = parent*2 + 1;
        int right_child = parent*2 + 2;
        int max_child = left_child;
        // Determine which child is larger
        if(right_child < ntemp && Heap[right_child] > Heap[left_child]) {
            max_child = right_child;
        }
        // If the current node is smaller than the larger child, swap them and continue bubbling down
        if(Heap[parent] < Heap[max_child]) {
            std::swap(Heap[parent], Heap[max_child]);
            parent = max_child;
        } else {
            break;
        }
    }
    return save;
}

void InsertHeap(int value, int *Heap, int ntemp) {
    // Add the new element to the end of the heap
    Heap[ntemp] = value;
    ntemp++;
    // Bubble up the new element to restore the max-heap property
    int child = ntemp - 1;
    while(child > 0) {
        int parent = (child - 1) / 2;
        // If the current node is larger than its parent, swap them and continue bubbling up
        if(Heap[child] > Heap[parent]) {
            std::swap(Heap[child], Heap[parent]);
            child = parent;
        } else {
            break;
        }
    }
}

void HeapSort(int *Heap, int n) {
    // Build a max-heap from the array
    for(int i = n/2 - 1; i >= 0; i--) {
        // Perform heapify on each non-leaf node
        int parent = i;
        while(parent*2 + 1 < n) {
            int left_child = parent*2 + 1;
            int right_child = parent*2 + 2;
            int max_child = left_child;
            // Determine which child is larger
            if(right_child < n && Heap[right_child] > Heap[left_child]) {
                max_child = right_child;
            }
            // If the current node is smaller than the larger child, swap them and continue bubbling down
            if(Heap[parent] < Heap[max_child]) {
                std::swap(Heap[parent], Heap[max_child]);
                parent = max_child;
            } else {
                break;
            }
        }
    }
    // Sort the array by repeatedly extracting the max element from the heap
    for(int i = n-1; i >= 0; i--) {
        std::swap(Heap[0], Heap[i]);
        int parent = 0;
        // Bubble down the new root to restore the max-heap property
        while(parent*2 + 1 < i) {
            int left_child = parent*2 + 1;
            int right_child = parent*2 + 2;
            int max_child = left_child;
            // Determine which child is larger
            if(right_child < i && Heap[right_child] > Heap[left_child]) {
                max_child = right_child;
            }
            // If the current node is smaller than the larger child, swap them and continue bubbling down
            if(Heap[parent] < Heap[max_child]) {
                std::swap(Heap[parent], Heap[max_child]);
                parent = max_child;
            } else {
                break;
            }
        }
    }
    // Print the sorted array
    std::cout << "Sorted array: ";
    for(int i = 0; i < n; i++) {
        std::cout << Heap[i] << " ";
    }
    std::cout << std::endl;
}

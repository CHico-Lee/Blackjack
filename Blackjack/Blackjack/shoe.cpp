/* FileName: shoe.cpp
 * Author: Chi Ho Lee
 * Project: DS Final Project
 * Date: 12/18/2015
 * Description: This file represent the card holder on the game table.
 */

#include <iostream>
#include <ctime>	// for seed the random number generator 
#include <stdlib.h>	// for random
using namespace std;
#include "shoe.h"

// Constructor
template <class T>
Shoe<T>::Shoe()
{
	frontP = NULL;
	lastP = NULL;
	numCards = 0;
}

// Insert a new card with a Node at the end of the linked list.
template <class T>
void Shoe<T>::addCard(T *newCard)
{
	Deck *newCardP = new Deck();
	newCardP->playingCardP = newCard;
	newCardP->linkP = NULL;

	if (frontP == NULL)
	{
		frontP = newCardP;
	}
	else 
	{
		lastP->linkP = newCardP;
	}

	lastP = newCardP;
	// increment cards counter
	numCards++;
}

// Remove a Node from the beginning of the linked list 
// and return the card object.
template <class T>
T* Shoe<T>::draw()
{
	// No more cards.
	if (frontP == NULL)
	{
		cout << "Error: No card in shoe" << endl;
		return NULL;
	}

	T *drawCardP;
	Deck *drawP = frontP;
	frontP = drawP->linkP;
	drawCardP = drawP->playingCardP;
	
	// delete the node
	delete drawP;
	
	// update cards counter
	numCards--;

	return drawCardP;
}

// This shuffling generate random numbers on a dynamic array.
// The array will perfroms a merge sort and both linked list 
// and array move simultaneously.
template <class T>
void Shoe<T>::sortShuffling()
{
	// Create an parallel array
	int *parallelArray = new int[numCards];
	
	// Seed the random number generator 
	srand(time(0));

	// Assign random number into the array
	for (int i = 0; i < numCards; i++)
	{
		parallelArray[i] = (int)((rand() / (RAND_MAX+1.0)) * numCards);
	}

	// Start merge Sort
	mergeSorting(parallelArray, 0, numCards-1, &frontP);

}

// Helper function to cut a linked list in half.
template <class T>
void Shoe<T>::separateList(Deck* headP, Deck **LeftP, Deck **RightP)
{
	// Finding mid point on linked list
	Deck *midP = headP;
	Deck *endP = headP->linkP;

	while(endP != NULL)
	{
		endP = endP->linkP;
		if (endP != NULL)
		{
			endP = endP->linkP;
			midP = midP->linkP;
		}
	}

	// Cut the linked list in half and disconnent linkP set to null 
	*LeftP = headP;
	*RightP = midP->linkP;
	midP->linkP = NULL;
}

// Helper function to add Node form one list to the other list.
template <class T>
void Shoe<T>::addSortNode(Deck **firstP, Deck **lastP , Deck **addP)
{
	if (*firstP == NULL)
	{
		*firstP = *addP;
		*lastP = *addP;
	}
	else
	{
		(*lastP)->linkP = *addP;
		*lastP = (*lastP)->linkP;
	}
	*addP = (*addP)->linkP;
}

// Recursive function for mergeSorting
template <class T>
void Shoe<T>::mergeSorting(int arr[], int startIndex, int endIndex, struct Deck **ListP)
{
	Deck *headP = *ListP;
	Deck *LeftP = NULL;
	Deck *RightP = NULL;
		
	// Base Case
	if (headP->linkP == NULL || startIndex == endIndex)
		return;

	// Finding mid point on array
	int midIndex = (endIndex + startIndex) / 2;

	// Finding mid point on linked list
	separateList(headP, &LeftP, &RightP);

	// Compute start and end of each sublist.
	int start1 = startIndex;
	int end1 = midIndex;
	int start2 = midIndex + 1;
	int end2 = endIndex;

	// Recursion
	mergeSorting(arr, start1, end1, &LeftP);
	mergeSorting(arr, start2, end2, &RightP);
		
	// Merging
	// ALLOCATE ARRAY INTO WHICH TO COPY SORTED ELEMENTS.
	int size = endIndex - startIndex + 1;
	int *merged = new int[size];

	// Allocate Linked list pointer
	Deck *newfirstP = NULL;
	Deck *newlastP = NULL;
	int i = 0;

	// COMPARE ELEMENTS, COPYING SMALLER (UNTIL ONE
	// SUBLIST EXHAUSTED).
	while (start1 <= end1 && start2 <= end2)
	{
		// ADD LOWER VALUE INTO merged, 
		// linked list add value follow the array 
		if (arr[start1] < arr[start2])
		{
			// Add lower value to array
			merged[i] = arr[start1];
			start1++;
				
			// Add lower value to linked list
			addSortNode(&newfirstP, &newlastP, &LeftP);
		}
		else
		{
			// Add lower value to array
			merged[i] = arr[start2];
			start2++;

			// Add lower value to linked list
			addSortNode(&newfirstP, &newlastP, &RightP);
		}

		i++;
	}

	// COMPARE ANY ELEMENTS REMAINING ON ONE OF
	// THE SUBLISTS.
	while (start1 <= end1)
	{
		merged[i] = arr[start1];
		start1++;

		// Add existing linked list.
		addSortNode(&newfirstP, &newlastP, &LeftP);
		i++;
	}

	while (start2 <= end2)
	{
		merged[i] = arr[start2];
		start2++;

		// Add existing linked list.
		addSortNode(&newfirstP, &newlastP, &RightP);
		i++;
	}

	// COPY SORTED ELEMENTS BACK INTO ORIGINAL ARRAY.
	for (int j = 0; j < size; j++)
		arr[startIndex + j] = merged[j];

	delete [] merged;  // deallocate copy

	// Update pointers
	*ListP = newfirstP;
	lastP = newlastP;
}

// A common shuffling technique called the riffle or dovetail shuffle
// Linked list will be separate in half and put back cards to the list
// by taking one or two cards from each list.
template <class T>
void Shoe<T>::riffleShuffling()
{
	Deck *firstHalfP;
	Deck *secondHalfP;

	// Cut Deck in Half
	separateList(frontP, &firstHalfP, &secondHalfP);

	frontP = lastP = NULL;
	// Seed the random number generator 
	srand(time(0));

	while(true)
	{
		// Random number between 1 and 2.
		int randRiffle = rand() % 2 + 1;

		for (int i = 0; i < randRiffle; i++)
		{
			if (firstHalfP != NULL)
			{
				addSortNode(&frontP, &lastP, &firstHalfP);
			}
		}
		 
		// Random number between 1 and 2.
		randRiffle = rand() % 2 + 1;
		for (int i = 0; i < randRiffle; i++)
		{
			if (secondHalfP != NULL)
			{
				addSortNode(&frontP, &lastP, &secondHalfP);
			}
		}
		// All cards sorted.
		if (firstHalfP == NULL && secondHalfP == NULL)
			break;
	} 
}

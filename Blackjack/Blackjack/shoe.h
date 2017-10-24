/* FileName: shoe.h
 * Author: Chi Ho Lee
 * Project: DS Final Project
 * Date: 12/18/2015
 * Description: This file represent the card holder on the game table.
 */

#ifndef SHOE_H_INCLUDED
#define SHOE_H_INCLUDED

#include "card.h"

template <class T>
class Shoe
{
private:
	// Inner class for Deck Node.
	struct Deck
	{
		T *playingCardP;
		Deck *linkP;
	};

	Deck *frontP;
	Deck *lastP;

	int numCards;

	// Helper function to cut a linked list in half.
	void separateList(Deck *headP, Deck **LeftP, Deck **RightP);

	// Helper function to add Node form one list to the other list.
	void addSortNode(Deck **firstP, Deck **lastP , Deck **addP);

	// Recursive function for mergeSorting
	void mergeSorting(int arr[], int startIndex, int endIndex, struct Deck **ListP);

public:
	// Constructor
	Shoe();

	// Insert a new card with a Node at the end of the linked list.
	void addCard(T *newCard);

	// Remove a Node from the beginning of the linked list 
	// and return the card object.
	T* draw();

	// This shuffling generate random numbers on a dynamic array.
	// The array will perfroms a merge sort and both linked list 
	// and array move simultaneously.
	void sortShuffling();

	// A common shuffling technique called the riffle or dovetail shuffle
	// Linked list will be separate in half and put back cards to the list
	// by taking one or two cards from each list.
	void riffleShuffling();

};

#endif
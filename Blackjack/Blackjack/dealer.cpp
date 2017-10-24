/* FileName: dealer.cpp
 * Author: Chi Ho Lee
 * Project: DS Final Project
 * Date: 12/18/2015
 * Description: This file represent the dealer's record
 *				and status in the game.
 */

#include <iostream>
#include "dealer.h"
using namespace std;

// Constructor
Dealer::Dealer():Player("", 0)
{
}

Dealer::Dealer(int initTokens):Player("Dealer", initTokens)
{
}


// Accessor Methods
// Return the rank of the first card
string Dealer::showFirstCard() const
{
	return handcards[0]->getRank();	 
}
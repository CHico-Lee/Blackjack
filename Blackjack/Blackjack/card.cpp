/* FileName: card.cpp
 * Author: Chi Ho Lee
 * Project: DS Final Project
 * Date: 12/18/2015
 * Description: This file represent the poker cards in blackjack.
 */

#include "card.h"

// Constructor
Card::Card(string initRank, int initValue)
{
	rank = initRank;
	value = initValue;
}

// Accessor Methods
string Card::getRank() const
{
	return rank;
}

int Card::getValue() const
{
	return value;
}
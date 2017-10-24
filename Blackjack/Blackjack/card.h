/* FileName: card.h
 * Author: Chi Ho Lee
 * Project: DS Final Project
 * Date: 12/18/2015
 * Description: This file represent the poker cards in blackjack.
 */

#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

#include <iostream>
using namespace std;

class Card
{
private:
	string rank;
	int value;

public:
	// Constructor
	Card(string initRank, int initValue);

	// Accessor Methods
	string getRank() const;

	int getValue() const;

};

#endif
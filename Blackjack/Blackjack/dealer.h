/* FileName: dealer.h
 * Author: Chi Ho Lee
 * Project: DS Final Project
 * Date: 12/18/2015
 * Description: This file represent the dealer's record
 *				and status in the game.
 */

#ifndef DEALER_H_INCLUDED
#define DEALER_H_INCLUDED

#include "player.h"

class Dealer : public Player
{
public:
	// Constructor
	Dealer();

	Dealer(int initTokens);

	// Accessor Methods
	// Return the rank of the first card
	string showFirstCard() const;
};

#endif
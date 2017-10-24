/* FileName: player.h
 * Author: Chi Ho Lee
 * Project: DS Final Project
 * Date: 12/18/2015
 * Description: This file represent the players' records 
 *				and status in the game.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "card.h"

class Player
{
protected:

	string name;
	int tokens;

	// Vector array use to store Card objects
	vector<Card*> handcards;

public:
	// Constructor
	Player();

	Player(string initName, int initTokens);

	// Accessor Methods
	string getName() const;

	int getTokens() const;

	// Return the vector list of cards.
	const vector<Card*>& getCards();

	// Update tokens,
	// increase by positive and decerease by negative parameters.
	void tokensChange(int changeTok);

	// Receive new card and store in vector array.
	void dealtCard(Card *newCard);

	// Return card and remove card from the array.
	Card* returnCard();

	// Read in name and tokens form file through stream.
	void readFile(ifstream *playerfile);

	// Write out name and tokens to file through stream.
	void writeFile(ofstream *playerfile);
};

#endif
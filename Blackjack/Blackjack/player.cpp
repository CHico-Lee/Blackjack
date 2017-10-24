/* FileName: player.cpp
 * Author: Chi Ho Lee
 * Project: DS Final Project
 * Date: 12/18/2015
 * Description: This file represent the players' records 
 *				and status in the game.
 */

#include <iostream>
#include <sstream>	// for combine string
#include <fstream>	// for file IO
#include "player.h"
using namespace std;

// Constructor
Player::Player()
{
	name = "";
	tokens = 0;
}

Player::Player(string initName, int initTokens)
{
	name = initName;
	tokens = initTokens;
}

// Accessor Methods
string Player::getName() const
{
	return name;
}

int Player::getTokens() const
{
	return tokens;
}

// Return the vector list of cards.
const vector<Card*>& Player::getCards() 
{
	return handcards;
}

// Update tokens,
// increase by positive and decerease by negative parameters.
void Player::tokensChange(int changeTok)
{
	tokens += changeTok;
}

// Receive new card and store in vector array.
void Player::dealtCard(Card *newCard)
{
	if (newCard == NULL)
		return;
	handcards.push_back(newCard);
}

// Return card and remove card from the array.
Card* Player::returnCard()
{
	if (handcards.empty())
		return NULL;
	Card *putBack = handcards.back();
	handcards.pop_back();
	
	return putBack;
}

// Read in name and tokens form file through stream.
void Player::readFile(ifstream *playerfile)
{
	string line;
	if ( getline (*playerfile,line) )
	{
		name = line;
	}
	
	if ( getline (*playerfile,line) )
	{
		// Converting string to int.
		stringstream ss(line);
		ss >> tokens;
	}
}

// Write out name and tokens to file through stream.
void Player::writeFile(ofstream *playerfile)
{
	*playerfile << endl;
	*playerfile << name;
	*playerfile << endl;
	*playerfile << tokens;
}
/* FileName: main.cpp
 * Author: Chi Ho Lee
 * Project: DS Final Project
 * Date: 12/18/2015
 * Description: This program is simulate a gambling card game "Blackjack".
 *				Players can access their records by their name. 
 *				Players will be able to set bets and make decision to 
 *				hit, stand, double down, and surrender during the game.
 *				Player data will be stored before the program exit.
 */

#include <iostream>
#include <fstream>	// for file IO
#include <sstream>	// for combine string
#include <vector>
#include <algorithm>  // for STL template functions (sort)
using namespace std;

#include "player.h"
#include "card.h"
#include "dealer.h"
#include "shoe.cpp"		//why CPP work??

const int NEW_PLAYER_TOKENS = 1000;
const int DEALER_TOKENS = 9999999;
const int NUM_DECKS = 6;
const double BLACKJACK_PAY_RATE = 2.5;

// Create card objects and insert cards into the shoe.
void setDeck(Shoe<Card> *cardShoe)
{
	for (int j = 1; j <= 4 * NUM_DECKS; j++)		// 4 suits
	{
		for (int i = 1; i <= 13 * NUM_DECKS; i++)	// 13 ranks
		{
			// Use for convert int to string.
			ostringstream ss;

			switch(i)
			{
			case 1:
				cardShoe->addCard(new Card("A", i));
				break;
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				ss << i;
				cardShoe->addCard(new Card(ss.str(), i));
				break;
			case 11:
				cardShoe->addCard(new Card("J", 10));
				break;
			case 12:
				cardShoe->addCard(new Card("Q", 10));
				break;
			case 13:
				cardShoe->addCard(new Card("K", 10));
				break;
			}
		}
	}
}

// Add up player's card values and return the higher total soft value
int pointSum(Player player)
{
	int sum = 0;
	//vector<int> points = player.getCardPoints();
	int ace = 0;
	for (int i = 0; i < player.getCards().size(); i++ )
	{
		if (player.getCards()[i]->getValue() == 1)
			ace += 1;
		sum += player.getCards()[i]->getValue();
	}
	if (ace > 0 && sum <= 11)
	{
		sum = sum + 10;
	}
	return sum;
}

// Return the string contain player's cards' rank.
string showCard(Player player)
{
	string holdCards = "";
	for (int i = 0; i < player.getCards().size(); i++ )
		holdCards += player.getCards()[i]->getRank() + " ";
	return holdCards;
}

// Return true if player get an 'A' and a face card.
bool blackjack(Player players)
{
	string card1 = players.getCards()[0]->getRank();
	string card2 = players.getCards()[1]->getRank();
	if (card1 == "A" && (card2 == "J" || card2 == "Q" || card2 == "K") ||
		card2 == "A" && (card1 == "J" || card1 == "Q" || card1 == "K"))
		return true;
	return false;
}

// Match name from array of players using binary search, 
// retrun the index number if found.
// return -1 if name not found.
int binarySearch(vector<Player> players, string name)
{
	int start = 0;
	int end = players.size() - 1;
	int mid;
	while (end >= start)
	{
		mid = (start + end) / 2;
		int matching = players[mid].getName().compare(name);
		if (matching == 0)	// equal
			return mid;
		else if (matching < 0)
			start = mid + 1;
		else if (matching > 0)
			end = mid - 1;
	}
	return -1;
}

// Comparing plyers' name, for STL sorting.
bool compareName(const Player &p1, const Player &p2)
{
	if (p1.getName().compare(p2.getName()) < 0)
	return true;
}

// Read player's record and store into the vector array.
void loadGameFile(vector<Player> *players)
{
	// Reading player data from file
	ifstream playerfile ("player.txt");
	if (playerfile.is_open())
	{
		string emptyLine;
		getline(playerfile, emptyLine);
		while (!playerfile.eof( ))
		{
			Player newplayer;
			// Call Player class method to read data.
			newplayer.readFile(&playerfile);
			players->push_back(newplayer);
		}
		playerfile.close();
	}
	else 
		cout << "Error: Unable to find records"; 
}

// Save player's record to file from vector array.
void saveGameFile(vector<Player> players)
{
	sort (players.begin(), players.end(), compareName);
	ofstream playerfile ("player.txt");
	if (playerfile.is_open())
	{
		for (int i = 0; i < players.size(); i++)
		{
			// Call player class method to write data.
			players[i].writeFile(&playerfile);
		}
		playerfile.close();
	}
	else
	{
		cout << "Unable to open file";
	}
}

int main()
{
	Shoe<Card> tableShoe;
	Dealer dealer(DEALER_TOKENS);
	vector<Player> players;

	bool blackjacked = false;
	
	// Read player's record and store into the vector array.
	loadGameFile(&players);

	string name;
	cout << "Please enter your name: ";
	getline(cin, name);

	// Sort player's record by name in ascending order.
	sort (players.begin(), players.end(), compareName);

	// Find play name in records. 
	int currPlayer = binarySearch(players, name);

	cout << "Welcome to BlackJack!" << endl;

	// New player and update index number.
	if (currPlayer == -1)
	{
		Player newplayer(name, NEW_PLAYER_TOKENS);
		players.push_back(newplayer);
		currPlayer = players.size() - 1;
		cout << "Enjoy you free $" << NEW_PLAYER_TOKENS << " tokens.\n";
	}

	// Put cards into the card shoe.
	setDeck(&tableShoe);
	
	// Shuffling the cards.
	cout << "Shuffling the cards////\\\\\\\\" << endl;
	tableShoe.riffleShuffling();
	tableShoe.sortShuffling();
	tableShoe.riffleShuffling();

	int cardUsed = 0;

	// Game start
	while(true)
	{
		if (NUM_DECKS*52/2 < cardUsed)
		{
			// Shuffling the cards.
			cout << "Shuffling the cards////\\\\\\\\" << endl;
			tableShoe.riffleShuffling();
			tableShoe.sortShuffling();
			tableShoe.riffleShuffling();
			cardUsed = 0;
		}
		
		bool surrender = false;
		int playerPoints = 0;
		int bet;
		cout << "Please place your bet: (Max: "<< 
			players[currPlayer].getTokens() << ", 0 to quit):\n";
		cin >> bet;
		
		// Check for incorrrect input, ask user to re-enter. 
		while(cin.fail() || bet < 0 || bet > players[currPlayer].getTokens())
		{
			cout << "Please enter a correct bets: (Max: " << 
				players[currPlayer].getTokens() << ")(no cents):\n";
			cin.clear();
			cin.ignore();
			cin >> bet;
		}

		// Quit Game
		if (bet == 0)
		{
			cout << "Saving Record and Exiting Game.";
			saveGameFile(players);
			return 0;
		}

		// Subtract bet from player's account.
		players[currPlayer].tokensChange(-bet);

		// Dealting Cards.
		players[currPlayer].dealtCard(tableShoe.draw());
		dealer.dealtCard(tableShoe.draw());
		players[currPlayer].dealtCard(tableShoe.draw());
		dealer.dealtCard(tableShoe.draw());

		// Display Cards and points.
		cout << "Dealer shows: " << dealer.showFirstCard() << endl;

		cout << "Your Cards: " << showCard(players[currPlayer]) << endl;
		playerPoints = pointSum(players[currPlayer]);
		cout << "Your Points: " << playerPoints << endl;

		// Check for Blackjack.
		blackjacked = blackjack(players[currPlayer]);
		if (blackjacked)
			cout << "BlackJack!" << endl;

		// Player decision loop
		char decision = 'A';
		while (decision != 'S' && decision != 'D' && !blackjacked)
		{
			cout << endl;
			cout << "H)it - Take another card.\n";
			cout << "S)tand - Take NO more card.\n";
			if (players[currPlayer].getCards().size() == 2)
			{
				cout << "D)ouble down - Increase your bet by $" << 
					bet << " and take another card.\n";
				cout << "R)Surrender - Return half of your bet.\n";
			}
			cout << ("Q)uit - You will loss your bet.\n");
			cout << "Please make a decisions:\n";
			string userDec;
			cin >> userDec;
			//getline(cin, userDec);

			// Convert String to an Uppcase char.
			decision = toupper(userDec[0]);
			cout << endl;

			switch(decision)
			{
			case 'H':
				cout << "You Hit.\n";
				players[currPlayer].dealtCard(tableShoe.draw());
				break;

			case 'S':
				cout << "You Stand.\n";
				break;

			case 'D':
				if (players[currPlayer].getCards().size() != 2)
				{
					cout << "I don't understand your decisions.\n" << endl;
					decision = 'A';
					break;
				}
				cout << "You Double down.\n";
				players[currPlayer].dealtCard(tableShoe.draw());
				players[currPlayer].tokensChange(-bet);
				bet *= 2;
				break;

			case 'R':
				if (players[currPlayer].getCards().size() != 2)
				{
					cout << "I don't understand your decisions.\n" << endl;
					break;
				}
				cout << "You Surrender.\n";
				bet = bet / 2;
				players[currPlayer].tokensChange(bet);
				surrender = true;
				break;

			case 'Q':
				cout << "Saving Record and Exiting Game.\n";
				saveGameFile(players);
				return 0;
				break;

			default:
				cout << "I don't understand your decisions.\n" << endl;
				break;
			}

			if (surrender)
				break;

			// Display Cards and points.
			playerPoints = pointSum(players[currPlayer]);

			cout << "Your Cards: " << showCard(players[currPlayer]) << endl;;
			cout << "Your Points: " << playerPoints << endl;

			if (playerPoints > 21)
			{
				cout << "You Busted!" << endl;
					break;
			}
		}

		cout << endl;

		if (!surrender)
		{
			// Dealer shows cards after player finish their decisions.
			cout << "Dealer Cards:" << showCard(dealer) << endl;
			int dealerPoints = pointSum(dealer);

			// Dealer draws card if less than 17 points.
			while (dealerPoints < 17)
			{
				dealer.dealtCard(tableShoe.draw());
				cout << "Dealer hits" << endl;
				cout << "Dealer Cards:" << showCard(dealer) << endl;
				dealerPoints = pointSum(dealer);
			}
			cout << "Dealer Points: " << dealerPoints << endl;
			if (dealerPoints > 21)
				cout << "Dealer Busted" << endl;
			cout << endl;

			// Determine win and loss.
			if (playerPoints < dealerPoints && dealerPoints <= 21 || playerPoints > 21)
			{
				cout << "You Loss! You loss $" << bet << " in this game." << endl;
				dealer.tokensChange(bet);
			}
			else if (playerPoints > dealerPoints || dealerPoints > 21)
			{
				
				int winBet;
				if (blackjacked)
					 winBet = bet * BLACKJACK_PAY_RATE;
				else
					winBet = bet * 2;
				cout << "You Win! You earn $" << bet << " in this game." << endl;
				players[currPlayer].tokensChange(winBet);
				dealer.tokensChange(bet-winBet);
			}
			else
			{
				cout << "Draw! " << endl;
				players[currPlayer].tokensChange(bet);
			}
		}

		// Separation Line for End the game.
		cout << "------------------------------------------------------" << endl;

		// Return Players and Dealer cards to the shoe.
		Card *returnedCard = players[currPlayer].returnCard();
		while (returnedCard != NULL)
		{
			tableShoe.addCard(returnedCard);
			returnedCard = players[currPlayer].returnCard();
			cardUsed++;
		}

		returnedCard = dealer.returnCard();
		while (returnedCard != NULL)
		{
			tableShoe.addCard(returnedCard);
			returnedCard = dealer.returnCard();
			cardUsed++;
		}
	}
	return 0;
}
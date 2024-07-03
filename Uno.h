#ifndef UNO_H
#define UNO_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>

using namespace std;

class Card;
class NumberCard;
class ActionCard;
class Deck;
class Player;
class Game;


class Card
{
protected:
    string color;

public:
    Card(string c);
    virtual ~Card();
    void setColor(string c);
    virtual string getColor() const;
    virtual string toString() const = 0;
    virtual bool isPlayableOn(const Card &topCard) const = 0;
};

class NumberCard : public Card
{
private:
    int number;

public:
    NumberCard(string c, int n);
    int getNumber() const;
    string toString() const override;
    bool isPlayableOn(const Card &topCard) const override;
};

class ActionCard : public Card
{
private:
    string action;

public:
    ActionCard(const string &c, const string &a);
    string getAction() const;
    string toString() const override;
    bool isPlayableOn(const Card &topCard) const override;
};

class Deck
{
private:
    vector<Card *> cards;

public:
    Deck();
    ~Deck();
    void shuffle();
    Card *drawCard();
    void addCard(Card *card);
};

class Player
{
private:
    string name;
    vector<Card *> hand;

public:
    Player(string playerName);
    ~Player();
    const string &getName() const;
    void drawCard(Deck &deck);
    bool playCard(int index, Card *&topCard);
    vector<Card *> &getHand();
    bool hasEmptyHand() const;
};

class Game
{
private:
    Deck deck;
    vector<Player> players;
    int currentPlayerIndex;
    int currentDirection;
    Card *topCard;
    bool skipNextTurn;
    int cardsToDraw;

public:
    Game(const vector<string> &playerNames);
    ~Game();
    void startGame();
    void nextTurn();
    void moveToNextPlayer();
    bool isGameOver();

private:
};
int Uno();
#endif

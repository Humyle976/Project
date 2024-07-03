#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
using namespace std;

class Card
{
	protected:
	    string color;
	public:
	    Card(string c) : color(c)
		{
		}
	    virtual ~Card()
		{
		}
		void setColor(string c)
		{
			color=c;
		}
	    virtual string getColor() const
		{
			return color;
		}
	    virtual string toString() const=0;
	    virtual bool isPlayableOn(const Card& topCard) const=0;
};

class NumberCard : public Card
{
	private:
	    int number;
	public:
	    NumberCard(string c, int n) : Card(c), number(n)
		{
		}
	    int getNumber() const	
		{
			return number;
		}
	    string toString() const override
		{
	        return color+" "+to_string(number);
	    }
	    bool isPlayableOn(const Card& topCard) const override
		{
	        if(topCard.getColor()==color)
	            return true;
	        const NumberCard* topNumberCard=dynamic_cast<const NumberCard*>(&topCard);
	        if(topNumberCard!=nullptr && topNumberCard->getNumber()==number)
			{
	            return true;
	        }
	        return false;
	    }
};

class ActionCard : public Card
{
	private:
	    string action;
	public:
	    ActionCard(const string& c, const string& a) : Card(c), action(a)
		{
		}
	    string getAction() const
		{
			return action;
		}
	    string toString() const override
		{
	        return color+" "+action;
	    }
	    bool isPlayableOn(const Card& topCard) const override
		{
	        return topCard.getColor()==color || action=="Wild" || action=="Wild Draw Four";
	    }
};

class Deck
{
	private:
	    vector<Card*> cards;
	public:
	    Deck()
		{
	        vector<string> colors={"Red", "Green", "Blue", "Yellow"};
	        for(const auto& color : colors)
			{
	            for(int number=0;number<=9;number++)
				{
	                cards.push_back(new NumberCard(color, number));
	                if(number!=0)
						cards.push_back(new NumberCard(color, number));
	            }
	            for(int i = 0; i < 2; i++)
				{
	                cards.push_back(new ActionCard(color, "Skip"));
	                cards.push_back(new ActionCard(color, "Reverse"));
	                cards.push_back(new ActionCard(color, "Draw Two"));
	            }
	        }
	        for (int i = 0; i < 4; i++)
			{
	            cards.push_back(new ActionCard("None", "Wild"));
	            cards.push_back(new ActionCard("None", "Wild Draw Four"));
	        }
	        shuffle();
	    }
	    ~Deck()
		{
	        for (Card* card : cards)
			{
	            delete card;
	        }
	    }
	    void shuffle()
		{
	        random_device rd;
	    	mt19937 g(rd());
	        std::shuffle(cards.begin(), cards.end(), g);
	    }
	    Card* drawCard()
		{
	        Card* drawnCard = cards.back();
	        cards.pop_back();
	        return drawnCard;
	    }
	    void addCard(Card* card)
		{
	        cards.push_back(card);
	    }
};

class Player
{
	private:
	    string name;
	    vector<Card*> hand;
	public:
	    Player(string playerName) : name(playerName)
		{
		}
	    ~Player()
		{
	        for(Card* card : hand)
			{
	            delete card;
	        }
	    }
	
	    const string& getName() const
		{
			return name;
		}
	    void drawCard(Deck& deck)
		{
	        hand.push_back(deck.drawCard());
	    }
	    bool playCard(int index, Card*& topCard)
		{
	        if(index>=0 && index<hand.size() && hand[index]->isPlayableOn(*topCard))
			{
	            delete topCard;
	            topCard=hand[index];
	            hand.erase(hand.begin()+index);
	            return true;
	        }
	        return false;
	    }
	    vector<Card*>& getHand()
		{
			return hand;
		}
	
	    bool hasEmptyHand() const
		{
	        return hand.empty();
	    }
};

class Game
{
	private:
	    Deck deck;
	    vector<Player> players;
	    int currentPlayerIndex;
	    int currentDirection;
	    Card* topCard;
	    bool skipNextTurn;
	    int cardsToDraw;
	public:
	    Game(const vector<string>& playerNames) : currentPlayerIndex(0), currentDirection(1), topCard(nullptr), skipNextTurn(false), cardsToDraw(0) {
	        for(const auto& name : playerNames)
			{
	            players.emplace_back(name);
	        }
	        topCard=deck.drawCard();
	    }
	    ~Game()
		{
	        delete topCard;
	    }
	    void startGame()
		{
	        for(int i=0;i<3;i++)
			{
	            for(auto& player : players)
				{
	                player.drawCard(deck);
	            }
	        }
	        while(!isGameOver())
			{
	            nextTurn();
	        }
	    }
	    void nextTurn()
		{
	    	system("cls");
	        Player& currentPlayer=players[currentPlayerIndex];
	        cout<<"Current Player: "<<currentPlayer.getName()<<endl;
	    	cout<<"Top Card: " <<topCard->toString()<<endl;
	        if(skipNextTurn)
			{
	            cout<<currentPlayer.getName()<<" is skipped!"<<endl;
	            skipNextTurn=false;
	            moveToNextPlayer();
	            return;
	        }
	        if(cardsToDraw>0)
			{
	            for(int i=0;i<cardsToDraw;i++)
				{
	                currentPlayer.drawCard(deck);
	            }
	            cardsToDraw=0;
	            moveToNextPlayer();
	            return;
	        }
	
	        while(1)
			{
	            for(size_t i=0;i<currentPlayer.getHand().size();i++)
				{
	                const Card* card=currentPlayer.getHand()[i];
	                cout<<i<<": "<<card->toString()<<endl;
	            }
	            int choice;
	            cout<<"Choose a card to play (or -1 to draw): ";
	            cin>>choice;
	            if(choice==-1)
				{
	                currentPlayer.drawCard(deck);
	                break;
	            }
				else if(currentPlayer.playCard(choice, topCard))
				{
	                const ActionCard* actionCard=dynamic_cast<const ActionCard*>(topCard);
	                if(actionCard)
					{
						if(actionCard->getAction()=="Wild Draw Four" || actionCard->getAction()=="Wild")
						{
							string temp;
							cout<<"Enter Color: ";
							cin>>temp;
							topCard->setColor(temp);
						}
	                    if(actionCard->getAction()=="Skip")
						{
	                        skipNextTurn=true;
	                    }
						else if(actionCard->getAction()=="Reverse")
						{
	                        currentDirection*=-1;
	                    }
						else if(actionCard->getAction()=="Draw Two")
						{
	                        cardsToDraw=2;
	                    }
						else if(actionCard->getAction()=="Wild Draw Four")
						{
	                        cardsToDraw=4;
	                    }
	                }
	                break;
	            }
				else
				{
	            	system("cls");
	            	cout<<"Top Card: "<<topCard->toString()<<endl;
	                cout<<"Invalid move. Please choose a valid card or draw a card."<<endl;
	            }
	        }
	        moveToNextPlayer();
	    }	
	    void moveToNextPlayer()
		{
	        currentPlayerIndex=(currentPlayerIndex+currentDirection+players.size())%players.size();
	    }
	    bool isGameOver() 
	    {
	        for(const auto& player : players)
			{
	            if(player.hasEmptyHand())
				{
	                cout<<"Player "<<player.getName()<<" wins!"<<endl;
	                return true;
	            }
	        }
	        return false;
	    }
};

int Uno()
{
    vector<string> playerNames={"A","B"};
    Game game(playerNames);
    game.startGame();
    return 0;
} 
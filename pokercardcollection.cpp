#include "pokercardcollection.h"
#include <iostream>
#include <algorithm>

using namespace std;


PokerCardCollection::PokerCardCollection()
{
    CardCount = 0;
}

void PokerCardCollection::add(PokerCard card)
{
    if (CardCount < PokerCard::MaxRanksPerDeck) {
        Cards[CardCount] = card;
        CardCount++;
    }
}

void PokerCardCollection::add(char rankCh, char suitCh)
{
    if (CardCount < PokerCard::MaxRanksPerDeck) {
        Cards[CardCount].set(rankCh, suitCh);
        CardCount++;
    }
}

// CARDS MAY OR MAY NOT BE SPACE-SEPARATED
void PokerCardCollection::add(string cardString)
{
    char rankCh, suitCh;
    bool rankFlag = true;
    for (int a=0;a<cardString.size();a++) {
        char nextChar = cardString.at(a);
        if (nextChar != ' ') {
            if (rankFlag == true) {
                rankCh = nextChar;
                rankFlag = false;
            }
            else {
                this->add(rankCh, nextChar);
                rankFlag = true;
            }
        }
    }
}

PokerCard PokerCardCollection::at(int ndx)
{
    if (ndx < PokerCard::MaxRanksPerDeck) {
        return(Cards[ndx]);
    }

    cerr << "PokerCardCollection Index Not Found. Was " << ndx;

    PokerCard badCard; // CREATE AN INVALID CARD TO RETURN
    return(badCard);
}

int PokerCardCollection::size()
{
    return(CardCount);
}

void PokerCardCollection::Reset()
{
    CardCount = 0;
}


void PokerCardCollection::print()
{
    for (int a=0;a<this->CardCount;a++) {
       cout << Cards[a].ToString() << ":";
    }
}

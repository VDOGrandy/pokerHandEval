// pokercardcollection.h
// THIS CLASS MANAGES COLLECTIONS OF POKER CARDS SUCH AS POKER HANDS
// AND POKER BOARD CARDS
#ifndef POKERCARDCOLLECTION_H
#define POKERCARDCOLLECTION_H
#include "pokercard.h"

class PokerCardCollection
{
public:
    PokerCardCollection();
    void      add(PokerCard pc);
    void      add(char rankCh, char guitCh); // OVERLOADED FUNCTION
    void      add(std::string cardString);
    PokerCard at(int ndx);
    int       size();
    void      Reset();
    void      print();

private:
    int       CardCount;
    PokerCard Cards[PokerCard::MaxRanksPerDeck];

};

#endif // POKERCARDCOLLECTION_H

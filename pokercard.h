// pokercard.h
// THIS IS A BASIC GETTER/PUTTER CLASS, WHICH IS SOMEWHAT FROWNED UPON ON C++, HOWEVER
// I WILL EXTEND THIS CLASS LATER. DEFINING THIS AS A CLASS ALSO ALLOWS ME TO PERFORM
// FIELD VALIDATIONS
#ifndef POKERCARDS_H
#define POKERCARDS_H
#include <iostream>

class PokerCard
{

public:
    PokerCard();
    PokerCard(char theRank, char theSuit);
    void        set(char theRank, char theSuit);
    char        RankChar();
    char        SuitChar();
    bool        IsValid();
    std::string RankString();
    std::string ToString();

    static const int MaxRanksPerDeck = 14; // GLOBAL DEFINE

protected:
    char   RankAsChar;
    char   SuitAsChar;
    int    RankWeight;
};

#endif // POKERCARDS_H

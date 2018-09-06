#include <iostream>
#include "pokercard.h"

PokerCard::PokerCard()
{

}

// theRank: The rank of this card, as a char, to be displayed. ('2', 'J', 'Q', etc.)
// theSuit: The suit of this card, as a char, to be displayed. Must be 'C', 'D', 'H', 'S', 'X', or 'B'
PokerCard::PokerCard(char theRank, char theSuit)
{
    this->set(theRank, theSuit);
}

void PokerCard::set(char theRank, char theSuit)
{
    if (theSuit == 'C' || theSuit == 'D' || theSuit == 'H' || theSuit == 'S' || theSuit == 'J') {
        SuitAsChar = theSuit;
    }
    else {
        std::cerr << "PokerCard Class Error. Invalid Suit. Was " << theSuit;
        SuitAsChar = 'Z';
    }

    RankAsChar  = theRank;

}

char PokerCard::RankChar()
{
    return(RankAsChar);
}

char PokerCard::SuitChar()
{
    return(SuitAsChar);
}

std::string PokerCard::RankString()
{
    std::string str(1, RankAsChar);
    return(str);
}

std::string PokerCard::ToString()
{
    std::string str;
    str += RankAsChar;
    str += SuitAsChar;
    return(str);
}

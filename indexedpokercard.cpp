#include "pokercard.h"
#include "indexedpokercard.h"

// C++ WON'T LET ME PUT A DEFINED ARRAY INTO A CLASS DEFINITION SO THE
// ALTERNATIVES I SEE ARE DOING THIS OR INITIALIZE THE VALUES IN
// THE CONSTRUCTOR (AND DOING THAT USES UNNECESSARY COMPUTE CYCLES)
static const int HighCardWeight[IndexedPokerCard::MaxRanksPerDeck] = {
    1,    // Low Ace
    2,    // Deuce
    3,    // Trey
    4,    // 4
    10,   // 5
    20,   // 6
    40,   // 7
    80,   // 8
    160,  // 9
    320,  // Ten
    640,  // Jack
    1280, // Queen
    2560, // King
    5120  // High Ace
};


IndexedPokerCard::IndexedPokerCard()
{
    Valid = false;
}

// THE END OF THIS DECLARATION PROVIDES FOR THE CALL TO THE PokerCard CONSTRUCTOR
IndexedPokerCard::IndexedPokerCard(char theRank, char theSuit, int rankIndex, int weight) : PokerCard(theRank, theSuit)
{
    Valid = false;
    set(theRank, theSuit, rankIndex, weight);
}

void IndexedPokerCard::set(char theRank, char theSuit, int rankIndex, int weight)
{
    RankAsChar = theRank;                    // FROM PokerCard CLASS
    SuitAsChar = theSuit;                    // FROM PokerCard CLASS


    // PROTECT THE INDEX INTO THE HIGH CARD WEIGHT ARRAY
    if (rankIndex < 0 || rankIndex >= MaxRanksPerDeck) {
        std::cerr << "IndexedPokerCard Class: Illegal index. Was " << rankIndex;
        RankIndex = 0;
        RankWeight = 0;
    }
    else {
        RankIndex  = rankIndex;
        RankWeight = HighCardWeight[rankIndex];
        Valid = true;
    }
}

int IndexedPokerCard::weight()
{
    return(RankWeight);
}

bool IndexedPokerCard::IsValid()
{
    return(Valid);
}

PokerCard IndexedPokerCard::toPokerCard()
{
    PokerCard pc(this->RankChar(), this->SuitChar());
    return(pc);
}

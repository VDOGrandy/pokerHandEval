// THIS CLASS IS USED WITH THE POKERHANDANALYZER AND POKERHANDEVALUATOR CLASSES.
// THE USER CONFIGURES A RANKING OF CARDS IN THIS CLASS THAT IS USED TO EVALUATE
// THE WINNING HAND BASED ON HIGH CARDS AND THE ABILITY TO KNOW IF CARDS ARE
// CONTIGUOUS FOR STRAIGHTS. THE USER SUBMITS THE RANK OF ALL CARDS IN THE DECK AND THIS
// CLASS WILL ASSIGN CARD WEIGHTS BASED ON THE RANK INDEX. THE RANK INDEX MUST
// BE A VALUE FROM 0 TO 13, REPRESENTING a23456789TJQKA. A SHORT DECK IS FINE,
// SO NOT ALL INDEXES ARE REQUIRED.
#ifndef EVALUATIONDECK_H
#define EVALUATIONDECK_H
#include "pokercard.h"

struct evalDeckStruct {
    char rankChar;
    int  rankIndex;
    int  weight;
};

// C++ WON'T LET ME PUT A DEFINED ARRAY INTO A CLASS DEFINITION SO THE
// ALTERNATIVES I SEE ARE DOING THIS OR INITIALIZE THE VALUES IN
// THE CONSTRUCTOR (AND DOING THAT USES UNNECESSARY COMPUTE CYCLES)
static const int HighCardWeight[PokerCard::MaxRanksPerDeck] = {
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


class EvaluationDeck
{
public:
    EvaluationDeck(bool reverseStrength);
    void add(char rankChar, int rankIndex);
    int  weight(char rankChar);
    int  rankIndex(char rankChar);
    bool reverseStrength();
    bool hasLowAce();
    bool hasHighAce();
    int  size();

private:
    int EvalDeckCount;
    struct evalDeckStruct EvalDeck[PokerCard::MaxRanksPerDeck];
    bool ReverseStrengthFlag;
    bool HasLowAceFlag;
    bool HasHighAceFlag;

};

#endif // EVALUATIONDECK_H

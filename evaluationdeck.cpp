#include "evaluationdeck.h"

EvaluationDeck::EvaluationDeck(bool reverseStrength)
{
    EvalDeckCount       = 0;  // NO CARDS IN THE TRUTH DECK YET
    ReverseStrengthFlag = reverseStrength;
    HasLowAceFlag       = false;
    HasHighAceFlag      = false;
}


// ADD A CARD TO THE EVALUATION DECK
// rankChar MUST BE ONE OF A23456789TJQK
// rankIndex MUST BE 0-13
void EvaluationDeck::add(char rankChar, int rankIndex)
{
    if (rankIndex < 0 || rankIndex >= PokerCard::MaxRanksPerDeck) {
        std::cerr << "EvaluationDeck Class Error: Illegal index. Was " << rankIndex << std::endl;
    }
    else if (EvalDeckCount >= PokerCard::MaxRanksPerDeck) {
        std::cerr << "EvaluationDeck Class Error: Too many card indexes requested." << std::endl;
    }
    else {
        EvalDeck[EvalDeckCount].rankChar  = rankChar;
        EvalDeck[EvalDeckCount].rankIndex = rankIndex;
        EvalDeck[EvalDeckCount].weight    = HighCardWeight[rankIndex];
        EvalDeckCount++;

        if (rankChar == 'a') HasLowAceFlag = true;
        if (rankChar == 'A') HasHighAceFlag = true;
    }
}


// RETURNS 0 IF rankChar IS NOT FOUND.
int EvaluationDeck::weight(char rankChar)
{
    int weightVal = 0;

    for (int a=0;a<EvalDeckCount;a++) {
        if (EvalDeck[a].rankChar == rankChar) {
            weightVal = EvalDeck[a].weight;
            break;
        }
    }
    return(weightVal);
}


int EvaluationDeck::rankIndex(char rankChar)
{
    int rankIndexVal = 0;

    for (int a=0;a<EvalDeckCount;a++) {
        if (EvalDeck[a].rankChar == rankChar) {
            rankIndexVal = EvalDeck[a].rankIndex;
            break;
        }
    }
    return(rankIndexVal);
}

bool EvaluationDeck::reverseStrength()
{
    return(ReverseStrengthFlag);
}

bool EvaluationDeck::hasLowAce()
{
    return(HasLowAceFlag);
}

bool EvaluationDeck::hasHighAce()
{
    return(HasHighAceFlag);
}

int EvaluationDeck::size()
{
    return(EvalDeckCount);
}


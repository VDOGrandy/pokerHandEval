// pokerhandevaluator.h
// THIS CLASS USES THE PokerHandEvaluator CLASS TO EVALUATE EACH OF
// THE POKER EVALUATION TYPES. NEW EVALUATIONS WILL BE ADDED TO
// THIS CLASS TO BUILD A LIBRARY OF ALL POKER EVALUATIONS
#ifndef POKERHANDEVALUATOR_H
#define POKERHANDEVALUATOR_H
#include <iostream>
#include "pokerhandanalyzer.h"

class PokerHandEvaluator
{
public:
    PokerHandEvaluator();
    int          evaluateStandardHand(PokerCardCollection allCards);
    int          evaluateAce5Hand(PokerCardCollection allCards);
    int          evaluateDeuceSevenHand(PokerCardCollection allCards);
    std::string  getLastHandString();
    bool         hasEightOrBetter();

private:
    PokerHandAnalyzer Hand;
    bool              Has8OrBetter;
    std::string       LastHandResultString;

    EvaluationDeck *StandardDeck;
    EvaluationDeck *Ace5Deck;
    EvaluationDeck *DeuceSevenDeck;


};

#endif // POKERHANDEVALUATOR_H

#include <iostream>
#include "pokerhandevaluator.h"
#include "pokercardcollection.h"

PokerHandEvaluator::PokerHandEvaluator()
{
    // TRUTH DECK DETERMINES CARD RANKING
    StandardDeck = new EvaluationDeck(false);
    StandardDeck->add('a', 0); // ACE CAN BE PLAYED LOW FOR A STRAIGHT
    StandardDeck->add('2', 1);
    StandardDeck->add('3', 2);
    StandardDeck->add('4', 3);
    StandardDeck->add('5', 4);
    StandardDeck->add('6', 5);
    StandardDeck->add('7', 6);
    StandardDeck->add('8', 7);
    StandardDeck->add('9', 8);
    StandardDeck->add('T', 9);
    StandardDeck->add('J', 10);
    StandardDeck->add('Q', 11);
    StandardDeck->add('K', 12);
    StandardDeck->add('A', 13); // ACE CAN BE HIGH FOR STRAIGHT

    DeuceSevenDeck = new EvaluationDeck(false);
    DeuceSevenDeck->add('2', 1);
    DeuceSevenDeck->add('3', 2);
    DeuceSevenDeck->add('4', 3);
    DeuceSevenDeck->add('5', 4);
    DeuceSevenDeck->add('6', 5);
    DeuceSevenDeck->add('7', 6);
    DeuceSevenDeck->add('8', 7);
    DeuceSevenDeck->add('9', 8);
    DeuceSevenDeck->add('T', 9);
    DeuceSevenDeck->add('J', 10);
    DeuceSevenDeck->add('Q', 11);
    DeuceSevenDeck->add('K', 12);
    DeuceSevenDeck->add('A', 13); // ACE IS ALWAYS HIGH

    Ace5Deck = new EvaluationDeck(false);
    Ace5Deck->add('a', 0); // Ace is low ALWAYS
    Ace5Deck->add('2', 1);
    Ace5Deck->add('3', 2);
    Ace5Deck->add('4', 3);
    Ace5Deck->add('5', 4);
    Ace5Deck->add('6', 5);
    Ace5Deck->add('7', 6);
    Ace5Deck->add('8', 7);
    Ace5Deck->add('9', 8);
    Ace5Deck->add('T', 9);
    Ace5Deck->add('J', 10);
    Ace5Deck->add('Q', 11);
    Ace5Deck->add('K', 12);

}

bool PokerHandEvaluator::hasEightOrBetter()
{
    return(Has8OrBetter);
}

// nCardPerHand DETERMINES THE MAXIMUM CARDS IN THE HAND THAT EVALUATE FOR A GIVEN HAND
// I.E. FOR OMAHA, THE NUMBER WOULD BE 2, ALTHOUGH THERE ARE 4 CARDS IN THE HAND
int PokerHandEvaluator::evaluateStandardHand(PokerCardCollection allCards)
{
    int handStrength = 0;

    Hand.Analyze(*StandardDeck, allCards);  // SUBMIT HAND TO ANALYZER


    // STRAIGHT FLUSH
    if (Hand.hasStraight() == true  &&  Hand.hasFlush() == true) {
        handStrength = Hand.getEvalStraightFlush();
        if (handStrength > 0) { // WE HAVE A STRAIGHT FLUSH
            LastHandResultString = Hand.getResultString();
            return(handStrength);
        }
    }

    // QUADS
    if (Hand.hasQuads()) {
        handStrength = Hand.getEvalQuads();
        LastHandResultString = Hand.getResultString();
        handStrength += Hand.getEvalRemainingCards(1);
    }

    // FULL HOUSE
    else if (Hand.hasFullHouse()) {
        handStrength = Hand.getEvalFullHouse();
        LastHandResultString = Hand.getResultString();
    }

    // FLUSH
    else if (Hand.hasFlush()) {
        handStrength = Hand.getEvalFlush();
        LastHandResultString = Hand.getResultString();
    }

    // STRAIGHT
    else if (Hand.hasStraight()) {
        handStrength = Hand.getEvalStraight();
        LastHandResultString = Hand.getResultString();
    }

    // 3 OF A KIND
    else if (Hand.hasTrips()) {
        handStrength = Hand.getEvalTrips();
        LastHandResultString = Hand.getResultString();
        handStrength += Hand.getEvalRemainingCards(2);
    }

    // TWO PAIR
    else if (Hand.has2Pair()) {
        handStrength = Hand.getEvalTopPair();
        handStrength += Hand.getEvalSecondPair();
        LastHandResultString = Hand.getResultString(); // SAVE THE 1 PAIR RESULT STRING
        Hand.getEvalRemainingCards(1);
    }

    // ONE PAIR
    else if (Hand.has1Pair()) {
        handStrength = Hand.getEvalTopPair();
        LastHandResultString = Hand.getResultString(); // SAVE THE 1 PAIR RESULT STRING
        handStrength += Hand.getEvalRemainingCards(3);
    }

    // HIGH CARD
    else {
        handStrength = Hand.getEvalRemainingCards(5);
        LastHandResultString = Hand.getResultString();
    }

    return(handStrength);
}


// ACE 5 LOW IS THE EVALUATOR FOR THE LOW IN OMAHA/8, RAZZ AND SIMILAR GAMES
// STRAIGHTS AND FLUSHES DO NOT COUNT AGAINST
// REMEMBER, THE analyzeAce5 DECK WE ARE USING THE CARD RANKS ARE REVERSED,
// IN OTHER WORDS, ACE IS HIGHEST, 2 IS NEXT HIGHEST, etc.
int PokerHandEvaluator::evaluateAce5Hand(PokerCardCollection allCards)
{
    int handStrength = 0;
    Has8OrBetter = false;

    Hand.Analyze(*Ace5Deck, allCards);  // SUBMIT HAND TO ANALYZER

    // HIGH CARD WHICH IS REALLY THE LOW CARD
    int singlesCount = Hand.singleCardCount();
    if (singlesCount >= 5) {

        handStrength = Hand.getEvalConsecutiveCards(5);


        // NEED TO CREATE MY OWN MESSAGE USING THE HIGH OF THE LOW CARDS
        PokerCard firstCard = Hand.resultCard(0);
        LastHandResultString = firstCard.RankString() + "-low";
        char chRank = firstCard.RankChar();
        if (chRank == '8' || chRank == '7' || chRank == '6' || chRank == '5')
        {
            Has8OrBetter = true;
        }
    }

    else if (Hand.hasQuads()) {
        handStrength = Hand.getEvalQuads();
        LastHandResultString = Hand.getResultString();
        handStrength += Hand.getEvalRemainingCards(1);
    }

    // FULL HOUSE
    else if (Hand.hasFullHouse()) {
        handStrength = Hand.getEvalFullHouse();
        LastHandResultString = Hand.getResultString();
    }

    // 3 OF A KIND
    else if (Hand.hasTrips()) {
        handStrength = Hand.getEvalTrips();
        LastHandResultString = Hand.getResultString();
        handStrength += Hand.getEvalRemainingCards(2);
    }

    // TWO PAIR
    else if (Hand.has2Pair()) {
        handStrength = Hand.getEvalTopPair();
        handStrength += Hand.getEvalSecondPair();
        LastHandResultString = Hand.getResultString(); // SAVE THE 1 PAIR RESULT STRING
        Hand.getEvalRemainingCards(1);
    }

    // ONE PAIR
    else if (Hand.has1Pair()) {
        handStrength = Hand.getEvalTopPair();
        //std::cout << "ONEPAIR! " << handStrength;
        LastHandResultString = Hand.getResultString(); // SAVE THE 1 PAIR RESULT STRING
        handStrength += Hand.getEvalRemainingCards(3);
    }


    // THE BIGGER THE STRENGTH, THE BETTER THE HAND
    handStrength *= -1;
    return(handStrength);
}



int PokerHandEvaluator::evaluateDeuceSevenHand(PokerCardCollection allCards)
{
    int handStrength = 0;
    Has8OrBetter = false;

    Hand.Analyze(*DeuceSevenDeck, allCards);  // SUBMIT HAND TO ANALYZER

    // HIGH CARD WHICH IS REALLY THE LOW CARD
    int singlesCount = Hand.singleCardCount();
    if (singlesCount >= 5) {

        handStrength = Hand.getEvalConsecutiveCards(5);

        // NEED TO CREATE MY OWN MESSAGE USING THE HIGH OF THE LOW CARDS
        PokerCard firstCard = Hand.resultCard(0);
        LastHandResultString = firstCard.RankString() + "-low";
    }

    // STRAIGHT FLUSH
    if (Hand.hasStraight() == true && Hand.hasFlush() == true) {
        handStrength = Hand.getEvalStraightFlush();
        if (handStrength > 0) {
            LastHandResultString = Hand.getResultString();
            return(handStrength);
        }
    }

    // QUADS
    if (Hand.hasQuads()) {
        handStrength = Hand.getEvalQuads();
        LastHandResultString = Hand.getResultString();
        handStrength += Hand.getEvalRemainingCards(1);

    }

    // FULL HOUSE
    else if (Hand.hasFullHouse()) {
        handStrength = Hand.getEvalFullHouse();
        LastHandResultString = Hand.getResultString();
    }

    // FLUSH
    else if (Hand.hasFlush()) {
        handStrength = Hand.getEvalFlush();
        LastHandResultString = Hand.getResultString();
    }

    // STRAIGHT
    else if (Hand.hasStraight()) {
        handStrength = Hand.getEvalStraight();
        LastHandResultString = Hand.getResultString();
    }

    // 3 OF A KIND
    else if (Hand.hasTrips()) {
        handStrength = Hand.getEvalTrips();
        LastHandResultString = Hand.getResultString();
        handStrength += Hand.getEvalRemainingCards(2);
    }

    // TWO PAIR
    else if (Hand.has2Pair()) {
        handStrength = Hand.getEvalTopPair();
        handStrength += Hand.getEvalSecondPair();
        LastHandResultString = Hand.getResultString(); // SAVE THE 1 PAIR RESULT STRING
        Hand.getEvalRemainingCards(1);
    }

    // ONE PAIR
    else if (Hand.has1Pair()) {
        handStrength = Hand.getEvalTopPair();
        LastHandResultString = Hand.getResultString(); // SAVE THE 1 PAIR RESULT STRING
        handStrength += Hand.getEvalRemainingCards(3);
    }

    handStrength *= -1;
    return(handStrength);
}



std::string PokerHandEvaluator::getLastHandString()
{
    std::string resultHandStr = LastHandResultString + (std::string)": " + Hand.getResultHandString();
    return(resultHandStr);
}

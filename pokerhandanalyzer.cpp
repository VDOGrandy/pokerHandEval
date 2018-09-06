#include <iostream>
#include "pokerhandanalyzer.h"
#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;
using namespace phaConstants;

/*
// NOT PART OF THE CLASS, THIS FUNCTION IS FOR USING std::sort
bool PokerHandAnalyzer::RankIndexCompareAscending(IndexedPokerCard const & a, IndexedPokerCard const & b)
{
    return(a.RankIndex > b.RankIndex);
}
bool PokerHandAnalyzer::RankIndexCompareDescending(IndexedPokerCard const & a, IndexedPokerCard const & b)
{
    return(a.RankIndex < b.RankIndex);
}
*/


// COMPARE FUNCTIONS USED FOR std::sort
bool PokerHandAnalyzer::RankIndexCompareAscending(IndexedPokerCard a, IndexedPokerCard b)
{
    return(a.RankIndex > b.RankIndex);
}
bool PokerHandAnalyzer::RankIndexCompareDescending(IndexedPokerCard a, IndexedPokerCard b)
{
    return(a.RankIndex < b.RankIndex);
}



// CONSTRUCTOR
PokerHandAnalyzer::PokerHandAnalyzer()
{
    TotalCards   = 0;
    AceIsLowForStraight  = true;
}


// FUNCTION: Analyze
// PERFORMS: Analyzes the quality of a poker hand and returns a number relative to its hand value
// RETURNS:  A relative hand value or 0 if an error occurred
// NOTES:    If handStr AND boardStr are NULL, the evaluation is performed on the settings from the
//           previously analyzed hand (this greatly enhances performance).
//           Card Rank must be one of A23456789TJQK
//           Card Suit must be one of CDHS
//           Cards must be contiguous with no spaces. Example: AH2STCKD
//           The maximum number of cards is 13
int PokerHandAnalyzer::Analyze(EvaluationDeck truthDeck, PokerCardCollection theCards)
{
    // INITIALIZE VARIABLES
    TotalCards            =  0;
    ICardCount            =  0;
    HandResultCardCount   =  0;
    HighFlushIndex        = -1;
    HighStraightIndex     = -1;
    FlushCharacter        = 'N';
    if (truthDeck.hasLowAce()) {
        AceIsLowForStraight = true;
    }

    for (int a=0;a<PokerCard::MaxRanksPerDeck;a++) {
        RankAccum[a].numCardsAtThisRank = 0;
    }

    if (truthDeck.reverseStrength()) StrengthMultiplier = -1;
    else                             StrengthMultiplier = 1;

    // IF THE RANK KEY HAS A LOW ACE, WE WILL
    // APPEND A LOW ACE TO THE HAND
    AceIsLowForStraight = false;
    if (truthDeck.hasLowAce()) {
        AceIsLowForStraight = true;
        // IF THERE ARE ACES AT BOTH ENDS OF THE DECK, ADD THE LOW ACE
        for (int a=0;a<theCards.size();a++) {
            if (theCards.at(a).RankChar() == 'A') {
                theCards.add('a', theCards.at(a).SuitChar());
            }
        }
    }

    TotalCards     = theCards.size();

    // BOUNDARY CHECK: ENSURE THERE ARE NOT TOO MANY CARDS
    if (TotalCards > MAX_CARDS_TO_ANALYZE) return(0);

    // THE CARDS MUST BE ASSIGNED INDEXES AND WEIGHTS FROM
    // THE SUPPLIED TRUTH DECK. WE WILL COMBINE THEM
    // INTO THE INDEXEDCARD CLASS
    for (ICardCount=0;ICardCount<TotalCards;ICardCount++) {
        char rank  = theCards.at(ICardCount).RankChar();
        char suit  = theCards.at(ICardCount).SuitChar();
        int ndx    = truthDeck.rankIndex(rank);
        int weight = truthDeck.weight(rank);
        ICards[ICardCount].set(rank, suit, ndx, weight);
    }

    // CARDS MUST BE SORTED. MUST KNOW IF THE ACE IS LOW ONLY FOR SORTING PURPOSES
    // IF ACE IS LOW ONLY FOR SORTING PURPOSES, USE THE LOWER CASE 'A'.
    std::sort(ICards, ICards + ICardCount, RankIndexCompareAscending);
    /*
    cout << "AFTER SORT ";
    for (int a=0;a<ICardCount;a++) {
        cout << ICards[a].ToString() << ":";
    }
    cout << "  ";
    */

    ////////////////////////////////////////////////////
    // ANALYZE ALL OF THE CARDS AND BUILD A SET OF FLAGS
    // BUFFER CONTAINING CARDS MUST ALREADY BE SORTED
    ////////////////////////////////////////////////////
    int prevRank          = -1;     // LAST RANK; FOR DETERMINING STRAIGHTS
    int straightStart     =  0;     // HIGHEST CARD OF A STRAIGHT
    int straightCount     =  0;     // NUMBER OF CARDS IN THE STRAIGHT
    int  clubCount        =  0;
    int  diamondCount     =  0;
    int  heartCount       =  0;
    int  spadeCount       =  0;
    int  highClubIndex    =  0;
    int  highDiamondIndex =  0;
    int  highHeartIndex   =  0;
    int  highSpadeIndex   =  0;

    for (int a=0;a<TotalCards;a++) {

        // GET CARD SPECIFICS
        char suitCh   = ICards[a].SuitChar();
        char rankCh   = ICards[a].RankChar();
        int  rankVal  = ICards[a].RankIndex;

        // PROCESS SUIT COUNT AND RANK COUNT. IGNORE 'a' AS IT IS ONLY FOR STRAIGHTS
        if (rankCh != 'a') {
            // COUNT THE SUITS
            if (suitCh == 'C') {
                clubCount++;
                if (clubCount == 1) highClubIndex = a; // SAVE HIGHEST CARD OF FLUSH
            }
            else if (suitCh == 'D') {
                diamondCount++;
                if (diamondCount == 1) highDiamondIndex = a;
            }
            else if (suitCh == 'H') {
                heartCount++;
                if (heartCount == 1) highHeartIndex = a;
            }
            else if (suitCh == 'S') {
                spadeCount++;
                if (spadeCount == 1) highSpadeIndex = a;
            }

            // COUNT PAIRS/TRIPS/QUADS BY INCREMENTING TOTALS IN
            // AN ARRAY CONTAINING EACH POSSIBLE RANK
            int ndx = RankAccum[rankVal].numCardsAtThisRank;
            RankAccum[rankVal].icard[ndx] = ICards[a];        // SAVE THE PAIRED CARDS
            RankAccum[rankVal].numCardsAtThisRank++;
        }

        // COUNT STRAIGHTS VIA A STARTING INDEX AND A COUNT
        // SINCE IT IS SORTED, THE START WILL BE THE HIGH STRAIGHT
        if (straightCount < 4) { // IF NOT ALREADY A COMPLETED STRAIGHT
            if (a > 0 && rankVal == prevRank - 1) { // IF THIS CARD RANK IS LAST RANK MINUS 1
                straightCount++;
            }
            else if (a > 0 && rankVal == prevRank) {
                // IGNORE PAIRS
            }
            else {
                straightStart = a;
                straightCount = 0;
            }
        }

        prevRank = rankVal; // STRAIGHT CALCULATION FOR NEXT ITERATION
    }


    //////////////////////////////////////////////////
    // WE HAVE COLLECTED STATISTICS, AGGREGATE THEM
    //////////////////////////////////////////////////

    // RESET FLUSH FLAGS THEN SET THEM
    FlushCharacter = 'N';
    if (clubCount >= 5) {
        FlushCharacter = 'C';
        HighFlushIndex = highClubIndex;
    }
    else if (diamondCount >= 5) {
        FlushCharacter = 'D';
        HighFlushIndex = highDiamondIndex;
    }
    else if (heartCount >= 5) {
        FlushCharacter = 'H';
        HighFlushIndex = highHeartIndex;
    }
    else if (spadeCount >= 5) {
        FlushCharacter = 'S';
        HighFlushIndex = highSpadeIndex;
    }

    SuitCount = 0; // USEFUL FOR BADUGI
    if (clubCount > 0)    SuitCount++;
    if (diamondCount > 0) SuitCount++;
    if (heartCount > 0)   SuitCount++;
    if (spadeCount > 0)   SuitCount++;


    // UPDATE STRAIGHT STATUS
    HasStraight = false;
    if (straightCount >= 4) {
        HasStraight = true;
        HighStraightIndex = straightStart;
    }

    // DETERMINE IF THERE ARE QUADS 3/KIND AND PAIRS
    QuadRank    =  -1;
    TripRank    =  -1;
    PairRank1   =  -1;
    PairRank2   =  -1;
    SingleCount  =   0;
    for (int a=PokerCard::MaxRanksPerDeck;a>=0;a--) {
        if (RankAccum[a].numCardsAtThisRank == 2) {
            if (PairRank1 == -1)      PairRank1 = a;
            else if (PairRank2 == -1) PairRank2 = a;
        }
        else if (RankAccum[a].numCardsAtThisRank == 3) TripRank = a;
        else if (RankAccum[a].numCardsAtThisRank == 4) QuadRank = a;
        else if (RankAccum[a].numCardsAtThisRank == 1) SingleCount++;
    }

    return(0);
}


// RETURN THE NUMBER OF CARDS
int PokerHandAnalyzer::size()
{
    return(TotalCards);
}



PokerCard PokerHandAnalyzer::resultCard(int whichCard)
{
    if (HandResultCardCount == 5) {
         return(HandResult[whichCard]);
    }
    PokerCard pk;
    pk.set('N','N');
    return(pk);
}

void PokerHandAnalyzer::PrintHandResultArray()
{
    cout << "RESULT HAND: " + getResultHandString();
}

string PokerHandAnalyzer::getResultString()
{
    return(ResultString);
}

string PokerHandAnalyzer::getResultHandString()
{
    string handStr;

    for (int a=0;a<HandResultCardCount;a++) {
        handStr += HandResult[a].ToString() + " ";
    }
    return(handStr);
}


//====================================================================================================//
//==================================== HAND QUALITY FLAGS ============================================//
//====================================================================================================//

bool PokerHandAnalyzer::hasQuads()
{
    if (QuadRank >= 0) return(true);
    return(false);
}

bool PokerHandAnalyzer::hasFullHouse()
{
    if (TripRank >= 0 && PairRank1 >= 0) return(true);
    return(false);
}

bool PokerHandAnalyzer::hasFlush()
{
    if (FlushCharacter != 'N' && HighFlushIndex >= 0) {
        return(true);
    }
    return(false);
}

bool PokerHandAnalyzer::hasStraight()
{
    return(HasStraight);
}

bool PokerHandAnalyzer::hasTrips()
{
    if (TripRank >= 0) return(true);
    return(false);
}

bool PokerHandAnalyzer::has2Pair()
{
    if (PairRank1 >= 0 && PairRank2 >= 0) return(true);
    return(false);
}

bool PokerHandAnalyzer::has1Pair()
{
    if (PairRank1 >= 0 && PairRank2 == -1) return(true);
    return(false);
}

int PokerHandAnalyzer::singleCardCount()
{
    return(SingleCount);
}

int PokerHandAnalyzer::getSuitCount()
{
    return(SuitCount);
}



//====================================================================================================//
//===================================== GET BEST 5 CARD HANDS ========================================//
//====================================================================================================//
// TEST THE FLAGS ABOVE TO DETERMINE IF IT IS APPROPRIATE TO CALL EACH OF THE FOLLOWING FUNCTIONS

// IT IS SOMEWHAT PROCESSOR INTENSIVE TO DETERMINE WHETHER THERE
// IS A STRAIGHT FLUSH OR NOT. CALL THIS FUNCTION ONLY IF THERE
// IS A STRAIGHT AND A FLUSH IDENTIFIED BY THE FLAG FUNCTIONS
// ABOVE, IN ORDER TO MAINTAIN TOP PERFORMANCE. IF THIS FUNCTION
// RETURNS 0, THERE IS NO STRAIGHT FLUSH, MEANING THE STRAIGHT AND
// THE FLUSH ARE SEPARATE FROM ONE ANOTHER.
int PokerHandAnalyzer::getEvalStraightFlush()
{
    // CYCLE THROUGH ALL OF THE CARDS AND SEE IF THERE IS
    // A STRAIGHT FLUSH
    int prevRank       = ICards[0].RankIndex;
    int highRank       = prevRank;
    int str8FlushCount = 1;
    HandResult[0]      = ICards[0].toPokerCard();

    for (int a=1;a<TotalCards;a++) {
        if (str8FlushCount >= 5) break;

        int  testRank = ICards[a].RankIndex;
        char testSuit = ICards[a].SuitChar();

        if (testRank == (prevRank -1)  &&  testSuit == FlushCharacter) {
            HandResult[str8FlushCount] = ICards[a].toPokerCard();
            prevRank = ICards[a].RankIndex;
            str8FlushCount++;
        }
        else if (testSuit == FlushCharacter){
            prevRank = ICards[a].RankIndex;
            highRank = prevRank;
            str8FlushCount = 1;
            HandResult[0] = ICards[a].toPokerCard();
        }
    }

    if (str8FlushCount != 5) {
        return(0);
    }

    HandResultCardCount = 5;

    // StrengthMultiplier is either 1 or -1 depending on whether this is
    // a high eval or a low eval.
    int handStrength = StrengthMultiplier * (STRAIGHTFLUSH_OFFSET + highRank);

    ResultString = ICards[0].RankString() + "-high straight flush";

    return(handStrength);
}

int PokerHandAnalyzer::getEvalQuads()
{
    int handStrength = StrengthMultiplier * (QUADS_OFFSET + (QUADS_FACTOR * QuadRank) );

    // STORE THESE CARDS INTO OUR OUTPUT HAND RESULTS FOR LATER DISPLAY
    HandResult[HandResultCardCount]   = RankAccum[QuadRank].icard[0].toPokerCard();
    HandResult[HandResultCardCount+1] = RankAccum[QuadRank].icard[1].toPokerCard();
    HandResult[HandResultCardCount+2] = RankAccum[QuadRank].icard[2].toPokerCard();
    HandResult[HandResultCardCount+3] = RankAccum[QuadRank].icard[3].toPokerCard();
    string actualRankString = HandResult[HandResultCardCount].RankString();
    HandResultCardCount += 4;

    ResultString = "four of a kind " + actualRankString + (string)"'s";
    return(handStrength);
}

int PokerHandAnalyzer::getEvalFullHouse()
{
    getEvalTrips();   // UPDATE THE HAND RESULT ARRAY
    getEvalTopPair();

    int handStrength  = StrengthMultiplier * (FULLHOUSE_FACTOR + (10000 * TripRank) + PairRank1);

    string tripStr = RankAccum[TripRank].icard[0].RankString();
    string pairStr = RankAccum[PairRank1].icard[0].RankString();
    ResultString      = "full house " + tripStr + (string)"'s full of " + pairStr + (string)"'s";

    return(handStrength);
}

int PokerHandAnalyzer::getEvalFlush()
{
    int handStrength = StrengthMultiplier * FLUSH_FACTOR;

    HandResultCardCount = 0;
    for (int a=HighStraightIndex;a<phaConstants::MAX_CARDS_TO_ANALYZE;a++) {
        if (ICards[a].SuitChar() == FlushCharacter) {
            HandResult[HandResultCardCount] = ICards[a].toPokerCard();
            handStrength += ICards[a].weight();
            HandResultCardCount++;
        }
    }

    ResultString = HandResult[0].RankString() + "-high flush";
    return(handStrength);
}

int PokerHandAnalyzer::getEvalStraight()
{
    int wt = ICards[HighStraightIndex].weight();
    int prevRank = ICards[HighStraightIndex].RankIndex + 1;

    int handStrength = StrengthMultiplier + (STRAIGHT_FACTOR + wt);

    HandResultCardCount = 0;
    for (int a=HighStraightIndex;a<phaConstants::MAX_CARDS_TO_ANALYZE;a++) {
        int thisRank = ICards[a].RankIndex;
        if (thisRank == prevRank - 1) {
            HandResult[HandResultCardCount] = ICards[a].toPokerCard();
            prevRank = thisRank;
            HandResultCardCount++;
        }
        if (HandResultCardCount == 5) break;
    }

    ResultString = HandResult[0].RankString() + "-high straight";

    return(handStrength);
}

int PokerHandAnalyzer::getEvalTrips()
{
    int handStrength = StrengthMultiplier * (TRIPS_OFFSET + (TRIPS_FACTOR * TripRank));

    // STORE THESE CARDS INTO OUR OUTPUT HAND RESULTS FOR LATER DISPLAY
    HandResult[HandResultCardCount]   = RankAccum[TripRank].icard[0].toPokerCard();
    HandResult[HandResultCardCount+1] = RankAccum[TripRank].icard[1].toPokerCard();
    HandResult[HandResultCardCount+2] = RankAccum[TripRank].icard[2].toPokerCard();
    string rankStr = HandResult[HandResultCardCount].RankString();
    HandResultCardCount += 3;

    ResultString = "3 of a kind " + rankStr + (string)"'s";
    return(handStrength);
}

// GET THE PAIR AND MARK THE CARDS AS NO LONGER AVAILABLE
int PokerHandAnalyzer::getEvalTopPair()
{
    int handStrength = StrengthMultiplier * (PAIR_OFFSET + (PAIR_FACTOR * PairRank1)); // 154480

    HandResult[HandResultCardCount] = RankAccum[PairRank1].icard[0].toPokerCard();
    HandResult[HandResultCardCount+1] = RankAccum[PairRank1].icard[1].toPokerCard();
    string rankStr = HandResult[HandResultCardCount].RankString();
    HandResultCardCount += 2;


    ResultString = "one pair " + rankStr + (string)"'s ";

    return(handStrength);
}

int PokerHandAnalyzer::getEvalSecondPair()
{
    int handStrength = StrengthMultiplier * (PAIR2_OFFSET + (PAIR2_FACTOR * PairRank2));

    HandResult[HandResultCardCount]   = RankAccum[PairRank2].icard[0].toPokerCard();
    HandResult[HandResultCardCount+1] = RankAccum[PairRank2].icard[1].toPokerCard();
    HandResultCardCount += 2;

    string rankStr1 = RankAccum[PairRank1].icard[0].toPokerCard().RankString();
    string rankStr2 = RankAccum[PairRank2].icard[0].toPokerCard().RankString();

    ResultString = "two pair " + rankStr1 + (string)"'s and " + rankStr2 + (string)"'s ";

    return(handStrength);

}

int PokerHandAnalyzer::getEvalConsecutiveCards(int numCards)
{
    return(getSingleCards(numCards, true));
}

int PokerHandAnalyzer::getEvalRemainingCards(int numCards)
{
    return(getSingleCards(numCards, false));
}

// THIS IS A COMPLEX, DUAL-USE FUNCTION THAT PROCESSES SINGLE CARDS
// IN ONE OF TWO WAYS. IF getConsecutiveCards IS TRUE, IT WILL
// PROCESS CONSECUTIVE CARDS FROM THE HIGHEST RANK VALUE UNTIL
// THE NUMBER OF CARDS REQUESTED HAS BEEN FOUND. IF THERE ARE
// AN INSUFFICIENT NUMBER OF UNIQUE CARDS, RETURNS 0.
// IF getConsecutiveCards IS FALSE, IT PROCESSES RANKS FOR WHICH
// THERE IS A SINGLE CARD (SKIPS PAIRS, TRIPS, ETC.). THIS MODE
// IS USED TO FILL-IN REMAINING CARDS TO FORM A 5 CARD HAND IN
// THE CASE OF QUADS, TRIPS, AND PAIRS.
int PokerHandAnalyzer::getSingleCards(int numCards, bool getConsecutiveCards)
{
     // ADD UP THE FIRST 5 CARDS SINCE THEY ARE SORTED
    int highestCard = -1;
    int handStrength = 0;
    int cardCount = 0;
    for (int a=PokerCard::MaxRanksPerDeck-1;a>=0;a--) { // LOOP THRU 14 RANKS
        if (cardCount == numCards) break;
        if ( (getConsecutiveCards == false && RankAccum[a].numCardsAtThisRank == 1) || // ONLY RANKS THAT HAVE A SINGLE CARD *OR*
             (getConsecutiveCards == true &&  RankAccum[a].numCardsAtThisRank > 0) ) { // ONLY RANKS THAT HAVE AT LEAST 1 CARD
               if (highestCard == -1) highestCard = a; // CARDS ARE SORTED, FIRST ONE FOUND IS HIGHEST
               handStrength += RankAccum[a].icard[0].weight();
               cardCount++;
               if (HandResultCardCount < 5) {
                   HandResult[HandResultCardCount] =  RankAccum[a].icard[0].toPokerCard();
                   HandResultCardCount++;
               }
               else {
                   cerr << "POKER HAND ANALYZER ERROR. getSingleCards OVERRAN HAND RESULTS ARRAY\n";
               }
        }
    }

    // BUILD THE RESULT STRING
    ResultString = RankAccum[highestCard].icard[0].RankString(); + "-high";

    handStrength = handStrength * StrengthMultiplier;

    if (HandResultCardCount != 5) { // DID NOT FIND ENOUGH SINGLE CARDS
        return(0);
    }

    return(handStrength);
}



//====================================================================================================//
//================================= MISCELLANEOUS HAND QUERIES =======================================//
//====================================================================================================//

// RETURNS: THE INDEX OF THE HIGHEST RANKED FLUSH CARD
//          -1 IF NO FLUSH EXISTS IN THE CURRENT HAND
int PokerHandAnalyzer::queryFlushIndex()
{
    return(HighFlushIndex);
}

int  PokerHandAnalyzer::queryStraightIndex()
{
    if (HasStraight) {
        return(HighStraightIndex);
    }
    return(-1);
}

int PokerHandAnalyzer::queryQuadsRank()
{
    return(QuadRank);
}

// RETURNS: -1 IF NO THREE OF A KIND
int PokerHandAnalyzer::queryTripsRank()
{
    return(TripRank);
}

// RETURNS: -1 IF NO PAIR
int PokerHandAnalyzer::queryPairRank1()
{
    return(PairRank1);
}

// RETURNS: -1 IF NO SECOND PAIR
int PokerHandAnalyzer::queryPairRank2()
{
    return(PairRank2);
}




//====================================================================================================//
//======================================== PRIVATE FUNCTIONS =========================================//
//====================================================================================================//
void PokerHandAnalyzer::PrintHandStatus()
{
    cout << "\n\n***** HAND STATUS *****  \nHAND: ";
    for (int a=0;a<TotalCards;a++) {
        cout << ICards[a].ToString() << " ";
    }
    cout << endl;

    if (FlushCharacter != 'N') cout << "HAS FLUSH. SUIT IS " << FlushCharacter << " HIGH CARD IS " << ICards[HighFlushIndex].ToString() << endl;
    if (HasStraight)           cout << "HAS STRAIGHT. HIGH CARD IS " << ICards[HighStraightIndex].ToString() << endl;
    if (QuadRank >= 0)         cout << "HAS QUADS, RANK = " << RankAccum[QuadRank].icard[0].ToString() << endl;
    if (TripRank >= 0)         cout << "HAS 3 OF A KIND, RANK = " << RankAccum[TripRank].icard[0].ToString() << endl;
    if (PairRank1 >= 0)        cout << "HAS AT LEAST ONE PAIR, RANK = " << RankAccum[PairRank1].icard[0].ToString() << endl;
    if (PairRank2 >= 0)        cout << "HAS TWO PAIR, RANK = " << RankAccum[PairRank2].icard[0].ToString() << endl;
    cout << "HIGH CARD IS " << ICards[0].ToString() << endl;

}


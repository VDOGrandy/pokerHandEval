// pokerhandanalyzer.h
// CALL THIS CLASS TO PERFORM ANALYSIS OF A COLLECTION OF POKER CARDS
// THIS DOES NOT DETERMINE WINNING HANDS, IT SIMPLY COLLECTS AND
// SUMMARIZES STATISTICS INFORMATION ABOUT THE POKER HAND AS IT
// RELATES TO THE truthDeck PASSED IN
#ifndef POKERHANDANALYZER_H
#define POKERHANDANALYZER_H
#include "pokercard.h"
#include "indexedpokercard.h"
#include "evaluationdeck.h"
#include "pokercardcollection.h"

namespace phaConstants {

    // HAND STRENGTH VALUES
    const int STRAIGHTFLUSH_OFFSET  = 25000000;
    const int QUADS_OFFSET          = 24000000;
    const int QUADS_FACTOR          = 10000;
    const int FULLHOUSE_FACTOR      = 23000000;
    const int FLUSH_FACTOR          = 22000000;
    const int STRAIGHT_FACTOR       = 21000000;
    const int TRIPS_OFFSET          = 20000000;
    const int TRIPS_FACTOR          = 10000;
    const int PAIR_OFFSET           = 20000;
    const int PAIR_FACTOR           = 200000;
    const int PAIR2_OFFSET          = 5000000;
    const int PAIR2_FACTOR          = 10000;    // MAX HAND=AAKQJ=134,480

    // RANGES
    const int MAX_CARDS_TO_ANALYZE  = 13; // MAXIMUM CARDS THIS CLASS CAN PROCESS
    const int MAX_RESULT_CARDS      =  5; // POKER RESULTS IN A 5 CARD HAND
    const int OVERHAND_VALUE        = STRAIGHTFLUSH_OFFSET * 2; // BIGGER THAN ANY HAND STRENGTH
}

// THE RankAccumStruct CONTAINS A TOTAL NUMBER OF CARDS OF EACH
// POSSIBLE RANK. IT ALSO CONTAINS THE 2-4 CARDS THAT MAKE UP
// EACH RANK.
struct RankAccumStruct {
    int              numCardsAtThisRank; // 1=SINGLE, 2=PAIR, 3=TRIPS, OR 4=QUADS
    IndexedPokerCard icard[4];            // THE PAIRED CARDS THEMSELVES
};

class PokerHandAnalyzer
{
public:
                 PokerHandAnalyzer();  // CONSTRUCTOR
    int          Analyze(EvaluationDeck truthDeck, PokerCardCollection theCards);
    int          size();
    PokerCard    resultCard(int whichCard);
    void         PrintHandResultArray();
    std::string  getResultString();
    std::string  getResultHandString();
    void         setAceCanBeLow(bool aceIsLow);
    void         PrintHandStatus();

    // HAND QUALITY FLAGS
    bool hasQuads();
    bool hasFullHouse();
    bool hasFlush();
    bool hasStraight();
    bool hasTrips();
    bool has2Pair();
    bool has1Pair();
    int  singleCardCount();
    int  getSuitCount();

    // OBTAIN BEST 5 CARD HANDS
    int getEvalStraightFlush();
    int getEvalQuads();
    int getEvalFullHouse();
    int getEvalFlush();
    int getEvalStraight();
    int getEvalTrips();
    int getEvalTopPair();
    int getEvalSecondPair();
    int getEvalConsecutiveCards(int numCards);
    int getEvalRemainingCards(int numCards);
    int getSingleCards(int numCards, bool getConsecutiveCards);

    // MISCELLANEOUS HAND INFORMATION
    int queryFlushIndex();
    int queryStraightIndex();
    int queryQuadsRank();
    int queryTripsRank();
    int queryPairRank1();
    int queryPairRank2();

private:
    int                    TotalCards;                            // TOTAL CARDS IN HAND PLUS BOARD
    struct RankAccumStruct RankAccum[PokerCard::MaxRanksPerDeck]; // TOTAL COUNT OF EACH CARD RANK
    int                    ICardCount;
    IndexedPokerCard       ICards[phaConstants::MAX_CARDS_TO_ANALYZE];
    int                    HandResultCardCount;           // CURRENT # OF CARDS IN HAND RESULT ARRAY
    PokerCard              HandResult[phaConstants::MAX_RESULT_CARDS];  // HAND RESULT ARRAY

    static bool RankIndexCompareAscending(IndexedPokerCard a, IndexedPokerCard b);
    static bool RankIndexCompareDescending(IndexedPokerCard a, IndexedPokerCard b);


    // SETTINGS
    bool AceIsLowForStraight;
    int  StrengthMultiplier;

    // FLUSH STATISTICS
    char  FlushCharacter;
    int   HighFlushIndex;
    int   SuitCount;

    // STRAIGHT STATISTICS
    bool  HasStraight;
    int   HighStraightIndex; // HIGHEST STRAIGHT CARD INDEX

    // DUPLICATE RANK (PAIR/TRIP/QUAD) STATISTICS
    int   QuadRank;
    int   TripRank;
    int   PairRank1;
    int   PairRank2;

    int   SingleCount;

    std::string       ResultString;

};

#endif // POKERHANDANALYZER_H

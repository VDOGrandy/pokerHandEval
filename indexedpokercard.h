// indexedpokercard.h (INHERITS FROM pokercard.h)
// RANK INDEXES MUST BE IN THE RANGE FROM 0 - 13 TO ACCOUNT FOR LOW ACE AND HIGH ACE
// THE WEIGHT WILL BE ASSIGNED BASED ON THE RANK INDEX. WEIGHT ENSURES THAT HIGH CARDS
// HAVE SUFFICIENT MATHEMATICAL SEPARATION SUCH THAT THE 4 LOWER CARDS MAY NOT ADD UP
// TO THE VALUE OF THE WEIGHTED CARD (i.e. TOTAL SUM OF 8+9+T+J DO NOT ADD UP TO Q
#ifndef INDEXEDPOKERCARD_H
#define INDEXEDPOKERCARD_H
#include "pokercard.h"

class IndexedPokerCard : public PokerCard
{
public:
    IndexedPokerCard();
    IndexedPokerCard(char theRank, char theSuit, int rankIndex, int weight);
    void      set(char theRank, char theSuit, int rankIndex, int weight);
    int       rank();
    int       weight();
    bool      IsValid();
    PokerCard toPokerCard();
    int       RankIndex;

private:
    int       RankWeight;
    bool      Valid;
};

#endif // INDEXEDPOKERCARD_H

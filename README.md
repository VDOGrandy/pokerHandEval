# pokerHandEval
A collection of c++ classes that enable and simplify the creation of historic and new poker hand evaluators. 

The project currently includes evaluators for the three most common poker hand evaluations:
- Standard High (Most poker games)
- Ace-to-Five (Razz, Omaha Hi/Lo, Stud Hi/Lo)
- Deuce-to-Seven aka Kansas City Lowball (Deuce-to-Seven, Badeucey)

OVERVIEW
The first step is to assemble a truth deck in pokerhandevaluator.cpp (or use one of those already defined). The truth deck identifies the relative rank value of each card. Step 2 is to use the pokerhandanalyzer class to collect information about the hand, based on the truth deck. The final step is to incorporate the logic necessary to support the new evaluation into pokerhandevaluator.cpp. Of course, you may choose to leave pokerhandevaluator as-is and create a separate class for supporting your new evaluator. 

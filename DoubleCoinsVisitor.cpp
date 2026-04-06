/**
* @file DoubleCoinsVisitor.cpp
 * @author ander
 * @brief Implementation of CDoubleCoinsVisitor.
 */

#include "pch.h"
#include "DoubleCoinsVisitor.h"
#include "Coin.h"


void CDoubleCoinsVisitor::VisitCoin(CCoin* coin)
{
    // Only double the coin if it's not collected AND not *already* doubled
    if (!coin->IsCollected() && !coin->IsDoubled())
    {
        coin->SetValue(2 * coin->GetValue());
        coin->SetDoubled(); // Set the flag so it can't run again
    }
}

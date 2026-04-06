/**
* @file DoubleCoinsVisitor.h
 * @author ander
 * @brief Visitor to double the value of coins.
 */

#ifndef PROJECT1_DOUBLECOINSVISITOR_H
#define PROJECT1_DOUBLECOINSVISITOR_H

#include "ItemVisitor.h"
#include "Item.h"
#include "Coin.h"
#include <memory>

/**
 * @class CDoubleCoinsVisitor
 * @brief Visitor that doubles the value of coins.
 *
 * This class implements the visitor pattern for CCoin objects.
 * It can be passed to each coin to double its value.
 */
class CDoubleCoinsVisitor : public CItemVisitor
{
public:
    /**
     * Visit a general item.
     * Calls Accept on the item to dispatch to the correct Visit function.
     * @param item Shared pointer to the item.
     */
    void DoubleCoin(std::shared_ptr<CItem> item) { item->Accept(this); }

    /**
     * Visit a coin and double its value if it has not been collected.
     * @param coin Pointer to the coin object.
     */
    virtual void VisitCoin(CCoin* coin) override;
};

#endif // PROJECT1_DOUBLECOINSVISITOR_H

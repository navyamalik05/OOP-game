/**
* @file LevelTest.cpp
 * @author Hartejas Sekhon, Jack Finlan-Hitt
 * @brief Unit tests for the CLevel class to verify XML data loading.
 */

#include <pch.h>
#include <gtest/gtest.h>


// Include the headers for the classes we need to test
#include <Level.h>
#include <Platform.h>
#include <Background.h>

/**
 * Test fixture to ensure CLevel correctly parses data from level0.xml.
 *
 * This test acts as a specification for the CLevel constructor. It verifies
 * that the level's dimensions, player start position, and all items are
 * loaded with the correct data attributes from the XML file.
 */
TEST(LevelTest, Level0DataLoading) {
    // Create a dummy game for the level
    CGame game;
    CLevel level(&game, L"levels/level0.xml");

    ASSERT_EQ(level.GetWidth(), 1024);
    ASSERT_EQ(level.GetHeight(), 1024);
    ASSERT_EQ(level.GetStartX(), 468);
    ASSERT_EQ(level.GetStartY(), 572);

    // Use visitor pattern instead of dynamic_cast
    class TestVisitor : public CItemVisitor {
    public:
        int platformCount = 0;
        int backgroundCount = 0;
        CPlatform* lastPlatform = nullptr;
        CBackground* lastBackground = nullptr;

        void VisitPlatform(CPlatform* p) override {
            platformCount++;
            lastPlatform = p;
        }

        void VisitBackground(CBackground* b) override {
            backgroundCount++;
            lastBackground = b;
        }
    };

    TestVisitor visitor;
    for (auto& itemPtr : level.GetItems()) {
        itemPtr->Accept(&visitor);
    }

    ASSERT_EQ(visitor.platformCount, 1);
    ASSERT_EQ(visitor.backgroundCount, 2); // level0.xml has 2 backgrounds now

    ASSERT_NEAR(visitor.lastPlatform->GetX(), 464, 0.001);
    ASSERT_NEAR(visitor.lastPlatform->GetY(), 720, 0.001);
}
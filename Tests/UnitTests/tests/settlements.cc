#include <gtest/gtest.h>
#include "Settlement.h"

// Test fixture for Settlement class
class SettlementTest : public ::testing::Test {
protected:
    // Set up some common objects used in multiple tests
    Settlement villageSettlement{"Smallville", SettlementType::VILLAGE};
    Settlement citySettlement{"BigCity", SettlementType::CITY};
    Settlement metropolisSettlement{"MegaCity", SettlementType::METROPOLIS};
};

// Test the getName() method
TEST_F(SettlementTest, GetNameReturnsCorrectName) {
    EXPECT_EQ(villageSettlement.getName(), "Smallville");
    EXPECT_EQ(citySettlement.getName(), "BigCity");
    EXPECT_EQ(metropolisSettlement.getName(), "MegaCity");
}

// Test the getType() method
TEST_F(SettlementTest, GetTypeReturnsCorrectType) {
    EXPECT_EQ(villageSettlement.getType(), SettlementType::VILLAGE);
    EXPECT_EQ(citySettlement.getType(), SettlementType::CITY);
    EXPECT_EQ(metropolisSettlement.getType(), SettlementType::METROPOLIS);
}

// Test the toString() method
TEST_F(SettlementTest, ToStringReturnsCorrectFormat) {
    EXPECT_EQ(villageSettlement.toString(), "Smallville (VILLAGE)");
    EXPECT_EQ(citySettlement.toString(), "BigCity (CITY)");
    EXPECT_EQ(metropolisSettlement.toString(), "MegaCity (METROPOLIS)");
}

#include <gtest/gtest.h>
#include "Facility.h"

// Test Fixture for FacilityType
class FacilityTypeTest : public ::testing::Test {
protected:
    FacilityTypeTest() : facilityType("Hospital", FacilityCategory::ECONOMY, 500, 80, 60, 70) {}
    FacilityType facilityType;
};

TEST_F(FacilityTypeTest, GetName) {
    EXPECT_EQ(facilityType.getName(), "Hospital");
}

TEST_F(FacilityTypeTest, GetCost) {
    EXPECT_EQ(facilityType.getPrice(), 500);
}

TEST_F(FacilityTypeTest, GetLifeQualityScore) {
    EXPECT_EQ(facilityType.getLifeQualityScore(), 80);
}

TEST_F(FacilityTypeTest, GetEnvironmentScore) {
    EXPECT_EQ(facilityType.getEnvironmentScore(), 70);
}

TEST_F(FacilityTypeTest, GetEconomyScore) {
    EXPECT_EQ(facilityType.getEconomyScore(), 60);
}

TEST_F(FacilityTypeTest, GetCategory) {
    ASSERT_EQ(facilityType.getCategory(), FacilityCategory::ECONOMY);
}

// Test Fixture for Facility
class FacilityTest : public ::testing::Test {
protected:
    FacilityTest() : facilityType("Hospital", FacilityCategory::ECONOMY, 500, 80, 60, 70),
                     facility(facilityType, "New York") {}
    FacilityType facilityType;
    Facility facility;
};

TEST_F(FacilityTest, GetSettlementName) {
    EXPECT_EQ(facility.getSettlementName(), "New York");
}

TEST_F(FacilityTest, GetStatusInitial) {
    EXPECT_EQ(facility.getStatus(), FacilityStatus::UNDER_CONSTRUCTIONS);
}

TEST_F(FacilityTest, SetStatusOperational) {
    facility.setStatus(FacilityStatus::OPERATIONAL);
    EXPECT_EQ(facility.getStatus(), FacilityStatus::OPERATIONAL);
}

TEST_F(FacilityTest, StepReducesTimeLeft) {
    int initialTimeLeft = facility.getTimeLeft();
    facility.step();
    EXPECT_EQ(facility.getTimeLeft(), initialTimeLeft - 1);
}

TEST_F(FacilityTest, StepChangesStatusToOperational) {
    for(int i = 0; i < facilityType.getPrice(); i++) {
        facility.step();
    }
    EXPECT_EQ(facility.getStatus(), FacilityStatus::OPERATIONAL);
}

TEST_F(FacilityTest, ToStringReturnsName) {
    EXPECT_EQ(facility.toString(), "Hospital");
}


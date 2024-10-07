#include <gtest/gtest.h>
#include "Plan.h"
#include "SelectionPolicy.h" // Assuming mock for SelectionPolicy for testing
#include <iostream>

class PlanTest : public ::testing::Test {
protected:
    Plan* plan;
    vector<FacilityType> facilityOptions;

    // SetUp method for initializing the members
    virtual void SetUp() override {
        // Initialize the objects inside the SetUp method
        const FacilityType facilitType1 = FacilityType("Hospital", FacilityCategory::ECONOMY, 4, 3, 4, 3);
        const FacilityType facilitType2 = FacilityType("Park", FacilityCategory::ENVIRONMENT, 4, 2, 1, 2);
        facilityOptions.push_back(facilitType1);
        facilityOptions.push_back(facilitType2);
        plan = new Plan(1, Settlement("SettlementA", SettlementType::CITY), new NaiveSelection(), facilityOptions);
    }

    // TearDown method for cleanup
    virtual void TearDown() override {
        // Clean up dynamically allocated memory
        delete plan;
    }
};

TEST_F(PlanTest, GetLifeQualityScore) {
    int lifeQualityScore = plan->getlifeQualityScore();
    EXPECT_EQ(lifeQualityScore, 0);
}

TEST_F(PlanTest, GetEconomyScore) {
    EXPECT_EQ(plan->getEconomyScore(), 0);
}

TEST_F(PlanTest, GetEnvironmentScore) {
    EXPECT_EQ(plan->getEnvironmentScore(), 0);
}

TEST_F(PlanTest, GetSelectionPolicyName) {
    EXPECT_EQ(plan->getSelectionPolicyName(), "nve");
}

TEST_F(PlanTest, SetSelectionPolicy) {
    SelectionPolicy* newPolicy = new EconomySelection();
    plan->setSelectionPolicy(newPolicy);
    EXPECT_EQ(plan->getSelectionPolicyName(), "eco"); // Assuming new policy has the same name
}

TEST_F(PlanTest, AddFacility) {
    FacilityType facilityType1 = FacilityType("Cinema",FacilityCategory::LIFE_QUALITY,5,3,2,1);
    Facility *facility = new Facility(facilityType1, "SettlementA");
    facility->setStatus(FacilityStatus::OPERATIONAL);
    plan->addFacility(facility);
    
    const vector<Facility*>& facilities = plan->getFacilities();
    EXPECT_EQ(facilities.size(), 1);
    EXPECT_EQ(facilities[0]->getName(), "Cinema");
}

TEST_F(PlanTest, StepFunctionality) {
    for(int i = 0; i < 4; i++) {
        plan->step();
    }
    EXPECT_EQ(plan->getFacilities().size(), 2);
}


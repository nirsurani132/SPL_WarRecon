#include <gtest/gtest.h>
#include "SelectionPolicy.h" // Adjust the include path as needed
#include "Facility.h" // Ensure this is included for FacilityType

// Test fixture for SelectionPolicy classes
class SelectionPolicyTest : public ::testing::Test {
protected:
    vector<FacilityType> facilitiesOptions;

    // Set up common test data
    void SetUp() override {
        // Populate facilitiesOptions with sample Facility objects
        facilitiesOptions.push_back(FacilityType("Park", FacilityCategory::ENVIRONMENT, 100, 2, 1, 2));
        facilitiesOptions.push_back(FacilityType("Hospital", FacilityCategory::ECONOMY, 200, 3, 4, 3));
        facilitiesOptions.push_back(FacilityType("Factory", FacilityCategory::ECONOMY, 150, 2, 1, 3));
    }

    void addFacilities(){
        facilitiesOptions.push_back(FacilityType("School", FacilityCategory::ENVIRONMENT, 150, 10, 5, 3));
        facilitiesOptions.push_back(FacilityType("Mall", FacilityCategory::ENVIRONMENT, 200, 5, 5, 5));
        facilitiesOptions.push_back(FacilityType("Cinema", FacilityCategory::ECONOMY, 150, 5, 20, 3));
    }
};


TEST_F(SelectionPolicyTest, NaiveSelection) {
    NaiveSelection selection;
    SelectionPolicy* cloned = selection.clone();
    ASSERT_NE(cloned, nullptr) << "Cloned selection policy should not be null.";
    EXPECT_EQ(cloned->toString(), selection.toString());
    const FacilityType& clonedSelected1 = cloned->selectFacility(facilitiesOptions);
    EXPECT_EQ(clonedSelected1.getName(), "Park") << "First selection should be Park(First Element).";
    cloned->selectFacility(facilitiesOptions);
    const FacilityType& originalSelected1 = selection.selectFacility(facilitiesOptions);
    EXPECT_EQ(originalSelected1.getName(), "Park") << "First selection should be Park(First Element).";
    cloned->selectFacility(facilitiesOptions);
    const FacilityType& clonedSelected4 = cloned->selectFacility(facilitiesOptions);
    EXPECT_EQ(clonedSelected4.getName(), "Park") << "Fourth selection should be Park."; 
    delete cloned;
}

TEST_F(SelectionPolicyTest, NaiveSelectionAfterFacilitiesAdded) {
    NaiveSelection selection;
    for(unsigned int i = 0; i < facilitiesOptions.size(); i++){
        selection.selectFacility(facilitiesOptions);
    }
    addFacilities();
    const FacilityType& result = selection.selectFacility(facilitiesOptions);
    EXPECT_EQ(result.getName(), "School") << "Selection should be School.";
}

TEST_F(SelectionPolicyTest, BalancedSelection) {
    BalancedSelection selection(1, 2, 0);
    SelectionPolicy* cloned = selection.clone();
    ASSERT_NE(cloned, nullptr) << "Cloned selection policy should not be null.";
    EXPECT_EQ(cloned->toString(), selection.toString());
    const FacilityType& clonedSelected1 = cloned->selectFacility(facilitiesOptions);
    EXPECT_EQ(clonedSelected1.getName(), "Factory") << "First selection should be Factory.";
    const FacilityType& originalSelected1 = selection.selectFacility(facilitiesOptions);
    EXPECT_EQ(originalSelected1.getName(), "Factory") << "First selection should be Factory.";
    const FacilityType &clonedSelected2 = cloned->selectFacility(facilitiesOptions);
    EXPECT_EQ(clonedSelected2.getName(), "Park") << "Second selection should be Park.";
    const FacilityType& clonedSelected3 = cloned->selectFacility(facilitiesOptions);
    EXPECT_EQ(clonedSelected3.getName(), "Hospital") << "Third selection should be Hospital.";
    const FacilityType& clonedSelected4 = cloned->selectFacility(facilitiesOptions);
    EXPECT_EQ(clonedSelected4.getName(), "Park") << "Fourth selection should be Park.";
    delete cloned;
}


// Test BalancedSelection
TEST_F(SelectionPolicyTest, BalancedSelectionAfterFacilitiesAdded) {
    BalancedSelection selection(1, 2, 0);
    selection.selectFacility(facilitiesOptions);
    addFacilities();
    const FacilityType& result = selection.selectFacility(facilitiesOptions);
    EXPECT_EQ(result.getName(), "Mall") << "Selection should be Mall.";
}

TEST_F(SelectionPolicyTest, EconomySelection) {
    EconomySelection selection;
    SelectionPolicy* cloned = selection.clone();
    ASSERT_NE(cloned, nullptr);
    EXPECT_EQ(cloned->toString(), selection.toString());
    const FacilityType& clonedSelected1 = cloned->selectFacility(facilitiesOptions);
    EXPECT_EQ(clonedSelected1.getName(), "Hospital") << "First selection should be Hospital";
    const FacilityType& originalSelected1 = selection.selectFacility(facilitiesOptions);
    EXPECT_EQ(originalSelected1.getName(), "Hospital") << "First selection should be Hospital";
    const FacilityType& clonedSelected2 = cloned->selectFacility(facilitiesOptions);
    EXPECT_EQ(clonedSelected2.getName(), "Factory") << "Second selection should be Factory";
    const FacilityType& clonedSelected3 = cloned->selectFacility(facilitiesOptions);
    EXPECT_EQ(clonedSelected3.getName(), "Hospital") << "Third selection should be Hospital";
    delete cloned;
}

// Test EconomySelection
TEST_F(SelectionPolicyTest, EconomySelectionAfterFacilitiesAdded) {
    EconomySelection selection;
    selection.selectFacility(facilitiesOptions);
    selection.selectFacility(facilitiesOptions);
    addFacilities();
    const FacilityType& result = selection.selectFacility(facilitiesOptions);
    EXPECT_EQ(result.getName(), "Cinema"); // Adjust based on actual selection logic
}

TEST_F(SelectionPolicyTest, SustainabilitySelection) {
    SustainabilitySelection selection;
    SelectionPolicy* cloned = selection.clone();
    ASSERT_NE(cloned, nullptr);
    EXPECT_EQ(cloned->toString(), selection.toString());
    const FacilityType& clonedSelected1 = cloned->selectFacility(facilitiesOptions);
    EXPECT_EQ(clonedSelected1.getName(), "Park") << "First selection should be Park.";
    const FacilityType& originalSelected1 = selection.selectFacility(facilitiesOptions);
    EXPECT_EQ(originalSelected1.getName(), "Park") << "First selection should be Park.";
    delete cloned;
}

// Test EconomySelection
TEST_F(SelectionPolicyTest, SustainabilitySelectionAfterFacilitiesAdded) {
    SustainabilitySelection selection;
    selection.selectFacility(facilitiesOptions);
    addFacilities();
    const FacilityType& result = selection.selectFacility(facilitiesOptions);
    EXPECT_EQ(result.getName(), "School");
    const FacilityType& result2 = selection.selectFacility(facilitiesOptions);
    EXPECT_EQ(result2.getName(), "Mall"); 
    const FacilityType& result3 = selection.selectFacility(facilitiesOptions);
    EXPECT_EQ(result3.getName(), "Park"); 
}
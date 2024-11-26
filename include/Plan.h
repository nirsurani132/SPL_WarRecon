#pragma once
#include <vector>
#include <tuple>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;
using std::tuple;


enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        //Copy constructor
        Plan(const Plan &other);
        //Move constructor
        Plan(Plan &&other);
        ~Plan();
        Plan& operator=(const Plan&) = delete;
        Plan& operator=(Plan&&) = delete;
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        const string getSelectionPolicyName() const;
        const Settlement &getSettlement() const;
        const int getPlanID() const;
        //getSelectionPolicy
        const SelectionPolicy *getSelectionPolicy() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        tuple<int,int,int> evaluateScores();
        const vector<Facility*> &getFacilities() const;
        const vector<Facility*> &getUnderConstruction() const;
        void addFacility(Facility* facility);
        const string toString() const;

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};
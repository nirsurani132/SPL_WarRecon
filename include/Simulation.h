#pragma once
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        //Copy constructor
        Simulation(const Simulation &other);
        //Move constructor
        Simulation(Simulation &&other);
        //Copy assignment operator
        Simulation& operator=(const Simulation &other);
        //Move assignment operator
        Simulation& operator=(Simulation &&other);
        ~Simulation();
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        const int getPlanCounter() const;
        void step();
        void printActionsLog();
        void close();
        void open();

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement> settlements;
        vector<FacilityType> facilitiesOptions;

};
#include "Simulation.h"
#include "Action.h"
#include "SelectionPolicy.h"
#include "Auxiliary.h"
//For parsing the config file
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
using namespace std;

Simulation::Simulation(const string &configFilePath): isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions(){
    ifstream configFile(configFilePath);
    if (!configFile) {
        throw invalid_argument("Unable to open config file");
    }
    string line;
    while (getline(configFile, line)) {
        //Ignore empty lines
        if (line.empty()) {
            continue;
        }
        //Ignore comments
        if (line[0] == '#') {
            continue;
        }
        //Parse the line with switch-case
        //First, find the first space
        vector<string> arguments = Auxiliary::parseArguments(line);
        const string &command = arguments[0];
        if (command == "settlement") {
            //Create a new settlement
            SettlementType type;
            switch(stoi(arguments[2])){
                case 0:
                    type = SettlementType::VILLAGE;
                    break;
                case 1:
                    type = SettlementType::CITY;
                    break;
                case 2:
                    type = SettlementType::METROPOLIS;
                    break;
                default:
                    throw invalid_argument("Unknown settlement type: " + arguments[2]);
            }
            Settlement settlement(arguments[1], type);
            addSettlement(settlement);
        } else if (command == "facility") {
            FacilityCategory category;
            switch(stoi(arguments[2])){
                case 0:
                    category = FacilityCategory::LIFE_QUALITY;
                    break;
                case 1:
                    category = FacilityCategory::ECONOMY;
                    break;
                case 2:
                    category = FacilityCategory::ENVIRONMENT;
                    break;
                default:
                    throw invalid_argument("Unknown facility type: " + arguments[2]);
            }
            FacilityType facility(arguments[1], category, stoi(arguments[3]),stoi(arguments[4]),stoi(arguments[5]),stoi(arguments[6]));
            facilitiesOptions.push_back(facility);
        }
        else if(command == "plan"){
            //Create a new plan
            const string &selectionPolicy = arguments[2];
            SelectionPolicy *policy;
            if(selectionPolicy == "nve"){
                policy = new NaiveSelection();
            }
            else if(selectionPolicy == "bal"){
                policy = new BalancedSelection(0,0,0);
            }
            else if(selectionPolicy == "eco"){
                policy = new EconomySelection();
            }
            else if(selectionPolicy == "sus"){
                policy = new SustainabilitySelection();
            }
            else{
                throw invalid_argument("Unknown selection policy: " + selectionPolicy);
            }
            const Settlement &settlement = getSettlement(arguments[1]);
            addPlan(settlement,policy);
        }
    }
};

void Simulation::step(){
    for(Plan &plan : plans){
        plan.step();
    }
};

bool Simulation::addSettlement(Settlement settlement){
    for(const Settlement &existingSettlement : settlements){
        if(existingSettlement.getName() == settlement.getName()){
            return false;
        }
    }
    settlements.push_back(settlement);
    return true;
};

Settlement &Simulation::getSettlement(const string &settlementName){
    for(Settlement &settlement : settlements){
        if(settlement.getName() == settlementName){
            return settlement;
        }
    }
    throw invalid_argument("Settlement not found: " + settlementName);
};

bool Simulation::addFacility(FacilityType facility){
    for(const FacilityType &existingFacility : facilitiesOptions){
        if(existingFacility.getName() == facility.getName()){
            return false;
        }
    }
    facilitiesOptions.push_back(facility);
    return true;
};

bool Simulation::isSettlementExists(const string &settlementName){
    for(const Settlement &settlement : settlements){
        if(settlement.getName() == settlementName){
            return true;
        }
    }
    return false;
};

Plan &Simulation::getPlan(const int planID){
    return plans[planID];
};

void Simulation::start(){
    open();
    while(isRunning){
        // get next command
        BaseAction *action;
        string command;
        getline(cin, command);
        vector<string> arguments = Auxiliary::parseArguments(command);
        const string &requestedAction = arguments[0];
        if(requestedAction == "plan"){
            const string& settlementName = arguments[1];
            const string &selectionPolicy = arguments[2];
            action = new AddPlan(settlementName,selectionPolicy);
        }
        else if(requestedAction == "settlement"){
            const string &settlementName = arguments[1];
            SettlementType settlementType;
            switch(stoi(arguments[2])){
                case 0:
                    settlementType = SettlementType::VILLAGE;
                    break;
                case 1:
                    settlementType = SettlementType::CITY;
                    break;
                case 2:
                    settlementType = SettlementType::METROPOLIS;
                    break;
                default:
                    throw invalid_argument("Unknown settlement type: " + arguments[2]);
            }
            action = new AddSettlement(settlementName,settlementType);
        }
        else if(requestedAction == "facility"){
            const string &facilityName = arguments[1];
            const int facilityPrice = stoi(arguments[2]);
            const FacilityCategory facilityCategory = FacilityCategory(stoi(arguments[3]));
            const int lifeQualityScore = stoi(arguments[4]);
            const int economyScore = stoi(arguments[5]);
            const int environmentScore = stoi(arguments[6]);
            action = new AddFacility(facilityName,facilityCategory,facilityPrice,lifeQualityScore,economyScore,environmentScore);
        }
        else if(requestedAction == "step"){
            const int numOfSteps = stoi(arguments[1]);
            action = new SimulateStep(numOfSteps);
        }
        else if(requestedAction == "planStatus"){
            const int planID = stoi(arguments[1]);
            action = new PrintPlanStatus(planID);
        }
        else if(requestedAction == "changePolicy"){
            const int planID = stoi(arguments[1]);
            const string &newPolicy = arguments[2];
            action = new ChangePlanPolicy(planID,newPolicy);
        }
        else if(requestedAction == "log"){
            action = new PrintActionsLog();
        }
        else if(requestedAction == "close"){
            action = new Close();
        }
        else if(requestedAction == "backup"){
            action = new BackupSimulation();
        }
        else if(requestedAction == "restore"){
            action = new RestoreSimulation();
        }
        else{
            cout << "Unknown command" << endl;
            continue;
        }
        action->act(*this);
        addAction(action);
    }
};

void Simulation::close(){
    isRunning = false;
};

void Simulation::open(){
    isRunning = true;
    cout << "The simulation has started" << endl;
};

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    Plan plan(planCounter++, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(plan);
};

void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
};

const int Simulation::getPlanCounter() const{
    return planCounter;
};

void Simulation::printActionsLog(){
    for(const BaseAction *action : actionsLog){
        cout << action->toString() << endl;
    }
};




//Rule of 5

//Copy constructor
Simulation::Simulation(const Simulation &other): isRunning(other.isRunning), planCounter(other.planCounter), actionsLog(), plans(), settlements(), facilitiesOptions(){
    for(const BaseAction *action : other.actionsLog){
        actionsLog.push_back(action->clone());
    }
    for(const Plan &plan : other.plans){
        plans.push_back(plan);
    }
    for(const Settlement &settlement : other.settlements){
        settlements.push_back(settlement);
    }
    for(const FacilityType &facility : other.facilitiesOptions){
        facilitiesOptions.push_back(facility);
    }
};

//Move constructor
Simulation::Simulation(Simulation &&other): isRunning(other.isRunning), planCounter(other.planCounter), actionsLog(std::move(other.actionsLog)), plans(std::move(other.plans)), settlements(std::move(other.settlements)), facilitiesOptions(std::move(other.facilitiesOptions)) {};

//Copy assignment operator
Simulation& Simulation::operator=(const Simulation &other){
    if(this == &other){
        return *this;
    }
    for(BaseAction *action : actionsLog){
        delete action;
    }
    actionsLog.clear();
    plans.clear();
    planCounter = other.planCounter;
    isRunning = other.isRunning;
    for(const BaseAction *action : other.actionsLog){
        actionsLog.push_back(action->clone());
    }
    for(const Plan &plan : other.plans){
        plans.push_back(plan);
    }
    for(const Settlement &settlement : other.settlements){
        settlements.push_back(settlement);
    }
    for(const FacilityType &facility : other.facilitiesOptions){
        facilitiesOptions.push_back(facility);
    }
    return *this;
};

//Move assignment operator
Simulation& Simulation::operator=(Simulation &&other){
    if(this == &other){
        return *this;
    }
    for(BaseAction *action : actionsLog){
        delete action;
    }
    actionsLog.clear();
    plans.clear();
    planCounter = other.planCounter;
    isRunning = other.isRunning;
    actionsLog = std::move(other.actionsLog);
    plans = std::move(other.plans);
    settlements = std::move(other.settlements);
    facilitiesOptions = std::move(other.facilitiesOptions);
    return *this;
};

Simulation::~Simulation(){
    for(BaseAction *action : actionsLog){
        delete action;
    }
};



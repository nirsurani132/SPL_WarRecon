#include "Action.h"
#include "SelectionPolicy.h"
#include <iostream>

using namespace std;
extern Simulation *backup;

BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::ERROR) {}
ActionStatus BaseAction::getStatus() const {
    return status;
}
void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}
const string &BaseAction::getErrorMsg() const {
    return errorMsg;
}

SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation) {
    for(int i = 0; i < numOfSteps; i++){
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const {
    return "SimulateStep " + std::to_string(numOfSteps) + " " + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) : settlementName(settlementName), selectionPolicy(selectionPolicy) {}
void AddPlan::act(Simulation &simulation) {
    if(!simulation.isSettlementExists(settlementName)){
        error("Cannot create this plan");
        cout << this->getErrorMsg() << endl;
        return;
    }
    const Settlement &planSettlement = simulation.getSettlement(settlementName);
    SelectionPolicy *selectedPolicy;
    if(this->selectionPolicy == "nve"){
        selectedPolicy = new NaiveSelection();
    }
    else if(this->selectionPolicy == "bal"){
        selectedPolicy = new BalancedSelection(0,0,0);    
    }
    else if(this->selectionPolicy == "eco"){
        selectedPolicy = new EconomySelection();
    }
    else if(this->selectionPolicy == "env"){
        selectedPolicy = new SustainabilitySelection();
    }
    else{
        error("Cannot create this plan");
        cout << this->getErrorMsg() << endl;
        return;
    }
    simulation.addPlan(planSettlement,selectedPolicy);
    complete();
}

const string AddPlan::toString() const {
    return "Plan " + settlementName + " " + selectionPolicy + " " + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}

AddPlan *AddPlan::clone() const {
    return new AddPlan(*this);
}

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType) : settlementName(settlementName), settlementType(settlementType) {}
void AddSettlement::act(Simulation &simulation) {
    Settlement newSettlement(settlementName,settlementType);
    if(simulation.addSettlement(newSettlement)){
        complete();
    }
    else{
        error("Settlement already exists");
        cout << this->getErrorMsg() << endl;
    }
}

const string AddSettlement::toString() const {
    return "Settlement " + settlementName + " " + std::to_string(static_cast<int>(settlementType)) + " " + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}

AddSettlement *AddSettlement::clone() const {
    return new AddSettlement(*this);
}

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore): facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation) {
    FacilityType facilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if(simulation.addFacility(facilityType)){
        complete();
    }
    else{
        error("Facility already exists");
        cout << this->getErrorMsg() << endl;
    }
}

const string AddFacility::toString() const {
    return "AddFacility " + facilityName + " " + std::to_string((int)(facilityCategory)) + " " + std::to_string(price) + " " + std::to_string(lifeQualityScore) + " " + std::to_string(economyScore) + " " + std::to_string(environmentScore) + " " + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}

AddFacility *AddFacility::clone() const {
    return new AddFacility(*this);
}

PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}
void PrintPlanStatus::act(Simulation &simulation) {
    if(this->planId >= simulation.getPlanCounter()){
        error("Plan doesn't exist");
        cout << this->getErrorMsg() << endl;
        return;
    }
    simulation.getPlan(planId).printStatus();
    complete();
}

const string PrintPlanStatus::toString() const {
    return "PrintPlanStatus " + std::to_string(planId) + " " + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}

PrintPlanStatus *PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy) : planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation) {
    if(this->planId >= simulation.getPlanCounter()){
        error("Cannot change selection policy");
        cout << this->getErrorMsg() << endl;
        return;
    }
    Plan &plan = simulation.getPlan(planId);
    if(plan.getSelectionPolicyName() == this->newPolicy){
        error("Cannot change selection policy");
        cout << this->getErrorMsg() << endl;
        return;
    }
    SelectionPolicy *selectedPolicy;
    if(this->newPolicy == "nve"){
        selectedPolicy = new NaiveSelection();
    }
    else if(this->newPolicy == "bal"){
        selectedPolicy = new BalancedSelection(plan.getlifeQualityScore(),plan.getEconomyScore(),plan.getEnvironmentScore());
    }
    else if(this->newPolicy == "eco"){
        selectedPolicy = new EconomySelection();
    }
    else if(this->newPolicy == "env"){
        selectedPolicy = new SustainabilitySelection();
    }
    else{
        error("Cannot change plan policy");
        cout << this->getErrorMsg() << endl;
        return;
    }
    plan.setSelectionPolicy(selectedPolicy);
    complete();
}

const string ChangePlanPolicy::toString() const {
    return "ChangePlanPolicy " + std::to_string(planId) + " " + newPolicy + " " + (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
}

ChangePlanPolicy *ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}

PrintActionsLog::PrintActionsLog() {}
void PrintActionsLog::act(Simulation &simulation) {
    simulation.printActionsLog();
    complete();
}

const string PrintActionsLog::toString() const {
    const string &status = (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
    return "PrintActionsLog " + status;
}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

Close::Close() {}
void Close::act(Simulation &simulation) {
    simulation.close();
    complete();
}

const string Close::toString() const {
    return "Close COMPLETED";
}

Close *Close::clone() const {
    return new Close(*this);
}

BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation &simulation) {
    delete backup;
    backup = new Simulation(simulation);
    complete();
}

const string BackupSimulation::toString() const {
    const string &status = (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
    return "BackupSimulation " + status;
}

BackupSimulation *BackupSimulation::clone() const {
    return new BackupSimulation(*this);
}

RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation &simulation) {
    if(backup == nullptr){
        error("No backup available");
        cout << this->getErrorMsg() << endl;
        return;
    }
    simulation = *backup;
    complete();
}

const string RestoreSimulation::toString() const {
    const string &status = (getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR");
    return "RestoreSimulation " + status;
}

RestoreSimulation *RestoreSimulation::clone() const {
    return new RestoreSimulation(*this);
}






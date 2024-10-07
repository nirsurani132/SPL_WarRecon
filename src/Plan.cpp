#include "Plan.h"
#include <iostream>
using namespace std;


Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions) : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE), facilities(), underConstruction(), facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0) {};
const int Plan::getlifeQualityScore() const {
    return life_quality_score;
}

const int Plan::getEconomyScore() const {
    return economy_score;
}

const int Plan::getEnvironmentScore() const {
    return environment_score;
}

const string Plan::getSelectionPolicyName() const {
    return selectionPolicy->toString();
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {
    if (this->selectionPolicy) {
        delete this->selectionPolicy;
    }
    this->selectionPolicy = selectionPolicy;
}

void Plan::step() {
    //Add a facility to the plan
    while(status == PlanStatus::AVALIABLE){
        FacilityType nextFacility = selectionPolicy->selectFacility(facilityOptions);
        Facility* facility = new Facility(nextFacility, settlement.getName());
        addFacility(facility);
        if(static_cast<unsigned int>(settlement.getType()) + 1 == underConstruction.size()){
            status = PlanStatus::BUSY;
        }
    }

    //take a step in the construction of the facilities
    for(int i = underConstruction.size()-1; i >= 0; i--){
        FacilityStatus facilityStatus = underConstruction[i]->step();
        if(facilityStatus == FacilityStatus::OPERATIONAL){
            facilities.push_back(underConstruction[i]);
            life_quality_score += underConstruction[i]->getLifeQualityScore();
            economy_score += underConstruction[i]->getEconomyScore();
            environment_score += underConstruction[i]->getEnvironmentScore();
            underConstruction.erase(underConstruction.begin() + i);
            this->status = PlanStatus::AVALIABLE;
        }
    }



}

void Plan::printStatus(){
    cout << "Plan ID: " << plan_id << endl;
    cout << "Settlement Name: " << settlement.getName() << endl;
    cout << "planStatus: " << (status == PlanStatus::AVALIABLE ? "AVALIABLE" : "BUSY") << endl;
    cout << "selectionPolicy: " << selectionPolicy->toString() << endl;
    cout << "LifeQualityScore: " << life_quality_score << endl;
    cout << "EconomyScore: " << economy_score << endl;
    cout << "EnvironmentScore: " << environment_score << endl;
    for(Facility *facility: facilities){
        cout << "facilityName: " + facility->toString() << endl;
        cout << "facilityStatus: OPERATIONAL" << endl;
    }
    for(Facility *facility: underConstruction){
        cout << "facilityName: " + facility->toString() << endl;
        cout << "facilityStatus: UNDER_CONSTRUCTION" << endl;
    }
}

const vector<Facility*> &Plan::getFacilities() const {
    return facilities;
}
void Plan::addFacility(Facility *facility) {
    if(facility->getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS) {
        underConstruction.push_back(facility);
    }
    else {
        facilities.push_back(facility);
        life_quality_score += facility->getLifeQualityScore();
        economy_score += facility->getEconomyScore();
        environment_score += facility->getEnvironmentScore();
    }
}

const string Plan::toString() const {
    return "Plan ID: " + std::to_string(plan_id) + " Life Quality: " + std::to_string(life_quality_score) + " Economy: " + std::to_string(economy_score) + " Environment: " + std::to_string(environment_score);
}


//Rule of 5

//Copy constructor
Plan::Plan(const Plan &other) : plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy->clone()), status(other.status), facilities(), underConstruction(), facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score) {
    
    for(Facility *facility: other.facilities){
        facilities.push_back(new Facility(*facility));
    }
    for(Facility *facility: other.underConstruction){
        underConstruction.push_back(new Facility(*facility));
    }
}

//Move constructor
Plan::Plan(Plan &&other) : plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy), status(other.status), facilities(std::move(other.facilities)), underConstruction(std::move(other.underConstruction)), facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score) {
    other.selectionPolicy = nullptr;
}

Plan::~Plan() {
    delete selectionPolicy;
    for(Facility *facility: facilities){
        delete facility;
    }
    for(Facility *facility: underConstruction){
        delete facility;
    }
}
#include "SelectionPolicy.h"
#include <algorithm>

NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilities) {
    lastSelectedIndex = (lastSelectedIndex + 1) % facilities.size();
    return facilities[lastSelectedIndex];
}

const string NaiveSelection::toString() const {
    return "nve";
}

NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilities) {
    int minScore = 100;
    int minIndex = 0;
    for(unsigned int i = 0; i < facilities.size(); i++) {
        int facilityLifeQualityScore = facilities[i].getLifeQualityScore() + LifeQualityScore;
        int facilityEconomyScore = facilities[i].getEconomyScore() + EconomyScore;
        int facilityEnvironmentScore = facilities[i].getEnvironmentScore() + EnvironmentScore;
        int currentDistance = std::max(std::abs(facilityLifeQualityScore - facilityEconomyScore), std::max(std::abs(facilityLifeQualityScore - facilityEnvironmentScore), std::abs(facilityEconomyScore - facilityEnvironmentScore)));
        if(currentDistance < minScore) {
            minScore = currentDistance;
            minIndex = i;
        }
    }
    this->LifeQualityScore += facilities[minIndex].getLifeQualityScore();
    this->EconomyScore += facilities[minIndex].getEconomyScore();
    this->EnvironmentScore += facilities[minIndex].getEnvironmentScore();
    return facilities[minIndex];

}

const string BalancedSelection::toString() const {
    return "bal";
}

BalancedSelection* BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}

EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilities) {
    while(true) {
        lastSelectedIndex = (lastSelectedIndex + 1) % facilities.size();
        if(facilities[lastSelectedIndex].getCategory() == FacilityCategory::ECONOMY) {
            return facilities[lastSelectedIndex];
        }
    }
}

const string EconomySelection::toString() const {
    return "eco";
}

EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this);
}

SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilities) {
    while(true) {
        lastSelectedIndex = (lastSelectedIndex + 1) % facilities.size();
        if(facilities[lastSelectedIndex].getCategory() == FacilityCategory::ENVIRONMENT) {
            return facilities[lastSelectedIndex];
        }
    }
}

const string SustainabilitySelection::toString() const {
    return "env";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}
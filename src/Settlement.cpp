#include "Settlement.h"

Settlement::Settlement(const string &name, SettlementType type) : name(name), type(type) {};
const string &Settlement::getName() const {
    return name;
}

SettlementType Settlement::getType() const {
    return type;
}

const string Settlement::toString() const {
    string typeString;
    switch (type) {
        case SettlementType::VILLAGE:
            typeString = "VILLAGE";
            break;
        case SettlementType::CITY:
            typeString = "CITY";
            break;
        case SettlementType::METROPOLIS:
            typeString = "METROPOLIS";
            break;
    }
    return name + " (" + typeString + ")";
}
#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"

int cmg::TriggerObject::getIndex(const std::string& name) const{
    //this list is ordered by bit the bit number of the strbitset
    const cmg::AbstractPhysicsObject::Strings names = getSelectionNames();
    const cmg::AbstractPhysicsObject::Strings::const_iterator it = std::find(names.begin(),names.end(),name);
    int index = -1;
    if(it != names.end()){
        index = it - names.begin();
    }
    return index;        
}
    
void cmg::TriggerObject::addSelectionWithPrescale(const std::string& name, const bool value, const int prescale){
    //first add the selection so that we know the index
    addSelection(name,value);
    const int index = getIndex(name);
    if(index != -1){
        prescales_[index] = prescale;
        //set the minimum prescale value and name
        if(value && (prescale > 0) && (prescale < minPrescale_) ){
            minPrescale_ = prescale;
            name_ = name;
        }
    }
}

int cmg::TriggerObject::getPrescale(const std::string& name) const{
    const int index = getIndex(name);
    return (index != -1) ? prescales_.at(index) : UnSet(int);
}
#include "FWCore/Framework/interface/Event.h"
#include "CMGTools/Common/interface/CutSummaryAnalyzer.h"

#include <algorithm>
#include <iostream>

void cmg::CutSummaryAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
    
    typedef edm::View<cmg::AbstractPhysicsObject> view;
    edm::Handle<view> cands;
    iEvent.getByLabel(inputLabel_,cands);

    for(view::const_iterator it = cands->begin(); it != cands->end(); ++it){
        //get the daughters including the object itself
        cmg::AbstractPhysicsObject::Daughters daughters = it->getDaughters(true);
        for(cmg::AbstractPhysicsObject::Daughters::const_iterator d = daughters.begin(); d != daughters.end(); ++d){
            const cmg::AbstractPhysicsObject::Strings strings = (*d)->getSelectionNames();
            for(cmg::AbstractPhysicsObject::Strings::const_iterator s = strings.begin(); s != strings.end(); ++s){
                    //create the entry if needed
                    if( !counter_.count(*s) ){
                        counter_[*s] = std::make_pair(0,0);
                    }
                    //increment the counter if it passed
                    const bool result = (*d)->getSelection(s->c_str());
                    if(result){
                        counter_[*s].first++;   
                    }
                    counter_[*s].second++;
                }
        }
    }
    
}

void cmg::CutSummaryAnalyzer::endJob(){
    
    std::cout << "cmg::CutSummaryAnalyzer\tSummary of cuts results for collection \"" <<    inputLabel_.label() << "\"." << std::endl; 
    for(CutCounts::const_iterator it = counter_.begin(); it != counter_.end(); ++it){
        const unsigned int before = it->second.second;
        const unsigned int after = it->second.first;
        std::cout << "\t" << it->first << "\t\t\t" << after << "/" << before << "\t(" << after/(1.*before) << ")"  <<std::endl;
    }
}

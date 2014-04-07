#include "FWCore/Framework/interface/Event.h"
#include "CMGTools/Common/plugins/PhysicsObjectPrinter.h"

#include <algorithm>
#include <iostream>

void cmg::PhysicsObjectPrinter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
    
    //bail out early if we don't need to print this event
    if( (eventCount_ % reportEvery_) != 0){
        eventCount_++;
        return;   
    } 
    
    typedef edm::View<cmg::AbstractPhysicsObject> view;
    edm::Handle<view> cands;
    iEvent.getByLabel(inputLabel_,cands);

    const unsigned int run = iEvent.id().run();
    const unsigned int lumi = iEvent.id().luminosityBlock();
    const unsigned int event = iEvent.id().event();

    std::cout << "Run/Lumi/Event: " << run << "/" << lumi << "/" << event << " : Size of " << inputLabel_.label() << ": " << cands->size() << std::endl; 
    unsigned int count = 0;
    for(view::const_iterator it = cands->begin(); it != cands->end(); ++it, count++){
        std::cout << "\t" <<count << ": " << *it << std::endl;
        if(printSelections_){
            it->printSelections(std::cout,"\t\t");   
        } 
        //get the daughters excluding the object itself
        cmg::AbstractPhysicsObject::Daughters daughters = it->getDaughters(false);
        unsigned int dcount = 0;
        for(cmg::AbstractPhysicsObject::Daughters::const_iterator d = daughters.begin(); d != daughters.end(); ++d, dcount++){
            std::cout << "\t\t" << count << ":" << dcount << ": " << **d << std::endl;
            if(printSelections_){
                (*d)->printSelections(std::cout,"\t\t\t");   
            } 
        }
    }
    //increment the number of times called
    eventCount_++;
}

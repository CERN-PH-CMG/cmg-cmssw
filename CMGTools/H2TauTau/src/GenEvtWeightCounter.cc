// -*- C++ -*-
//
// Package:    PhysicsTools/GenEvtWeightCounter
// Class:      GenEvtWeightCounter
// 
/**\class GenEvtWeightCounter GenEvtWeightCounter.cc PhysicsTools/GenEvtWeightCounter/plugins/GenEvtWeightCounter.cc

 Description: saves a collection of generator weights at the end of a Run (or at least at each job)

*/
//
// Original Author:  Riccardo Manzoni
//         Created:  Thu, 06 Aug 2015 09:07:24 GMT
//
//


#include <memory>
#include <iostream>
#include <string>
#include <map>
#include <TH1D.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDProducer.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"


class GenEvtWeightCounter : public edm::one::EDProducer<edm::EndRunProducer>{
    public:
        explicit GenEvtWeightCounter(const edm::ParameterSet&);
        ~GenEvtWeightCounter();

    private:
        virtual void produce(edm::Event&, const edm::EventSetup&) override;

        virtual void beginRun(edm::Run const&, edm::EventSetup const&); 
        virtual void endRun(edm::Run const&, edm::EventSetup const&); 
        virtual void endRunProduce(edm::Run& iRun, edm::EventSetup const&);
                
        bool verbose_;
        std::vector<double> weights_;
};

GenEvtWeightCounter::GenEvtWeightCounter(const edm::ParameterSet& iConfig):
    verbose_  (iConfig.getUntrackedParameter<bool>("verbose", false))
{
   produces<std::vector<double>, edm::InRun>("genWeight");
}


GenEvtWeightCounter::~GenEvtWeightCounter()
{
}


void
GenEvtWeightCounter::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
    Handle<GenEventInfoProduct> genInfoHandle;
    iEvent.getByLabel("generator", genInfoHandle);

    const GenEventInfoProduct *genInfo = genInfoHandle.product();
    const std::vector<double> weights = genInfo -> weights();
   
    if (verbose_)
    {
        std::cout << "\nweight   " << genInfo -> weight()
                  << "\nqScale   " << genInfo -> qScale()
                  << "\nalphaQCD " << genInfo -> alphaQCD()
                  << "\nalphaQED " << genInfo -> alphaQED()
                  << std::endl;
    }
    
    int i = 0;
    
    for (std::vector<double>::const_iterator iweight  = weights.begin(); 
                                             iweight != weights.end()  ;
                                             iweight++, i++)
    {
        if (verbose_) std::cout << "weight #" << i << " = " << *iweight << std::endl;
    }
    
    weights_.push_back(genInfo -> weight());
    
}


void
GenEvtWeightCounter::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
    weights_.clear();
    return;
}

void
GenEvtWeightCounter::endRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
}

void
GenEvtWeightCounter::endRunProduce(edm::Run& iRun, edm::EventSetup const& iSetup)
{
    std::auto_ptr<std::vector<double> > pweights( new std::vector<double>(weights_) ); 
    iRun.put(pweights, "genWeight");
    return;
}

DEFINE_FWK_MODULE(GenEvtWeightCounter);

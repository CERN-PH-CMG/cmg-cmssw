#ifndef TGraphGetterFromDB_h
#define TGraphGetterFromDB_h

#include <iostream>
#include <string>

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/PhysicsToolsObjects/interface/PhysicsTGraphPayload.h"
#include "CondFormats/DataRecord/interface/PhysicsTGraphPayloadRcd.h"
#include <TFile.h>
#include <TGraph.h>


class TGraphGetterFromDB: public edm::one::EDAnalyzer<>
{
    public:
        explicit TGraphGetterFromDB( const edm::ParameterSet & ) ;
        ~TGraphGetterFromDB() override ;
        void analyze( const edm::Event &, const edm::EventSetup & ) override ;

    private:
        std::string theTGraphName;
        std::string theOutputFileName;
        std::string theOutputObjectName;
        edm::ESHandle<PhysicsTGraphPayload> theTGraphHandle;
};

TGraphGetterFromDB::TGraphGetterFromDB( const edm::ParameterSet & conf ) :
    theTGraphName(conf.getParameter<std::string>("tGraphName")),
    theOutputFileName(conf.getUntrackedParameter<std::string>("outputFileName")),
    theOutputObjectName(conf.getUntrackedParameter<std::string>("outputObjectName", theTGraphName.empty() ? "TGraph" : theTGraphName))
{
}

TGraphGetterFromDB::~TGraphGetterFromDB()
{
}

void
TGraphGetterFromDB::analyze( const edm::Event & iEvent, const edm::EventSetup & iSetup ) 
{
    iSetup.get<PhysicsTGraphPayloadRcd>().get(theTGraphName, theTGraphHandle);
    TFile *fOut = TFile::Open(theOutputFileName.c_str(), "RECREATE");
    std::unique_ptr<const TGraph> graph(new TGraph(*theTGraphHandle.product()));
    fOut->WriteObject(graph.get(), theOutputObjectName.c_str());
    fOut->Close();
    std::cout << "Wrote output to " << theOutputFileName << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TGraphGetterFromDB);

#endif

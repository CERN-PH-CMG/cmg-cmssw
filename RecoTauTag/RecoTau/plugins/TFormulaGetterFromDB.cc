#ifndef TFormulaGetterFromDB_h
#define TFormulaGetterFromDB_h

#include <iostream>
#include <string>

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/PhysicsToolsObjects/interface/PhysicsTFormulaPayload.h"
#include "CondFormats/DataRecord/interface/PhysicsTFormulaPayloadRcd.h"
#include <TFile.h>
#include <TFormula.h>
#include <TF1.h>


class TFormulaGetterFromDB: public edm::one::EDAnalyzer<>
{
    public:
        explicit TFormulaGetterFromDB( const edm::ParameterSet & ) ;
        ~TFormulaGetterFromDB() override ;
        void analyze( const edm::Event &, const edm::EventSetup & ) override ;

    private:
        std::string theTFormulaName;
        std::string theOutputFileName;
        std::string theOutputObjectName;
        edm::ESHandle<PhysicsTFormulaPayload> theTFormulaHandle;
};

TFormulaGetterFromDB::TFormulaGetterFromDB( const edm::ParameterSet & conf ) :
    theTFormulaName(conf.getParameter<std::string>("tFormulaName")),
    theOutputFileName(conf.getUntrackedParameter<std::string>("outputFileName")),
    theOutputObjectName(conf.getUntrackedParameter<std::string>("outputObjectName", theTFormulaName.empty() ? "TFormula" : theTFormulaName))
{
}

TFormulaGetterFromDB::~TFormulaGetterFromDB()
{
}

std::unique_ptr<TFormula> loadTFormulaFromDB(const edm::EventSetup& es, const std::string& formulaName, const TString& newName, bool verbosity = false)
{
  if (verbosity) {
    std::cout << "<loadTFormulaFromDB>:" << std::endl;
    std::cout << " formulaName = " << formulaName << std::endl;
  }
  edm::ESHandle<PhysicsTFormulaPayload> formulaPayload;
  es.get<PhysicsTFormulaPayloadRcd>().get(formulaName, formulaPayload);
  
  if ( formulaPayload->formulas().size() == 1 && formulaPayload->limits().size() == 1 ) {
    return std::unique_ptr<TFormula> {new TFormula(newName, formulaPayload->formulas().at(0).data()) };
  } else {
    throw cms::Exception("PATTauDiscriminantCutMultiplexer::loadTFormulaFromDB") 
      << "Failed to load TFormula = " << formulaName << " from Database !!\n";
  }
  return std::unique_ptr<TFormula>{};
} 

void
TFormulaGetterFromDB::analyze( const edm::Event & iEvent, const edm::EventSetup & iSetup ) 
{
  TFile *fOut = TFile::Open(theOutputFileName.c_str(), "RECREATE");  
  std::unique_ptr<TFormula> formula = loadTFormulaFromDB( iSetup, 
							  theTFormulaName, 
							  theTFormulaName, true);
  // beware: use file.Get to access the tformula in root 
  // using formula_name.Eval after opening the root file (automatic access)
  // will lead to a segmentation fault (root bug)
  fOut->WriteObject(formula.get(), theOutputObjectName.c_str());
  fOut->Close();
  std::cout << "Wrote output to " << theOutputFileName << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TFormulaGetterFromDB);

#endif

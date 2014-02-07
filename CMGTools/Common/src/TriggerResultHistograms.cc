#include "CMGTools/Common/interface/TriggerResultHistograms.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TAxis.h"
#include "TH1.h"
#include "TH2.h"

#include <algorithm>

cmg::TriggerResultHistograms::TriggerResultHistograms(const edm::ParameterSet& ps):
    cmg::HistogramCreator<type>::HistogramCreator(ps),
    names_(ps.getUntrackedParameter<std::vector<std::string> >("names",std::vector<std::string>())){
}

void cmg::TriggerResultHistograms::fill(const edm::Event& iEvent, const edm::EventSetup& iSetup){

    edm::Handle<type> trigger;
    iEvent.getByLabel(labelX_,trigger);
    const edm::TriggerNames& triggerNames = iEvent.triggerNames(*trigger);

    edm::TriggerNames::Strings const& s = triggerNames.triggerNames();
    defineHistograms(s);//call the histogram creation as we need the names

    //i.e. you can change the pointer but not the object
    std::vector<std::string>const * filteredPaths = 0;
    if(names_.size()){
        filteredPaths = &names_;
    }else{
        filteredPaths = &s;
    }

    TH2* corr = get2DHistogram("triggerCorrelation");
    TH1* passed = get1DHistogram("passedTriggers");
    TH1* failed = get1DHistogram("failedTriggers");
    TH1* run = get1DHistogram("runTriggers");

    Int_t binX = 1;
    Int_t binY = 1;

    for(std::vector<std::string>::const_iterator it = filteredPaths->begin(); it != filteredPaths->end(); ++it){
        const unsigned int i = triggerNames.triggerIndex(*it);
        if(i >= triggerNames.size()) continue;
        binY = 1;
        for(std::vector<std::string>::const_iterator jt = filteredPaths->begin(); jt != filteredPaths->end(); ++jt){
            const unsigned int j = triggerNames.triggerIndex(*jt);
            if(j >= triggerNames.size()) continue;
            //require that both triggers were run
            if( (trigger->wasrun(i) && trigger->accept(i)) && (trigger->wasrun(j) && trigger->accept(j)) ){
                const Double_t binContent = corr->GetBinContent(binX,binY);
                corr->SetBinContent(binX,binY,binContent+1.0);
            }
            binY++;
        }
        if(trigger->wasrun(i)){
            if(trigger->accept(i)){
                const Double_t binContent = passed->GetBinContent(binX);
                passed->SetBinContent(binX,binContent+1);
            }else{
                const Double_t binContent = failed->GetBinContent(binX);
                failed->SetBinContent(binX,binContent+1);
            }
            const Double_t binContent = run->GetBinContent(binX);
            run->SetBinContent(binX,binContent+1);
        }
        binX++;
    }
}

void cmg::TriggerResultHistograms::defineHistograms(const std::vector<std::string>& trigPaths){

    if(cmg::HistogramCreator<type>::histosInitialized_)
      return;

    //i.e. you can change the pointer but not the object
    std::vector<std::string>const * filteredPaths = 0;
    if(names_.size()){
        filteredPaths = &names_;
    }else{
        filteredPaths = &trigPaths;
    }
    const unsigned int nPaths = filteredPaths->size();

    //passed and failed triggers
    add1DHistogram(
        "passedTriggers","passedTriggers",
        nPaths,0,nPaths,
        cmg::HistogramCreator<type>::fs_.operator->());
    add1DHistogram(
        "failedTriggers","failedTriggers",
        nPaths,0,nPaths,
        cmg::HistogramCreator<type>::fs_.operator->());
    add1DHistogram(
        "runTriggers","runTriggers",
        nPaths,0,nPaths,
        cmg::HistogramCreator<type>::fs_.operator->());

    //correlation plot
    add2DHistogram(
        "triggerCorrelation","triggerCorrelation",
        nPaths,0,nPaths,
        nPaths,0,nPaths,
        cmg::HistogramCreator<type>::fs_.operator->());

    TAxis* x = get2DHistogram("triggerCorrelation")->GetXaxis();
    TAxis* y = get2DHistogram("triggerCorrelation")->GetYaxis();

    TAxis* xPassed = get1DHistogram("passedTriggers")->GetXaxis();
    TAxis* xFailed = get1DHistogram("failedTriggers")->GetXaxis();
    TAxis* xRun = get1DHistogram("runTriggers")->GetXaxis();

    for(unsigned int i = 0; i < nPaths; i++){
        const std::string name = filteredPaths->at(i);

        x->SetBinLabel(static_cast<Int_t>(i+1),name.c_str());
        y->SetBinLabel(static_cast<Int_t>(i+1),name.c_str());

        xPassed->SetBinLabel(static_cast<Int_t>(i+1),name.c_str());
        xFailed->SetBinLabel(static_cast<Int_t>(i+1),name.c_str());
        xRun->SetBinLabel(static_cast<Int_t>(i+1),name.c_str());
    }

    cmg::HistogramCreator<type>::histosInitialized_ = true;

}

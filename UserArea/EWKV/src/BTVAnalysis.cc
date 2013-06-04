#include "UserCode/EWKV/interface/BTVAnalysis.h"

using namespace std;

//
BTVAnalysis::BTVAnalysis(SmartSelectionMonitor &mon,bool runSystematics)
  : mon_(&mon)
{
  ///////////////////////////////////
  // b-tagging                     //
  ///////////////////////////////////
  fillCategories(runSystematics); 
  const size_t nSystVars=systVars_.size();
  const size_t nJetRanges=jetRanges_.size();
  TH2F *hjp  = new TH2F("jp",";Jet probability;Range;Jets",50,0,2.5,nJetRanges,0,nJetRanges);
  TH2F *hkin  = new TH2F("kin",";Mass(lepton,jet);Range;Jets",50,0,500,nJetRanges,0,nJetRanges);
  for(int ybin=1; ybin<=hjp->GetYaxis()->GetNbins(); ybin++){
    hjp->GetYaxis()->SetBinLabel(ybin,jetRanges_[ybin-1]);
    hkin->GetYaxis()->SetBinLabel(ybin,jetRanges_[ybin-1]);
  }
  
  TH2F *hrunjp  = new TH2F("runjp",";Jet probability;Working point;Jets",50,0,2.5,taggers_[0].wps.size(),0,taggers_[0].wps.size());
  TH2F *hrunkin = new TH2F("runkin",";Mass(lepton,jet);Working point;Jets",50,0,500,taggers_[0].wps.size(),0,taggers_[0].wps.size());
  int ibin(1);
  for(std::map<TString,float>::iterator it = taggers_[0].wps.begin(); it!= taggers_[0].wps.end(); it++, ibin++)
    {
      char buf[20]; sprintf(buf,"%3.3f",it->second);
      hrunjp->GetYaxis()->SetBinLabel(ibin,buf);
      hrunkin->GetYaxis()->SetBinLabel(ibin,buf);
    }

  //instantiate histograms per tagger
  for(size_t i=0; i<taggers_.size(); i++)
    {
      BTVTagger_t &tagger=taggers_[i];

      //discriminator
      mon_->addHistogram( new TH1F("inc"+tagger.name,";Discriminator;Jets", 50, tagger.minVal, tagger.maxVal) );

      //b-tag counting
      int nwps=tagger.wps.size();
      TH2 *hcount= (TH2 *)mon_->addHistogram( new TH2F(tagger.name+"btagsextended",";b-tag multiplicity;Events", 3*3*5, 0.,3*3*5.,nwps,0.,nwps*1.) );
      int ibin(1);
      for(std::map<TString,float>::iterator it = tagger.wps.begin(); it!= tagger.wps.end(); it++, ibin++)
	hcount->GetYaxis()->SetBinLabel(ibin,it->first);
      for(int ibin=1; ibin<=hcount->GetXaxis()->GetNbins(); ibin++)
	{
	  TString label(""); label += (ibin-1)%5;
	  hcount->GetXaxis()->SetBinLabel(ibin,label);
	}

      //jp or kinematics fit
      for(size_t j=0; j<nSystVars; j++)
	{  
	  if(tagger.name=="jp")
	    {
	      mon_->addHistogram( (TH2F *)hjp->Clone("jp"+systVars_[j]) );
	      mon_->addHistogram( (TH2F *)hjp->Clone("jpb"+systVars_[j]) );
	      mon_->addHistogram( (TH2F *)hjp->Clone("jpc"+systVars_[j]) );
	      mon_->addHistogram( (TH2F *)hjp->Clone("jpudsg"+systVars_[j]) );
	      mon_->addHistogram( (TH2F *)hrunjp->Clone("runjp"+systVars_[j]) );
	      mon_->addHistogram( (TH2F *)hrunjp->Clone("runjpb"+systVars_[j]) );
	      mon_->addHistogram( (TH2F *)hrunjp->Clone("runjpc"+systVars_[j]) );
	      mon_->addHistogram( (TH2F *)hrunjp->Clone("runjpudsg"+systVars_[j]) );
	    }
	  else{
	    mon_->addHistogram( (TH2F *)hrunjp->Clone("runjp"+systVars_[j]+tagger.name) );
	    mon_->addHistogram( (TH2F *)hrunjp->Clone("runjpb"+systVars_[j]+tagger.name) );
	    mon_->addHistogram( (TH2F *)hrunjp->Clone("runjpc"+systVars_[j]+tagger.name) );
	    mon_->addHistogram( (TH2F *)hrunjp->Clone("runjpudsg"+systVars_[j]+tagger.name) );
	  }
	  
	  if(i==0){
	    mon_->addHistogram( (TH2F *)hkin->Clone("kin"+systVars_[j]) );
	    mon_->addHistogram( (TH2F *)hkin->Clone("kinb"+systVars_[j]) );
	    mon_->addHistogram( (TH2F *)hkin->Clone("kinothers"+systVars_[j]) );
	    mon_->addHistogram( (TH2F *)hrunkin->Clone("runkin"+systVars_[j]) );
	    mon_->addHistogram( (TH2F *)hrunkin->Clone("runkinb"+systVars_[j]) );
	    mon_->addHistogram( (TH2F *)hrunkin->Clone("runkinothers"+systVars_[j]) );
	  }
	  mon_->addHistogram( (TH2F *)hrunkin->Clone("runkin"+systVars_[j]+tagger.name) );
	  mon_->addHistogram( (TH2F *)hrunkin->Clone("runkinb"+systVars_[j]+tagger.name) );
	  mon_->addHistogram( (TH2F *)hrunkin->Clone("runkinothers"+systVars_[j]+tagger.name) );
	  
	  for(std::map<TString,float>::iterator it = tagger.wps.begin(); it!= tagger.wps.end(); it++)
	    {
	      if(!it->first.IsAlpha()) continue;
	      if(tagger.name!="jp")
		{
		  mon_->addHistogram( (TH2 *)hjp->Clone( "jp"     +systVars_[j]+tagger.name+it->first) );
		  mon_->addHistogram( (TH2 *)hjp->Clone( "jpb"    +systVars_[j]+tagger.name+it->first) );
		  mon_->addHistogram( (TH2 *)hjp->Clone( "jpc"    +systVars_[j]+tagger.name+it->first) );
		  mon_->addHistogram( (TH2 *)hjp->Clone( "jpudsg" +systVars_[j]+tagger.name+it->first) );
		}
	      mon_->addHistogram( (TH2 *)hkin->Clone( "kin"      +systVars_[j]+tagger.name+it->first) );
	      mon_->addHistogram( (TH2 *)hkin->Clone( "kinb"     +systVars_[j]+tagger.name+it->first) );
	      mon_->addHistogram( (TH2 *)hkin->Clone( "kinothers"+systVars_[j]+tagger.name+it->first) );
	    }
	}
    }
}

//
void BTVAnalysis::analyze(data::PhysicsObjectCollection_t &leptons, 
			  data::PhysicsObjectCollection_t &jets,
			  bool isMC, int nVtx, float weight, float weightUp, float weightDown)
{
  //check the category
  int lid1(leptons[0].get("id")), lid2(leptons[1].get("id"));
  std::vector<TString> ch(1,"all");
  if     (abs(lid1)*abs(lid2)==11*11) ch.push_back("ee");
  else if(abs(lid1)*abs(lid2)==13*13) ch.push_back("mumu");
  else if(abs(lid1)*abs(lid2)==11*13) ch.push_back("emu");
  else return;

  //run 
  for(size_t j=0; j<systVars_.size(); j++)
    {
      TString var(systVars_[j]);
      float iweight(weight);
      if(var=="puup")   iweight=weightUp;
      if(var=="pudown") iweight=weightDown;

      int njets30(0);
      std::map<TString,std::map<TString,int> > btagCounts;
      for(size_t ijet=0; ijet<jets.size(); ijet++)
	{
	  if( fabs(jets[ijet].eta())>2.5) continue;
	  float pt=jets[ijet].pt();
	  if(var=="jerup")        pt=jets[ijet].getVal("jerup");
	  else if(var=="jerdown") pt=jets[ijet].getVal("jerdown");
	  else if(var=="jesup")   pt=jets[ijet].getVal("jesup");
	  else if(var=="jesdown") pt=jets[ijet].getVal("jesdown");
	  if(pt<30) continue;
	  njets30++;

	  LorentzVector varJetKin=jets[ijet]; 
	  varJetKin*=(pt/jets[ijet].pt());

	  //parton match
	  const data::PhysicsObject_t &genParton=jets[ijet].getObject("gen");
	  int genPartonId=genParton.info.find("id")->second;

	  //flavor
	  const data::PhysicsObject_t &genJet=jets[ijet].getObject("genJet");
	  int flavId=genJet.info.find("id")->second;
	  TString jetFlav("udsg");
	  if(abs(flavId)==5)      jetFlav="b";
	  else if(abs(flavId)==4) jetFlav="c";
	  
	  //compute mass(lepton,jet) and discriminate at generator level
	  std::vector<std::pair<TString,float> > mljPerFlavor;
	  for(size_t ilep=0; ilep<2; ilep++){
	    LorentzVector lj=leptons[ilep]+varJetKin;
	    float mlj=lj.mass();
	    TString flav("others");
	    if(abs(flavId)==5)
	      {
		const data::PhysicsObject_t &genLepton=leptons[ilep].getObject("gen");
		int genLeptonId=genLepton.info.find("id")->second;
		if(genLeptonId*genPartonId<0) flav="b";
	      }
	    mljPerFlavor.push_back( std::pair<TString,float>(flav,mlj) );  
	  }
	  
	  //kinematics
	  float eta = fabs(jets[ijet].eta());
	  std::vector<int> jetCategs;
	  jetCategs.push_back(0);
	  if(pt>30 && pt<=50)     jetCategs.push_back(1);
	  if(pt>50 && pt<=80)     jetCategs.push_back(2);
	  if(pt>80 && pt<=120)    jetCategs.push_back(3);
	  if(pt>120 && pt<=210)   jetCategs.push_back(4);
	  if(pt>210 && pt<=320)   jetCategs.push_back(5);
	  if(pt>320)              jetCategs.push_back(6);
	  if(eta<=0.5)            jetCategs.push_back(7);
	  if(eta>0.5 && eta<=1.0) jetCategs.push_back(8);
	  if(eta>1.0 && eta<=1.5) jetCategs.push_back(9);
	  if(eta>1.5)             jetCategs.push_back(10);
	  if(nVtx<=10)            jetCategs.push_back(11);
	  if(nVtx>10 && nVtx<=14) jetCategs.push_back(12);
	  if(nVtx>14 && nVtx<=18) jetCategs.push_back(13);
	  if(nVtx>19)             jetCategs.push_back(14);

	  //b-tag
	  float jpDisc=jets[ijet].getVal("jp");
	  for(size_t ib=0; ib<taggers_.size(); ib++){
	    BTVTagger_t &tagger=taggers_[ib];

	    float disc = jets[ijet].getVal(tagger.name);
	    mon_->fillHisto("inc"+tagger.name+var,     ch, disc,     iweight);
	    
	    //check if it passes WP requirement
	    std::map<TString, bool> hasWPtag;
	    for(std::map<TString,float>::iterator it = tagger.wps.begin(); it!= tagger.wps.end(); it++)  
	      {
		hasWPtag[it->first]=(disc>it->second);
		if(btagCounts.find(tagger.name)==btagCounts.end()) {
		  std::map<TString,int> countTemplate;
		  btagCounts[tagger.name]=countTemplate;
		}
		if(btagCounts[tagger.name].find(it->first)==btagCounts[tagger.name].end()) btagCounts[tagger.name][it->first]=0;
		btagCounts[tagger.name][it->first] += (disc>it->second);
	      }
	    
	    //fill histograms according to jet kinematics categories
	    for(size_t ijcat=0; ijcat<jetCategs.size(); ijcat++)
	      {
		if(tagger.name=="jp") mon_->fillHisto("jp"+var, ch, jpDisc, jetCategs[ijcat], iweight);
		if(ib==0) {
		  mon_->fillHisto("kin"+var, ch, mljPerFlavor[0].second, jetCategs[ijcat], iweight);
		  mon_->fillHisto("kin"+var, ch, mljPerFlavor[1].second, jetCategs[ijcat], iweight);
		}
		if(isMC) {
		  if(tagger.name=="jp") mon_->fillHisto("jp"+jetFlav+var, ch, jpDisc, jetCategs[ijcat], iweight);
		  if(ib==0){
		    mon_->fillHisto("kin"+mljPerFlavor[0].first+var, ch, mljPerFlavor[0].second, jetCategs[ijcat], iweight);
		    mon_->fillHisto("kin"+mljPerFlavor[1].first+var, ch, mljPerFlavor[1].second, jetCategs[ijcat], iweight);
		  }
		}
		
		for(std::map<TString, bool>::iterator it=hasWPtag.begin(); it!=hasWPtag.end(); it++)
		  {
		    if(!it->second) continue;
		    if(!it->first.IsAlpha()) continue;
		    
		    if(tagger.name!="jp") mon_->fillHisto("jp"+var+tagger.name+it->first,ch, jpDisc, jetCategs[ijcat], iweight);
		    mon_->fillHisto("kin"+var+tagger.name+it->first, ch, mljPerFlavor[0].second, jetCategs[ijcat], iweight);
		    mon_->fillHisto("kin"+var+tagger.name+it->first, ch, mljPerFlavor[1].second, jetCategs[ijcat], iweight);
		    
		    if(isMC){
		      if(tagger.name!="jp") mon_->fillHisto( "jp"+jetFlav+var+tagger.name+it->first, ch, jpDisc, jetCategs[ijcat], iweight);
		      mon_->fillHisto("kin"+mljPerFlavor[0].first+var+tagger.name+it->first,   ch, mljPerFlavor[0].second, jetCategs[ijcat], iweight);
		      mon_->fillHisto("kin"+mljPerFlavor[1].first+var+tagger.name+it->first,   ch, mljPerFlavor[1].second, jetCategs[ijcat], iweight);
		    }
		  }
	      }

	    //fill histogram according to WPs
	    int iwpbin(1);
	    for(std::map<TString, bool>::iterator it=hasWPtag.begin(); it!=hasWPtag.end(); it++,iwpbin++)
	      {
		if(tagger.name=="jp") mon_->fillHisto("runjp"+var, ch, jpDisc, iwpbin, iweight);
		if(ib==0) {
		  mon_->fillHisto("runkin"+var, ch, mljPerFlavor[0].second, iwpbin, iweight);
		  mon_->fillHisto("runkin"+var, ch, mljPerFlavor[1].second, iwpbin, iweight);
		}
		if(isMC) {
		  if(tagger.name=="jp") mon_->fillHisto("runjp"+jetFlav+var, ch, jpDisc, iwpbin, iweight);
		  if(ib==0){
		    mon_->fillHisto("runkin"+mljPerFlavor[0].first+var, ch, mljPerFlavor[0].second, iwpbin, iweight);
		    mon_->fillHisto("runkin"+mljPerFlavor[1].first+var, ch, mljPerFlavor[1].second, iwpbin, iweight);
		  }
		}
		if(!it->second) continue;
		if(tagger.name!="jp") mon_->fillHisto("runjp"+var+tagger.name,ch, jpDisc, iwpbin, iweight);
		mon_->fillHisto("runkin"+var+tagger.name, ch, mljPerFlavor[0].second, iwpbin, iweight);
		mon_->fillHisto("runkin"+var+tagger.name, ch, mljPerFlavor[1].second, iwpbin, iweight);

		if(isMC){
		  if(tagger.name!="jp") mon_->fillHisto( "runjp"+jetFlav+var+tagger.name, ch, jpDisc, iwpbin, iweight);
		  mon_->fillHisto("runkin"+mljPerFlavor[0].first+var+tagger.name,   ch, mljPerFlavor[0].second, iwpbin, iweight);
		  mon_->fillHisto("runkin"+mljPerFlavor[1].first+var+tagger.name,   ch, mljPerFlavor[1].second, iwpbin, iweight);
		}
	      }
	  }
	}
      
      //
      if(njets30>=2 && njets30<=4)
	{
	  int addBin(0);
	  if(njets30==3)    addBin += 5;
	  if(njets30==4)    addBin += 10;
	  if(ch[1]=="mumu") addBin += 15;
	  if(ch[1]=="emu")  addBin += 2*15;
	  
	  for(std::map<TString,std::map<TString, int> >::iterator it=btagCounts.begin(); it!=btagCounts.end(); it++){
	    int iwpbin(1);
	    for(std::map<TString, int>::iterator jIt=it->second.begin(); jIt!=it->second.end(); jIt++,iwpbin++){
	      mon_->fillHisto(it->first+"btagsextended"+var,ch,jIt->second+addBin,iwpbin,iweight);
	    }
	  }
	}
    }
}


//
void BTVAnalysis::fillCategories(bool runSystematics)
{

  systVars_.push_back("");
  if(runSystematics) { 
    systVars_.push_back("jesup"); systVars_.push_back("jesdown"); 
    systVars_.push_back("jerup"); systVars_.push_back("jerdown");
    systVars_.push_back("puup"); systVars_.push_back("pudown"); 
  }
  
  jetRanges_.push_back("inc"); 
  jetRanges_.push_back("30to50"); 
  jetRanges_.push_back("50to80");   
  jetRanges_.push_back("80to120");  
  jetRanges_.push_back("120to210"); 
  jetRanges_.push_back("210to320"); 
  jetRanges_.push_back("320toInf"); 
  jetRanges_.push_back("0to0.5"); 
  jetRanges_.push_back("0.5to1.0"); 
  jetRanges_.push_back("1.0to1.5"); 
  jetRanges_.push_back("1.5to2.5"); 
  jetRanges_.push_back("0to10");  
  jetRanges_.push_back("11to14"); 
  jetRanges_.push_back("15to18");  
  jetRanges_.push_back("18toInf");



  //note TCHPL, TCHPM, IFVM, IFVT, SSVHET are dummy
  TString btagger[]  = {"origcsv", "csv", "jpcsv", "slcsv", "supercsv", "jp", "tchp", "ivf", "ssvhe" };
  float btaggerMin[] = {-0.2,      -0.2,  -0.2,     -0.2,    -0.2,       0.0,   -2,    -2,    -2     };
  float btaggerMax[] = {1.2,       1.2,   1.2,      1.2,     1.2,        2.5,   20,    8,     8      };
  float btaggerL[]   = {0.244,     0.405, 0.529,    0.527,   0.531,      0.264, 1.19,  0,     0      };
  float btaggerM[]   = {0.679,     0.783, 0.753,    0.756,   0.758,      0.753, 1.93,  2,     1.74   };
  float btaggerT[]   = {0.898,     0.920, 0.855,    0.859,   0.849,      0.855, 3.41,  3,     3      };
  
  const size_t nTaggers=sizeof(btagger)/sizeof(TString);
  for(size_t ib=0; ib<nTaggers; ib++){
    BTVTagger_t itagger;
    itagger.name=btagger[ib];
    itagger.minVal=btaggerMin[ib];
    itagger.maxVal=btaggerMax[ib];

    float deltaLM((btaggerM[ib]-btaggerL[ib])*0.25);
    float deltaMT((btaggerT[ib]-btaggerM[ib])*0.25);
    float deltaTmax((0.90*btaggerMax[ib]-btaggerT[ib])*0.25);
    for(int istep=0; istep<=3; istep++)
      {
	TString pf(""); if(istep) pf+=istep;
	itagger.wps["L"+pf]=btaggerL[ib]+deltaLM*0.25;
	itagger.wps["M"+pf]=btaggerM[ib]+deltaMT*0.25;
	itagger.wps["T"+pf]=btaggerT[ib]+deltaTmax*0.25;
      }
    taggers_.push_back(itagger);
  }
}

#include "UserCode/EWKV/interface/BTVAnalysis.h"

using namespace std;

//
BTVAnalysis::BTVAnalysis(SmartSelectionMonitor &mon,bool runSystematics)
  : mon_(&mon)
{
  ///////////////////////////////////
  // b-tagging                     //
  ///////////////////////////////////
  systVars_.push_back("");
  if(runSystematics) { systVars_.push_back("jesup"); systVars_.push_back("jesdown"); systVars_.push_back("jerup"); systVars_.push_back("jerdown"); systVars_.push_back("puup"); systVars_.push_back("pudown"); }
  size_t nSystVars=systVars_.size();
  TString btagger[]  = {"origcsv", "csv", "jpcsv", "slcsv", "supercsv", "jp", "tchp", "ivf", "ssvhe" };
  float btaggerMin[] = {-0.2,      -0.2,  -0.2,     -0.2,    -0.2,       0.0,   -2,    -2,    -2     };
  float btaggerMax[] = {1.2,       1.2,   1.2,      1.2,     1.2,        2.5,   20,    8,     8      };
  TString btaggerWPs[]={"L","M","T"};
  jetRanges_.push_back("inc"); 
  jetRanges_.push_back("30to50"); jetRanges_.push_back("50to80");   jetRanges_.push_back("80to120");  jetRanges_.push_back("120to210"); jetRanges_.push_back("210to320"); jetRanges_.push_back("320toInf"); 
  jetRanges_.push_back("0to0.5"); jetRanges_.push_back("0.5to1.0"); jetRanges_.push_back("1.0to1.5"); jetRanges_.push_back("1.5to2.5"); 
  jetRanges_.push_back("0to10");  jetRanges_.push_back("11to14");   jetRanges_.push_back("15to18");   jetRanges_.push_back("18toInf");
  const size_t nJetRanges=jetRanges_.size();
  for(size_t i=0; i<sizeof(btagger)/sizeof(TString); i++)
    {
      //discriminator
      mon_->addHistogram( new TH1F("inc"+btagger[i],";Discriminator;Jets", 50, btaggerMin[i],btaggerMax[i]) );

      //b-tag counting
      TH1 *hl=mon_->addHistogram( new TH1F(btagger[i]+"Lbtagsextended",";b-tag multiplicity;Events", 3*3*5, 0.,3*3*5.) );
      TH1 *hm=mon_->addHistogram( new TH1F(btagger[i]+"Mbtagsextended",";b-tag multiplicity;Events", 3*3*5, 0.,3*3*5.) );
      TH1 *ht=mon_->addHistogram( new TH1F(btagger[i]+"Tbtagsextended",";b-tag multiplicity;Events", 3*3*5, 0.,3*3*5.) );
      for(int ibin=1; ibin<=hl->GetXaxis()->GetNbins(); ibin++)
	{
	  TString label(""); label += (ibin-1)%5;
	  hl->GetXaxis()->SetBinLabel(ibin,label);
	  hm->GetXaxis()->SetBinLabel(ibin,label);
	  ht->GetXaxis()->SetBinLabel(ibin,label);
	}

      for(size_t j=0; j<nSystVars; j++)
	{  
	  //use templates based on a given discriminator except in the case we're measuring the discriminator efficiency itself (switch to alternative)
	  TH2F *hinc  = (TH2F *)mon_->addHistogram( new TH2F("jp"+systVars_[j],";Jet probability;Jets",50,0,2.5,nJetRanges,0,nJetRanges) );
	  for(int ybin=1; ybin<=hinc->GetYaxis()->GetNbins(); ybin++) hinc->GetYaxis()->SetBinLabel(ybin,jetRanges_[ybin-1]);
	  mon_->addHistogram( (TH2F *)hinc->Clone("jpb"+systVars_[j]) );
	  mon_->addHistogram( (TH2F *)hinc->Clone("jpc"+systVars_[j]) );
	  mon_->addHistogram( (TH2F *)hinc->Clone("jpudsg"+systVars_[j]) );
	  for(size_t k=0; k<sizeof(btagger)/sizeof(TString); k++)
	    {
	      for(size_t iwp=0; iwp<sizeof(btaggerWPs)/sizeof(TString); iwp++)
		{
		  for(size_t ipf=0; ipf<2; ipf++)
		    {
		      TString pfix(ipf==0?"":"v");
		      mon_->addHistogram( (TH2 *)hinc->Clone( "jp"     +systVars_[j]+btagger[k]+btaggerWPs[iwp]+pfix) );
		      mon_->addHistogram( (TH2 *)hinc->Clone( "jpb"    +systVars_[j]+btagger[k]+btaggerWPs[iwp]+pfix) );
		      mon_->addHistogram( (TH2 *)hinc->Clone( "jpc"    +systVars_[j]+btagger[k]+btaggerWPs[iwp]+pfix) );
		      mon_->addHistogram( (TH2 *)hinc->Clone( "jpudsg" +systVars_[j]+btagger[k]+btaggerWPs[iwp]+pfix) );
		    }
		}
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
      std::map<TString, int> btagCounts;
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

	  //flavor
	  const data::PhysicsObject_t &genJet=jets[ijet].getObject("genJet");
	  int flavId=genJet.info.find("id")->second;
	  TString jetFlav("udsg");
	  if(abs(flavId)==5)      jetFlav="b";
	  else if(abs(flavId)==4) jetFlav="c";
	  
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
	  float tchp     = jets[ijet].getVal("tchp");
	  float jp       = jets[ijet].getVal("jp");
	  float ivf      = jets[ijet].getVal("ivf");
	  float ssvhe    = jets[ijet].getVal("ssvhe");
	  float origcsv  = jets[ijet].getVal("origcsv");
	  float csv      = jets[ijet].getVal("csv");
	  float jpcsv    = jets[ijet].getVal("jpcsv");
	  float slcsv    = jets[ijet].getVal("slcsv");
	  float supercsv = jets[ijet].getVal("supercsv");
	  
	  std::map<TString, bool> hasTagger;
	  hasTagger["tchpT"]    = (tchp>3.41);
	  hasTagger["jpL"]      = (jp>0.274);
	  hasTagger["jpM"]      = (jp>0.545);
	  hasTagger["jpT"]      = (jp>0.790);
	  hasTagger["ivfL"]     = (ivf>0);
	  hasTagger["ssvheL"]   = (ssvhe>0);
	  hasTagger["ssvheM"]   = (ssvhe>1.74);
	  hasTagger["origcsvL"] = (origcsv>0.244);
	  hasTagger["origcsvM"] = (origcsv>0.679);
	  hasTagger["origcsvT"] = (origcsv>0.898);
	  hasTagger["csvL"]     = (csv>0.405);
	  hasTagger["csvM"]     = (csv>0.783);
	  hasTagger["csvT"]     = (csv>0.92);
	  hasTagger["jpcsvL"]   = (jpcsv>0.529);
	  hasTagger["jpcsvM"]   = (jpcsv>0.753);
	  hasTagger["jpcsvT"]   = (jpcsv>0.855);
	  hasTagger["slcsvL"]   = (slcsv>0.527);
	  hasTagger["slcsvM"]   = (slcsv>0.756);
	  hasTagger["slcsvT"]   = (slcsv>0.859);
	  hasTagger["supercsvL"]= (supercsv>0.531);
	  hasTagger["supercsvM"]= (supercsv>0.758);
	  hasTagger["supercsvT"]= (supercsv>0.849);

	  //count the b-tags
	  for(std::map<TString,bool>::iterator it = hasTagger.begin(); it!=hasTagger.end(); it++)
	    {
	      if(btagCounts.find(it->first)==btagCounts.end()) btagCounts[it->first]=0;
	      btagCounts[it->first] += it->second;
	    }


	  mon_->fillHisto("inctchp"+var,     ch, tchp,     iweight);
	  mon_->fillHisto("incjp"+var,       ch, jp,       iweight);
	  mon_->fillHisto("incivf"+var,      ch, ivf,      iweight);
	  mon_->fillHisto("incssvhe"+var,    ch, ssvhe,    iweight);
	  mon_->fillHisto("incorigcsv"+var,  ch, origcsv,  iweight);
	  mon_->fillHisto("inccsv"+var,      ch, csv,      iweight);	  
	  mon_->fillHisto("incjpcsv"+var,    ch, jpcsv,    iweight);	  
	  mon_->fillHisto("incslcsv"+var,    ch, slcsv,    iweight);	  
	  mon_->fillHisto("incsupercsv"+var, ch, supercsv, iweight);	  
	  for(size_t ijcat=0; ijcat<jetCategs.size(); ijcat++)
	    {
	      mon_->fillHisto("jp"+var, ch, jp, jetCategs[ijcat], iweight);
	      if(isMC) mon_->fillHisto("jp"+jetFlav+var, ch, jp, jetCategs[ijcat], iweight);
	      for(std::map<TString, bool>::iterator it=hasTagger.begin(); it!=hasTagger.end(); it++)
		{
		  TString pfix(it->second?"":"v");
		  mon_->fillHisto("jp"        +var+it->first+pfix,ch, jp, jetCategs[ijcat], iweight);
		  if(isMC) mon_->fillHisto( "jp"+jetFlav+var+it->first+pfix,     ch, jp, jetCategs[ijcat], iweight);
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
	 
	  for(std::map<TString, int>::iterator it=btagCounts.begin(); it!=btagCounts.end(); it++){
	    mon_->fillHisto(it->first+"btagsextended"+var,ch,it->second+addBin,weight);
	  }
	}
    }
}

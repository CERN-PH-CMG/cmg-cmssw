#include "UserCode/EWKV/interface/LxyAnalysis.h"
#include <Math/VectorUtil.h>

using namespace std;
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >::BetaVector BetaVector;

//
LxyAnalysis::LxyAnalysis(SmartSelectionMonitor &mon,bool runSystematics)
  : mon_(&mon)
{
  //start monitoring histograms of this analysis
  mon_->addHistogram( new TH2F ("bjetlxybhad",    "; L_{xy}(reco) [cm]; B-hadron; Jets", 50,0,5, 4,0,4) );
  for(size_t i=0; i<4; i++)
    {
      TString pf(""); pf+=i;
      mon_->addHistogram( new TH2F ("bjetlxygenb"+pf+"had", "; L_{xy}(reco) [cm]; B-hadron; Jets", 50,0,5,50,0,0.15) );
    }
  mon_->addHistogram( new TH2F ("bjetlxyrespt",   "; L_{xy}(reco)-L_{xy}(B) [cm]; Jet p_{T} [GeV]; Events", 50, -2.5,2.5, 10,30.,280.) );
  mon_->addHistogram( new TH2F ("bjetlxyreseta",  "; L_{xy}(reco)-L_{xy}(B) [cm]; Jet #eta; Events", 50, -2.5,2.5, 4,0.,2.5) );
  mon_->addHistogram( new TH2F ("xbvslxy",        "; x_{b}; L_{xy} [cm]; Events",100, 0, 2, 50, 0, 5) );
  mon_->addHistogram( new TH2F ("ptbvslxy",       "; x_{b}; L_{xy} [cm]; Events",100, 0, 200, 50, 0, 5) );
  mon_->addHistogram( new TH2F ("topptvslxy",     "; Top p_{T} [GeV]; L_{xy} [cm]; Events",100, 0, 1000, 50, 0, 5) );

  mon_->addHistogram( new TH1F ("jetlxy", ";L_{xy} [cm]; Jets", 50, 0.,5) );
}

//
void LxyAnalysis::analyze(data::PhysicsObjectCollection_t & leptons, 
			  data::PhysicsObjectCollection_t &jets,
			  data::PhysicsObject_t &met, 
			  data::PhysicsObjectCollection_t &mctruth,
			  float weight)
{

  //check channel
  TString ch("");
  int lid1(leptons[0].get("id")), lid2(leptons[1].get("id"));
  if     (abs(lid1)*abs(lid2)==11*11)  ch="ee";
  else if(abs(lid1)*abs(lid2)==11*13)  ch="emu";
  else if(abs(lid1)*abs(lid2)==13*13)  ch="mumu";
  if(ch=="") return;
 
  //select
  float mll=(leptons[0]+leptons[1]).mass();
  bool isZcand((ch=="ee" || ch=="mumu") && fabs(mll-91)<15);
  bool passMet( ch=="emu" || ((ch=="ee" || ch=="mumu") && met.pt()>40));
  bool isOS( leptons[0].get("id")*leptons[1].get("id") < 0 );
  bool passJet(jets.size()>2);
  if(isZcand || !passMet || !isOS || !passJet) return;

  //get leading Lxy jet
  data::PhysicsObject_t *lxyJet=0;
  float leadingLxy(-1);
  for(size_t ijet=0; ijet<jets.size(); ijet++)
    {
      //get reconstructed secondary vertex
      const data::PhysicsObject_t &svx=jets[ijet].getObject("svx");
      float lxy=svx.vals.find("lxy")->second;
      if(lxy<=0) continue;
      if(lxy<leadingLxy) continue;
    
      //use lxy if it's larger, prefer also more central jets
      if(lxyJet==0 || (fabs(lxyJet->eta())>1.1 && fabs(jets[ijet].eta())<fabs(lxyJet->eta())))
	{
	  lxyJet=&(jets[ijet]); 
	  leadingLxy=lxy;
	}
    }
  if(leadingLxy<0) return;
  bool passFiducialCut(fabs(lxyJet->eta())<1.1);

  //MC truth
  //get flavor
  const data::PhysicsObject_t &genJet=lxyJet->getObject("genJet");
  const data::PhysicsObject_t &genParton=lxyJet->getObject("gen");
  int flavId=genJet.info.find("id")->second;
  int genId=genParton.info.find("id")->second;
  //match to a b-hadron
  data::PhysicsObject_t *bHad=0,*top=0,*antitop=0;
  for(size_t imc=0; imc<mctruth.size(); imc++)
    {
      int id=mctruth[imc].get("id");
      if(id==6)       top=&(mctruth[imc]);
      else if(id==-6) antitop=&(mctruth[imc]);
      else if(abs(id)<500) continue;

      if(deltaR(mctruth[imc],*lxyJet)>0.5) continue;
      bHad=&mctruth[imc];
      break;
    }
  float genLxy(-1);
  if(bHad) genLxy=bHad->getVal("lxy");

  //resolution plots
  if(genLxy>0 && abs(flavId)==5)
    {
      mon_->fillHisto("bjetlxyreseta",ch,leadingLxy-genLxy,fabs(lxyJet->eta()),weight);

      if(passFiducialCut)
	{
	  mon_->fillHisto("bjetlxyrespt", ch,leadingLxy-genLxy,lxyJet->pt(),weight);
            
	  int absid=abs(bHad->get("id"));
	  int bHadronBin(3); //other not so relevant
	  if(absid==511 || absid==10511 || absid==513 || absid==10513 || absid==20513 || absid==515) bHadronBin=0;       //B0 family
	  else if(absid==521 || absid==10521 || absid==523 || absid==10523 || absid==20523 || absid==525) bHadronBin=1;  //B+ family
	  else if(absid==531 || absid==10531 || absid==533 || absid==10533 || absid==20533 || absid==535) bHadronBin=2;  //B0s family
	  //  else if(absid==541 || absid==10541 || absid==543 || absid==10543 || absid==20543 || absid==545) bHadronBin=3;  //B+c family
	  mon_->fillHisto("bjetlxybhad",   ch,leadingLxy, bHadronBin,weight);
	  
	  if(genParton.pt()>0)
	    {
	      float xb(bHad->pt()/genParton.pt());
	      float bBoost = genParton.BoostToCM().R();
	      float genLxyCM=sqrt(1-bBoost*bBoost)*genLxy;
	      
	      TString pf(""); pf+=bHadronBin;
	      mon_->fillHisto("bjetlxygenb"+pf+"had",ch,leadingLxy, genLxyCM,    weight);
	      mon_->fillHisto("xbvslxy",ch, xb, leadingLxy, weight);
	      mon_->fillHisto("ptbvslxy",ch,genParton.pt(), leadingLxy, weight);
	    }
	}
    }

  //lxy distribution
  if(!passFiducialCut) return;
  mon_->fillHisto("jetlxy",ch,leadingLxy,weight);
  if(top && antitop && abs(genId)==5)
    {
      float genpt( genId==-5 ? antitop->pt() : top->pt() );
      mon_->fillHisto ("topptvslxy", ch, genpt, leadingLxy, weight );
    }
}





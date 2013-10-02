
/////SM
///Crossections taken from here:
///https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt8TeV
///Higgs --> tau tau branching ratios taken from http://arxiv.org/abs/1101.0593 v3

// #define NMASS 8
// long massValues[NMASS]={110,115,120,125,130,135,140,145};

// Float_t HiggsTauTauBF[NMASS]={8.03e-2, 7.65e-2,  7.11e-2,  6.37e-2,  5.49e-2,  4.52e-2, 3.54e-2,2.62e-2};
// Float_t HiggsGGcross[NMASS]={25.04,22.96,21.13,19.52,18.07,16.79,15.63,14.59};
// Float_t HiggsVBFcross[NMASS]={1.809,1.729,1.649,1.578,1.511,1.448,1.389,1.333 };
// Float_t HiggsWHcross[NMASS]= {1.0600,0.9165,0.7966,0.6966,0.6095,0.5351,0.47130,0.41640};
// Float_t HiggsZHcross[NMASS]= {0.5869,0.5117,0.4483,0.3943,0.3473,0.3074,0.27280,0.24240};
// Float_t HiggsttHcross[NMASS]={0.1887,0.1663,0.1470,0.1302,0.1157,0.1031,0.09207,0.08246 };

#define NMASS 15
long massValues[NMASS]={90,95,100,105,110,115,120,125,130,135,140,145,150,155,160};
Float_t HiggsTauTauBF[NMASS]={1,1,1,1,8.03e-2, 7.65e-2,  7.11e-2,  6.37e-2,  5.49e-2,  4.52e-2, 3.54e-2,2.62e-2,1,1,1};
Float_t HiggsGGcross[NMASS]={1,1,1,1,25.04,22.96,21.13,19.52,18.07,16.79,15.63,14.59,1,1,1};
Float_t HiggsVBFcross[NMASS]={1,1,1,1,1.809,1.729,1.649,1.578,1.511,1.448,1.389,1.333,1,1,1};
Float_t HiggsWHcross[NMASS]= {1,1,1,1,1.0600,0.9165,0.7966,0.6966,0.6095,0.5351,0.47130,0.41640,1,1,1};
Float_t HiggsZHcross[NMASS]= {1,1,1,1,0.5869,0.5117,0.4483,0.3943,0.3473,0.3074,0.27280,0.24240,1,1,1};
Float_t HiggsttHcross[NMASS]={1,1,1,1,0.1887,0.1663,0.1470,0.1302,0.1157,0.1031,0.09207,0.08246,1,1,1};


#define NCAT 5
TString catdirname[NCAT]={
 "0jet_low",
 "0jet_high",
 "boost_low",
 "boost_high",
 "vbf"};

#define NCATSUMMER13 13
TString catdirnameSummer13[NCATSUMMER13]={
 "0jet_low",
 "0jet_medium",
 "0jet_high",
 "1jet_medium",
 "1jet_high_lowhiggs",
 "1jet_high_mediumhiggs",
 "vbf_loose",
 "vbf_tight",
 "inclusive",
 "btag",
 "nobtag",
 "btag",
 "nobtag"};

#define NXBINS0JET 37
Float_t xbinsValues0Jet[NXBINS0JET+1]={0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,160,170,180,190,200,225,250};
#define NXBINS 26
Float_t xbinsValues[NXBINS+1]={0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350};
#define NXBINSVBF 13
Float_t xbinsValuesVBF[NXBINSVBF+1]={0,20,40,60,80,100,120,140,160,180,200,250,300,350};

//Float_t xbinsValues[27]={0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350};
//Float_t xbinsValuesVBF[14]={0,20,40,60,80,100,120,140,160,180,200,250,300,350};

/////SUSY
#define NSUSY 21
Int_t SUSYMass[NSUSY]={80,90,100,110,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000};  
//Int_t  SUSYMass[NSUSY]={90,100,110,120,130,140,160,180,250,300,400,450,500,600,800,1000};  

#define NXBINSMSSM 47
Float_t xbinsValuesMSSM[NXBINSMSSM+1]={0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500,1600,1800,2000};  

TString mvBlindSel="(ditaumass<60||120<ditaumass)";
TString svBlindSel="(svfitmass<100||160<svfitmass)";  
//TString mvBlindSel="";
//TString svBlindSel="";

Float_t c[4]={.6,.96,.57,.87};//default legend coordinates
Float_t c2[4]={.25,.49,.52,.87};//on the left side
Float_t c3[4]={.6,.96,.22,.57};//on the right side bottom
Float_t c4[4]={.25,.49,.22,.57};//on the left  bottom

float DYNJetFrac[4]={0.1901,0.0613,0.0173,0.0078};


/////////////data-card tools//////////////////
void fix0Bins(TH1F* h){

//   //determine if this is a low stats shape
//   float binUnc=0.;
//   int nNon0bins=0;
//   for(Int_t i=1;i<=h->GetNbinsX();i++){
//     if(h->GetBinContent(i)>0.){
//       nNon0bins++;
//       binUnc+=h->GetBinError(i)/h->GetBinContent(i);
//     }
//   }
//   binUnc=binUnc/nNon0bins;

//   //if the average bin uncertainty is greater than 30% then smooth
//   if(binUnc>0.5){

//     //TH1F* hs=new TH1F(TString(h->GetName())+"smeared",h->GetTitle(),h->GetXaxis()->GetNbins(),h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
//     TH1F* hs=h->Clone("hs");
//     TF1 gaus("gauss","[0]*exp(-0.5*(x-[1])**2/[2]**2)",h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
//     gaus.SetParameter(2,10);

//     for(Int_t b=1;b<=h->GetXaxis()->GetNbins();b++){
//       gaus.SetParameter(0,h->GetBinContent(b));
//       gaus.SetParameter(1,h->GetBinCenter(b));
//       for(Int_t bs=1;bs<=h->GetXaxis()->GetNbins();bs++){
//         hs->AddBinContent(bs,hs->GetBinWidth(bs)*gaus.Eval(hs->GetBinCenter(bs))) ;
//       }
//     }
//     for(Int_t bs=1;bs<=h->GetXaxis()->GetNbins();bs++){
//       hs->SetBinError(bs,0.);//not sure this is necessary
//     }
    
//     if(hs->Integral()>0.)
//       hs->Scale(h->Integral()/hs->Integral());//make sure the output histo has the same integral
//     else hs->Scale(0.);

//     for(Int_t b=1;b<=h->GetXaxis()->GetNbins();b++){
//       h->SetBinContent(b,hs->GetBinContent(b));
//       h->SetBinError(b,hs->GetBinContent(b)*0.4);
//     }
//     delete hs;
//   }


  //fix empty distributions for limit to run
  if(h->Integral()<=0.){
    for(Int_t i=1;i<=h->GetNbinsX();i++){
      h->SetBinContent(i,0.0001);
      h->SetBinError(i,0.0001);
    }
  }

}




void fixSignal(TH1F*hData,TH1F*hMC,TH1F*hsig){
  for(Int_t b=1;b<=hData->GetNbinsX();b++)
    if(hData->GetBinContent(b)>0.&&hMC->GetBinContent(b)==0.){
      hsig->SetBinContent(b,0.);
      hsig->SetBinError(b,0.);
    }
}

void rebinHisto(TH1F* hinput,TH1F* houtput){
  for(Int_t bo=1;bo<=houtput->GetNbinsX();bo++){
    float err=0.;
    for(Int_t bi=1;bi<=hinput->GetNbinsX();bi++){
      if( houtput->GetBinLowEdge(bo) < hinput->GetBinCenter(bi)
	  && hinput->GetBinCenter(bi) < houtput->GetBinLowEdge(bo)+houtput->GetBinWidth(bo)){
	houtput->AddBinContent(bo,hinput->GetBinContent(bi));
	err+=hinput->GetBinError(bi)*hinput->GetBinError(bi);
      }
    } 
    houtput->SetBinError(bo,sqrt(err));
  }
}

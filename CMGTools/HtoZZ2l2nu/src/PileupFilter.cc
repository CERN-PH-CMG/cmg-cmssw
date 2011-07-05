#include "CMGTools/HtoZZ2l2nu/interface/PileupFilter.h"

namespace pileup
{
  TH1D * generateWeightsFrom(std::vector<double> mc, std::vector<double> data, float &maxweight)
    {
      
      TH1D *dataH = new TH1D("datah","datah",25,0,25);
      TH1D *mcH = new TH1D("mch","mch",25,0,25);
      for(size_t i=0; i<data.size();i++)
	{
	  dataH->SetBinContent(i+1,data[i]);
	  mcH->SetBinContent(i+1,mc[i]);
	}
      dataH->Scale(1./dataH->Integral());
      mcH->Scale(1./mcH->Integral());

      //compute the weights
      TH1D *puUnfoldedH = (TH1D *)dataH->Clone("puunfoldedweights");
      puUnfoldedH->Divide(mcH);
      
      maxweight=0;
      for(int ibin=1; ibin<=puUnfoldedH->GetXaxis()->GetNbins(); ibin++)
	{
	  float iwgt=puUnfoldedH->GetBinContent(ibin);
	  if(iwgt<maxweight) continue;
	  maxweight=iwgt;
	}

      return puUnfoldedH;
    }
  

  std::vector<double> getWeightsFor(int npuevents, TH1D *wgtDist, double maxWeight)
    {
      double weight = wgtDist->GetBinContent( wgtDist->GetXaxis()->FindBin(npuevents) );
      double renWeight(1);
      if(maxWeight>0)
        {
          float randWeight=gRandom->Uniform();
          if(randWeight>weight/maxWeight) renWeight=0;
          else                            renWeight=1;
        }

      std::vector<double> wgtColl(2,1.0);
      wgtColl[MCTODATA]=weight;
      wgtColl[MCTODATAINTEGER]=renWeight;
      return wgtColl;

    }
}

#ifndef _pdfinfo_h_
#define _pdfinfo_h_

#include "TFile.h"
#include "TString.h"
#include "TTree.h"

class PDFInfo
{
 public:

  PDFInfo(TString fInUrl,TString pdf)
    {
      fIn_=TFile::Open(fInUrl);
      if(fIn_==0) return;
      if(fIn_->IsZombie()) { fIn_->Close(); fIn_=0; return; }

      TIter next(fIn_->GetListOfKeys());
      TObject *obj=0;
      while ((obj=next()))
	{
	  TString key(obj->GetName());
	  if(!key.Contains(pdf)) continue;
	  TTree *tree=(TTree *)fIn_->Get(key);
	  if(tree==0) continue;
	  tree->GetBranch("w")->SetAddress(&iwgt_);
	  trees_.push_back(tree);
	}
    }

  int numberPDFs() { return trees_.size(); }

  std::vector<float> getWeights(int entry)
    {
      std::vector<float> wgts(trees_.size(),1);
      float normWgt(1.0);
      for(size_t i=0; i<trees_.size(); i++)
	{
	  trees_[i]->GetEntry(entry);
	  wgts[i]=iwgt_;
	  if(i==0) normWgt=iwgt_;
	  wgts[i]=wgts[i]/normWgt;
	}
      return wgts;
    }
  
  ~PDFInfo()
    {
      if(fIn_) fIn_->Close();
    }

 private:

  TFile *fIn_;
  std::vector<TTree *> trees_;
  float iwgt_;
};


#endif

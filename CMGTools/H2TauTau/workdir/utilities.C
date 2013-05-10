#include "TString.h"
#include "TCanvas.h"


TH1F* plotComparisonWithRatio(TCanvas*C,TH1F*h1,TH1F*h2,float yrange=0.1){

  TPad * pad1=new TPad("pad1","",0,0.2,1,1);
  TPad * pad2=new TPad("pad2","",0,0,1,0.2);
    
  ////////////////////////////////////////////
  pad1->cd();
  if(h1->GetMaximum()>h2->GetMaximum())
    h1->GetYaxis()->SetRangeUser(0,h1->GetMaximum()*1.1);
  else h1->GetYaxis()->SetRangeUser(0,h2->GetMaximum()*1.1);
  h1->Draw("hist");
  h2->Draw("histpesame");
  
  ////////////////////////////////////////////
  pad2->cd();
  TH1F*HDiff=(TH1F*)h2->Clone("HDiff");
  HDiff->Divide(h1);
  HDiff->GetYaxis()->SetRangeUser(1-yrange,1.+yrange);
  HDiff->GetYaxis()->SetNdivisions(5);
  HDiff->GetYaxis()->SetLabelSize(0.2);
  HDiff->GetYaxis()->SetTitleSize(0.2);
  HDiff->GetYaxis()->SetTitleOffset(0.32);
  HDiff->GetYaxis()->SetTitle("Ratio");
  HDiff->GetXaxis()->SetNdivisions(-1);
  HDiff->GetXaxis()->SetTitle("");
  HDiff->GetXaxis()->SetLabelSize(0.0001);
  HDiff->SetMarkerColor(h1->GetMarkerColor());
  HDiff->SetTitle("");
  HDiff->Draw("histpe");
  TLine line;
  line.DrawLine(HDiff->GetXaxis()->GetXmin(),1,HDiff->GetXaxis()->GetXmax(),1);


  C->Clear();
  pad1->Draw();
  pad2->Draw();

  return HDiff;
}

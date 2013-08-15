void Plot(const char* var, const char* cut, float frac=1, const char* title = "", const char* fileName = "") {
  
  string sTitle(title); 
  
  if(!sTitle.empty() ) {
    samt1.SetTitle( title );
    samz2.SetTitle( title ); 
  }
  string sFileName = fileName; 

  string varcut = var;
  varcut += "_";
  varcut += cut;
  TCanvas *c = new TCanvas(varcut.c_str(), varcut.c_str(), 1500, 700); 

  c->Divide(2,1);

  c->cd(1);
  samt1.Draw(var, cut, frac, true); 
  Styles::FormatPad( gPad, true, true );
  gPad->Update();
  gPad->Modified();

  
  c->cd(2);
  samz2.Draw(var, cut, frac, true); 
  Styles::FormatPad( gPad, true, true );
  gPad->Update();
  gPad->Modified();

  c->cd();
  
  if(!sFileName.empty() ) {
    c->SaveAs( sFileName.c_str() );
    c->cd(1);
    gPad->SetLogy(false);
    c->cd(2);
    gPad->SetLogy(false);
    c->Modified();
    string lin = "lin_";
    lin += sFileName;
    c->SaveAs( lin.c_str() );
    
  }  
}


void PlotAll( float frac ) {

  Plot("cmgPFBaseJets.obj[0].pt()","cmgPFBaseJets.obj[0].pt()>130",frac,";p_{T,0} (GeV)","pt0.png");   
  Plot("cmgPFBaseJets.obj[1].pt()","cmgPFBaseJets.obj[0].pt()>130",frac,";p_{T,1} (GeV)","pt1.png");   
  Plot("cmgPFBaseJets.obj[2].pt()","cmgPFBaseJets.obj[0].pt()>130",frac,";p_{T,2} (GeV)","pt2.png");   
  Plot("cmgPFBaseJets.obj[3].pt()","cmgPFBaseJets.obj[0].pt()>130",frac,";p_{T,3} (GeV)","pt3.png");   
  Plot("cmgPFBaseJets.obj[4].pt()","cmgPFBaseJets.obj[0].pt()>130",frac,";p_{T,4} (GeV)","pt4.png");   
  Plot("cmgPFMET.obj.pt()","cmgPFBaseJets.obj[0].pt()>130",frac,";MET (GeV)","met.png");   
  Plot("cmgPFMET.obj.sumEt()","cmgPFBaseJets.obj[0].pt()>130",frac,";MET (GeV)","sumEt.png");   


}

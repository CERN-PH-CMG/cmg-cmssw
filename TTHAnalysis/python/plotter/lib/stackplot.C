THStack* getStack(long s) {return (THStack*)s;}
TH1F* getHisto(long h) {return (TH1F*)h;}

TH1F* setOverflowBin(TH1F* histo, float xmax) {  
  TH1F* h = histo->Clone();
  int ovfBin = h->FindBin(xmax);
  
  float ovfTot=0; 
  float ovfTotErr2=0;
 for (int i=ovfBin; i<h->GetNbinsX()+1; ++i) {
    ovfTot+=h->GetBinContent(i); 
    ovfTotErr2 += pow(h->GetBinError(i),2);
    h->SetBinContent(i,0);
    h->SetBinError(i,0);
  }
  h->SetBinContent(ovfBin,ovfTot);
  h->SetBinError(ovfBin,sqrt(ovfTotErr2));
  return h;
}

// THStack* setOverflowBin(THStack* s, float xmax) {
//   TListIter hist(s->GetHists());
//   TH1F* h;
//   while (h = (TH1F*) hist.Next()) {
//     setOverflowBin(h,xmax);
//   }
//   return s;
// }

void storePlot(TDirectory* dir, TString destDir, THStack*s, TH1F* d) {
  TDirectory* home = gDirectory;
  TDirectory* dest = dir->mkdir(destDir);
  s->SetName("MC");
  d->SetName("data");
  dest->Add(s);
  dest->Add(d);
  home->cd();
}


void getSum(TString dirname, THStack*& s, TH1F*& d) {

  TString dataname = "data";
  TString stackname = "MC";


  gDirectory->cd("Rint:/4mu");
  gDirectory->cd(dirname);
  gDirectory->pwd();
  gDirectory->ls();
  TH1F*    d4mu= (TH1F*)    gDirectory->FindObject(dataname);
  THStack* s4mu= (THStack*) gDirectory->FindObject(stackname);
  cout << (long) d4mu << " " << (long) s4mu << endl;
  gDirectory->cd("Rint:/4e");
  gDirectory->cd(dirname);
  TH1F*    d4e= (TH1F*)    gDirectory->FindObject(dataname);
  THStack* s4e= (THStack*) gDirectory->FindObject(stackname);
  gDirectory->cd("Rint:/2e2mu");
  gDirectory->cd(dirname);
  TH1F*    d2mu2e= (TH1F*)    gDirectory->FindObject(dataname);
  THStack* s2mu2e= (THStack*) gDirectory->FindObject(stackname);
  
  THStack* ssum = add(s4mu,s4e,s2mu2e);
  TH1F*    dsum = (TH1F*) d4mu->Clone();
  dsum->Add(d4e);
  dsum->Add(d2mu2e);

  s= ssum;
  d= dsum;
}


// void getSum(TString dirname, THStack*& s, TH1F*& d) {

//   cout << dirname << " " << stackname << " " <<  dataname << endl;

//   gDirectory->cd("Rint:/4mu");
//   gDirectory->cd(dirname);
//   gDirectory->pwd();
//   gDirectory->ls();
//   TH1F*    d4mu= (TH1F*)    gDirectory->FindObject(dataname);
//   THStack* s4mu= (THStack*) gDirectory->FindObject(stackname);
//   cout << (long) d4mu << " " << (long) s4mu << endl;
//   gDirectory->cd("Rint:/4e");
//   gDirectory->cd(dirname);
//   TH1F*    d4e= (TH1F*)    gDirectory->FindObject(dataname);
//   THStack* s4e= (THStack*) gDirectory->FindObject(stackname);
//   gDirectory->cd("Rint:/2e2mu");
//   gDirectory->cd(dirname);
//   TH1F*    d2mu2e= (TH1F*)    gDirectory->FindObject(dataname);
//   THStack* s2mu2e= (THStack*) gDirectory->FindObject(stackname);
  
//   THStack* ssum = add(s4mu,s4e,s2mu2e);
//   TH1F*    dsum = (TH1F*) d4mu->Clone();
//   dsum->Add(d4e);
//   dsum->Add(d2mu2e);

//   s= ssum;
//   d= dsum;
// }



void getSum(TString dirname, TString stackname, TString dataname, THStack*& s, TH1F*& d) {

  cout << dirname << " " << stackname << " " <<  dataname << endl;

  gDirectory->cd("Rint:/4mu");
  gDirectory->cd(dirname);
  gDirectory->pwd();
  gDirectory->ls();
  TH1F*    d4mu= (TH1F*)    gDirectory->FindObject(dataname);
  THStack* s4mu= (THStack*) gDirectory->FindObject(stackname);
  cout << (long) d4mu << " " << (long) s4mu << endl;
  gDirectory->cd("Rint:/4e");
  gDirectory->cd(dirname);
  TH1F*    d4e= (TH1F*)    gDirectory->FindObject(dataname);
  THStack* s4e= (THStack*) gDirectory->FindObject(stackname);
  gDirectory->cd("Rint:/2e2mu");
  gDirectory->cd(dirname);
  TH1F*    d2mu2e= (TH1F*)    gDirectory->FindObject(dataname);
  THStack* s2mu2e= (THStack*) gDirectory->FindObject(stackname);
  
  THStack* ssum = add(s4mu,s4e,s2mu2e);
  TH1F*    dsum = (TH1F*) d4mu->Clone();
  dsum->Add(d4e);
  dsum->Add(d2mu2e);

  s= ssum;
  d= dsum;
}



//Apply blinding. This works fine as long as binning is aligned with blinding region boundaries!!!
void blindHisto(TH1F* h) {  
  for (int i = 1; i <= h->GetNbinsX(); ++i){
    float xx = h->GetBinCenter(i);
    if ((xx>110&&xx<140) || xx>300) {  
      //      if (h->GetBinContent(i)>0) cout << h->GetBinLowEdge(i) << " " << h->GetBinCenter(i) << " " << h->GetBinLowEdge(i+1) << endl;
      h->SetBinContent(i,0);
    }
  }
}


//Apply m4l > 100 GeV. This works fine as long as binning is aligned with blinding region boundaries!!!
void m4l100Histo(TH1F* h) {  
  for (int i = 1; i <= h->GetNbinsX(); ++i){
    float xx = h->GetBinCenter(i);
    if (xx<100) {  
      //      if (h->GetBinContent(i)>0) cout << h->GetBinLowEdge(i) << " " << h->GetBinCenter(i) << " " << h->GetBinLowEdge(i+1) << endl;
      h->SetBinContent(i,0);
    }
  }
}


void m4l100Histo(THStack* s) {

  TListIter hist(s->GetHists());
  TH1F* h;
  while (h = (TH1F*) hist.Next()) {
    m4l100Histo(h);
  }
}



TH1F* selectStepPlotBins(long h){
  return selectStepPlotBins((TH1F*) h);
}

TH1F* selectStepPlotBins(TH1F* h){
  int j=0;
  //  TH1F* h1 = new TH1F("nEventComplete", "nEventComplete", 12, 0., 12.);
  TH1F* h1 = h->Clone();
  h->SetBinContent(12,0); //FIXME: fix uninitialized value
//   h->GetXaxis()->SetBinLabel(3,"Z1");    
//   h->GetXaxis()->SetBinLabel(4,"m_{Z1}");
//   h->GetXaxis()->SetBinLabel(5,"Z1+l");  
//   h->GetXaxis()->SetBinLabel(7,"m_{Z2}");
//   h->GetXaxis()->SetBinLabel(8,"pT>20,10");
//   h->GetXaxis()->SetBinLabel(9,"m_{ll}>4");
//   h->GetXaxis()->SetBinLabel(10,"m_{4l}>70");
//   h->GetXaxis()->SetBinLabel(11,"m_{4l}>100");

  for (int i = 1; i <= h->GetNbinsX(); ++i){
    h1->SetBinContent(i,0);
    h1->SetBinError(i,0);    
    if (i == 6 || i == 8) continue;
    ++j;
    h1->SetBinContent(j,h->GetBinContent(i));
    h1->SetBinError(j,h->GetBinError(i));
    //h1->SetBinError(j,1);
    h1->GetXaxis()->SetBinLabel(j,h->GetXaxis()->GetBinLabel(i));
  }
  h1->SetStats(false);
  return h1;
}

THStack* selectStepPlotBins(long s) {
  return selectStepPlotBins((THStack*)s);
}

THStack* selectStepPlotBins(THStack* s) {
  THStack* res = new THStack("res","res");
  TListIter hist(s->GetHists());
  TH1F* h;
  while (h = (TH1F*) hist.Next()) {
    res->Add(selectStepPlotBins(h));
  }
  return res;
}



// Tongguang's scaling for m4l<100 (on an already scaled sample)
TH1F* scaleBuggyPowheg(TH1F* h) {  
  TF1* sc = new TF1("sc",  "1+(3.95*(exp(-pow((x-91.2)/2.60,2))))*1.2566");
  TH1F* h1 = (TH1F*) h->Clone();
  TH1F* hh = (TH1F*) h->Clone();
  hh->Reset();
  for (int i = 1; i<= hh->GetNbinsX(); ++i) { // Skip over/underflow
    hh->SetBinContent(i,sc->Eval(hh->GetBinCenter(i)));
  }
  h1->Multiply(hh);
  return h1;
}

THStack* scaleBuggyPowheg(THStack* stack) {
  THStack* s = (THStack*) stack->Clone();

  TH1F* h;
  TListIter hist(s->GetHists());
  while (h = (TH1F*) hist.Next()) {
    TString sample = h->GetTitle();
    if (sample=="ZZ") {
      TH1F* h2 = scaleBuggyPowheg(h);
      (*h)=(*h2);
    }
  }
  return s;
}


THStack* scaleBuggyPowheg(long stack) {
  return scaleBuggyPowheg((THStack*) stack);
}


TLegend* getLegend() {
  return (TLegend*) gPad->GetListOfPrimitives()->FindObject("TPave");
}


void drawCMSPrel(float lumi=-1) {
//   if (lumi<0) lumi=LumiNormalization::getLuminosity();
//   int CME = LumiNormalization::getCME();
  if (lumi<0) lumi=20000.;
  int CME = 8;

  TString lumist;
//   if (lumi<1000.){
//     lumist = lumist + (long) (lumi+0.5) + " pb^{-1}";
//   } else {
//     long ipart = (long) (lumi/1000.+0.5);
//     long dpart = (long) ((lumi/1000.-ipart)*100. + 0.5);
//     lumist = lumist + ipart + "." + dpart +" fb^{-1}";
//   }
  if (fabs(lumi-20000)<1.) {
    lumist = "20.0 fb^{-1}";
  }
  
  
  if (CME == 7 || CME == 8) {
    TPaveText *ll = new TPaveText(0.15, 0.95, 0.95, 0.99, "NDC");
    ll->SetTextSize(0.03);
    ll->SetTextFont(42);
    ll->SetFillColor(0);
    ll->SetBorderSize(0);
    ll->SetMargin(0.01);
    ll->SetTextAlign(12); // align left
    TString text = "CMS Preliminary";
    ll->AddText(0.01,0.5,text);
    
    text = "#sqrt{s} = ";
    text += (long) CME;
    text += " TeV, L = ";
    text = text + lumist;
    //  ll->SetTextAlign(32); // align right
    ll->AddText(0.65, 0.6, text);
    ll->Draw();
  } else {
    TPaveText *ll = new TPaveText(0.15, 0.95, 0.95, 0.99, "NDC");
    ll->SetTextSize(0.03);
    ll->SetTextFont(42);
    ll->SetFillColor(0);
    ll->SetBorderSize(0);
    ll->SetMargin(0.01);
    ll->SetTextAlign(12); // align left
    TString text = "CMS Preliminary";
    ll->AddText(0.01,0.5,text);
    
    text = "#sqrt{s} = 7 TeV, L = 5.05 fb^{-1} ; #sqrt{s} = 8 TeV, L = 12.21 fb^{-1}" ;
    //  ll->SetTextAlign(32); // align right
    ll->AddText(0.3, 0.6, text);
    ll->Draw();


  }
}


TString getName(TString sample) {
  if (sample.BeginsWith("h", TString::kIgnoreCase)) {
    sample.Replace(0,1,"m_{H}=");
    sample=sample+" GeV";
  } else if (sample == "tt") {
    sample = "t#bar{t}";
  } else if (sample == "Zbb/cc") {
    sample = "Zb#bar{b}/c#bar{c}";
  } else if (sample == "ZZtau") {
    sample = "ZZ (#tau)";
  } else if (sample == "Z+jets") {
    sample = "Z+light jets";
  } else if (sample == "ZZ") {
    sample = "Z#gamma^{*},ZZ";
  } else if (sample == "WG") {
    sample = "W#gamma^{*}";
  }
  return sample;
}


TGraphAsymmErrors* getDataGraph(TH1F* h, bool drawZeroBins=false) {

  float fX[1000];
  float fY[1000];
  float fEXlow[1000];
  float fEXhigh[1000];
  float fEYlow[1000];
  float fEYhigh[1000];  

  TAxis *xaxis = ((TH1*)h)->GetXaxis();

  double q=(1-0.6827)/2.;

  int ibin=0;
  for (Int_t i=0; i<h->GetNbinsX(); ++i) {
    float yy = h->GetBinContent(i+1);
    if (drawZeroBins || yy > 0){
      float xx = xaxis->GetBinCenter(i+1);
      fX[ibin] = xx;
      fY[ibin] = yy;
      fEXlow[ibin]  = 0.;
      fEXhigh[ibin] = 0.;
      double N = yy;
      fEYlow[ibin]  = (N==0)?0:(N-ROOT::Math::chisquared_quantile_c(1-q,2*N)/2.);
      fEYhigh[ibin] = ROOT::Math::chisquared_quantile_c(q,2*(N+1))/2.-N;
      //      cout << fEYlow[ibin] << " " << N << " " << fEYhigh[ibin] << endl;
// Old formula from http://www-cdf.fnal.gov/physics/statistics/notes/pois_eb.txt 
//       fEYlow[ibin]  = -0.5+sqrt(yy+0.25); 
//       fEYhigh[ibin] =  0.5+sqrt(yy+0.25);
      ++ibin;
    }
  }


  TGraphAsymmErrors* g = new TGraphAsymmErrors(ibin, fX, fY, fEXlow, fEXhigh, fEYlow, fEYhigh);
  h->TAttLine::Copy(g);
  h->TAttFill::Copy(g);
  h->TAttMarker::Copy(g);
  g->SetMarkerStyle(20);
  g->SetMarkerSize(1.1);

//   g->SetName(h->GetName());
//   g->SetTitle(h->GetTitle());

  return g;
}


//Replace reducible bgs with data-driven estimates.
//Set: 0 = baseline, 1 = relaxed(m>70); 2 = high-mass, 3 = mass range around signal, 4 = mela>0.5
//var: "ZZ" -> ZZ plot, use Z+jet landau shape
//   : "Z2" -> Z2 plot, use Z+jet landau shape in either 12,120 or 60, 120
//   : "Z1" -> Z1 plot, use ZZ histogram
THStack* replaceDD(THStack* stack, float lumi, TString finalState, TString epoch,  int sel, TString var)
{

  int fs=-1;
  if (finalState=="2e2mu") { 
    fs=0;
  } else if (finalState=="4e") {
    fs=1;
  } else if (finalState=="4mu"){
    fs=2;
  } else return 0;


  THStack* s = (THStack*) stack->Clone();

  TF1 ZjetLandau("ZjetLandau","landau",0,1000);
  float xMinNorm=0;
  float xMaxNorm=1000;

  float MPV   = 0;
  float Sigma = 0;

  TH1F* Z1Hist = 0;
  

  //Shape paramenters (mzz, mz1, mz2)

  if(var=="ZZ"){ // Set shape/norm 
    if (sel==2)  { // High mass (taken from 8TeV, 7TeV are almost identical)
      if (fs==0) { // 2e2mu
	MPV          = 209.2; 
	Sigma        = 24.3; 
      } else if (fs==1) { // 4e
	MPV          = 210.8; 
	Sigma        = 34.9; 
      } else { // 4mu
	MPV          = 209.2;
	Sigma        = 24.3;
      }
    } else { // baseline, relaxed (taken from 8TeV, 7TeV are almost identical)
      if (fs==0) { // 2e2mu
	MPV          = 142.6;
	Sigma        = 19.8;
      } else if (fs==1) { // 4e
	MPV          = 146.9;
	Sigma        = 21.3;
      } else { // 4mu
	MPV          =  138.4;
	Sigma        =  20.8;
      }
    } 
  } else if(var=="Z2") { // Set shape/norm
     if (sel==2) { // High mass
      xMinNorm =60.;
      xMaxNorm =120.;
      if (fs==0) { // 2e2mu
	MPV          = 58.68.; 
	Sigma        = 15.40; 
      } else if (fs==1) { // 4e
	MPV          = 58.68; 
	Sigma        = 15.40; 
      } else { // 4mu
	MPV          = 58.68; 
	Sigma        = 15.40; 
      }
    } else  { // baseline, relaxed
      xMinNorm =12.;
      xMaxNorm =120.;
      if (fs==0) { // 2e2mu
	MPV          = 22.11;
	Sigma        = 7.831;
      } else if (fs==1) { // 4e
	MPV          = 25.75;
	Sigma        = 7.762;
      } else { // 4mu
	MPV          =  25.26;
	Sigma        =  20.08;
      }
    }
  } else if(var=="Z1"||var.Contains("LD")){ // For Z1, pick shape from ZZ
    TListIter hist(s->GetHists());
    while (h = (TH1F*) hist.Next()) {
      TString sample = h->GetTitle();
      if (sample=="ZZ") Z1Hist = h->Clone(); // Here we clone the ZZ histo
    }
  }
  
  
  
  ZjetLandau.SetParameters(1, MPV,Sigma);
    
  
  float refLumi = 0;
  if (epoch.Contains("2011")){
    //7 TeV normalization. Approval
    refLumi = 5051.; //7TeV


    // values in the 121-131 column  are obtained as:
    // ZjetLandau->Integral(121,131)/ZjetLandau->Integral(100,600)

    // 0 = baseline
    // 1 = relazed (M>70)
    // 2 = ZZ hgh-mass for x-section
    // 3 = within range (fraction is computed from shape)
    // 4 = cut on MELA (from California)
    
    //                     0    1    2   3    4
    double Zjet[3][5] = {{ 2.6, 2.6, 0., 2.6, 0.98}, // 2e2mu 
			 { 1.6, 1.6, 0., 1.6, 0.78}, // 4e  
			 { 1.0, 1.0, 0., 1.0, 0.15}};// 4mu
    
  } else if (epoch.Contains("2012")) {
    //8 TeV normalization. Approval
    refLumi = 5261.; //8TeV

    // 0 = baseline
    // 1 = relazed (M>70)
    // 2 = ZZ hgh-mass for x-section
    // 3 = within range (fraction is computed from shape)
    // 4 = cut on MELA (average between CJLST and California)

    //                     0    1    2     3    4
    double Zjet[3][5] = {{ 2.4, 2.4, 0.51, 2.4, 1.38}, // 2e2mu 
			 { 1.4, 1.4, 0.41, 1.4, 0.78}, // 4e  
			 { 1.2, 1.2, 0.44, 1.2, 0.50}};// 4mu

  } else {
    cout "ERROR: replaceDD: epoch not recognized" << endl;
    return;
  }
  



  for (int i=0; i<3; ++i){    
    for (int j=0; j<5; ++j){  
      Zjet[i][j] *=lumi/refLumi;
    }    
  }
  
  
  if (var!="ZZ"&&sel==3) {
    Zjet[0][sel] *= 0.0837; //FIXME hardcoded range!!! ZjetLandau.Integral(121,131)/ZjetLandau.Integral(0,1000)
    Zjet[1][sel] *= 0.0733; 
    Zjet[2][sel] *= 0.0856; 
  }

  if (var=="LDlow"&&sel==0) {
    Zjet[0][sel] *= 0.569; //FIXME hardcoded range!!! ZjetLandau.Integral(100,180)/ZjetLandau.Integral(0,1000)
    Zjet[1][sel] *= 0.532;
    Zjet[2][sel] *= 0.586;    
  }
  if (var=="LDhi"&&sel==0) {
    Zjet[0][sel] *= 0.422; //FIXME hardcoded range!!! ZjetLandau.Integral(180,1000)/ZjetLandau.Integral(0,1000)
    Zjet[1][sel] *= 0.460;
    Zjet[2][sel] *= 0.409;    
  }

  if (var=="pseudoLD"&&sel==4) {
    Zjet[0][sel] *= 0.0837; //FIXME hardcoded range!!! ZjetLandau.Integral(121,131)/ZjetLandau.Integral(0,1000)
    Zjet[1][sel] *= 0.0733; 
    Zjet[2][sel] *= 0.0856; 
  }


  TH1F* h;
  int nentry =  100000;
  TListIter hist(s->GetHists());
  while (h = (TH1F*) hist.Next()) {
    TString sample = h->GetTitle();

    if (sample=="Z+jets") {
      if (var =="Z1"||var.Contains("LD")) { // Take histo from Z1Hist
	Z1Hist->SetTitle("Z+X");
	Z1Hist->SetFillColor(kGreen-5); // kCyan+3
	Z1Hist->SetLineColor(kGreen-5); //
	*h = *Z1Hist;	
	h->Scale(Zjet[fs][sel]/h->Integral());

	cout << var << " " << sel << " " << Zjet[fs][sel] << endl;

      } else { // Take hardcoded shape/norm
	h->Reset();	
	h->FillRandom("ZjetLandau", nentry);	
	float integral = h->Integral(h->FindBin(xMinNorm), h->FindBin(xMaxNorm));
	h->Scale(Zjet[fs][sel]/integral);
	h->SetTitle("Z+X");
	h->SetFillColor(kGreen-5); // kCyan+3
	h->SetLineColor(kGreen-5); // 
      }
    }
//     if (sample=="H126") {
//       h->Scale(H126[fs]/h->Integral());
//     }
  }

  cout << (long) s <<endl;
  return s;
}

THStack* replaceDD(long stack, float lumi, TString finalState, TString epoch, int sel, TString var) {
  THStack* s(stack);
  replaceDD(s,lumi,finalState,epoch,sel,var);
}


void integrate(TH1F* h, float min, float max) {
  THStack s("stemp","stemp");
  s.Add(h);
  integrate(&s,min,max);
}

// Compute total integral and fraction below a given cut (including overflow)
// Nota: attenzione al binning (FindBin(X) prende il bin che contiene X -> controllare l'edge del bin)
void integrate(long stack, float min, float max, TString samples = "") {
  THStack* s(stack);
  integrate(s,min,max,samples);
}

void integrate(THStack* s, float min, float max, TString samples = "") {
  TListIter hist(s->GetHists());

  cout << setprecision(7);
  
  cout << setw(12)  <<"Sample" 
       << setw(12) <<"Total" 
       << setw(12) <<"Range" 
       << setw(12) <<"Range/tot (Range: " << min << " - " << max << " )"
       << endl;
  

  float tottot = 0;
  float totlt = 0;

  while (h = (TH1F*) hist.Next()) {
    TString sample = h->GetTitle();
    if (samples!="" && !samples.Contains(sample)) continue; 
    int ibin1 = h->GetXaxis()->FindBin(min);
    int ibin2 = h->GetXaxis()->FindBin(max);
    int nBins = h->GetXaxis()->GetNbins();

    float binedge1 = h->GetXaxis()->GetBinLowEdge(ibin1);
    float binedge2 = h->GetXaxis()->GetBinLowEdge(ibin2);


    if(binedge1!=min) cout << "PANIC: asked to integrate from:" << min << " but using bin edge:" << binedge1 <<endl;

    if (ibin2<=nBins) {
      ibin2=ibin2-1;
      if(binedge2!=max) cout << "PANIC: asked to integrate up to:" << max << " but using bin edge:" << binedge2 << " " << (long) h << endl;
    }
      
    //cout << ibin1 << " " << ibin2 << endl;
    
    //if (sample.BeginsWith("h"),TString::kIgnoreCase) continue;
    //cout <<sample<< " " << (long)h << endl;

    //if (sample.BeginsWith("Z+gamma"),TString::kIgnoreCase) continue;


    float total = h->Integral(0,9999999);
    if (total == 0) continue;
    double lterr =0;
    
    float lt  = h->IntegralAndError(ibin1,ibin2,lterr); // Remember to CHECK that bin boundaries are correct!    
    cout << setw(12) << sample
	 << setw(12) << total
	 << setw(12) << lt <<"+-"<< lterr
	 << setw(12) << lt/total
         //	 << setw(10) << h->Integral(ibin,99999)/total
         //	 << setw(10) << h->Integral(ibin,99999)
         //	 << setw(10) << h->Integral()
	 << endl;
    if (!sample.BeginsWith("h", TString::kIgnoreCase)) {
      tottot += total;
      totlt  += lt;
    }
  }
  if (tottot!=0) cout << setw(12) << "Tot (no sig)" 
		      << setw(12) << tottot
		      << setw(12) << totlt
		      << setw(12) << totlt/tottot << endl;
}



// Step plots are filled without errors... set them (before any scaling!)
void stepPlotError(TH1F*h) {
  h->Sumw2();
  for (int i = 1; i <= h->GetNbinsX(); ++i){
    h->SetBinError(sqrt(h->GetBinContent(i)));
  }
}




TH1F* getTotalMC(THStack* s) {
  TH1F* MC = 0;
  TListIter hist(s->GetHists());

  while (h = (TH1F*) hist.Next()) {
    TString sample = h->GetTitle();
    //    if (!sample.BeginsWith("ttH", TString::kIgnoreCase)) // To avoid stacking signal
    {  
      if (MC==0) {
	MC = (TH1F*) h->Clone();
      } else {
	MC->Add(h);
      }
    }
  }
  return MC;
}



TString getPitchString(TH1 *h, int prec) const {
  float pitch = h->GetBinWidth(1);
  stringstream ss;
  ss << setprecision(prec);
  ss << pitch;
  TString buffer;
  ss >> buffer;
  return buffer;
}


void setAxisTitles(TH1F* h, TString xTitle, TString yTitle) {
  if (yTitle!="Arbitraty units" && yTitle!="Events") {
    TString pitch = getPitchString(h,2);
    yTitle = "Events / "+pitch+" "+yTitle;
  }
  h->GetXaxis()->SetTitle(xTitle.Data());
  h->GetYaxis()->SetTitle(yTitle.Data());
}


void setAxisTitles(THStack* s, TString xTitle, TString yTitle) {
  TListIter hist(s->GetHists());
  TH1F* h = (TH1F*) hist.Next();
  if (yTitle!="Arbitraty units" && yTitle!="Events") {
    TString pitch = getPitchString(h,2);
    yTitle = "Events / "+pitch+" "+yTitle;
  }
  s->GetXaxis()->SetTitle(xTitle.Data());
  s->GetYaxis()->SetTitle(yTitle.Data());
}

// Return h = sum of all stack histograms
TH1F* addStack(THStack *s){
  TH1F* result;
  TListIter hist(s->GetHists());
  while (h = (TH1F*) hist.Next()) {
    if (result==0) {
      result = (TH1F*) h->Clone();
    } else {
      result->Add(h);
    }
  }
  result->SetFillColor(0);
  result->SetLineColor(1);
  return result;
}

TH1F* addStack(long stack){
  THStack* s(stack);
  return addStack(s);
}

// mode: 
//   normUnity = normalize to unity
// drawopt: options passed to Draw()

void drawStack(long stack, long data=0, int rebin = 1, TString mode = "", TString drawopt = "", TString xTitle ="", TString yTitle = "", float xmin=-1., float xmax=-100., float ymin=-1, float ymax=-100) {
  THStack* s(stack);
  TH1F* d(data);
  drawStack(s,d,rebin,mode,drawopt,xTitle,yTitle,xmin, xmax, ymin, ymax);
}


void drawStack(THStack* stack, TH1F* data=0, int rebin = 1, TString mode = "", TString drawopt = "", TString xTitle ="", TString yTitle = "", float xmin=-1., float xmax=-100., float ymin=-1, float ymax=-100) {
  
  float labelThreshold = 5e-3; // Do not list in legend if below this //-1 if remove this option

  if (stack==0) { 
    cout << "DrawStack: no plot" << endl;
    return;
  }
  cout << "drawStack: " << stack->GetName() << " " << (long) stack
       << ", "  << (long) data << endl;
  THStack* s = (THStack*) stack->Clone();
  TH1F* d = 0;

  bool blind = false;
  bool m4l100 = false;
  bool drawZeroBins = false;
  bool drawOverflow = false;

  if (mode.Contains("blind")) blind=true;

  if (mode.Contains("m4l100")) m4l100=true;

  if (mode.Contains("draw0Bins")) drawZeroBins=true;

  if (mode.Contains("drawOverflow")) drawOverflow=true;


  TLegend* legend = new TLegend( 0.67, 0.64, 0.92, 0.95);
  cout << (long) legend <<endl;
  legend->SetFillColor(kWhite);
  legend->SetTextSize(0.04);
  legend->SetTextFont(42);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);

  float maxy =0;
  if (data) {
    d = (TH1F*) data->Clone();

    if (drawOverflow) {
      d = setOverflowBin(d,xmax);
    }

    if (blind) blindHisto(d);
   
    if (m4l100) m4l100Histo(d);

    d->Rebin(rebin);
    setAxisTitles(d, xTitle, yTitle);
    //    drawopt = drawopt + "AH";
    maxy = d->GetMaximum()+1;
    TGraphAsymmErrors* g = getDataGraph(d, drawZeroBins);
    legend->AddEntry(g, "Data", "P");
  }
  
  if (m4l100) m4l100Histo(s);

  bool noStack = false;
  if(mode.Contains("noStack")){
    noStack = true;
    drawopt = drawopt + "nostack";
  }

  // Draw signal without fill
  bool noFillSignal = true;
  bool signalNoStacked = false;
  bool signalNoStackedFaint = false;

  bool normUnity = false;
  if (mode.Contains("normUnity")) {
    normUnity=true;
    drawopt = drawopt + "nostack";
    if (yTitle=="") yTitle = "Arbitraty units";
  }


  //  int ifill = 1;
  THStack* ssig = 0;
  THStack* sback = 0;

  
  int lineStyles[4] = {2,5,8,6};
  int lineColors[4] = {kRed,kRed+3,kBlue+3,kBlue};
  int iLineStyle=0;

  if (signalNoStacked) {
    ssig = new THStack("ssig","ssig");
  }
  sback = new THStack("sback","sback");

  TListIter hist(s->GetHists());
  TH1F* h;
  while (h = (TH1F*) hist.Next()) {
    h->Rebin(rebin);
    if (drawOverflow) {
      h = setOverflowBin(h,xmax);
    }

    bool isSignal = false;
    TString sname = h->GetTitle();
    TString sample = getName(sname);
    if (sname.BeginsWith("h", TString::kIgnoreCase)) {
      isSignal = true;
    }

//    cout << h->GetTitle() << sample << " " << isSignal << " " << noFillSignal << endl;    
    if (normUnity) {      
      float integral = h->Integral();
      if (integral>0.) h->Scale(1./integral);      
      h->SetFillStyle(0);
      h->SetLineWidth(2);
    } else {
      if (!noFillSignal || !isSignal) {
	h->SetLineColor(1);
	h->SetLineWidth(2); 
	
	//	if (signalNoStackedFaint && sname=="ZZ") h->SetFillColor(kMagenta-10);

	if (noStack){
	  h->SetLineColor(h->GetFillColor());
	  h->SetFillStyle(0);
	  //	h->SetLineWidth(7);
	}
	sback->Add(h,"hist");
      } else { // Signal with only line
	h->SetLineColor(h->GetFillColor());
	h->SetFillColor(kWhite);
	h->SetFillStyle(1001);
// 	h->SetFillStyle(3000+ifill); ifill++;
	//	h->SetLineColor(1);
	if(signalNoStacked) {
	  h->SetLineStyle(lineStyles[iLineStyle]);
	  if(!signalNoStackedFaint) h->SetLineColor(lineColors[iLineStyle]);
	  iLineStyle++;
	}
	
	//h->SetLineStyle(2);
	//h->SetLineColor(kRed+3);
	h->SetLineWidth(2);
	if (signalNoStacked) ssig->Add(h,"hist");
	else sback->Add(h,"hist");
      }
    }
    

//    if (h->Integral() > labelThreshold) legend->AddEntry(h, sample, (isSignal&&noFillSignal?"L":"F"));
    if (h->Integral() > labelThreshold) legend->AddEntry(h, sample, "F");
  }

  s = sback;

  float ymaxMC= s->GetMaximum();
  float yminMC= s->GetMinimum();  
  
  float ymaxdata = 0;
  float ymindata = 0;
  if (d) {
    ymaxdata = d->GetMaximum();
    ymindata = d->GetMinimum();
  }

  //  cout << xmin << " " << ymin << " " << xmax << " " << ymax << endl;

  if (xmin>xmax) { // if x range not specified, keep default
    TListIter hist2(s->GetHists());
    h = (TH1F*) hist2.Next();
    xmin= h->GetBinLowEdge(1);
    xmax= h->GetBinLowEdge(h->GetNbinsX()+1);
  }
  
  if (ymin>ymax) { // if y range not specified, try autorange default
    ymin = min(yminMC,ymindata);
    ymax = max(ymaxMC,ymaxdata)*1.05;
    if (gPad->GetLogy()) {
      ymax *= 2;
      if (ymin<=0) ymin = ymax/1.e4;
    }
  } else { // Fix stupid scaling of max/min
    ymax = ymax/1.05;
    if (gPad->GetLogy()) ymin=ymin*3.;
  }
  

  // This works fine, but then you can't unzoom, and SetLogY interactively
  //  TH1F* hframe = gPad->DrawFrame(xmin,ymin, xmax, ymax);
  //  setAxisTitles(hframe, xTitle, yTitle);
  //  drawopt = "same"+drawopt;
  

  // This does not work, should move it before s is taken to remove signal (see above)
  if (mode.Contains("normToData")){
    float norm = d->Integral()/getTotalMC(s)->Integral();
    cout << "NORMTODATA" <<endl;
    TListIter hist3(s->GetHists());
    TH1F* h3;
    while (h3 = (TH1F*) hist3.Next()) {
      cout << norm << endl;
      h3->Scale(norm); //FIXME does not seem to work???
    }
    cout << (long) s << endl;

  }

  s->Draw(drawopt.Data());
  if (xmax>xmin) s->GetXaxis()->SetRangeUser(xmin, xmax);


  //  if (ymax>ymin) s->GetYaxis()->SetRangeUser(ymin, ymax);
  if (ymax>ymin) {
    if (ymin!=0) s->SetMinimum(ymin);
    s->SetMaximum(ymax);
  }

  if (ymax>ymin) s->GetYaxis()->SetRangeUser(ymin, ymax);
  setAxisTitles(s, xTitle, yTitle);
  
//   cout << xmin << xmax << s->GetXaxis()->GetXmin() << " " << s->GetXaxis()->GetXmax() << endl;

  gPad->Update();
  

  if (signalNoStacked) ssig->Draw("samenostack");

  if (d) {
    // d->Draw("esame");
    if (g->GetN()>0) g->Draw("sameP"); //add X for no errorbars
  }

  if (blind) {
    //    legend->SetFillStyle(1001);
    TBox * myBlindingBox1 = new TBox (111,0,139,ymax*1.05);
    myBlindingBox1->SetFillColor(kGray);
    myBlindingBox1->SetFillStyle(3001);
    myBlindingBox1->Draw();

    float xmaxbox = xmax;
    if (d) xmaxbox = d->GetXaxis()->GetBinUpEdge(d->FindBin(xmax));
    cout << xmax << " " << xmaxbox << " " << d->GetBin(xmax) << endl;
    TBox * myBlindingBox2 = new TBox (299,0,xmaxbox,ymax*1.05);
    myBlindingBox2->SetFillColor(kGray);
    myBlindingBox2->SetFillStyle(3001);
    myBlindingBox2->Draw();
  }
  legend->Draw("same");

  drawCMSPrel();
}


void drawStepPlot(long stack, long data, TString xTitle ="", TString yTitle = "") {
  THStack* s(stack);
  TH1F* d(data);
  drawStepPlot(s,d,xTitle,yTitle);
}

void drawStepPlot(THStack* stack, TH1F* data, TString xTitle ="", TString yTitle = "", int minX=0, int maxX=-1, float ymin=1e-2, float ymax=2e5) {
  cout << "drawStepPlot: " << stack->GetName() << " " << (long) stack
       << ", "  << (long) data << endl;


  TLegend* legend = new TLegend( 0.6, 0.5, 0.92, 0.92);
  legend->SetFillColor(kWhite);
  legend->SetTextSize(0.035);

  THStack* s = (THStack*) stack->Clone();
  TH1F* d=0;
  if (data) {
    d = (TH1F*) data->Clone();
    legend->AddEntry(d, "DATA", "P");
  }
  
  //  cout << min << " " << max << endl;

  TListIter hist(s->GetHists());
  TH1F* h;
  int i=0;
  TString opt = "phistsame"; //PE1X0 : error bars Y only

  TH1F* MC = getTotalMC(s);

  if (maxX>minX) MC->GetXaxis()->SetRangeUser(minX-1, maxX-1);
  if (ymax>ymin) MC->GetYaxis()->SetRangeUser(ymin, ymax);

  MC->SetMarkerStyle(1);
  MC->SetMarkerColor(1);
  //  TH1F* MC2 = (TH1F*) MC->Clone();
  TH1F* MC2 = MC;
  MC2->SetMarkerColor(kGray+1);
  MC2->SetFillColor(kGray+1);
  MC2->SetFillStyle(1);

  MC2->GetYaxis()->SetTitleSize(0.05);
  setAxisTitles(MC2, xTitle, yTitle);

  MC2->Draw("e2");
  //   MC->SetLineColor(1);
  //   MC->SetFillColor(0);
  //  MC->Draw("histsame");



  while (h = (TH1F*) hist.Next()) {

    //FIXME: skip first bin (skim)

    //FIXME mark signal in a decent way
    TString sample = h->GetTitle();
    
//     if (i==0) {
//       //No idea why this is not taken from the style
//       //      h->GetXaxis()->SetLabelFont(42);
//       //      h->GetXaxis()->SetTitleFont(42);
//       //      h->GetYaxis()->SetLabelFont(42);
//       //      h->GetYaxis()->SetTitleFont(42);
//       h->GetYaxis()->SetTitleSize(0.05);
//       //h->SetLabelSize(0.005);
//       setAxisTitles(h, xTitle, yTitle);
//     }
      
//     TAxis * a = h->GetXaxis();
//     a->SetBinLabel(1,"Skim"); 
//     a->SetBinLabel(2,"HLT");   
//     a->SetBinLabel(3,"Z+l");    
//     a->SetBinLabel(4,"Z+l,loose iso");  
//     a->SetBinLabel(5,"Z+ll");  
//     a->SetBinLabel(6,"Preselection"); 
//     a->SetBinLabel(7,"Iso");      
//     a->SetBinLabel(8,"Iso(pT)");
//     a->SetBinLabel(9,"IP");
//     a->SetBinLabel(10,"Kin");
    h->SetFillStyle(0);
    h->Draw(opt.Data());


    TString sample = getName(h->GetTitle());
    legend->AddEntry(h, sample, "P");
    i++;
  }

  if (d) {
    d->Draw("esame");
  }

  legend->Draw("same");
  drawCMSPrel();
}

void draw2D(long is, long ib, int binx=1, int biny=1) {
  TH2F* s(is);
  TH2F* b(ib);
  draw2D(s, b, binx, biny);
}


void draw2D(TH2F* s, TH2F* b, int binx=1, int biny=1, TString xTitle ="", TString yTitle = "", float xmin=-1, float xmax=-1, float ymin=-1, float ymax=-1)
{
  if (s==0 || b==0) {
    cout << "draw2D: invald plot" << endl;
    return;
  }
  
  cout << "draw2D: " << s->GetName() << " " << b->GetName() << " "
       << (long) s << ", " << (long) b << endl;
  TH2F* h2s = (TH2F*) s->Clone();
  TH2F* h2b = (TH2F*) b->Clone();

  h2s->Rebin2D(binx, biny);
  h2b->Rebin2D(binx, biny);

//   float integral = h2s->Integral();
//   if (integral>0.) h2s->Scale(1./integral);
//   integral = h2b->Integral();
//   if (integral>0.) h2b->Scale(1./integral);


  h2s->(binx, biny);
  h2b->Rebin2D(binx, biny);

  //   h2s->SetFillColor(kRed);
  h2s->SetMarkerColor(kRed);
  h2s->SetMarkerStyle(1);

  h2b->SetMarkerColor(kGreen);
  h2b->SetMarkerStyle(1);

  
  h2b->Draw("");
  if (xmax>xmin) h2b->GetXaxis()->SetRangeUser(xmin, xmax);
  if (ymax>ymin) h2b->GetYaxis()->SetRangeUser(ymin, ymax);
  h2b->GetXaxis()->SetTitle(xTitle.Data());
  h2b->GetYaxis()->SetTitle(yTitle.Data());
  h2s->Draw("same");
}

void stepPlotYield2(THStack* s, TH1F* d, int firstbin=2, int lastbin=-1) {

  TListIter hist(s->GetHists());
  bool first = true;
  bool printErr = true;
  TString sep = " ";
  TString pm  = "+-";

  int precision = 4;
  int colw = 8;

  TH1F* MC = 0;
  cout << setprecision (precision);
  //  cout << setiosflags(ios::fixed);

  while (h = (TH1F*) hist.Next()) {
    if (first) {
      if (lastbin<0) lastbin = h->GetNbinsX();

      TAxis * a = h->GetXaxis();
//       a->SetBinLabel(1,"Skim"); 
//       a->SetBinLabel(2,"HLT");   
//       a->SetBinLabel(3,"Z+l");    
//       a->SetBinLabel(4,"Z+l(LI)");  
//       a->SetBinLabel(5,"Z+ll(LI)");  
//       a->SetBinLabel(6,"Presel"); 
//       a->SetBinLabel(7,"Iso");      
//       a->SetBinLabel(8,"Iso(pT)");
//       a->SetBinLabel(9,"IP");
//       a->SetBinLabel(10,"Kin");

      cout << setw (colw) << "Sample";
      for (int i=firstbin; i<=lastbin; ++i) {
	TString label = a->GetBinLabel(i);
	int colsize=colw;
	//	if (printErr) colsize =2*colw+2;
	if (label.Sizeof()>colsize+1) label.Resize(colsize);
	if (label=="") label+=(long)i;
	cout << setw (colsize) << label.Data();
	if (printErr) cout << setw (colsize) << " ";
      }
      cout << endl;
      first = false;
    }

    TString sample = h->GetTitle();
    if (sample.Sizeof()>colw+1) sample.Resize(colw);
    cout << setw (colw) << sample;    
    for (int i=firstbin; i<=lastbin; ++i) {
      cout << setw (colw) << h->GetBinContent(i);
      if (printErr)  cout  << pm << setw(colw) << setiosflags(ios::left) << h->GetBinError(i) << resetiosflags(ios::left);
    }
    cout << endl;

    if (!sample.BeginsWith("h",TString::kIgnoreCase)) {
      if (MC==0) {
	MC = (TH1F*) h->Clone();
      } else {
	MC->Add(h);
      }
    }
  }
  cout << "--------" << endl;
  cout << setw (colw) << "Total";
  for (int i=firstbin; i<=lastbin; ++i) {
    cout << setw (colw) << MC->GetBinContent(i);
    if (printErr) cout << pm << setw(colw) << setiosflags(ios::left) << MC->GetBinError(i) << resetiosflags(ios::left);;
  }
  cout << endl;
    
  if (d) {
    cout << setw (colw) << "Data";
    for (int i=firstbin; i<=lastbin; ++i) {
      cout << setw (colw) << d->GetBinContent(i);
    }
    cout << endl;
  }
}


//format: 0 = all errors
//        1 = error only on total
//        2 = same as 1, in latex
void stepPlotYield(THStack* s, TH1F* d, int firstbin=2, int lastbin=-1, int precision=3, int format =0) {

//   float v =2.34554;
//   float e = 0.0234;
//   if (e<1.) {
//     int dig = -floor(TMath::Log10(e));
//     if (int(e*pow(10,dig)+0.05)==1)
//   }

//   int precision = 3;  
//   int colw      = 10; 
//   bool printErr = true;

  bool printErr    = (format==0);
  bool printErrTot = true;
  bool latex = (format==2);
  int colw = 7+precision;

//   if (printErr) {
//     colw      = 9;
//   }
  
  int firstcolw = 12;

  TH1F* MC = 0;
  cout << setprecision (precision);

  TList* hists = s->GetHists();
  int nSamples = hists->GetSize();  

  TString sep = " ";
  TString pm  = "+-";
  TString end = "";
  if (latex){
    sep = " & ";
    pm = "$\pm$";
    end = " \\\\";
  }
  

  // Header
  cout << setw (firstcolw) << "Cut";
  for (int j=0; j<nSamples; ++j) {
    TH1F* h = (TH1F*) hists->At(j);
    TString sample = h->GetTitle();
    if (latex && sample=="WW" || sample=="h130" || sample=="h140" || sample=="h150" || sample=="h350" || sample=="Single top" || sample=="W+jets") continue;
    cout << sep << setw (colw) << sample; 
    if (printErr)  cout  << sep << setw (colw) << setiosflags(ios::left) << pm << resetiosflags(ios::left);
  }
  cout << sep << setw (colw) << " Total";
  if (printErrTot)  cout  << setw (colw) << setiosflags(ios::left) << pm << resetiosflags(ios::left);
  if (d) cout << sep << setw (colw) << "Data";
  cout << end << endl;
  
  TH1F* MC = getTotalMC(s);

  if (lastbin<0) lastbin = MC->GetNbinsX();

  // Table
  for (int i=firstbin; i<=lastbin; ++i) {
    TH1F* h0 = (TH1F*) hists->At(0);
    TString label = h0->GetXaxis()->GetBinLabel(i);
    if (label.Sizeof()>firstcolw) label.Resize(firstcolw);    
    cout << setw (firstcolw) << label.Data();
    for (int j=0; j<nSamples; ++j) {      
      TH1F* h = (TH1F*) hists->At(j);
      TString sample = h->GetTitle();
      if (latex && sample=="WW" || sample=="h130" || sample=="h140" || sample=="h150" || sample=="h350" || sample=="Single top" || sample=="W+jets") continue;
      cout  << sep << setw (colw) << h->GetBinContent(i);
      if (printErr)  cout  << pm << setw(colw) << setiosflags(ios::left) << h->GetBinError(i) << resetiosflags(ios::left);

    }
    // Total column
    cout  << sep << setw (colw) << MC->GetBinContent(i);
    if (printErrTot)  cout  << pm << setw(colw) << setiosflags(ios::left) << MC->GetBinError(i) << resetiosflags(ios::left);
    // Data column
    if (d) cout  << sep << setw (colw) << d->GetBinContent(i);
    cout << end << endl;
  }

  return;
  
  // Old format, swap rows and columns..
  bool first = true;

  while (h = (TH1F*) hist.Next()) {
    if (first) {
      if (lastbin<0) lastbin = h->GetNbinsX();

      TAxis * a = h->GetXaxis();
//       a->SetBinLabel(1,"Skim"); 
//       a->SetBinLabel(2,"HLT");   
//       a->SetBinLabel(3,"Z+l");    
//       a->SetBinLabel(4,"Z+l(LI)");  
//       a->SetBinLabel(5,"Z+ll(LI)");  
//       a->SetBinLabel(6,"Presel"); 
//       a->SetBinLabel(7,"Iso");      
//       a->SetBinLabel(8,"Iso(pT)");
//       a->SetBinLabel(9,"IP");
//       a->SetBinLabel(10,"Kin");

      cout << setw (colw) << "Sample";
      for (int i=firstbin; i<=lastbin; ++i) {
	TString label = a->GetBinLabel(i);
	if (label.Sizeof()>colw+1) label.Resize(colw);
	cout << setw (colw) << label.Data();
      }
      cout << endl;
      first = false;
    }

    TString sample = h->GetTitle();

    cout << setw (colw) << sample;    
    for (int i=firstbin; i<=lastbin; ++i) {
      cout << setw (colw) << h->GetBinContent(i);
    }
    cout << endl;

    if (!sample.BeginsWith("h", TString::kIgnoreCase)) {
      if (MC==0) {
	MC = (TH1F*) h->Clone();
      } else {
	MC->Add(h);
      }
    }
  }
  cout << "---" << endl;
  cout << setw (colw) << "Total";
  for (int i=firstbin; i<=lastbin; ++i) {
    cout << setw (colw) << MC->GetBinContent(i);
  }
  cout << endl;
    
  if (d) {
    cout << setw (colw) << "Data";
    for (int i=firstbin; i<=lastbin; ++i) {
      cout << setw (colw) << d->GetBinContent(i);
    }
    cout << endl;
  }
}

void stepPlotYield(long stack, long data, int firstbin=2, int lastbin=-1, int precision=3, int format =0) {
  THStack* s(stack);
  TH1F* d(data);
  stepPlotYield(s,d, firstbin,lastbin, precision, format);
}

void writeStack(long stack, TString file) {
  THStack* s(stack);
  TListIter hist(s->GetHists());

  TFile f(file,"RECREATE");
  f.cd();
  while (h = (TH1F*) hist.Next()) {
    TString name = h->GetName();
    name = name + h->GetTitle();
    h->Write(h->GetTitle());
    cout << name << endl;    
  }
}



TH1F* skipBins(long hist, float xStartFrom=5) {
  TH1F* h(hist);
  return skipBins(h,xStartFrom);
}


// Allow bin start from the desired value.
// This macro in not general-purpose, it assumes a TH1 made with (2000, 0., 1000.)!
TH1F* skipBins(TH1F* h, float xStartFrom=5) {
  if (h==0) return 0;
  TH1F* h2 = new TH1F("temp1", "temp1", 2000, xStartFrom, 1000+xStartFrom);
  h2->SetFillColor(h->GetFillColor());
  h2->SetFillStyle(h->GetFillStyle());    
  h2->SetTitle(h->GetTitle());
  int firstbin=xStartFrom/0.5;
  for (int i=firstbin+1; i<h->GetNbinsX(); ++i) {
    h2->SetBinContent(i-firstbin,h->GetBinContent(i));
  }
  return h2;
}


// THStack* skipBins(long stack, float xStartFrom=5) {
//   THStack* s(stack);
//   return skipBins(s,xStartFrom);
// }


//Skip the first bins, so we can move bins a bit...
// THStack* skipBins(THStack* s) {
//   THStack* sr = new THStack("temp", "temp");

//   TH1F* hh = new TH1F("temp1", "temp1", 2000, 5., 1005.);
//   TListIter hist1(s->GetHists());
//   TH1F* h1 =0;
//   while (h1 = (TH1F*) hist1.Next()) {
//     TH1F* h2 = hh->Clone();
    
//     h2->SetFillColor(h1->GetFillColor());
//     h2->SetFillStyle(h1->GetFillStyle());    
//     h2->SetTitle(h1->GetTitle());
//     for (int i=11; i<h1->GetNbinsX(); ++i) {
//       h2->SetBinContent(i-10,h1->GetBinContent(i));
//     }
//     sr->Add(h2, "hist");
//     cout << h1->GetName() << endl;
//   }
//   return sr;
// }
THStack* skipBins(THStack* s, float xStartFrom=5) {
  THStack* sr = new THStack("temp", "temp");

  TListIter hist1(s->GetHists());
  TH1F* h1 =0;
  while (h1 = (TH1F*) hist1.Next()) {
    TH1F* h2 = skipBins(h1,xStartFrom);
    sr->Add(h2, "hist");
  }
  return sr;
}

THStack* add(long stack1, long stack2, long stack3) {
  THStack* s1(stack1);
  THStack* s2(stack2);
  THStack* s3(stack3);
  return add(s1,s2,s3);
}

//Add stack plots for three finals states (beware of overlaps!!)
THStack* add(THStack* s1, THStack* s2, THStack* s3) {

  //  THStack* s = new THStack("sum", "sum");
  THStack* s = s1->Clone();
  
  cout << (long) s << endl;

  TListIter hist1(s->GetHists());
  TListIter hist2(s2->GetHists()); 
  TListIter hist3(s3->GetHists());
 
  TH1F* h1 =0;
  while (h1 = (TH1F*) hist1.Next()) {
    TH1F* h2 = (TH1F*) hist2.Next();
    TH1F* h3 = (TH1F*) hist3.Next();
    if (h2==0 || h3 == 0) { // || h2->GetTitle()!=h1-GetTitle()) {
      cout << "ERROR" << endl;
      return;
    }
    //   TH1F* h = h1->Clone();
//    h->Add(h2);
//    s->Add(h1);
    h1->Add(h2);
    h1->Add(h3);
  }
  
  return s;
}

//Add stack plots for three finals states (beware of overlaps!!)
THStack* add(THStack* s1, THStack* s2) {

  //  THStack* s = new THStack("sum", "sum");
  THStack* s = s1->Clone();
  
  cout << (long) s << endl;

  TListIter hist1(s->GetHists());
  TListIter hist2(s2->GetHists()); 
 
  TH1F* h1 =0;
  while (h1 = (TH1F*) hist1.Next()) {
    TH1F* h2 = (TH1F*) hist2.Next();
    if (h2==0) { // || h2->GetTitle()!=h1-GetTitle()) {
      cout << "ERROR" << endl;
      return;
    }
    //   TH1F* h = h1->Clone();
//    h->Add(h2);
//    s->Add(h1);
    h1->Add(h2);
  }
  
  return s;
}

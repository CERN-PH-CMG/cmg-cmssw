#define NPOINTS 8

void plotLimits(TString channel="muTau",long sm=0){

  int mass[NPOINTS]={110,115,120,125,130,135,140,145};

  Float_t obs[NPOINTS];
  Float_t exp2[NPOINTS];
  Float_t exp16[NPOINTS];
  Float_t exp50[NPOINTS];
  Float_t exp84[NPOINTS];
  Float_t exp97[NPOINTS];


  for(Int_t m=0;m<NPOINTS;m++){
    float ob=0.;
    float ex2=0.;
    float ex16=0.;
    float ex50=0.;
    float ex84=0.;
    float ex97=0.;

    TString filenm="";
    if(sm==3)filenm=channel+"/combine_"+(long)(mass[m])+".log";
    else filenm=channel+"/combine_"+(long)sm+"_"+(long)(mass[m])+".log";
    cout<<"sm="<<sm<<" m="<<m<<" file="<<filenm.Data()<<endl;

    ifstream file;
    file.open(filenm.Data());  
    std::string str;
    file>>str;
    while(!file.eof()){
      if(TString(str.c_str())== "Observed"){//Observed Limit: r < 5.7200
	file>>str>>str>>str>>ob;
      }
      if(TString(str.c_str())== "Expected"){//Expected 50.0%: r < 4.7183
	file>>str;
	if(TString(str.c_str())== "2.5%:"){
	  file>>str>>str>>ex2;
	}
	if(TString(str.c_str())== "16.0%:"){
	  file>>str>>str>>ex16;
	}
	if(TString(str.c_str())== "50.0%:"){
	  file>>str>>str>>ex50;
	}
	if(TString(str.c_str())== "84.0%:"){
	  file>>str>>str>>ex84;
	}
	if(TString(str.c_str())== "97.5%:"){
	  file>>str>>str>>ex97;
	}
      }
      file>>str;
    }
    file.close();

    cout<<"mass="<<mass[m]<<" exp2="<<ex2<<" exp16="<<ex16<<" exp50="<<ex50<<" exp84="<<ex84<<" exp97="<<ex97<<" obs="<<ob<<endl;

    obs[m]=ob;
    exp2[m]=ex2;
    exp16[m]=ex16;
    exp50[m]=ex50;
    exp84[m]=ex84;
    exp97[m]=ex97;

  }


  TH1F h("hframe","",1,110,145);
  h.GetYaxis()->SetRangeUser(0,20);
  h.GetYaxis()->SetTitle("#sigma_{95%CL}/#sigma_{SM}");
  h.GetXaxis()->SetTitle("m_{H} (GeV)");
  h.SetStats(0);

  TGraphAsymmErrors GObs(NPOINTS);
  TGraphAsymmErrors GExp(NPOINTS);
  TGraphAsymmErrors GExp1(NPOINTS);
  TGraphAsymmErrors GExp2(NPOINTS);

  for(Int_t m=0;m<NPOINTS;m++){
    GObs.SetPoint(m,mass[m],obs[m]);
    GExp.SetPoint(m,mass[m],exp50[m]);
    GExp1.SetPoint(m,mass[m],exp50[m]);
    GExp1.SetPointError(m,0,0,exp50[m]-exp16[m],exp84[m]-exp50[m]);
    GExp2.SetPoint(m,mass[m],exp50[m]);
    GExp2.SetPointError(m,0,0,exp50[m]-exp2[m],exp97[m]-exp50[m]);
  }
  
  
  TCanvas C(channel+"limits_SM"+(long)sm);
  C.Clear();
  h.Draw("hist");

  GExp2.SetFillColor(5);
  GExp2.Draw("E3same");

  GExp1.SetFillColor(3);
  GExp1.Draw("E3same");

  GExp.SetLineColor(2);
  GExp.Draw("lsame");

  GObs.SetFillColor(0);
  GObs.Draw("plsame");

  TLegend legend;
  legend.SetFillColor(0);
  legend.SetLineColor(0);
  legend.SetBorderSize(1);
  legend.SetTextSize(.04);
  legend.SetX1NDC(.15);
  legend.SetX2NDC(.3);
  legend.SetY1NDC(.7);
  legend.SetY2NDC(.8);
  legend.AddEntry(&GObs,"observed","pl");
  legend.AddEntry(&GExp,"expected","l");
  legend.Draw("same");

  
  C.Print(channel+"/plotLimit_SM"+(long)sm+".ps");

  gROOT->ProcessLine(".q");
}

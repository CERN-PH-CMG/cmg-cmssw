#include <TString.h>
#include <TFile.h>
#include <TH1F.h>

std::string fillString(std::string input){
  if(input.size()>15){
    cout<<"input string size too long"<<endl;
    exit(0);
  }
  std::string output=input;
  for(Int_t i=0;i<15;i++)
    if(i>=input.size())output+=" ";
  
  return output;
}

std::string fillStringLong(std::string input){
  if(input.size()>40){
    cout<<"input string size too long"<<endl;
    exit(0);
  }
  std::string output=input;
  for(Int_t i=0;i<40;i++)
    if(i>=input.size())output+=" ";
  
  return output;
}

std::string fillInt(int input){

  char  inchar[100];
  sprintf(inchar,"%d",input);
  std::string newinput(inchar);
  if(newinput.size()>15){
    cout<<"input string size too long"<<endl;
    exit(0);
  }
  std::string output=newinput;
  for(Int_t i=0;i<15;i++)
    if(i>=newinput.size())output+=" ";
  
  return output;
}

std::string fillFloat(float input){

  char  inchar[100];
  sprintf(inchar,"%.3f",input);
  std::string newinput(inchar);
  if(newinput.size()>15){
    cout<<"input string size too long"<<endl;
    exit(0);
  }
  std::string output=newinput;
  for(Int_t i=0;i<15;i++)
    if(i>=newinput.size())output+=" ";
    
 return output;
}

void makeDataCard(TString channel="muTau",long sm=0, long mass=120){

  TString basename=channel+"_SM"+sm+"_mH"+mass;

  //open the root file with the distributions
  TString rootfilename=basename+".root";
  TFile rootfile(channel+"/"+rootfilename,"read");
  if(rootfile.IsZombie()){
    cout<<" could not open root file "<<rootfilename<<", check directory exists "<<channel<<endl;
    return;
  }
  rootfile.ls();
  
  TH1F* VH=  (TH1F*)rootfile.Get(channel+"_SM"+sm+"/VH"+mass);
  TH1F* SM=  (TH1F*)rootfile.Get(channel+"_SM"+sm+"/SM"+mass);
  TH1F* VBF= (TH1F*)rootfile.Get(channel+"_SM"+sm+"/VBF"+mass);
  TH1F* ZTT= (TH1F*)rootfile.Get(channel+"_SM"+sm+"/ZTT");
  TH1F* ZL=  (TH1F*)rootfile.Get(channel+"_SM"+sm+"/ZL");
  TH1F* ZJ=  (TH1F*)rootfile.Get(channel+"_SM"+sm+"/ZJ");
  TH1F* W=   (TH1F*)rootfile.Get(channel+"_SM"+sm+"/W");
  TH1F* TT=  (TH1F*)rootfile.Get(channel+"_SM"+sm+"/TT");
  TH1F* VV=  (TH1F*)rootfile.Get(channel+"_SM"+sm+"/VV");
  TH1F* QCD= (TH1F*)rootfile.Get(channel+"_SM"+sm+"/QCD");
  TH1F* data_obs= (TH1F*)rootfile.Get(channel+"_SM"+sm+"/data_obs");

  //read in additional parameters 
  Float_t WJetsScaleFactor=0.34;//FIX !!!!!!!!!!!!!!!!!!!


  ///////////////////////create data cards
  TString cardfilename=basename+".txt";
  ofstream file;
  file.open((channel+"/"+cardfilename).Data());
  if(!file.is_open()){
    cout<<"could not open file "<<cardfilename<<" check if directory "<<channel<<"  exists"<<endl;
    return;
  }

  file << "imax 1" <<endl;
  file << "jmax *" <<endl;
  file << "kmax *" <<endl;
  file << "shapes *  *  "<< rootfilename.Data() << "  $CHANNEL/$PROCESS $CHANNEL/$PROCESS_$SYSTEMATIC" << endl;
  file << endl;
  file << "observation " <<data_obs->Integral()<<endl;
  file<<endl;


  file <<fillStringLong("bin")<<fillString(" ")
       <<fillString(" ")
       <<fillString((channel+"_SM"+sm).Data())
       <<fillString((channel+"_SM"+sm).Data())
       <<fillString((channel+"_SM"+sm).Data())
       <<fillString((channel+"_SM"+sm).Data())
       <<fillString((channel+"_SM"+sm).Data())
       <<fillString((channel+"_SM"+sm).Data())
       <<fillString((channel+"_SM"+sm).Data())
       <<fillString((channel+"_SM"+sm).Data())
       <<fillString((channel+"_SM"+sm).Data())
       <<fillString((channel+"_SM"+sm).Data())<<endl;

  file <<fillStringLong("process")<<fillString(" ")
       <<fillString(" ")
       <<fillString((const char*)(TString("VH")+mass))
       <<fillString((const char*)(TString("SM")+mass))
       <<fillString((const char*)(TString("VBF")+mass))
       <<fillString("ZTT")
       <<fillString("QCD")
       <<fillString("W")
       <<fillString("ZJ")
       <<fillString("ZL")
       <<fillString("TT")
       <<fillString("VV")<<endl;

  file <<fillStringLong("process")<<fillString(" ")
       <<fillString(" ")
       <<fillInt(-2)
       <<fillInt(-1)
       <<fillInt(0)
       <<fillInt(1)
       <<fillInt(2)
       <<fillInt(3)
       <<fillInt(4)
       <<fillInt(5)
       <<fillInt(6)
       <<fillInt(7)<<endl;

  file <<fillStringLong("rate")<<fillString(" ")
       <<fillString(" ")
       <<fillFloat(VH->Integral())
       <<fillFloat(SM->Integral())
       <<fillFloat(VBF->Integral())
       <<fillFloat(ZTT->Integral())
       <<fillFloat(QCD->Integral())
       <<fillFloat(W->Integral())
       <<fillFloat(ZJ->Integral())
       <<fillFloat(ZL->Integral())
       <<fillFloat(TT->Integral())
       <<fillFloat(VV->Integral())<<endl;

  file << endl;
  file <<"-------------------------------"<<endl;


  ///lumi systematic------------------------------------------------
  file <<fillStringLong("lumi")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(1.045)
       <<fillFloat(1.045)
       <<fillFloat(1.045)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"luminosity"<<endl;

//   ///tau energy scale systematic------------------------------------------------
//   //may need to turn off the shape systematics on the small backgrounds due to instabilities
//   file <<fillStringLong("CMS_scale_t")<<fillString("shape")
//        <<fillString(" ")
//        <<fillInt(1)
//        <<fillInt(1)
//        <<fillInt(1)
//        <<fillInt(1)
//        <<fillString("-")
//        <<fillInt(1)
//        <<fillString("-")
//        <<fillString("-")
//        <<fillString("-")
//        <<fillString("-")
//        <<"shape"<<endl;

  /// mu efficiency systematic------------------------------------------------
  file <<fillStringLong("CMS_eff_m")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(1.020)
       <<fillFloat(1.020)
       <<fillFloat(1.020)
       <<fillFloat(1.020)
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.020)
       <<fillFloat(1.020)
       <<fillFloat(1.020)
       <<fillFloat(1.020)
       <<"Muon ID/HLT"<<endl;

  ///tau efficiency systematic------------------------------------------------
  file <<fillStringLong("CMS_eff_t")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(1.06)
       <<fillFloat(1.06)
       <<fillFloat(1.06)
       <<fillFloat(1.06)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.06)
       <<fillFloat(1.06)
       <<"Tau ID"<<endl;

  /// ? systematic------------------------------------------------
  file <<fillStringLong("CMS_scale_j")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(0.96)
       <<fillFloat(0.99)
       <<fillFloat(0.92)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(0.97)
       <<fillFloat(0.94)
       <<"Jet scale"<<endl;

  ///W+jets systematic------------------------------------------------
  if(sm==0){
    ///This assumes we calculate W+jets from the high m_T region
    std::string wsys=(TString("CMS_htt_muTau_SM")+sm+"_WNorm").Data();
    file <<fillStringLong(wsys)<<fillString("gmN")
	 <<fillInt(W->Integral()/WJetsScaleFactor)
	 <<fillString("-")
	 <<fillString("-")
	 <<fillString("-")
	 <<fillString("-")
	 <<fillString("-")
	 <<fillFloat(WJetsScaleFactor)
	 <<fillString("-")
	 <<fillString("-")
	 <<fillString("-")
	 <<fillString("-")
	 <<"W Background"<<endl;
  }
  if(sm==1 || sm==2){
    std::string wsys=(TString("CMS_htt_muTau_SM")+sm+"_WNorm").Data();
    file <<fillStringLong(wsys)<<fillString("lnN")
	 <<fillString(" ")
	 <<fillString("-")
	 <<fillString("-")
	 <<fillString("-")
	 <<fillString("-")
	 <<fillString("-")
	 <<fillFloat(1.3)
	 <<fillString("-")
	 <<fillString("-")
	 <<fillString("-")
	 <<fillString("-")
	 <<"W Background"<<endl;
  }
  
  ///QCD systematic------------------------------------------------
  std::string qcdsys=(TString("CMS_htt_muTau_SM")+sm+"_QCDNorm").Data();
  file <<fillStringLong(qcdsys)<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.3)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"QCD Background"<<endl;
  
  ///Z crossection systematic------------------------------------------------
  file <<fillStringLong("CMS_htt_zttNorm")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.033)
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.033)
       <<fillFloat(1.033)
       <<fillString("-")
       <<fillString("-")
       <<"Norm Ztt"<<endl;

  ///t-tbar crossection systematic------------------------------------------------
  file <<fillStringLong("CMS_htt_ttbarNorm")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.075)
       <<fillString("-")
       <<"Norm ttbar"<<endl;

  ///VV bosons systematic------------------------------------------------
  file <<fillStringLong("CMS_htt_DiBosonNorm")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.30)
       <<"Norm DiBoson"<<endl;

  ///jet fake rate systematic------------------------------------------------
  file <<fillStringLong("CMS_htt_ZJetFakeTau")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.10)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"Z(jet->tau) background"<<endl;

  ///muon fake rate systematic------------------------------------------------
  file <<fillStringLong("CMS_htt_ZLeptonFakeTau")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.10)
       <<fillString("-")
       <<fillString("-")
       <<"Z(lepton->tau) background"<<endl;

  ///MET systematic------------------------------------------------
  file <<fillStringLong("CMS_scale_met")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(1.05)
       <<fillFloat(1.05)
       <<fillFloat(1.05)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.05)
       <<fillFloat(1.05)
       <<fillFloat(1.02)
       <<fillFloat(1.06)
       <<"Jet scale"<<endl;

  ///? systematic------------------------------------------------
  file <<fillStringLong("pdf_vh")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(1.08)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"PDF VBF"<<endl;

  file <<fillStringLong("pdf_gg")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillFloat(1.03)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"PDF VBF"<<endl;

  file <<fillStringLong("pdf_qqbar")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.03)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"PDF VBF"<<endl;


  file <<fillStringLong("QCDscale_ggH")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillFloat(1.12)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"QCD scale GGF"<<endl;

  file <<fillStringLong("QCDscale_qqH")<<fillString("lnN")
       <<fillString(" ")
       <<fillString("-")
       <<fillString("-")
       <<fillFloat(1.035)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"QCD scale VBF"<<endl;


  file <<fillStringLong("UEPS")<<fillString("lnN")
       <<fillString(" ")
       <<fillFloat(0.96)
       <<fillFloat(0.96)
       <<fillFloat(0.96)
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<fillString("-")
       <<"Underlying events & PS"<<endl;

  file.close();

  //gROOT->ProcessLine(".q");
}


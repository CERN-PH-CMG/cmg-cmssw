string Resonance;

void Write(string HISTO){

  bool grid = true;
  bool logx = false;
  bool logy = false;
  Styles::FormatPad( gPad, grid, logy , logx);
  Styles::SetMax( gPad, 1.3, 1);
  gPad->Modified();

  string outname = Resonance;
  outname = outname + "_" + HISTO;
  Styles::SavePlot(outname.c_str(), "Plots_RESONANCES");

  logy = true;

  Styles::FormatPad( gPad, grid, logy, logx );
  Styles::SetMax( gPad, 1, 1);
  gPad->Modified();
  outname = outname+"_log";
  Styles::SavePlot(outname.c_str(), "Plots_RESONANCES");
  

}




void Resonances_new(){

  gROOT->Macro("Macros/initTree.C");
  // for MetDyn studies 

  vector<string> vZoom;
  vZoom.push_back(string(""));
  vZoom.push_back(string("_zoom"));

  //  Chain RD("dijetNtuple", "Samples/JetMETTau/Run2010A-Jul26thReReco-v1/RECO/CorrectJetPtSelection/HBHENoiseFilter/DPhiJetMET/PFAnalysis*root", "RD");

  float intLumi = 0.066;

  // Resonance = "RSGraviton";
  Resonance = "Zprime";

  string sFile =  "rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/JetAndMetAnalyses/";
  sFile = sFile + Resonance + "_DiJet1200/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/MassShapesDijet_Test1/genParticle_*.root";


  Chain RSGraviton("Events", 
		   sFile.c_str(), "RSGraviton");







  Samples metdyn; 
  metdyn.SetLegendCorners(0.7,0.8,0.93,0.88);
  metdyn.AddTree(&RSGraviton, "Resonance"); 


  metdyn.SetAlias("Res", "recoGenParticles_zprime__PRUNNER.obj");
  metdyn.SetAlias("Res_E", "sqrt(Res.mass()*Res.mass() + Res.pz()*Res.pz() + Res.pt()*Res.pt())");
  metdyn.SetAlias("Empz", "Res_E - Res.pz()");
  metdyn.SetAlias("Ecol", "3500.*2");
  metdyn.SetAlias("x1", "Empz/Ecol");
  metdyn.SetAlias("x2", "Res.mass()*Res.mass()/(Empz*Ecol)");



  Styles styles;
  metdyn.SetStyle( "Resonance", false, styles.spred );


  /*  
  metdyn.SetTitle(Title(string("PDG Id")));
  metdyn.Draw("Res.pdgId() >> PDGID(100, -50.5, 49.5)", "", 1, false); 
  Write(string("PDGID"));

  metdyn.SetTitle(Title(string("N daughters")));

  metdyn.Draw("Res.numberOfDaughters() >> DAUGHTERS(5, -0.5, 4.5)", "" , 1, false); 
  Write(string("DAUGHTERS"));


  metdyn.SetTitle(Title(string("N mothers")));
  metdyn.Draw("Res.numberOfMothers() >> MOTHERS(5, -0.5, 4.5)",  "", 1, false); 
  Write(string("MOTHERS"));


  metdyn.SetTitle(Title(string("p_{T} (GeV)")));
  metdyn.Draw("Res.pt() >> PT(100, 0, 1000)", "", 1, false); 
  Write(string("PT"));
  
  metdyn.SetTitle(Title(string("p_{z} (GeV)")));
  metdyn.Draw("Res.pz() >> PZ(100, 0, 1000)", "", 1, false); 
  Write(string("PZ"));

  metdyn.SetTitle(Title(string("M (GeV)")));
  metdyn.Draw("Res.mass() >> MASS(200, 0, 2000)", "", 1, false); 
  Write(string("MASS"));

  metdyn.SetTitle(Title(string("#eta")));
  metdyn.Draw("Res.eta() >> ETA(60, -6, 6)", "", 1, false); 
  Write(string("ETA"));

  metdyn.SetTitle(Title(string("#phi")));
  metdyn.Draw("Res.phi() >> PHI(40, -4, 4)", "", 1, false); 
  Write(string("PHI"));

  metdyn.SetTitle(Title(string("x1")));
  metdyn.Draw("x1>>X1(100, 0, 1)", "", 1, false); 
  Write(string("X1"));
  
  metdyn.SetTitle(Title(string("x1")));
  metdyn.Draw("x2>>X2(100, 0, 1)", "", 1, false); 
  Write(string("X2"));

  metdyn.SetTitle(Title(string("x2-x1")));
  metdyn.Draw("x2-x1>>deltaX(100, -1, 1)", "", 1, false); 
  Write(string("DELTAX"));
  */ 






  metdyn.SetAlias("Parton", "recoGenParticles_partons__PRUNNER.obj");
  metdyn.SetAlias("theta", "TMath::ATan(TMath::Exp(-1*Parton.eta()))*2");
  metdyn.SetAlias("Pz", "Parton[0].pz() + Parton[1].pz()");
  metdyn.SetAlias("Px", "Parton[0].px() + Parton[1].px()");
  metdyn.SetAlias("Py", "Parton[0].py() + Parton[1].py()");

  metdyn.SetAlias("E1", "TMath::Sqrt(Parton[0].mass()*Parton[0].mass() + Parton[0].pz()*Parton[0].pz() + Parton[0].px()*Parton[0].px() + Parton[0].py()*Parton[0].py())");
  metdyn.SetAlias("E2", "TMath::Sqrt(Parton[1].mass()*Parton[1].mass() + Parton[1].pz()*Parton[1].pz() + Parton[1].px()*Parton[1].px() + Parton[1].py()*Parton[1].py())");
  metdyn.SetAlias("E", "E1 + E2");
  metdyn.SetAlias("PT", "TMath::Sqrt(Px*Px + Py*Py)");
  

  metdyn.SetAlias("Empz_Reco", "E-Pz");
  metdyn.SetAlias("M_Reco", "TMath::Sqrt(E*E - (PT*PT + Pz*Pz))");
  metdyn.SetAlias("x1_Reco", "Empz_Reco/Ecol");
  metdyn.SetAlias("x2_Reco", "M_Reco*M_Reco/(Empz_Reco*Ecol)");
  
  metdyn.SetAlias("beta", "TMath::Abs(x1_Reco-x2_Reco)/(x1_Reco + x2_Reco)*Pz/TMath::Abs(Pz)");
  metdyn.SetAlias("gamma", "TMath::Sqrt(1./(1-beta*beta))");

  metdyn.SetAlias("theta1", "TMath::ATan(TMath::Exp(-1*Parton[0].eta()))*2");
  metdyn.SetAlias("cosTheta1_Star", "(TMath::Cos(theta1) - beta)/(1 - beta*TMath::Cos(theta1))");

  metdyn.SetAlias("theta2", "TMath::ATan(TMath::Exp(-1*Parton[1].eta()))*2");
  metdyn.SetAlias("cosTheta2_Star", "(TMath::Cos(theta2) - beta)/(1 - beta*TMath::Cos(theta2))");




  /*
  metdyn.SetTitle(Title(string("#eta partons")));
  metdyn.Draw("Parton.eta() >> ETA_PARTONS(60, -6, 6)", "", 1, false); 
  Write(string("ETA_PARTONS"));

  metdyn.SetTitle(Title(string("#pt partons")));
  metdyn.Draw("Parton.pt() >> PT_PARTONS(100, 0, 1000)", "", 1, false); 
  Write(string("PT_PARTONS"));

  metdyn.SetTitle(Title(string("#phi partons")));
  metdyn.Draw("Parton.phi() >> PHI_PARTONS(40, -4, 4)", "", 1, false); 
  Write(string("PHI_PARTONS"));


  metdyn.SetTitle(Title(string("Delta#phi")));
  metdyn.Draw("Parton[0].phi() - Parton[1].phi() - TMath::Pi() >> PHI_PARTONS(40, -6, 6)", "", 1, false); 
  Write(string("DELTA_PHI_PARTONS"));

  metdyn.SetTitle(Title(string("Delta#eta")));
  metdyn.Draw("Parton[0].eta() - Parton[1].eta() >> ETA_PARTONS(80, -8, 8)", "", 1, false); 
  Write(string("DELTA_ETA_PARTONS"));
  
 
  metdyn.SetTitle(Title(string("cos #theta")));
  metdyn.Draw("TMath::Cos(theta) >> COS_THETA_LAB(50, -1, 1)", "", 1, false); 
  Write(string("COS_THETA_LAB"));
 
  
  metdyn.SetTitle(Title(string("M_reco (GeV)")));
  metdyn.Draw("M_Reco >> DELTAX_RECO(200, 0, 2000)", "", 1, false); 
  Write(string("M_RECO"));

  
  metdyn.SetTitle(Title(string("P_{T, Reco} (GeV)")));
  metdyn.Draw("PT >> PT_RECO(100, 0, 1000)", "", 1, false); 
  Write(string("PT_RECO"));

  metdyn.SetTitle(Title(string("P_{x, Reco} (GeV)")));
  metdyn.Draw("Pz >> Pz_RECO(100, 0, 1000)", "", 1, false); 
  Write(string("PZ_RECO"));


  
  metdyn.SetTitle(Title(string("x1_Reco")));
  metdyn.Draw("x1_Reco >> X1_RECO(100, 0, 1)", "", 1, false); 
  Write(string("X1_RECO"));

  metdyn.SetTitle(Title(string("x2_Reco")));
  metdyn.Draw("x2_Reco >> X2_RECO(100, 0, 1)", "", 1, false); 
  Write(string("X2_RECO"));

  metdyn.SetTitle(Title(string("x2_Reco-x1_Reco")));
  metdyn.Draw("x2_Reco-x1_Reco >> DELTAX_RECO(100, -1, 1)", "", 1, false); 
  Write(string("DELTAX_RECO"));

  metdyn.SetTitle(Title(string("x2_Reco+x1_Reco")));
  metdyn.Draw("x2_Reco+x1_Reco >> DELTAX_RECO(100, 0, 2)", "", 1, false); 
  Write(string("SUMX_RECO"));

  metdyn.SetTitle(Title(string("#beta")));
  metdyn.Draw("beta >> BETA(50, -1, 1)", "", 1, false); 
  Write(string("BETA_RECO"));

  metdyn.SetTitle(Title(string("#gamma")));
  metdyn.Draw("gamma >> GAMMA(100, 1, 5)", "", 1, false); 
  Write(string("GAMMA_RECO"));
  

  
  metdyn.SetTitle(Title(string("E* (GeV)")));
  metdyn.Draw("gamma*(E-beta*Pz) >> E_STAR_RECO(200, 0, 2000)", "", 1, false); 
  Write(string("E_STAR_RECO"));

  metdyn.SetTitle(Title(string("P_{z}* (GeV)")));
  metdyn.Draw("gamma*(Pz-beta*E) >> PZ_STAR_RECO(100, 0, 1000)", "", 1, false); 
  Write(string("PZ_STAR_RECO"));
  

  metdyn.SetTitle(Title(string("cos #theta_{1} *")));
  metdyn.Draw("cosTheta1_Star >> COSTHETA1_STAR(50, -1, 1)", "", 1, false); 
  Write(string("COSTHETA1_STAR_RECO"));


  metdyn.SetTitle(Title(string("cos^{2} #theta_{1} *")));
  metdyn.Draw("cosTheta1_Star*cosTheta1_Star >> COS2THETA1_STAR(25, 0, 1)", "", 1, false); 
  Write(string("COS2THETA1_STAR_RECO"));

  metdyn.SetTitle(Title(string("cos #theta_{2} *")));
  metdyn.Draw("cosTheta2_Star >> COSTHETA2_STAR(50, -1, 1)", "", 1, false); 
  Write(string("COSTHETA2_STAR_RECO"));
  
  
  metdyn.SetTitle(Title(string("cos #theta_{1}* + cos #theta_{2}*")));
  metdyn.Draw("cosTheta2_Star - cosTheta1_Star >> SUMCOSTHETA_STAR_RECO(40, -2, 2)", "", 1, false); 
  Write(string("SUMCOSTHETA_STAR_RECO"));

  metdyn.SetTitle(Title(string("cos #theta_{1}* - cos #theta_{2}*")));
  metdyn.Draw("cosTheta2_Star + TMath::Cos(theta1) >> COSTHETA_STAR_MINUS_COSTHETA_RECO(40, -2, 2)", "", 1, false); 
  Write(string("COSTHETA_STAR_MINUS_COSTHETA_RECO"));

  
  string title = ""; title = Resonance + " ; cos #theta_{1}* ; cos #theta_{2}*";
  metdyn.SetTitle(title.c_str());

  metdyn.Draw("cosTheta1_Star : cosTheta2_Star >> COSTHETA_STAR_2D(100, -1, 1, 100, -1, 1)", "", 1, false); 
  gStyle->SetPalette(1);
  char* plot = "cosTheta1_Star : cosTheta2_Star >> COSTHETA_STAR_2D(100, -1, 1, 100, -1, 1)_";
  ((TH2F*) metdyn.GetPlot(plot).GetHistogram("Resonance"))->Draw("COLZ");
  Write(string("COSTHETA_STAR_2D"));


  string title = ""; title = Resonance + " ; cos #theta_{1} ; cos #theta_{2}";
  metdyn.SetTitle(title.c_str());

  metdyn.Draw("TMath::Cos(theta1) : TMath::Cos(theta2) >> COSTHETA_2D(100, -1, 1, 100, -1, 1)", "", 1, false); 
  gStyle->SetPalette(1);
  char* plot = "TMath::Cos(theta1) : TMath::Cos(theta2) >> COSTHETA_2D(100, -1, 1, 100, -1, 1)_";
  ((TH2F*) metdyn.GetPlot(plot).GetHistogram("Resonance"))->Draw("COLZ");
  Write(string("COSTHETA_2D"));


  string title = ""; title = Resonance + " ; #theta_{1} ; #theta_{2}";
  metdyn.SetTitle(title.c_str());

  metdyn.Draw("theta1 : theta2 >> COSTHETA_2D(40, 0, 4, 40, 0, 4)", "", 1, false); 
  gStyle->SetPalette(1);
  char* plot = "theta1 : theta2 >> COSTHETA_2D(40, 0, 4, 40, 0, 4)_";
  ((TH2F*) metdyn.GetPlot(plot).GetHistogram("Resonance"))->Draw("COLZ");
  Write(string("THETA_2D"));


  string title = ""; title = Resonance + " ; #eta_{1} ; #eta_{2}";
  metdyn.SetTitle(title.c_str());

  metdyn.Draw("Parton[0].eta() : Parton[1].eta() >> ETA_2D(60, -6, 6, 60, -6, 6)", "", 1, false); 
  gStyle->SetPalette(1);
  char* plot = "Parton[0].eta() : Parton[1].eta() >> ETA_2D(60, -6, 6, 60, -6, 6)_";
  ((TH2F*) metdyn.GetPlot(plot).GetHistogram("Resonance"))->Draw("COLZ");
  Write(string("ETA_2D"));
*/



  
  string title = ""; title = Resonance + " ; x1 ; x2";
  metdyn.SetTitle(title.c_str());

  metdyn.Draw("x1 : x2 >> X1X2(100, 0, 1, 100, 0, 1)", "", 1, false); 
  gStyle->SetPalette(1);
  char* plot = "x1 : x2 >> X1X2(100, 0, 1, 100, 0, 1)_";
  ((TH2F*) metdyn.GetPlot(plot).GetHistogram("Resonance"))->Draw("COLZ");
  Write(string("X1X2"));
  

  string title = ""; title = Resonance + "; x1 ; Mass (GeV)";
  metdyn.SetTitle(title.c_str());
  metdyn.Draw("Res.mass() : x1 >> X1Mass(100, 0, 1, 200, 0, 2000)", "", 1, false); 
  gStyle->SetPalette(1);
  char* plot = "Res.mass() : x1 >> X1Mass(100, 0, 1, 200, 0, 2000)_";
  ((TH2F*) metdyn.GetPlot(plot).GetHistogram("Resonance"))->Draw("COLZ");
  Write(string("X1Mass"));
  
}

char* Title(string name){

  string title = ""; title = Resonance + " ; " + name + " ; Npart";
  return title.c_str();

}


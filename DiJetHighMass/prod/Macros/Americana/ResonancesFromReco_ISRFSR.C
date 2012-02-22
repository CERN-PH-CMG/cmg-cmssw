  gROOT->Macro("Macros/initTree.C");

  string Resonance;
double norm;

void Write(string HISTO){

  bool grid = true;
  bool logx = false;
  bool logy = false;
  Styles::FormatPad( gPad, grid, logy , logx);
  Styles::SetMax( gPad, 1.3, 0.1);
  gPad->Modified();

  string outname = Resonance;
  outname = outname + "_" + HISTO;
  Styles::SavePlot(outname.c_str(), "Plots_RESONANCES_ISRFSR_PTDENSITIES_RS2000");

  logy = true;

  Styles::FormatPad( gPad, grid, logy, logx );
  Styles::SetMax( gPad, 1, 0.1);
  gPad->Modified();
  outname = outname+"_log";
  Styles::SavePlot(outname.c_str(), "Plots_RESONANCES_ISRFSR_PTDENSITIES_RS2000");
  

}




void Resonances_ISRFSR(){

  gROOT->Macro("Macros/initTree.C");

  // for MetDyn studies 

  vector<string> vZoom;
  vZoom.push_back(string(""));
  vZoom.push_back(string("_zoom"));

  //  Chain RD("dijetNtuple", "Samples/JetMETTau/Run2010A-Jul26thReReco-v1/RECO/CorrectJetPtSelection/HBHENoiseFilter/DPhiJetMET/PFAnalysis*root", "RD");

  float intLumi = 0.066;

  Resonance = "RSGraviton";
  //  Resonance = "Zprime";

  //  string sFile =  "rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/JetAndMetAnalyses/";
  //  sFile = sFile + Resonance + "_DiJet1200/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO/MassShapesDijet_Test1/genParticle_*.root";


  Chain RSGraviton("Events", 
		   //		   	   "/tmp/mgouzevi/RSGraviton_1200.root", "RSGraviton");
		   "rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/JetAndMetAnalyses/RSGraviton1200_Simulated/RSGraviton_2000.root", "RSGraviton");
		   //sFile.c_str(), "RSGraviton");
  




  metdyn->SetLegendCorners(0.7,0.8,0.93,0.88);
  metdyn->AddTree(&RSGraviton, "Resonance"); 


  metdyn->SetAlias("Res", "recoGenParticles_resonanceGenParticles__GEN.obj");
  metdyn->SetAlias("Res_E", "sqrt(Res.mass()*Res.mass() + Res.pz()*Res.pz() + Res.pt()*Res.pt())");
  metdyn->SetAlias("Empz", "Res_E - Res.pz()");
  metdyn->SetAlias("Ecol", "3500.*2");
  metdyn->SetAlias("Pi", "3.14159");
  metdyn->SetAlias("x1", "Empz/Ecol");
  metdyn->SetAlias("x2", "Res.mass()*Res.mass()/(Empz*Ecol)");


  metdyn->SetAlias("Parton", "recoGenParticles_signalGenParticles__GEN.obj");
  metdyn->SetAlias("theta", "TMath::ATan(TMath::Exp(-1*Parton.eta()))*2");
  metdyn->SetAlias("Pz", "Parton[0].pz() + Parton[1].pz()");
  metdyn->SetAlias("Px", "Parton[0].px() + Parton[1].px()");
  metdyn->SetAlias("Py", "Parton[0].py() + Parton[1].py()");

  metdyn->SetAlias("E1", "TMath::Sqrt(Parton[0].mass()*Parton[0].mass() + Parton[0].pz()*Parton[0].pz() + Parton[0].px()*Parton[0].px() + Parton[0].py()*Parton[0].py())");
  metdyn->SetAlias("E2", "TMath::Sqrt(Parton[1].mass()*Parton[1].mass() + Parton[1].pz()*Parton[1].pz() + Parton[1].px()*Parton[1].px() + Parton[1].py()*Parton[1].py())");
  metdyn->SetAlias("E", "E1 + E2");
  metdyn->SetAlias("PT", "TMath::Sqrt(Px*Px + Py*Py)");
  

  metdyn->SetAlias("Empz_Reco", "E-Pz");
  metdyn->SetAlias("M_Reco", "TMath::Sqrt(E*E - (PT*PT + Pz*Pz))");
  metdyn->SetAlias("x1_Reco", "Empz_Reco/Ecol");
  metdyn->SetAlias("x2_Reco", "M_Reco*M_Reco/(Empz_Reco*Ecol)");
  
  metdyn->SetAlias("beta", "TMath::Abs(x1_Reco-x2_Reco)/(x1_Reco + x2_Reco)*Pz/TMath::Abs(Pz)");
  metdyn->SetAlias("gamma", "TMath::Sqrt(1./(1-beta*beta))");

  metdyn->SetAlias("theta1", "TMath::ATan(TMath::Exp(-1*Parton[0].eta()))*2");
  metdyn->SetAlias("cosTheta1_Star", "(TMath::Cos(theta1) - beta)/(1 - beta*TMath::Cos(theta1))");

  metdyn->SetAlias("theta2", "TMath::ATan(TMath::Exp(-1*Parton[1].eta()))*2");
  metdyn->SetAlias("cosTheta2_Star", "(TMath::Cos(theta2) - beta)/(1 - beta*TMath::Cos(theta2))");





  gStyle->SetPalette(1);
  Styles styles;
  metdyn->SetStyle( "Resonance", false, styles.spred, "COLZ");


  metdyn->SetTitle(Title(string("#sqrt{x2*x1}")));
  metdyn->Draw("TMath::Sqrt(x2*x1)>>prodX(50, 0, 0.5)", "", 1, false); 
  Write(string("PRODX"));
  

  /*
      
  metdyn->SetTitle(Title(string("PDG Id")));
  metdyn->Draw("Res.pdgId() >> PDGID(100, -50.5, 49.5)", "", 1, false); 
  Write(string("PDGID"));

  metdyn->SetTitle(Title(string("N daughters")));

  metdyn->Draw("Res.numberOfDaughters() >> DAUGHTERS(5, -0.5, 4.5)", "" , 1, false); 
  Write(string("DAUGHTERS"));


  metdyn->SetTitle(Title(string("N mothers")));
  metdyn->Draw("Res.numberOfMothers() >> MOTHERS(5, -0.5, 4.5)",  "", 1, false); 
  Write(string("MOTHERS"));


  metdyn->SetTitle(Title(string("p_{T} (GeV)")));
  metdyn->Draw("Res.pt() >> PT(100, 0, 1000)", "", 1, false); 
  Write(string("PT"));
  
  metdyn->SetTitle(Title(string("p_{z} (GeV)")));
  metdyn->Draw("Res.pz() >> PZ(100, 0, 1000)", "", 1, false); 
  Write(string("PZ"));

  metdyn->SetTitle(Title(string("M (GeV)")));
  metdyn->Draw("Res.mass() >> MASS(200, 0, 2000)", "", 1, false); 
  Write(string("MASS"));

  metdyn->SetTitle(Title(string("#eta")));
  metdyn->Draw("Res.eta() >> ETA(60, -6, 6)", "", 1, false); 
  Write(string("ETA"));

  metdyn->SetTitle(Title(string("#phi")));
  metdyn->Draw("Res.phi() >> PHI(40, -4, 4)", "", 1, false); 
  Write(string("PHI"));

  metdyn->SetTitle(Title(string("x1")));
  metdyn->Draw("x1>>X1(100, 0, 1)", "", 1, false); 
  Write(string("X1"));
  
  metdyn->SetTitle(Title(string("x1")));
  metdyn->Draw("x2>>X2(100, 0, 1)", "", 1, false); 
  Write(string("X2"));

  metdyn->SetTitle(Title(string("x2-x1")));
  metdyn->Draw("x2-x1>>deltaX(100, -1, 1)", "", 1, false); 
  Write(string("DELTAX"));
  








  metdyn->SetTitle(Title(string("#theta partons")));
  metdyn->Draw("theta*180/Pi >> THETA_PARTONS(36, 0, 180)", "", 1, false); 
  Write(string("THETA_PARTONS"));



  
  
  metdyn->SetTitle(Title(string("#eta partons")));
  metdyn->Draw("Parton.eta() >> ETA_PARTONS(60, -6, 6)", "", 1, false); 
  Write(string("ETA_PARTONS"));

  metdyn->SetTitle(Title(string("#pt partons")));
  metdyn->Draw("Parton.pt() >> PT_PARTONS(100, 0, 1000)", "", 1, false); 
  Write(string("PT_PARTONS"));

  metdyn->SetTitle(Title(string("#phi partons")));
  metdyn->Draw("Parton.phi() >> PHI_PARTONS(40, -4, 4)", "", 1, false); 
  Write(string("PHI_PARTONS"));
  

  metdyn->SetTitle(Title(string("Delta#phi")));
  metdyn->Draw("TMath::Abs(Parton[0].phi() - Parton[1].phi()) - Pi >> PHI_PARTONS(40, -6, 6)", "", 1, false); 
  Write(string("DELTA_PHI_PARTONS"));
  
  metdyn->SetTitle(Title(string("Delta#eta")));
  metdyn->Draw("Parton[0].eta() - Parton[1].eta() >> ETA_PARTONS(80, -8, 8)", "", 1, false); 
  Write(string("DELTA_ETA_PARTONS"));
  
 
  metdyn->SetTitle(Title(string("cos #theta")));
  metdyn->Draw("TMath::Cos(theta) >> COS_THETA_LAB(50, -1, 1)", "", 1, false); 
  Write(string("COS_THETA_LAB"));
 
  
  metdyn->SetTitle(Title(string("M_reco (GeV)")));
  metdyn->Draw("M_Reco >> DELTAX_RECO(200, 0, 2000)", "", 1, false); 
  Write(string("M_RECO"));

  
  metdyn->SetTitle(Title(string("P_{T, Reco} (GeV)")));
  metdyn->Draw("PT >> PT_RECO(100, 0, 1000)", "", 1, false); 
  Write(string("PT_RECO"));

  metdyn->SetTitle(Title(string("P_{x, Reco} (GeV)")));
  metdyn->Draw("Pz >> Pz_RECO(100, 0, 1000)", "", 1, false); 
  Write(string("PZ_RECO"));


  
  metdyn->SetTitle(Title(string("x1_Reco")));
  metdyn->Draw("x1_Reco >> X1_RECO(100, 0, 1)", "", 1, false); 
  Write(string("X1_RECO"));

  metdyn->SetTitle(Title(string("x2_Reco")));
  metdyn->Draw("x2_Reco >> X2_RECO(100, 0, 1)", "", 1, false); 
  Write(string("X2_RECO"));

  metdyn->SetTitle(Title(string("x2_Reco-x1_Reco")));
  metdyn->Draw("x2_Reco-x1_Reco >> DELTAX_RECO(100, -1, 1)", "", 1, false); 
  Write(string("DELTAX_RECO"));

  metdyn->SetTitle(Title(string("x2_Reco+x1_Reco")));
  metdyn->Draw("x2_Reco+x1_Reco >> DELTAX_RECO(100, 0, 2)", "", 1, false); 
  Write(string("SUMX_RECO"));

  metdyn->SetTitle(Title(string("#beta")));
  metdyn->Draw("beta >> BETA(50, -1, 1)", "", 1, false); 
  Write(string("BETA_RECO"));

  metdyn->SetTitle(Title(string("#gamma")));
  metdyn->Draw("gamma >> GAMMA(100, 1, 5)", "", 1, false); 
  Write(string("GAMMA_RECO"));
  

  
  metdyn->SetTitle(Title(string("E* (GeV)")));
  metdyn->Draw("gamma*(E-beta*Pz) >> E_STAR_RECO(200, 0, 2000)", "", 1, false); 
  Write(string("E_STAR_RECO"));

  metdyn->SetTitle(Title(string("P_{z}* (GeV)")));
  metdyn->Draw("gamma*(Pz-beta*E) >> PZ_STAR_RECO(100, 0, 1000)", "", 1, false); 
  Write(string("PZ_STAR_RECO"));
  

  metdyn->SetTitle(Title(string("cos #theta_{1} *")));
  metdyn->Draw("cosTheta1_Star >> COSTHETA1_STAR(50, -1, 1)", "", 1, false); 
  Write(string("COSTHETA1_STAR_RECO"));


  metdyn->SetTitle(Title(string("cos^{2} #theta_{1} *")));
  metdyn->Draw("cosTheta1_Star*cosTheta1_Star >> COS2THETA1_STAR(25, 0, 1)", "", 1, false); 
  Write(string("COS2THETA1_STAR_RECO"));

  metdyn->SetTitle(Title(string("cos #theta_{2} *")));
  metdyn->Draw("cosTheta2_Star >> COSTHETA2_STAR(50, -1, 1)", "", 1, false); 
  Write(string("COSTHETA2_STAR_RECO"));
  
  
  metdyn->SetTitle(Title(string("cos #theta_{1}* + cos #theta_{2}*")));
  metdyn->Draw("cosTheta2_Star - cosTheta1_Star >> SUMCOSTHETA_STAR_RECO(40, -2, 2)", "", 1, false); 
  Write(string("SUMCOSTHETA_STAR_RECO"));

  metdyn->SetTitle(Title(string("cos #theta_{1}* - cos #theta_{2}*")));
  metdyn->Draw("cosTheta2_Star + TMath::Cos(theta1) >> COSTHETA_STAR_MINUS_COSTHETA_RECO(40, -2, 2)", "", 1, false); 
  Write(string("COSTHETA_STAR_MINUS_COSTHETA_RECO"));

  
  string title = ""; title = Resonance + " ; cos #theta_{1}* ; cos #theta_{2}*";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("cosTheta1_Star : cosTheta2_Star >> COSTHETA_STAR_2D(100, -1, 1, 100, -1, 1)", "", 1, false); 
  Write(string("COSTHETA_STAR_2D"));
  

  string title = ""; title = Resonance + " ; cos #theta_{1}* ; cos #theta_{2}*";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("cosTheta1_Star : cosTheta2_Star >> COSTHETA_STAR_SMALLRESPT_2D(100, -1, 1, 100, -1, 1)", "Res[0].pt() < 10", 1, false); 
  Write(string("COSTHETA_STAR_SMALLRESPT_2D"));SignalPS.pt() < SignalPS[1] && dR0_Rad < dR0_Rad
  

  metdyn->SetTitle(Title(string("cos #theta_{1} *")));
  metdyn->Draw("cosTheta1_Star >> COSTHETA1_STAR_SMALLRESPT(10, -1, 1)", "Res[0].pt() < 10", 1, false); 
  Write(string("COSTHETA1_STAR_RECO_SMALLRESPT"));


  metdyn->SetTitle(Title(string("cos^{2} #theta_{1} *")));
  metdyn->Draw("cosTheta1_Star*cosTheta1_Star >> COS2THETA1_STAR_SMALLRESPT(10, 0, 1)", "Res[0].pt() < 10", 1, false); 
  Write(string("COS2THETA1_STAR_RECO_SMALLRESPT"));


   
  string title = ""; title = Resonance + " ; cos #theta_{1} ; cos #theta_{2}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("TMath::Cos(theta1) : TMath::Cos(theta2) >> COSTHETA_2D(100, -1, 1, 100, -1, 1)", "", 1, false); 
  Write(string("COSTHETA_2D"));


  string title = ""; title = Resonance + " ; #theta_{1} ; #theta_{2}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("theta1 : theta2 >> COSTHETA_2D(40, 0, 4, 40, 0, 4)", "", 1, false); 
  Write(string("THETA_2D"));


  string title = ""; title = Resonance + " ; #eta_{1} ; #eta_{2}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("Parton[0].eta() : Parton[1].eta() >> ETA_2D(60, -6, 6, 60, -6, 6)", "", 1, false); 
  Write(string("ETA_2D"));
    



  
  string title = ""; title = Resonance + " ; x1 ; x2";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("x1 : x2 >> X1X2(100, 0, 1, 100, 0, 1)", "", 1, false); 
  Write(string("X1X2"));
  

  string title = ""; title = Resonance + "; x1 ; Mass (GeV)";
  metdyn->SetTitle(title.c_str());
  metdyn->Draw("Res.mass() : x1 >> X1Mass(100, 0, 1, 200, 0, 2000)", "", 1, false); 
  Write(string("X1Mass"));
  */    

  metdyn->SetAlias("Signal", "recoGenJets_ak7GenJetsSignal__GEN.obj");
  metdyn->SetAlias("SignalPS", "recoGenJets_ak7GenJetsSignalPS__GEN.obj");
  metdyn->SetAlias("dEta0", "SignalPS[0].eta()-SignalPS.eta()");
  metdyn->SetAlias("dEta1", "SignalPS[1].eta()-SignalPS.eta()");
  metdyn->SetAlias("dTheta0", "TMath::ATan(TMath::Exp(-1*SignalPS.eta()))*2-TMath::ATan(TMath::Exp(-1*SignalPS.eta()))*2");
  metdyn->SetAlias("dTheta1", "TMath::ATan(TMath::Exp(-1*SignalPS.eta()))*2-TMath::ATan(TMath::Exp(-1*SignalPS.eta()))*2");
  metdyn->SetAlias("dPhi0", "SignalPS[0].phi()-SignalPS.phi()");
  metdyn->SetAlias("dPhi1", "SignalPS[1].phi()-SignalPS.phi()");
  metdyn->SetAlias("dR02", "dEta0*dEta0+dPhi0*dPhi0");
  metdyn->SetAlias("dR12", "dEta1*dEta1+dPhi1*dPhi1");
  metdyn->SetAlias("dR0", "sqrt(dEta0*dEta0+dPhi0*dPhi0)");
  metdyn->SetAlias("dR1", "sqrt(dEta1*dEta1+dPhi1*dPhi1)");

  metdyn->SetAlias("theta_PS", "TMath::ATan(TMath::Exp(-1*SignalPS.eta()))*2*180/Pi");
  metdyn->SetAlias("E_PS", "sqrt(SignalPS.mass()*SignalPS.mass()+SignalPS.px()*SignalPS.px()+SignalPS.py()*SignalPS.py()+SignalPS.pz()*SignalPS.pz())");

  metdyn->SetAlias("dEta0_PartonJet", "Signal[0].eta()-SignalPS[0].eta()");
  metdyn->SetAlias("dEta1_PartonJet", "Signal[1].eta()-SignalPS[1].eta()");
  metdyn->SetAlias("dPhi0_PartonJet", "Signal[0].phi()-SignalPS[0].phi()");
  metdyn->SetAlias("dPhi1_PartonJet", "Signal[1].phi()-SignalPS[1].phi()");

  metdyn->SetAlias("vecX0", "SignalPS[0].py()*SignalPS.pz()-SignalPS.py()*SignalPS[0].pz()");
  metdyn->SetAlias("vecZ0", "SignalPS[0].pz()*SignalPS.px()-SignalPS.px()*SignalPS[0].pz()");
  metdyn->SetAlias("vecY0", "SignalPS[0].px()*SignalPS.py()-SignalPS.py()*SignalPS[0].px()");

  metdyn->SetAlias("vecX1", "SignalPS[1].py()*SignalPS.pz()-SignalPS.py()*SignalPS[1].pz()");
  metdyn->SetAlias("vecZ1", "SignalPS[1].pz()*SignalPS.px()-SignalPS.px()*SignalPS[1].pz()");
  metdyn->SetAlias("vecY1", "SignalPS[1].px()*SignalPS.py()-SignalPS.py()*SignalPS[1].px()");

  metdyn->SetAlias("vec0", "sqrt(vecX0*vecX0+vecY0*vecY0+vecZ0*vecZ0)");
  metdyn->SetAlias("vec1", "sqrt(vecX1*vecX1+vecY1*vecY1+vecZ1*vecZ1)");


  metdyn->SetAlias("pt_rel0", "vec0/(SignalPS[0].p()*SignalPS[0].pt())");
  metdyn->SetAlias("pt_rel1", "vec1/(SignalPS[1].p()*SignalPS[1].pt())");

  metdyn->SetAlias("Signal", "recoGenJets_ak7GenJetsSignal__GEN.obj");
  metdyn->SetAlias("Rad", "recoGenJets_ak7GenJetsRad__GEN.obj");
  metdyn->SetAlias("dEta0_Rad", "SignalPS[0].eta()-Rad.eta()");
  metdyn->SetAlias("dEta1_Rad", "SignalPS[1].eta()-Rad.eta()");
  metdyn->SetAlias("dPhi0_Rad", "SignalPS[0].phi()-Rad.phi()");
  metdyn->SetAlias("dPhi1_Rad", "SignalPS[1].phi()-Rad.phi()");
  metdyn->SetAlias("dTheta0_Rad", "TMath::ATan(TMath::Exp(-1*SignalPS[0].eta()))*2-TMath::ATan(TMath::Exp(-1*Rad.eta()))*2");
  metdyn->SetAlias("dTheta1_Rad", "TMath::ATan(TMath::Exp(-1*SignalPS[1].eta()))*2-TMath::ATan(TMath::Exp(-1*Rad.eta()))*2");
  metdyn->SetAlias("dR0_Rad", "sqrt(dEta0_Rad*dEta0_Rad + dPhi0_Rad*dPhi0_Rad)");
  metdyn->SetAlias("dR1_Rad", "sqrt(dEta1_Rad*dEta1_Rad + dPhi1_Rad*dPhi1_Rad)");

  metdyn->SetAlias("vecX0_Rad", "SignalPS[0].py()*Rad.pz()-Rad.py()*SignalPS[0].pz()");
  metdyn->SetAlias("vecZ0_Rad", "SignalPS[0].pz()*Rad.px()-Rad.px()*SignalPS[0].pz()");
  metdyn->SetAlias("vecY0_Rad", "SignalPS[0].px()*Rad.py()-Rad.py()*SignalPS[0].px()");

  metdyn->SetAlias("vecX1_Rad", "SignalPS[1].py()*Rad.pz()-Rad.py()*SignalPS[1].pz()");
  metdyn->SetAlias("vecZ1_Rad", "SignalPS[1].pz()*Rad.px()-Rad.px()*SignalPS[1].pz()");
  metdyn->SetAlias("vecY1_Rad", "SignalPS[1].px()*Rad.py()-Rad.py()*SignalPS[1].px()");

  metdyn->SetAlias("vec0_Rad", "sqrt(vecX0_Rad*vecX0_Rad+vecY0_Rad*vecY0_Rad+vecZ0_Rad*vecZ0_Rad)");
  metdyn->SetAlias("vec1_Rad", "sqrt(vecX1_Rad*vecX1_Rad+vecY1_Rad*vecY1_Rad+vecZ1_Rad*vecZ1_Rad)");


  metdyn->SetAlias("pt_rel0_Rad", "vec0_Rad/(SignalPS[0].p()*SignalPS[0].pt())");
  metdyn->SetAlias("pt_rel1_Rad", "vec1_Rad/(SignalPS[1].p()*SignalPS[1].pt())");

  /*
  string title = ""; title = Resonance + " ; x_{gen part 0}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenParticles_genParticles__GEN[0].obj.pz()/Ecol*2 >> PZPARTON()", "", 1, false);
  Write(string("X_GENPART_0"));



  string title = ""; title = Resonance + " ; x_{gen part 1}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenParticles_genParticles__GEN[1].obj.pz()/Ecol*2 >> PZPARTON()", "", 1, false);
  Write(string("X_GENPART_1"));
  


  string title = ""; title = Resonance + " ; x_{gen part 2}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenParticles_genParticles__GEN[2].obj.pz()/Ecol*2 >> PZPARTON()", "", 1, false);
  Write(string("X_GENPART_2"));

  string title = ""; title = Resonance + " ; x_{gen part 3}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenParticles_genParticles__GEN[3].obj.pz()/Ecol*2 >> PZPARTON()", "", 1, false);
  Write(string("X_GENPART_3"));
  


  string title = ""; title = Resonance + " ; x_{gen gluon}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenParticles_genParticles__GEN[2].obj.pz()/Ecol*2 >> PZPARTON()", "recoGenParticles_genParticles__GEN[2].obj.pdgId() == 21", 1, false);
  Write(string("X_GENPART_GLUON_2"));

  string title = ""; title = Resonance + " ; x_{gen gluon}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenParticles_genParticles__GEN[3].obj.pz()/Ecol*2 >> PZPARTON()", "recoGenParticles_genParticles__GEN[3].obj.pdgId() == 21", 1, false);
  Write(string("X_GENPART_GLUON_3"));


  string title = ""; title = Resonance + " ; x_{gen quark}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenParticles_genParticles__GEN[2].obj.pz()/Ecol*2 >> PZPARTON()", "recoGenParticles_genParticles__GEN[2].obj.pdgId() != 21 && recoGenParticles_genParticles__GEN[2].obj.pdgId() > 0", 1, false);
  Write(string("X_GENPART_QUARK_2"));

  string title = ""; title = Resonance + " ; x_{gen quark}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenParticles_genParticles__GEN[3].obj.pz()/Ecol*2 >> PZPARTON()", "recoGenParticles_genParticles__GEN[3].obj.pdgId() != 21 && recoGenParticles_genParticles__GEN[3].obj.pdgId() > 0", 1, false);
  Write(string("X_GENPART_QUARK_3"));


  string title = ""; title = Resonance + " ; x_{gen anti-quark}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenParticles_genParticles__GEN[2].obj.pz()/Ecol*2 >> PZPARTON()", "recoGenParticles_genParticles__GEN[2].obj.pdgId() != 21 && recoGenParticles_genParticles__GEN[2].obj.pdgId() < 0", 1, false);
  Write(string("X_GENPART_ANTIQUARK_2"));

  string title = ""; title = Resonance + " ; x_{gen anti-quark}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenParticles_genParticles__GEN[3].obj.pz()/Ecol*2 >> PZPARTON()", "recoGenParticles_genParticles__GEN[3].obj.pdgId() != 21 && recoGenParticles_genParticles__GEN[3].obj.pdgId() < 0", 1, false);
  Write(string("X_GENPART_ANTIQUARK_3"));
  */

  /*
  string title = ""; title = Resonance + " ; PDG ID parton from PDF";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenParticles_genParticles__GEN[3].obj.pdgId() >> PDGID()", "", 1, false);
  Write(string("PDGID_GENPART_3"));
 

  metdyn->SetTitle(Title(string("x2_{u} - x1_{ubar}")));
  metdyn->Draw("x2_Reco-x1_Reco >> DELTAX_RECO(100, -1, 1)", "recoGenParticles_genParticles__GEN[2].obj.pdgId() == 2 && recoGenParticles_genParticles__GEN[3].obj.pdgId() == -2", 1, false); 
  Write(string("DELTAX_RECO_ASYMETRY"));

  
  metdyn->SetTitle(Title(string("x2_Reco-x1_Reco for u or d quark from PDF")));
  metdyn->Draw("x2_Reco-x1_Reco >> DELTAX_RECO(100, -1, 1)", "recoGenParticles_genParticles__GEN[2].obj.pdgId() == 21", 1, false); 
  Write(string("DELTAX_RECO_FORGfromPDF"));
  

  metdyn->SetTitle(Title(string("x2_Reco-x1_Reco for u or d quark from PDF")));
  metdyn->Draw("x2_Reco-x1_Reco >> DELTAX_RECO(100, -1, 1)", "recoGenParticles_genParticles__GEN[2].obj.pdgId() == 21 && recoGenParticles_genParticles__GEN[3].obj.pdgId() == 21", 1, false); 
  Write(string("DELTAX_RECO_FORGGfromPDF"));

  
  string title = ""; title = Resonance + " ; PDG ID gen particle 3";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenParticles_genParticles__GEN[3].obj.pdgId() >> PDGID()", "recoGenParticles_genParticles__GEN[2].obj.pdgId() == 2", 1, false);
  Write(string("PDGID_GENPART_3_PDGID2is2"));

  string title = ""; title = Resonance + " ; PDG ID gen particle 3";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenParticles_genParticles__GEN[4].obj.pdgId() >> PDGID()", "", 1, false);
  Write(string("PDGID_GENPART_4_PDID"));
  
  string title = ""; title = Resonance + " ; PDG ID RSGraviton mother";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenParticles_genParticles__GEN[5].obj.pdgId() >> PDGID()", "", 1, false);
  Write(string("PDGID_GENPART_5_PDID"));

  
  string title = ""; title = Resonance + " ; p_{T, jet0} - p_{T, parton0}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("SignalPS[0].pt()-Signal[0].pt() >> DeltaPT_PS0()", "", 1, false); 
  Write(string("DeltaPt_PS0"));

 string title = ""; title = Resonance + " ; p_{T, jet1} - p_{T, parton1}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("SignalPS[1].pt()-Signal[1].pt() >> DeltaPT_PS1()", "", 1, false); 
  Write(string("DeltaPt_PS1"));


 string title = ""; title = Resonance + " ; p_{T, jet0} + p_{T, jet1} - p_{T, jet2} - p_{T, jet3} - p_{T, jet4} - p_{T, jet5} - p_{T, jet6}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("SignalPS[0].pt()+SignalPS[1].pt()-(SignalPS[2].pt()+SignalPS[3].pt()+SignalPS[4].pt()+SignalPS[5].pt()+SignalPS[6].pt()) >> DeltaPT_PS1()", "", 1, false); 
  Write(string("DeltaTotPt"));
  
  
  string title = ""; title = Resonance + " ; #Delta #phi ; #Delta #eta";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("dEta0 : dPhi0 >> DeltaR0_PS(60, -6, 6, 60, -6, 6)", "dR02<dR12&&SignalPS.pt() <SignalPS[1].pt()", 1, false); 
  Write(string("DeltaR0_PS"));
  

  string title = ""; title = Resonance + " ; #Delta #phi ; #Delta #eta";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("dEta1 : dPhi1 >> DeltaR1_PS(60, -6, 6, 60, -6, 6)", "dR02>dR12&&SignalPS.pt() <SignalPS[1].pt()", 1, false); 
  Write(string("DeltaR1_PS"));
  

    string title = ""; title = Resonance + " ; #Delta #phi ; #Delta #theta";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("dTheta0 : dPhi0 >> DeltaR0_Theta_PS(60, -6, 6, 60, -6, 6)", "dR02<dR12&&SignalPS.pt() <SignalPS[1].pt()", 1, false); 
  Write(string("DeltaR0_Theta_PS"));
  

  string title = ""; title = Resonance + " ; #Delta #phi ; #Delta #theta";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("dTheta1 : dPhi1 >> DeltaR1_Theta_PS(60, -6, 6, 60, -6, 6)", "dR02>dR12&&SignalPS.pt( <SignalPS[1].pt()", 1, false); 
  Write(string("DeltaR1_Theta_PS"));


  
  string title = ""; title = Resonance + " ; #Delta #phi ; #Delta #eta";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("dEta0_PartonJet : dPhi0_PartonJet >> DeltaR0_PartonJet(20, -2, 2, 20, -2, 2)", "", 1, false); 
  Write(string("DeltaR0_PartonJet"));


  string title = ""; title = Resonance + " ; #Delta #phi ; #Delta #eta";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("dEta1_PartonJet : dPhi1_PartonJet >> DeltaR1_PartonJet(20, -2, 2, 20, -2, 2)", "", 1, false); 
  Write(string("DeltaR1_PartonJet"));
  */




 
  /*
  string title = ""; title = Resonance + " ; p_{T, rel1}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("pt_rel0 >> PTREL(100, 0, 1)", "SignalPS.pt() < SignalPS[1].pt() && dR0_Rad < dR1_Rad", 1, false);
  Write(string("FSR_PT_REL0"));


  string title = ""; title = Resonance + " ; p_{T, rel2}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("pt_rel1 >> PTREL(100, 0, 1)", "SignalPS.pt() < SignalPS[1].pt() && dR0_Rad > dR1_Rad", 1, false);
  Write(string("FSR_PT_REL1"));


  string title = ""; title = Resonance + " ; p_{T, rel1}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("pt_rel0_Rad >> PTREL(100, 0, 1)", "dR0 < dR1", 1, false);
  Write(string("ISR_PT_REL0"));


  string title = ""; title = Resonance + " ; p_{T, rel2}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("pt_rel1_Rad >> PTREL(100, 0, 1)", "dR0 > dR1", 1, false);
  Write(string("ISR_PT_REL1"));
  */

  /*
  string title = ""; title = Resonance + " ; p_{T, FSR partons} (GeV)";
  metdyn->Draw("recoGenParticles_radGenParticles__GEN.obj.pt() >> DeltaPT_FSRPart(20, 0, 20)", "", 1, false); 
  Write(string("FSR_Partons_PT_ZOOM"));
  
  
  string title = ""; title = Resonance + " ; p_{T, FSR jet3} (GeV)";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("SignalPS[2].pt() >> DeltaPT_FSRPart(20, 0, 20)", "", 1, false); 
  Write(string("FSR_Jet2_ZOOM"));
  */


  /*
  string title = ""; title = Resonance + " ;N_{FSR jets}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenJets_ak7GenJetsSignalPS__GEN.@obj.size() >> DeltaPT_FSRPart(16, -0.5, 15.5)", "", 1, false); 
  Write(string("FSR_Jets"));


  string title = ""; title = Resonance + " ;N_{ISR jets}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("recoGenJets_ak7GenJetsRad__GEN.@obj.size() >> DeltaPT_ISRPart(16, -0.5, 15.5)", "", 1, false); 
  Write(string("ISR_Jets"));
  

 
  metdyn->SetLegendCorners(0.7,0.70,0.93,0.88);

  string title = ""; title = Resonance + " ; p_{T, FSR jet3} (GeV)";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("SignalPS[2].pt() >> DeltaPT_FSRPart(50, 0, 500)", "", 1, false); 
  Write(string("FSR_Jet2"));
  


  string title = ""; title = Resonance + " ; p_{T, FSR jet4} (GeV)";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("SignalPS[3].pt() >> DeltaPT_FSRPart(50, 0, 500)", "", 1, false); 
  Write(string("FSR_Jet3"));



  string title = ""; title = Resonance + " ; p_{T, FSR jet5} (GeV)";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("SignalPS[4].pt() >> DeltaPT_FSRPart(50, 0, 500)", "", 1, false); 
  Write(string("FSR_Jet4"));

  string title = ""; title = Resonance + " ; p_{T, FSR jet6} (GeV)";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("SignalPS[5].pt() >> DeltaPT_FSRPart(50, 0, 500)", "", 1, false); 
  Write(string("FSR_Jet5"));

  TH2F* Jet2 = ((TH2F*) metdyn->GetPlot("SignalPS[2].pt() >> DeltaPT_FSRPart(50, 0, 500)_")->GetHistogram("Resonance"));
  TH2F* Jet3 = ((TH2F*) metdyn->GetPlot("SignalPS[3].pt() >> DeltaPT_FSRPart(50, 0, 500)_")->GetHistogram("Resonance"));
  TH2F* Jet4 = ((TH2F*) metdyn->GetPlot("SignalPS[4].pt() >> DeltaPT_FSRPart(50, 0, 500)_")->GetHistogram("Resonance"));
  TH2F* Jet5 = ((TH2F*) metdyn->GetPlot("SignalPS[5].pt() >> DeltaPT_FSRPart(50, 0, 500)_")->GetHistogram("Resonance"));

  Jet2->SetXTitle("FSR Jet p_{T}");
  Jet2->SetMaximum(Jet2->GetMaximum()/1.3);

  Jet2->SetLineColor(kBlack);
  Jet3->SetLineColor(kRed);
  Jet4->SetLineColor(kGreen);
  Jet5->SetLineColor(kBlue);

  Jet2->SetMarkerColor(kBlack);
  Jet3->SetMarkerColor(kRed);
  Jet4->SetMarkerColor(kGreen);
  Jet5->SetMarkerColor(kBlue);

  Jet2->SetMarkerStyle(20);
  Jet3->SetMarkerStyle(21);
  Jet4->SetMarkerStyle(22);
  Jet5->SetMarkerStyle(23);

  Jet3->SetLineStyle(2);
  Jet4->SetLineStyle(3);
  Jet5->SetLineStyle(4);


  Jet2->Draw();
  Jet3->Draw("SAME");
  Jet4->Draw("SAME");
  Jet5->Draw("SAME");


  metdyn->GetPlot("SignalPS[2].pt() >> DeltaPT_FSRPart(50, 0, 500)_")->legend()->Clear();

  metdyn->GetPlot("SignalPS[2].pt() >> DeltaPT_FSRPart(50, 0, 500)_")->legend()->AddEntry(Jet2, "FSR Jet 3", "l");

  metdyn->GetPlot("SignalPS[2].pt() >> DeltaPT_FSRPart(50, 0, 500)_")->legend()->AddEntry(Jet3, "FSR Jet 4", "l");

  metdyn->GetPlot("SignalPS[2].pt() >> DeltaPT_FSRPart(50, 0, 500)_")->legend()->AddEntry(Jet4, "FSR Jet 5", "l");

  metdyn->GetPlot("SignalPS[2].pt() >> DeltaPT_FSRPart(50, 0, 500)_")->legend()->AddEntry(Jet5, "FSR Jet 6", "l");

  metdyn->GetPlot("SignalPS[2].pt() >> DeltaPT_FSRPart(50, 0, 500)_")->legend()->Draw();

  Write(string("FSR_PT_OF_JETS"));
  











  metdyn->SetLegendCorners(0.7,0.70,0.93,0.88);

  string title = ""; title = Resonance + " ; p_{T, ISR jet0} (GeV)";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("Rad[0].pt() >> DeltaPT_ISRPart(50, 0, 500)", "", 1, false); 
  Write(string("ISR_Jet0"));
  


  string title = ""; title = Resonance + " ; p_{T, ISR jet1} (GeV)";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("Rad[1].pt() >> DeltaPT_ISRPart(50, 0, 500)", "", 1, false); 
  Write(string("ISR_Jet1"));



  string title = ""; title = Resonance + " ; p_{T, ISR jet2} (GeV)";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("Rad[2].pt() >> DeltaPT_ISRPart(50, 0, 500)", "", 1, false); 
  Write(string("ISR_Jet2"));

  string title = ""; title = Resonance + " ; p_{T, ISR jet3} (GeV)";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("Rad[3].pt() >> DeltaPT_ISRPart(50, 0, 500)", "", 1, false); 
  Write(string("ISR_Jet3"));

  TH2F* Jet2 = ((TH2F*) metdyn->GetPlot("Rad[0].pt() >> DeltaPT_ISRPart(50, 0, 500)_")->GetHistogram("Resonance"));
  TH2F* Jet3 = ((TH2F*) metdyn->GetPlot("Rad[1].pt() >> DeltaPT_ISRPart(50, 0, 500)_")->GetHistogram("Resonance"));
  TH2F* Jet4 = ((TH2F*) metdyn->GetPlot("Rad[2].pt() >> DeltaPT_ISRPart(50, 0, 500)_")->GetHistogram("Resonance"));
  TH2F* Jet5 = ((TH2F*) metdyn->GetPlot("Rad[3].pt() >> DeltaPT_ISRPart(50, 0, 500)_")->GetHistogram("Resonance"));

  Jet2->SetXTitle("ISR Jet p_{T}");
  Jet2->SetMaximum(Jet2->GetMaximum()/1.3);

  Jet2->SetLineColor(kBlack);
  Jet3->SetLineColor(kRed);
  Jet4->SetLineColor(kGreen);
  Jet5->SetLineColor(kBlue);

  Jet2->SetMarkerColor(kBlack);
  Jet3->SetMarkerColor(kRed);
  Jet4->SetMarkerColor(kGreen);
  Jet5->SetMarkerColor(kBlue);

  Jet2->SetMarkerStyle(20);
  Jet3->SetMarkerStyle(21);
  Jet4->SetMarkerStyle(22);
  Jet5->SetMarkerStyle(23);

  Jet3->SetLineStyle(2);
  Jet4->SetLineStyle(3);
  Jet5->SetLineStyle(4);


  Jet2->Draw();
  Jet3->Draw("SAME");
  Jet4->Draw("SAME");
  Jet5->Draw("SAME");


  metdyn->GetPlot("Rad[0].pt() >> DeltaPT_ISRPart(50, 0, 500)_")->legend()->Clear();

  metdyn->GetPlot("Rad[0].pt() >> DeltaPT_ISRPart(50, 0, 500)_")->legend()->AddEntry(Jet2, "ISR Jet 1", "l");

  metdyn->GetPlot("Rad[0].pt() >> DeltaPT_ISRPart(50, 0, 500)_")->legend()->AddEntry(Jet3, "ISR Jet 2", "l");

  metdyn->GetPlot("Rad[0].pt() >> DeltaPT_ISRPart(50, 0, 500)_")->legend()->AddEntry(Jet4, "ISR Jet 3", "l");

  metdyn->GetPlot("Rad[0].pt() >> DeltaPT_ISRPart(50, 0, 500)_")->legend()->AddEntry(Jet5, "ISR Jet 4", "l");

  metdyn->GetPlot("Rad[0].pt() >> DeltaPT_ISRPart(50, 0, 500)_")->legend()->Draw();

  Write(string("ISR_PT_OF_JETS"));
  */

  /*

  string title = ""; title = Resonance + " ; p_{T, FSR jet} (GeV)";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("SignalPS.pt() >> DeltaPT_FSRPart(20, 0, 20)", "", 1, false); 
  Write(string("FSR_Jet_ZOOM"));



  
  string title = ""; title = Resonance + " ; p_{x, partons} - p_{x, FSR}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("(Signal[0].px()+Signal[1].px())-(SignalPS[0].px()+SignalPS[1].px()+SignalPS[2].px()) >> DeltaPT_FSRPart()", "recoGenJets_ak7GenJetsSignalPS__GEN.@obj.size() == 3", 1, false); 
  Write(string("DELTAPX_FSRPARTONS"));

  string title = ""; title = Resonance + " ; p_{x, partons} + p_{x, ISR}";
  metdyn->SetTitle(title.c_str());

  metdyn->Draw("Signal[0].px()+Signal[1].px()+(Rad[0].px()+Rad[1].px()+Rad[2].px())", "recoGenJets_ak7GenJetsRad__GEN.@obj.size() == 3", 1, false); 
  Write(string("DELTAPX_ISRPARTONS"));
  

  
  metdyn->SetAlias("theta_rad", "TMath::ATan(TMath::Exp(-1*Rad.eta()))*2*180/Pi");
  metdyn->SetAlias("E_rad", "sqrt(Rad.mass()*Rad.mass()+Rad.px()*Rad.px()+Rad.py()*Rad.py()+Rad.pz()*Rad.pz())");
  metdyn->SetAlias("moins", "0");

  
  string title = ""; title = Resonance + " ; #Delta #phi ; #Delta #eta";
  metdyn->SetTitle(title.c_str());
  metdyn->Draw("dEta0_Rad : dPhi0_Rad >> DeltaR_Rad(60, -6, 6, 60, -6, 6)", "dR0_Rad<dR1_Rad", 1, false); 
  Write(string("DeltaR0_Rad"));

  string title = ""; title = Resonance + " ; #Delta #phi ; #Delta #theta";
  metdyn->SetTitle(title.c_str());
  metdyn->Draw("dTheta0_Rad : dPhi0_Rad >> DeltaR_Theta_Rad(60, -6, 6, 60, -6, 6)", "dR0_Rad<dR1_Rad", 1, false); 
  Write(string("DeltaR0_Theta_Rad"));
  

  string title = ""; title = Resonance + " ; #Delta R ; p_{T,3}";
  metdyn->SetTitle(title.c_str());
  metdyn->Draw("Rad.pt() : dR0_Rad >> DeltaR0vsPT_Rad(60, 0, 6, 100, 0, 1000)", "dR0_Rad<dR1_Rad", 1, false); 
  Write(string("DeltaR0vsPT_Rad"));
  

  metdyn->SetTitle(Title(string("#eta_{ISR}")));
  metdyn->Draw("Rad.eta() >> RadJetEta(60, -6, 6)", "", 1, false); 
  Write(string("RAD_JET_ETA"));

  metdyn->SetTitle(Title(string("#p_{T, ISR}")));
  metdyn->Draw("Rad.pt() >> RadJetEta(100, 0, 1000)", "", 1, false); 
  Write(string("RAD_JET_PT"));
  
  metdyn->SetTitle(Title(string("#theta_{ISR}")));
  metdyn->Draw("TMath::ATan(TMath::Exp(-1*Rad.eta()))*2*180/Pi >> RadJetEta(36, 0, 180)", "", 1, false); 
  Write(string("RAD_JET_THETA"));
  



  metdyn->SetTitle(Title(string("#eta_{FSR}")));
  metdyn->Draw("SignalPS.eta() >> SignalFSRJetEta(60, -6, 6)", "SignalPS.pt() < SignalPS[1].pt()", 1, false); 
  Write(string("FSR_JET_ETA_2"));

  metdyn->SetTitle(Title(string("#p_{T, FSR}")));
  metdyn->Draw("SignalPS.pt() >> SignalFSRJetEta(100, 0, 1000)", "SignalPS.pt() < SignalPS[1].pt()", 1, false); 
  Write(string("FSR_JET_PT_2"));
  
  metdyn->SetTitle(Title(string("#theta_{FSR}")));
  metdyn->Draw("TMath::ATan(TMath::Exp(-1*SignalPS.eta()))*2*180/Pi >> SignalPSJetEta(36, 0, 180)", "SignalPS.pt() < SignalPS[1].pt()", 1, false); 
  Write(string("FSR_JET_THETA_2"));
  */





  ///---------------------------------------------------------------------------------------///
  ///------------------ PTWEIGHTED IN THE PHASE SPACE --------------------------------------///
  ///---------------------------------------------------------------------------------------///





  /* 

  */
  /*
  metdyn->SetTitle(Title(string(" p_{T, rel1}")));
  metdyn->Draw("SignalPS.pt() : pt_rel0 >> SignalPSJetEtaWeighted2D(50, 0, 1, 3000, 0, 3000)", "SignalPS.pt() < SignalPS[1].pt() && dR0 < dR1 && TMath::Abs(SignalPS.eta()) < 2 && TMath::Abs(SignalPS[0].eta()) < 2 && TMath::Abs(SignalPS[1].eta()) < 2 && SignalPS.pt() > 10", 1, false); 
  string s("SignalPS.pt() : pt_rel0 >> SignalPSJetEtaWeighted2D(50, 0, 1, 3000, 0, 3000)_SignalPS.pt() < SignalPS[1].pt() && dR0 < dR1 && TMath::Abs(SignalPS.eta()) < 2 && TMath::Abs(SignalPS[0].eta()) < 2 && TMath::Abs(SignalPS[1].eta()) < 2 && SignalPS.pt() > 10");
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj, norm);
  TH1D* ProjLeading = new TH1D(); Proj1->Copy(*ProjLeading);
  Write(string("FSR_PT_REL_PTDENSITY_0"));


  metdyn->SetTitle(Title(string(" p_{T, rel1}")));
  string s("Rad.pt() : pt_rel0_Rad >> SignalPSJetEtaWeighted2D(50, 0, 1, 3000, 0, 3000)_dR0_Rad < dR1_Rad && dR0_Rad > 0.7 && TMath::Abs(Rad.eta()) < 2 && TMath::Abs(SignalPS[0].eta()) < 2 && TMath::Abs(SignalPS[1].eta()) < 2 && Rad.pt() > 10");
  metdyn->Draw("Rad.pt() : pt_rel0_Rad >> SignalPSJetEtaWeighted2D(50, 0, 1, 3000, 0, 3000)", "dR0_Rad < dR1_Rad && dR0_Rad > 0.7 && TMath::Abs(Rad.eta()) < 2 && TMath::Abs(SignalPS[0].eta()) < 2 && TMath::Abs(SignalPS[1].eta()) < 2 && Rad.pt() > 10", 1, false); 
  TH2F* Proj = ((TH2
F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj, norm);
  TH1D* ProjLeading = new TH1D(); Proj1->Copy(*ProjLeading);
  Write(string("ISR_PT_REL_PTDENSITY_0"));
  
  
  
  
  metdyn->SetTitle(Title(string("p_{T} weighted #theta_{LeadingJets}")));
  metdyn->Draw("SignalPS.pt() : theta_PS >> SignalPSJetEtaWeighted2D(36, 0, 180, 3000, 0, 3000)", "SignalPS.pt() > SignalPS[1].pt()-0.01", 1, false); 
  string s("SignalPS.pt() : theta_PS >> SignalPSJetEtaWeighted2D(36, 0, 180, 3000, 0, 3000)_SignalPS.pt() > SignalPS[1].pt()-0.01");
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj);
  TH1D* ProjLeading = new TH1D(); Proj1->Copy(*ProjLeading);
  Write(string("LEADINGJETS_JET_THETA_PTWEIGHTED"));
  
  
  metdyn->SetTitle(Title(string("p_{T} weighted #theta_{FSR}")));
  metdyn->Draw("SignalPS.pt() : theta_PS >> SignalPSJetEtaWeighted2D(36, 0, 180, 3000, 0, 3000)", "SignalPS.pt() < SignalPS[1].pt()", 1, false); 
  TH2F* Proj = ((TH2F*) metdyn->GetPlot("SignalPS.pt() : theta_PS >> SignalPSJetEtaWeighted2D(36, 0, 180, 3000, 0, 3000)_SignalPS.pt() < SignalPS[1].pt()")->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj);
  TH1D* FSR = new TH1D(); Proj1->Copy(*FSR);
  Write(string("FSR_JET_THETA_PTWEIGHTED"));


  metdyn->SetTitle(Title(string("p_{T} weighted #theta_{ISR}")));
  metdyn->Draw("Rad.pt() : theta_rad >> SignalPSJetEtaWeighted2D(36, 0, 180, 3000, 0, 3000)", "", 1, false); 
  TH2F* Proj = ((TH2F*) metdyn->GetPlot("Rad.pt() : theta_rad >> SignalPSJetEtaWeighted2D(36, 0, 180, 3000, 0, 3000)_")->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj);
  TH1D* ISR = new TH1D(); Proj1->Copy(*ISR);
  Write(string("ISR_JET_THETA_PTWEIGHTED"));

  ProjLeading->SetStats(0);
  ProjLeading->SetXTitle("#theta_{jet}");
  ProjLeading->SetYTitle("p_{T} density (GeV) / Event");
  ProjLeading->SetMaximum(ProjLeading->GetMaximum());

  ProjLeading->SetLineColor(kBlack);
  FSR->SetLineColor(kRed);
  ISR->SetLineColor(kBlue);

  ProjLeading->SetMarkerColor(kBlack);
  FSR->SetMarkerColor(kRed);
  ISR->SetMarkerColor(kBlue);

  ProjLeading->SetMarkerStyle(20);
  FSR->SetMarkerStyle(21);
  ISR->SetMarkerStyle(22);

  FSR->SetLineStyle(2);
  ISR->SetLineStyle(3);

  ProjLeading->Draw("P");
  FSR->Draw("SAMEP");
  ISR->Draw("SAMEP");


  metdyn->SetLegendCorners(0.7,0.65,0.93,0.88);
  metdyn->GetPlot("SignalPS.pt() : theta_PS >> SignalPSJetEtaWeighted2D(36, 0, 180, 3000, 0, 3000)_SignalPS.pt() < SignalPS[1].pt()")->legend()->Clear();

  metdyn->GetPlot("SignalPS.pt() : theta_PS >> SignalPSJetEtaWeighted2D(36, 0, 180, 3000, 0, 3000)_SignalPS.pt() < SignalPS[1].pt()")->legend()->AddEntry(ProjLeading, "Leading jets", "P");

  metdyn->GetPlot("SignalPS.pt() : theta_PS >> SignalPSJetEtaWeighted2D(36, 0, 180, 3000, 0, 3000)_SignalPS.pt() < SignalPS[1].pt()")->legend()->AddEntry(FSR, "FSR", "P");

  metdyn->GetPlot("SignalPS.pt() : theta_PS >> SignalPSJetEtaWeighted2D(36, 0, 180, 3000, 0, 3000)_SignalPS.pt() < SignalPS[1].pt()")->legend()->AddEntry(ISR, "ISR", "P");

  metdyn->GetPlot("SignalPS.pt() : theta_PS >> SignalPSJetEtaWeighted2D(36, 0, 180, 3000, 0, 3000)_SignalPS.pt() < SignalPS[1].pt()")->legend()->Draw();

  Write(string("PT_DENSITY_ISRFSR_THETA"));
  




  metdyn->SetTitle(Title(string("p_{T} weighted #eta_{LeadingJets}")));
  metdyn->Draw("SignalPS.pt() : SignalPS.eta() >> SignalPS(50, -5, 5, 3000, 0, 3000)", "SignalPS.pt() > SignalPS[1].pt()-0.01", 1, false); 
  string s("SignalPS.pt() : SignalPS.eta() >> SignalPS(50, -5, 5, 3000, 0, 3000)_SignalPS.pt() > SignalPS[1].pt()-0.01");
  metdyn->Print();
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj);
  TH1D* ProjLeading = new TH1D(); Proj1->Copy(*ProjLeading);
  Write(string("LEADINGJETS_JET_ETA_PTWEIGHTED"));
  
  
  metdyn->SetTitle(Title(string("p_{T} weighted #eta_{FSR}")));

  string s("SignalPS.pt() : SignalPS.eta() >> SignalPSJetEtaWeighted2D(50, -5, 5, 3000, 0, 3000)");
  string cut("SignalPS.pt() < SignalPS[1].pt()");
  metdyn->Draw(s.c_str(), cut.c_str(), 1, false); 

  s = s + "_" + cut;
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj);
  TH1D* FSR = new TH1D(); Proj1->Copy(*FSR);
  Write(string("FSR_JET_ETA_PTWEIGHTED"));


  metdyn->SetTitle(Title(string("p_{T} weighted #eta_{ISR}")));

  string s("Rad.pt() : Rad.eta() >> SignalPSJetEtaWeighted2D(50, -5, 5, 3000, 0, 3000)");
  string cut("");
  metdyn->Draw(s.c_str(), cut.c_str(), 1, false); 
  s = s + "_" + cut;
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj);
  TH1D* ISR = new TH1D(); Proj1->Copy(*ISR);
  Write(string("ISR_JET_ETA_PTWEIGHTED"));

  ProjLeading->SetStats(0);
  ProjLeading->SetXTitle("#eta_{Jet}");
  ProjLeading->SetYTitle("Jet p_{T} density (GeV) / Event");
  ProjLeading->SetMaximum(ProjLeading->GetMaximum());  
  ProjLeading->SetMinimum(1);

  ProjLeading->SetLineColor(kBlack);
  FSR->SetLineColor(kRed);
  ISR->SetLineColor(kBlue);

  ProjLeading->SetMarkerColor(kBlack);
  FSR->SetMarkerColor(kRed);
  ISR->SetMarkerColor(kBlue);

  ProjLeading->SetMarkerStyle(20);
  FSR->SetMarkerStyle(21);
  ISR->SetMarkerStyle(22);

  FSR->SetLineStyle(2);
  ISR->SetLineStyle(3);

  ProjLeading->Draw("P");
  FSR->Draw("SAMEP");
  ISR->Draw("SAMEP");

  string s("SignalPS.pt() : SignalPS.eta() >> SignalPSJetEtaWeighted2D(50, -5, 5, 3000, 0, 3000)");
  string cut("SignalPS.pt() < SignalPS[1].pt()");
  s = s + "_" + cut;

  metdyn->SetLegendCorners(0.7,0.65,0.93,0.88);
  metdyn->GetPlot(s.c_str())->legend()->Clear();

  metdyn->GetPlot(s.c_str())->legend()->AddEntry(ProjLeading, "Leading jets", "P");

  metdyn->GetPlot(s.c_str())->legend()->AddEntry(FSR, "FSR", "P");

  metdyn->GetPlot(s.c_str())->legend()->AddEntry(ISR, "ISR", "P");

  metdyn->GetPlot(s.c_str())->legend()->Draw();

  Write(string("PT_DENSITY_ISRFSR_ETA"));
  */









  string additionalCut = "";//"Parton[0].pdgId() == 21 && Parton[1].pdgId() == 21 && recoGenParticles_genParticles__GEN[4].obj.pdgId() == 21 && recoGenParticles_genParticles__GEN[5].obj.pdgId() == 21 && ";
  string generalCut  =  additionalCut + "TMath::Abs(SignalPS[0].eta()) < 2.5 && TMath::Abs(SignalPS[1].eta()) < 2.5 && TMath::Abs(SignalPS[0].eta()-SignalPS[1].eta()) < 1.3";
  string generalCut10 =  generalCut + " && TMath::Abs(Rad.eta()) < 2.5 && Rad.pt() > 10 && TMath::Abs(SignalPS[0].phi() - Rad.phi()) < TMath::Pi()/2";
  string generalCut11 =  generalCut + " && TMath::Abs(Rad.eta()) < 2.5 && Rad.pt() > 10 && TMath::Abs(SignalPS[1].phi() - Rad.phi()) < TMath::Pi()/2";
  string generalCut20 =  generalCut + " && TMath::Abs(SignalPS.eta()) < 2.5 && SignalPS.pt() > 10  && TMath::Abs(SignalPS[0].phi() - SignalPS.phi()) < TMath::Pi()/2";
  string generalCut21 =  generalCut + " && TMath::Abs(SignalPS.eta()) < 2.5 && SignalPS.pt() > 10  && TMath::Abs(SignalPS[1].phi() - SignalPS.phi()) < TMath::Pi()/2";
  string label = "_DIJETPHASESPACE_PHIpi2_ETA";


  metdyn->Draw("1 >> Norm(1, 0.5, 1.5)", generalCut.c_str(), 1, false); 
  string s("1 >> Norm(1, 0.5, 1.5)");
  s = s + "_" + generalCut;
  norm = metdyn->GetPlot(s.c_str())->GetHistogram("Resonance")->Integral();
  cout << "Norm = " << norm << endl;






  ProduceDensityPlots(generalCut, generalCut10, generalCut11, generalCut20, generalCut21, label);


  


















  //  string generalCut =  "TMath::Abs(Rad.eta()) < 2 && TMath::Abs(SignalPS[0].eta()) < 2 && TMath::Abs(SignalPS[1].eta()) < 2 && Rad.pt() > 10";
  // "dR0<dR1 && TMath::Abs(SignalPS.eta()) < 2 && TMath::Abs(SignalPS[0].eta()) < 2 && TMath::Abs(SignalPS[1].eta()) < 2 && SignalPS.pt() > 10"



  
}



















void ProduceDensityPlots(string generalCut, string generalCut10, string generalCut11, string generalCut20, string generalCut21,  string label){


///---------------------------------------------------------------------------------------///
  ///------------------------- RELATIVE PT DENSITY -----------------------------------------///
  ///---------------------------------------------------------------------------------------///


  //-----------------------------
  // ISR and FSR for JET 1 and 2 with ETA cut
  //-----------------------------



  
  // - ISR DR0

  metdyn->SetTitle(Title(string("p_{T} weighted dR(Jet[0]-ISR[Jet])")));

  string cut("dR0_Rad<dR1_Rad");
  cut = cut + " && " + generalCut10;
  string s("Rad.pt() : dR0_Rad>> SignalPSJetEtaWeighted2D(30, 0, 5.99, 3000, 0, 3000)");
  metdyn->Draw(s.c_str(), cut.c_str(), 1, false); 

  s = s + "_"+ cut; 
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj, norm);
  TH1D* ISR_DR0 = new TH1D(); Proj1->Copy(*ISR_DR0);
  string TITLEOUT("ISR_DR0_PTWEIGHTED");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);
  
  // - ISR DR1
  
  metdyn->SetTitle(Title(string("p_{T} weighted dR(Jet[1]-ISR[Jet])")));

  string cut("dR0_Rad>dR1_Rad");
  cut = cut + " && " + generalCut11;
  string s("Rad.pt() : dR1_Rad>> SignalPSJetEtaWeighted2D(30, 0, 5.99, 3000, 0, 3000)");

  metdyn->Draw(s.c_str(), cut.c_str(), 1, false); 

  s = s + "_"+ cut; 
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj, norm);
  TH1D* ISR_DR1 = new TH1D(); Proj1->Copy(*ISR_DR1);
  string TITLEOUT("ISR_DR1_PTWEIGHTED");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);

  // - FSR DR0

  metdyn->SetTitle(Title(string("p_{T} weighted dR(Jet[0]-FSR[Jet])")));

  string cut("dR0<dR1");
  cut = cut + " && " + generalCut20;
  string s("SignalPS.pt() : dR0>> SignalPSJetEtaWeighted2D(30, 0, 5.99, 3000, 0, 3000)");
  metdyn->Draw(s.c_str(), cut.c_str(), 1, false); 

  s = s + "_"+ cut; 
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj, norm);
  TH1D* FSR_DR0 = new TH1D(); Proj1->Copy(*FSR_DR0);
  string TITLEOUT("FSR_DR0_PTWEIGHTED");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);
  
  // - FSR DR1  

  metdyn->SetTitle(Title(string("p_{T} weighted dR(Jet[1]-FSR[Jet])")));

  string cut("dR0>dR1");
  cut = cut + " && " + generalCut21;
  string s("SignalPS.pt() : dR1>> SignalPSJetEtaWeighted2D(30, 0, 5.99, 3000, 0, 3000)");

  metdyn->Draw(s.c_str(), cut.c_str(), 1, false); 
  s = s + "_"+ cut; 
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj, norm);
  TH1D* FSR_DR1 = new TH1D(); Proj1->Copy(*FSR_DR1);
  string TITLEOUT("FSR_DR1_PTWEIGHTED");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);




  FSR_DR0->SetStats(0);
  FSR_DR0->SetXTitle("dR_{LeadingJet1 - Jet_FSR}");
  FSR_DR0->SetYTitle("Jet p_{T} density (GeV) / Event");
  FSR_DR0->SetMaximum(FSR_DR0->GetMaximum());  
  FSR_DR0->SetMinimum(1);

  FSR_DR0->SetLineColor(kRed);
  ISR_DR0->SetLineColor(kBlue);

  FSR_DR0->SetMarkerColor(kRed);
  ISR_DR0->SetMarkerColor(kBlue);

  FSR_DR0->SetMarkerStyle(21);
  ISR_DR0->SetMarkerStyle(22);

  FSR_DR0->SetLineStyle(2);
  ISR_DR0->SetLineStyle(3);

  FSR_DR0->Draw("P");
  ISR_DR0->Draw("SAMEP");

  metdyn->SetLegendCorners(0.7,0.65,0.93,0.88);
  metdyn->GetPlot(s.c_str())->legend()->Clear();

  metdyn->GetPlot(s.c_str())->legend()->AddEntry(FSR_DR0, "FSR_DR1", "P");

  metdyn->GetPlot(s.c_str())->legend()->AddEntry(ISR_DR0, "ISR_DR1", "P");

  metdyn->GetPlot(s.c_str())->legend()->Draw();

  string TITLEOUT("PT_DENSITY_DR0");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);



  FSR_DR1->SetStats(0);
  FSR_DR1->SetXTitle("dR_{LeadingJet2 - Jet_FSR}");
  FSR_DR1->SetYTitle("Jet p_{T} density (GeV) / Event");
  FSR_DR1->SetMaximum(FSR_DR1->GetMaximum());  
  FSR_DR1->SetMinimum(1);

  FSR_DR1->SetLineColor(kRed);
  ISR_DR1->SetLineColor(kBlue);

  FSR_DR1->SetMarkerColor(kRed);
  ISR_DR1->SetMarkerColor(kBlue);

  FSR_DR1->SetMarkerStyle(21);
  ISR_DR1->SetMarkerStyle(22);

  FSR_DR1->SetLineStyle(2);
  ISR_DR1->SetLineStyle(3);

  FSR_DR1->Draw("P");
  ISR_DR1->Draw("SAMEP");

  metdyn->SetLegendCorners(0.7,0.65,0.93,0.88);
  metdyn->GetPlot(s.c_str())->legend()->Clear();

  metdyn->GetPlot(s.c_str())->legend()->AddEntry(FSR_DR1, "FSR_DR2", "P");

  metdyn->GetPlot(s.c_str())->legend()->AddEntry(ISR_DR1, "ISR_DR2", "P");

  metdyn->GetPlot(s.c_str())->legend()->Draw();

  string TITLEOUT("PT_DENSITY_DR1");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);





 //-----------------------------
  // ISR and FSR for JET 1 and 2 as function of dETA
  //-----------------------------

  // - ISR DR0

  metdyn->SetTitle(Title(string("p_{T} weighted dR(Jet[0]-ISR[Jet])")));

  string cut("dR0_Rad<dR1_Rad");
  cut = cut + " && " + generalCut10;
  string s("Rad.pt() : dEta0_Rad>> SignalPSJetEtaWeighted2D(30, 0, 5.99, 3000, 0, 3000)");
  metdyn->Draw(s.c_str(), cut.c_str(), 1, false); 

  s = s + "_"+ cut; 
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj, norm);
  TH1D* ISR_DR0 = new TH1D(); Proj1->Copy(*ISR_DR0);
  string TITLEOUT("ISR_DETA0_PTWEIGHTED");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);
  

  // - ISR DR1
  
  metdyn->SetTitle(Title(string("p_{T} weighted dR(Jet[1]-ISR[Jet])")));

  string cut("dR0_Rad>dR1_Rad");
  cut = cut + " && " + generalCut11;
  string s("Rad.pt() : dEta1_Rad>> SignalPSJetEtaWeighted2D(30, 0, 5.99, 3000, 0, 3000)");

  metdyn->Draw(s.c_str(), cut.c_str(), 1, false); 

  s = s + "_"+ cut; 
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj, norm);
  TH1D* ISR_DR1 = new TH1D(); Proj1->Copy(*ISR_DR1);
  string TITLEOUT("ISR_DETA1_PTWEIGHTED");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);


  // - FSR DR0

  metdyn->SetTitle(Title(string("p_{T} weighted dR(Jet[0]-FSR[Jet])")));

  string cut("dR0<dR1");
  cut = cut + " && " + generalCut20;
  string s("SignalPS.pt() : dEta0>> SignalPSJetEtaWeighted2D(30, 0, 5.99, 3000, 0, 3000)");
  metdyn->Draw(s.c_str(), cut.c_str(), 1, false); 

  s = s + "_"+ cut; 
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj, norm);
  TH1D* FSR_DR0 = new TH1D(); Proj1->Copy(*FSR_DR0);
  string TITLEOUT("FSR_DETA0_PTWEIGHTED");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);
  
  
  // - FSR DR1

  metdyn->SetTitle(Title(string("p_{T} weighted dR(Jet[1]-Rad[Jet])")));

  string cut("dR0>dR1");
  cut = cut + " && " + generalCut21;
  string s("SignalPS.pt() : dEta1>> SignalPSJetEtaWeighted2D(30, 0, 5.99, 3000, 0, 3000)");

  metdyn->Draw(s.c_str(), cut.c_str(), 1, false); 

  s = s + "_"+ cut; 
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj, norm);
  TH1D* FSR_DR1 = new TH1D(); Proj1->Copy(*FSR_DR1);
  string TITLEOUT("FSR_DETA1_PTWEIGHTED");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);




  FSR_DR0->SetStats(0);
  FSR_DR0->SetXTitle("d#eta_{LeadingJet1 - Jet_FSR}");
  FSR_DR0->SetYTitle("Jet p_{T} density (GeV) / Event");
  FSR_DR0->SetMaximum(FSR_DR0->GetMaximum());  
  FSR_DR0->SetMinimum(1);

  FSR_DR0->SetLineColor(kRed);
  ISR_DR0->SetLineColor(kBlue);

  FSR_DR0->SetMarkerColor(kRed);
  ISR_DR0->SetMarkerColor(kBlue);

  FSR_DR0->SetMarkerStyle(21);
  ISR_DR0->SetMarkerStyle(22);

  FSR_DR0->SetLineStyle(2);
  ISR_DR0->SetLineStyle(3);

  FSR_DR0->Draw("P");
  ISR_DR0->Draw("SAMEP");

  metdyn->SetLegendCorners(0.7,0.65,0.93,0.88);
  metdyn->GetPlot(s.c_str())->legend()->Clear();

  metdyn->GetPlot(s.c_str())->legend()->AddEntry(FSR_DR0, "FSR_D#eta1", "P");

  metdyn->GetPlot(s.c_str())->legend()->AddEntry(ISR_DR0, "ISR_D#eta1", "P");

  metdyn->GetPlot(s.c_str())->legend()->Draw();

  string TITLEOUT("PT_DENSITY_DETA0");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);



  FSR_DR1->SetStats(0);
  FSR_DR1->SetXTitle("d#eta_{LeadingJet2 - Jet_FSR}");
  FSR_DR1->SetYTitle("Jet p_{T} density (GeV) / Event");
  FSR_DR1->SetMaximum(FSR_DR1->GetMaximum());  
  FSR_DR1->SetMinimum(1);

  FSR_DR1->SetLineColor(kRed);
  ISR_DR1->SetLineColor(kBlue);

  FSR_DR1->SetMarkerColor(kRed);
  ISR_DR1->SetMarkerColor(kBlue);

  FSR_DR1->SetMarkerStyle(21);
  ISR_DR1->SetMarkerStyle(22);

  FSR_DR1->SetLineStyle(2);
  ISR_DR1->SetLineStyle(3);

  FSR_DR1->Draw("P");
  ISR_DR1->Draw("SAMEP");

  metdyn->SetLegendCorners(0.7,0.65,0.93,0.88);
  metdyn->GetPlot(s.c_str())->legend()->Clear();

  metdyn->GetPlot(s.c_str())->legend()->AddEntry(FSR_DR1, "FSR_D#eta2", "P");

  metdyn->GetPlot(s.c_str())->legend()->AddEntry(ISR_DR1, "ISR_D#eta2", "P");

  metdyn->GetPlot(s.c_str())->legend()->Draw();

  string TITLEOUT("PT_DENSITY_DETA1");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);





 //-----------------------------
  // ISR and FSR for JET 1 and 2 as function of dPHI
  //-----------------------------

  // - ISR DR0

 metdyn->SetTitle(Title(string("p_{T} weighted dR(Jet[0]-ISR[Jet])")));

  string cut("dR0_Rad<dR1_Rad");
  cut = cut + " && " + generalCut10;
  string s("Rad.pt() : dPhi0_Rad>> SignalPSJetEtaWeighted2D(30, 0, 6, 3000, 0, 3000)");
  metdyn->Draw(s.c_str(), cut.c_str(), 1, false); 

  s = s + "_"+ cut; 
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj, norm);
  TH1D* ISR_DR0 = new TH1D(); Proj1->Copy(*ISR_DR0);
  string TITLEOUT("ISR_DPHI0_PTWEIGHTED");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);
  
  // - ISR DR1  

  metdyn->SetTitle(Title(string("p_{T} weighted dR(Jet[1]-ISR[Jet])")));

  string cut("dR0_Rad>dR1_Rad");
  cut = cut + " && " + generalCut11;
  string s("Rad.pt() : dPhi1_Rad>> SignalPSJetEtaWeighted2D(30, 0, 6, 3000, 0, 3000)");

  metdyn->Draw(s.c_str(), cut.c_str(), 1, false); 

  s = s + "_"+ cut; 
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj, norm);
  TH1D* ISR_DR1 = new TH1D(); Proj1->Copy(*ISR_DR1);
  string TITLEOUT("ISR_DPHI1_PTWEIGHTED");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);

  // - FSR DR0


  metdyn->SetTitle(Title(string("p_{T} weighted dR(Jet[0]-FSR[Jet])")));

  string cut("dR0<dR1");
  cut = cut + " && " + generalCut20;
  string s("SignalPS.pt() : dPhi0>> SignalPSJetEtaWeighted2D(30, 0, 6, 3000, 0, 3000)");
  metdyn->Draw(s.c_str(), cut.c_str(), 1, false); 

  s = s + "_"+ cut; 
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj, norm);
  TH1D* FSR_DR0 = new TH1D(); Proj1->Copy(*FSR_DR0);
  string TITLEOUT("FSR_DPHI0_PTWEIGHTED");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);
  
  
  // - FSR DR1

  metdyn->SetTitle(Title(string("p_{T} weighted dR(Jet[1]-Rad[Jet])")));

  string cut("dR0>dR1");
  cut = cut + " && " + generalCut21;
  string s("SignalPS.pt() : dPhi1>> SignalPSJetEtaWeighted2D(30, 0, 6, 3000, 0, 3000)");

  metdyn->Draw(s.c_str(), cut.c_str(), 1, false); 

  s = s + "_"+ cut; 
  TH2F* Proj = ((TH2F*) metdyn->GetPlot(s.c_str())->GetHistogram("Resonance"));
  TH1D* Proj1 = Project(Proj, norm);
  TH1D* FSR_DR1 = new TH1D(); Proj1->Copy(*FSR_DR1);
  string TITLEOUT("FSR_DPHI1_PTWEIGHTED");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);




  FSR_DR0->SetStats(0);
  FSR_DR0->SetXTitle("d#phi_{LeadingJet1 - Jet_FSR}");
  FSR_DR0->SetYTitle("Jet p_{T} density (GeV) / Event");
  FSR_DR0->SetMaximum(FSR_DR0->GetMaximum());  
  FSR_DR0->SetMinimum(1);

  FSR_DR0->SetLineColor(kRed);
  ISR_DR0->SetLineColor(kBlue);

  FSR_DR0->SetMarkerColor(kRed);
  ISR_DR0->SetMarkerColor(kBlue);

  FSR_DR0->SetMarkerStyle(21);
  ISR_DR0->SetMarkerStyle(22);

  FSR_DR0->SetLineStyle(2);
  ISR_DR0->SetLineStyle(3);

  FSR_DR0->Draw("P");
  ISR_DR0->Draw("SAMEP");

  metdyn->SetLegendCorners(0.7,0.65,0.93,0.88);
  metdyn->GetPlot(s.c_str())->legend()->Clear();

  metdyn->GetPlot(s.c_str())->legend()->AddEntry(FSR_DR0, "FSR_D#phi1", "P");

  metdyn->GetPlot(s.c_str())->legend()->AddEntry(ISR_DR0, "ISR_D#phi1", "P");

  metdyn->GetPlot(s.c_str())->legend()->Draw();

  string TITLEOUT("PT_DENSITY_DPHI0");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);



  FSR_DR1->SetStats(0);
  FSR_DR1->SetXTitle("d#phi_{LeadingJet2 - Jet_FSR}");
  FSR_DR1->SetYTitle("Jet p_{T} density (GeV) / Event");
  FSR_DR1->SetMaximum(FSR_DR1->GetMaximum());  
  FSR_DR1->SetMinimum(1);

  FSR_DR1->SetLineColor(kRed);
  ISR_DR1->SetLineColor(kBlue);

  FSR_DR1->SetMarkerColor(kRed);
  ISR_DR1->SetMarkerColor(kBlue);

  FSR_DR1->SetMarkerStyle(21);
  ISR_DR1->SetMarkerStyle(22);

  FSR_DR1->SetLineStyle(2);
  ISR_DR1->SetLineStyle(3);

  FSR_DR1->Draw("P");
  ISR_DR1->Draw("SAMEP");

  metdyn->SetLegendCorners(0.7,0.65,0.93,0.88);
  metdyn->GetPlot(s.c_str())->legend()->Clear();

  metdyn->GetPlot(s.c_str())->legend()->AddEntry(FSR_DR1, "FSR_D#phi2", "P");

  metdyn->GetPlot(s.c_str())->legend()->AddEntry(ISR_DR1, "ISR_D#phi2", "P");

  metdyn->GetPlot(s.c_str())->legend()->Draw();

  string TITLEOUT("PT_DENSITY_DPHI1");
  TITLEOUT = TITLEOUT + "" + label;
  Write(TITLEOUT);


}


char* Title(string name){

  string title = ""; title = Resonance + " ; " + name + " ; Npart";
  return title.c_str();

}

TH1D* Project(TH2F* Proj, double norm = 10000){
  
  TH1D* ProjWeighted_Norm = Proj->ProjectionX();
  //ProjWeighted_Norm->Draw();
  //  return ProjWeighted_Norm;
  
  TH1D* ProjWeighted = Proj->ProjectionX();
  TH1D* ProjWeighted_Y = Proj->ProjectionY();

  ProjWeighted->Scale(0);
  for (int i = 1; i < Proj->GetNbinsX()+1; i++){
    for (int j = 1; j < Proj->GetNbinsY()+1; j++){
      double binContent = Proj->GetBinContent(i, j)*ProjWeighted_Y->GetBinCenter(j);
      ProjWeighted->SetBinContent(i, ProjWeighted->GetBinContent(i) + binContent);
    }
    ProjWeighted->SetBinContent(i, ProjWeighted->GetBinContent(i)/norm);
  }

  cout << "Integral total Pt = " << ProjWeighted->Integral() << endl;

  ProjWeighted->Draw();

  return ProjWeighted;
  
}











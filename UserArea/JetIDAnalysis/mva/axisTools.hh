std::vector<std::string> fMap;
std::vector<std::string> fXAxis;
std::vector<std::string> fYAxis;
std::vector<int>         fNBins;
std::vector<double>      fXMin;
std::vector<double>      fXMax;

void loadfMap() {
  fMap.push_back("nvtx");        fNBins.push_back(60); fXMin.push_back(-0.5); fXMax.push_back(59.5);   fXAxis.push_back("# pv");                       fYAxis.push_back("Events");
  fMap.push_back("jmva");        fNBins.push_back(50); fXMin.push_back(-1);    fXMax.push_back(1);    fXAxis.push_back("leading jet mva");             fYAxis.push_back("Events/0.05");
  fMap.push_back("jetPt");       fNBins.push_back(90); fXMin.push_back(10);    fXMax.push_back(100);  fXAxis.push_back("p_{T} leading Jet (GeV/c)");   fYAxis.push_back("Events/10 GeV/c");
  fMap.push_back("jetEta");      fNBins.push_back(31); fXMin.push_back(-5.);   fXMax.push_back(5.);   fXAxis.push_back("#eta^{J}");                    fYAxis.push_back("Events/0.3");
  fMap.push_back("jetPhi");      fNBins.push_back(20); fXMin.push_back(-3.15); fXMax.push_back(3.15); fXAxis.push_back("leading jet #phi");            fYAxis.push_back("Events/0.32");
  fMap.push_back("dZ");          fNBins.push_back(25); fXMin.push_back(-0.50); fXMax.push_back(0.50); fXAxis.push_back("d^{jet}_{Z}(cm)");             fYAxis.push_back("Events/0.002");
  fMap.push_back("d0");          fNBins.push_back(25); fXMin.push_back(-0.10); fXMax.push_back(0.10); fXAxis.push_back("d^{jet}_{0}(cm)");             fYAxis.push_back("Events/0.002");
  fMap.push_back("nParticles");  fNBins.push_back(50); fXMin.push_back(-0.5);  fXMax.push_back(49.5); fXAxis.push_back("jet n particles");             fYAxis.push_back("Events");
  fMap.push_back("nCharged");    fNBins.push_back(50); fXMin.push_back(-0.5);  fXMax.push_back(49.5); fXAxis.push_back("jet n charged");               fYAxis.push_back("Events");
  fMap.push_back("nNeutrals");   fNBins.push_back(50); fXMin.push_back(-0.5);  fXMax.push_back(49.5); fXAxis.push_back("jet n neutral");               fYAxis.push_back("Events");
  fMap.push_back("dRLeadCent");  fNBins.push_back(30); fXMin.push_back(-0);    fXMax.push_back(0.5);  fXAxis.push_back("<#Delta R lead to Jet>");      fYAxis.push_back("Events");
  fMap.push_back("dRLead2nd");   fNBins.push_back(30); fXMin.push_back(-0);    fXMax.push_back(0.5);  fXAxis.push_back("<#Delta R lead to 2nd>");      fYAxis.push_back("Events");
  fMap.push_back("dRMean");      fNBins.push_back(30); fXMin.push_back(-0);    fXMax.push_back(0.5);  fXAxis.push_back("<#Delta R>");                  fYAxis.push_back("Events");
  fMap.push_back("dRMeanNeut");  fNBins.push_back(30); fXMin.push_back(-0);    fXMax.push_back(0.5);  fXAxis.push_back("<#Delta R neutral>");          fYAxis.push_back("Events");
  fMap.push_back("dRMeanEm");    fNBins.push_back(30); fXMin.push_back(-0);    fXMax.push_back(0.5);  fXAxis.push_back("<#Delta R em>");               fYAxis.push_back("Events");
  fMap.push_back("dRMeanCh");    fNBins.push_back(30); fXMin.push_back(-0);    fXMax.push_back(0.5);  fXAxis.push_back("<#Delta R charge>");           fYAxis.push_back("Events");
  fMap.push_back("leadPt");      fNBins.push_back(50); fXMin.push_back(-0.5);  fXMax.push_back(49.5); fXAxis.push_back("leading cand p_{T} (GeV)");    fYAxis.push_back("Events");
  fMap.push_back("secondPt");    fNBins.push_back(50); fXMin.push_back(-0.5);  fXMax.push_back(49.5); fXAxis.push_back("2nd leading cand p_{T}(GeV)"); fYAxis.push_back("Events");
  fMap.push_back("leadNeutPt");  fNBins.push_back(50); fXMin.push_back(-0.5);  fXMax.push_back(49.5); fXAxis.push_back("leading neu cand p_{T}(GeV)"); fYAxis.push_back("Events");
  fMap.push_back("leadEmPt");    fNBins.push_back(50); fXMin.push_back(-0.5);  fXMax.push_back(49.5); fXAxis.push_back("leading em cand p_{T} (GeV)"); fYAxis.push_back("Events");
  fMap.push_back("leadChPt");    fNBins.push_back(50); fXMin.push_back(-0.5);  fXMax.push_back(49.5); fXAxis.push_back("leading ch cand p_{T} (GeV)"); fYAxis.push_back("Events");
  fMap.push_back("leadEta");     fNBins.push_back(31); fXMin.push_back(-5.);   fXMax.push_back(5.);   fXAxis.push_back("#eta leading cand");           fYAxis.push_back("Events/0.3");  
  fMap.push_back("secondEta");   fNBins.push_back(31); fXMin.push_back(-5.);   fXMax.push_back(5.);   fXAxis.push_back("#eta 2nd leading cand");       fYAxis.push_back("Events/0.3");  
  fMap.push_back("leadNeutEta"); fNBins.push_back(31); fXMin.push_back(-5.);   fXMax.push_back(5.);   fXAxis.push_back("#eta leading neut cand");      fYAxis.push_back("Events/0.3");  
  fMap.push_back("leadEmEta");   fNBins.push_back(31); fXMin.push_back(-5.);   fXMax.push_back(5.);   fXAxis.push_back("#eta leading em cand");        fYAxis.push_back("Events/0.3");  
  fMap.push_back("leadChEta");   fNBins.push_back(31); fXMin.push_back(-5.);   fXMax.push_back(5.);   fXAxis.push_back("#eta leading ch cand");        fYAxis.push_back("Events/0.3");  
  fMap.push_back("leadPhi");     fNBins.push_back(20); fXMin.push_back(-3.15); fXMax.push_back(3.15); fXAxis.push_back("leading #phi");                fYAxis.push_back("Events/0.32");
  fMap.push_back("secondPhi");   fNBins.push_back(20); fXMin.push_back(-3.15); fXMax.push_back(3.15); fXAxis.push_back(" 2nd leading #phi");           fYAxis.push_back("Events/0.32");
  fMap.push_back("leadNeutPhi"); fNBins.push_back(20); fXMin.push_back(-3.15); fXMax.push_back(3.15); fXAxis.push_back(" leading neutral #phi");       fYAxis.push_back("Events/0.32");
  fMap.push_back("leadEmPhi");   fNBins.push_back(20); fXMin.push_back(-3.15); fXMax.push_back(3.15); fXAxis.push_back(" leading em #phi");            fYAxis.push_back("Events/0.32");
  fMap.push_back("leadChPhi");   fNBins.push_back(20); fXMin.push_back(-3.15); fXMax.push_back(3.15); fXAxis.push_back(" leading ch #phi");            fYAxis.push_back("Events/0.32");
  fMap.push_back("jetM");        fNBins.push_back(30); fXMin.push_back(0);     fXMax.push_back(30);   fXAxis.push_back("mass leading Jet(GeV/c^{2})"); fYAxis.push_back("Events GeV/c^{2}");
  fMap.push_back("leadFrac");    fNBins.push_back(40); fXMin.push_back(0.);    fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of lead cand");      fYAxis.push_back("Events/0.02");  
  fMap.push_back("leadChFrac");  fNBins.push_back(40); fXMin.push_back(0.);    fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of lead ch cand");   fYAxis.push_back("Events/0.02");  
  fMap.push_back("leadEmFrac");  fNBins.push_back(40); fXMin.push_back(0.);    fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of lead em cand");   fYAxis.push_back("Events/0.02");  
  fMap.push_back("leadNeutFrac");   fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of lead neut cand"); fYAxis.push_back("Events/0.02");  
  fMap.push_back("secondFrac");     fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of 2nd cand");      fYAxis.push_back("Events/0.02");  
  fMap.push_back("secondChFrac");   fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of 2nd ch cand");   fYAxis.push_back("Events/0.02");  
  fMap.push_back("secondEmFrac");   fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of 2nd em cand");   fYAxis.push_back("Events/0.02");  
  fMap.push_back("secondNeutFrac"); fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of 2nd neut cand"); fYAxis.push_back("Events/0.02"); 
  fMap.push_back("thirdFrac");      fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of 3rd cand");      fYAxis.push_back("Events/0.02");  
  fMap.push_back("thirdChFrac");    fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of 3rd ch cand");   fYAxis.push_back("Events/0.02");  
  fMap.push_back("thirdEmFrac");    fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of 3rd em cand");   fYAxis.push_back("Events/0.02");  
  fMap.push_back("thirdNeutFrac");  fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of 3rd neut cand"); fYAxis.push_back("Events/0.02");  
  fMap.push_back("fourthFrac");     fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of 4th cand");      fYAxis.push_back("Events/0.02");  
  fMap.push_back("fourhtChFrac");   fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of 4th ch cand");   fYAxis.push_back("Events/0.02");  
  fMap.push_back("fourthEmFrac");   fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of 4th em cand");   fYAxis.push_back("Events/0.02");  
  fMap.push_back("fourhtNeutFrac"); fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("p_{T} frac of 4th neut cand"); fYAxis.push_back("Events/0.02");  
  fMap.push_back("ptD");            fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("p_{T} Discrim");               fYAxis.push_back("Events/0.02");  
  fMap.push_back("chgEMfrac");      fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.5);  fXAxis.push_back("chg em fraction");             fYAxis.push_back("Events/0.02");  
  fMap.push_back("chgHadrfrac");    fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.5);  fXAxis.push_back("chg neut fraction");           fYAxis.push_back("Events/0.02");  
  fMap.push_back("neuEMfrac");      fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.5);  fXAxis.push_back("neut em fraction");            fYAxis.push_back("Events/0.02");  
  fMap.push_back("neuHadrfrac");    fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.5);  fXAxis.push_back("neut neut fraction");          fYAxis.push_back("Events/0.02");  
  fMap.push_back("majW");           fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(0.5);  fXAxis.push_back("major axis width");            fYAxis.push_back("Events/0.02");  
  fMap.push_back("minW");           fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(0.5);  fXAxis.push_back("minor axis width");            fYAxis.push_back("Events/0.02");  
  fMap.push_back("phiW");           fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(0.5);  fXAxis.push_back("#phi width");                  fYAxis.push_back("Events/0.02");  
  fMap.push_back("etaW");           fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(0.5);  fXAxis.push_back("#eta width");                  fYAxis.push_back("Events/0.02");  
  fMap.push_back("jetW");           fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(0.5);  fXAxis.push_back("jet width");                   fYAxis.push_back("Events/0.0125");  
  fMap.push_back("chFrac01");       fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("chg frac #Delta R <0.1");      fYAxis.push_back("Events/0.02");  
  fMap.push_back("emFrac01");       fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("em frac #Delta R <0.1");       fYAxis.push_back("Events/0.02");  
  fMap.push_back("neutFrac01");     fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("neut frac #Delta R <0.1");     fYAxis.push_back("Events/0.02");  
  fMap.push_back("frac01");         fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("frac #Delta R <0.1");          fYAxis.push_back("Events/0.02");  
  fMap.push_back("chFrac02");       fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("chg frac 0.1 < #Delta R <0.2");  fYAxis.push_back("Events/0.02");  
  fMap.push_back("emFrac02");       fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("em frac 0.1 < #Delta R <0.2");   fYAxis.push_back("Events/0.02");  
  fMap.push_back("neutFrac02");     fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("neut frac 0.1 < #Delta R <0.2"); fYAxis.push_back("Events/0.02");  
  fMap.push_back("frac02");         fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("frac 0.1 <#Delta R <0.2");       fYAxis.push_back("Events/0.02");  
  fMap.push_back("chFrac03");       fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("chg frac 0.2 < #Delta R <0.3");  fYAxis.push_back("Events/0.02");  
  fMap.push_back("emFrac03");       fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("em frac 0.2 < #Delta R <0.3");   fYAxis.push_back("Events/0.02");  
  fMap.push_back("neutFrac03");     fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("neut frac 0.2 < #Delta R <0.3"); fYAxis.push_back("Events/0.02");  
  fMap.push_back("frac03");         fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("frac 0.2 <#Delta R <0.3");       fYAxis.push_back("Events/0.02");  
  fMap.push_back("chFrac04");       fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("chg frac 0.3 < #Delta R <0.4");  fYAxis.push_back("Events/0.02");  
  fMap.push_back("emFrac04");       fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("em frac 0.3 < #Delta R <0.4");   fYAxis.push_back("Events/0.02");  
  fMap.push_back("neutFrac04");     fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("neut frac 0.3 < #Delta R <0.4"); fYAxis.push_back("Events/0.02");  
  fMap.push_back("frac04");         fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("frac 0.3 <#Delta R <0.4");       fYAxis.push_back("Events/0.02");  
  fMap.push_back("chFrac05");       fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("chg frac 0.4 < #Delta R <0.5");  fYAxis.push_back("Events/0.02");  
  fMap.push_back("emFrac05");       fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("em frac 0.4 < #Delta R <0.5");   fYAxis.push_back("Events/0.02");  
  fMap.push_back("neutFrac05");     fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("neut frac 0.4 < #Delta R <0.5"); fYAxis.push_back("Events/0.02");  
  fMap.push_back("frac05");         fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("frac 0.4 <#Delta R <0.5");       fYAxis.push_back("Events/0.02");  
  fMap.push_back("beta");           fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(1.0);  fXAxis.push_back("#beta");                         fYAxis.push_back("Events/0.02");  
  fMap.push_back("betaStar");       fNBins.push_back(40); fXMin.push_back(0.); fXMax.push_back(3.0);  fXAxis.push_back("#beta*");                        fYAxis.push_back("Events/0.06");  
}

int getId(std::string iStr) { 
  for(unsigned int i0 = 0; i0 < fMap.size(); i0++) { 
    if(iStr == fMap[i0]) return i0;
  }
  return 0;
}
const char*       getXAxis(std::string iStr) { return fXAxis[getId(iStr)].c_str();}
const char*       getYAxis(std::string iStr) { return fYAxis[getId(iStr)].c_str();}
int         getNBins(std::string iStr) { return fNBins[getId(iStr)];}
double      getXMin (std::string iStr) { return fXMin [getId(iStr)];}
double      getXMax (std::string iStr) { return fXMax [getId(iStr)];}

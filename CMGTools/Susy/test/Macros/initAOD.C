{
  loadFWLite();
  TFile f("aod.root");
  Events.Print();
  
  Events.SetAlias("met", "recoGenMETs_genMetTrue__HLT");
  Events.SetAlias("mpt", "recoMETs_genMPT__HLT");
  Events.SetAlias("pfmet", "recoPFMETs_pfMet__HLT");

  Events.Draw("met.obj.et()");
  Events.SetLineColor(4);
  Events.Draw("met.obj.pt()","","same");
//   Events.SetLineColor(5);
//   Events.Draw("recoMETs_MPT__AODSIMMU.obj.pt()","","same");
  
}

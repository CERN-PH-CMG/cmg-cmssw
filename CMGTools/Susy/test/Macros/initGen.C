{
  loadFWLite();
  TFile f("gen.root");
  Events.Print();
  
  Events.SetAlias("met", "recoGenMETs_genMetTrue__AODSIMMU");
  Events.SetAlias("mpt", "recoMETs_MPT__AODSIMMU");
//   Events.SetAlias("pfmet", "recoPFMETs_pfMet__HLT");

  Events.Draw("met.obj.et()");
  Events.SetLineColor(4);
  Events.Draw("met.obj.pt()","","same");
//   Events.SetLineColor(5);
//   Events.Draw("recoMETs_MPT__AODSIMMU.obj.pt()","","same");
  
}

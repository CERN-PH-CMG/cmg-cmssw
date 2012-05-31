//-----------------------------------------------------------------------------
// Subsystem:   Ntuples
// Package:     TNMc1
// Description: TheNtupleMaker helper class for edm::Event
// Created:     Tue May  8 09:28:47 2012
// Author:      Sezen Sekmen      
//-----------------------------------------------------------------------------
#include "Ntuples/TNMc1/interface/edmEventHelperExtra.h"
//-----------------------------------------------------------------------------
using namespace std;
using namespace edm;
//-----------------------------------------------------------------------------
// This constructor is called once per job
EventHelperExtra::EventHelperExtra()
  : HelperFor<edm::Event>() {}
    
EventHelperExtra::~EventHelperExtra() {}

// -- Called once per event
void EventHelperExtra::analyzeEvent()
{

  double initval = -9999;

  // Get the jet collections::
  edm::Handle<std::vector<pat::Jet> > jets;
  event->getByLabel("selectedPatJets", jets);

  edm::Handle<std::vector<pat::Jet> > jetsCHS;
  event->getByLabel("selectedPatJetsCHS", jetsCHS);    

  edm::Handle<std::vector<pat::Jet> > jetsCHSpruned;
  event->getByLabel("selectedPatJetsCHSpruned", jetsCHSpruned);

  // dijet invariant mass:

  dijet_invmass_ = initval;
  dijetCHS_invmass_ = initval;
  dijetCHSpruned_invmass_ = initval;

  // for selectedPatJets:
  std::vector<const pat::Jet*> seljets15;
  for (unsigned int i=0; i<jets->size(); i++) {
    const pat::Jet* j = &((*jets)[i]);
    if (!(j->pt() > 15) ) continue;
    seljets15.push_back(j);
    if (seljets15.size() == 2) break;
  }

  if (seljets15.size() == 2) {
    dijet_invmass_ = sqrt(pow((seljets15[0]->energy() + seljets15[1]->energy()), 2) -
                          pow((seljets15[0]->px() + seljets15[1]->px()), 2) -
                          pow((seljets15[0]->py() + seljets15[1]->py()), 2) -
                          pow((seljets15[0]->pz() + seljets15[1]->pz()), 2)
                          );
  }

  // for selectedPatJetsCHS:
  std::vector<const pat::Jet*> seljetsCHS15;
  for (unsigned int i=0; i<jets->size(); i++) {
    const pat::Jet* j = &((*jets)[i]);
    if (!(j->pt() > 15) ) continue;
    seljetsCHS15.push_back(j);
    if (seljetsCHS15.size() == 2) break;
  }

  if (seljetsCHS15.size() == 2) {
    dijetCHS_invmass_ = sqrt(pow((seljetsCHS15[0]->energy() + seljetsCHS15[1]->energy()), 2) -
                             pow((seljetsCHS15[0]->px() + seljetsCHS15[1]->px()), 2) -
                             pow((seljetsCHS15[0]->py() + seljetsCHS15[1]->py()), 2) -
                             pow((seljetsCHS15[0]->pz() + seljetsCHS15[1]->pz()), 2)
                             );
  }

  // for selectedPatJetsCHSpruned:
  std::vector<const pat::Jet*> seljetsCHSp15;
  for (unsigned int i=0; i<jets->size(); i++) {
    const pat::Jet* j = &((*jets)[i]);
    if (!(j->pt() > 15) ) continue;
    seljetsCHSp15.push_back(j);
    if (seljetsCHSp15.size() == 2) break;
  }

  if (seljetsCHSp15.size() == 2) {
    dijetCHSpruned_invmass_ = sqrt(pow((seljetsCHSp15[0]->energy() + seljetsCHSp15[1]->energy()), 2) -
                                   pow((seljetsCHSp15[0]->px() + seljetsCHSp15[1]->px()), 2) -
                                   pow((seljetsCHSp15[0]->py() + seljetsCHSp15[1]->py()), 2) -
                                   pow((seljetsCHSp15[0]->pz() + seljetsCHSp15[1]->pz()), 2)
                                   );
  }

  // Finding the wide jets:

  // Variables:
  // Normal jets:
  // wide jet 1:
  wj1_pt_ = initval;
  wj1_eta_ = initval;
  wj1_phi_ = initval;
  wj1_energy_ = initval;
  wj1_mass_ = initval;
  wj1_nconst_ = initval;
  // wide jet 2:
  wj2_pt_ = initval;
  wj2_eta_ = initval;
  wj2_phi_ = initval;
  wj2_energy_ = initval;
  wj2_mass_ = initval;
  wj2_nconst_ = initval;
  // Number of common constituents
  wj1wj2_nconst_ = initval;
  wj1wj2_invmass_ = initval;
  // CHS jets:
  // wide jet 1:
  wj1CHS_pt_ = initval;
  wj1CHS_eta_ = initval;
  wj1CHS_phi_ = initval;
  wj1CHS_energy_ = initval;
  wj1CHS_mass_  = initval;
  wj1CHS_nconst_ = initval;
  // wide jet 2:
  wj2CHS_pt_ = initval;
  wj2CHS_eta_ = initval;
  wj2CHS_phi_ = initval;
  wj2CHS_energy_ = initval;
  wj2CHS_mass_ = initval;
  wj2CHS_nconst_ = initval;
  // Number of common constituents
  wj1wj2CHS_nconst_ = initval;
  wj1wj2CHS_invmass_ = initval;

  // Distance parameter:
  double rmax = 1.1;


  // Wide jets for non-CHS:
  
  // Select the jets with pt>30
  // and put those jets in a selected patjets vector
  
  std::vector<const pat::Jet*> seljets;
  
  for (unsigned int i=0; i<jets->size(); i++) {
    const pat::Jet* j = &((*jets)[i]);
    if (!(j->pt() > 30) ) continue;
    seljets.push_back(j);
  }
  
  // Reconstruct wide jets only if njets > 2:
  if (seljets.size() >= 2) {
    
    TLorentzVector j1;
    j1.SetPtEtaPhiE(seljets[0]->pt(), seljets[0]->eta(), 
		    seljets[0]->phi(), seljets[0]->energy());
    //cout << j1.Pt() << " " << j1.Eta() << " " << j1.Phi() << " " << j1.Energy() << endl;
    TLorentzVector j2;
    j2.SetPtEtaPhiE(seljets[1]->pt(), seljets[1]->eta(), 
		    seljets[1]->phi(), seljets[1]->energy());
    //cout << j2.Pt() << " " << j2.Eta() << " " << j2.Phi() << " " << j2.Energy() << endl;
    TLorentzVector wj1 = j1;
    TLorentzVector wj2 = j2;
    
    wj1_nconst_ = 0;
    wj2_nconst_ = 0;
    wj1wj2_nconst_ = 0;
    
    
    // Loop through the selected jets
    for (unsigned int i=2; i<seljets.size(); i++) {
      
      const pat::Jet* sj = seljets[i];
      if (!(fabs(sj->eta()) < 2.5) ) continue;
      // Loose jet ID:
      if (!(sj->neutralHadronEnergyFraction() < 0.99) ) continue;
      if (!(sj->neutralEmEnergyFraction() < 0.99) ) continue;
      if (!(sj->nConstituents() > 1) ) continue;
      if (fabs(sj->eta()) < 2.4) {
	if (!(sj->chargedHadronEnergyFraction() > 0) ) continue;
	if (!(sj->chargedMultiplicity() > 0) ) continue;
	if (!(sj->chargedEmEnergyFraction() < 0.99) ) continue;       
      }
      TLorentzVector j;
      j.SetPtEtaPhiE(sj->pt(), sj->eta(), sj->phi(), sj->energy());
      double dR1 = j.DeltaR(j1);
      double dR2 = j.DeltaR(j2);
      //cout << "dRs: " << dR1 << " " << dR2 << endl;
      if (dR1 < rmax && dR1 < dR2) {
	wj1 += j;
	wj1_nconst_++;
	//cout << "1" << wj1.Pt() << " " << wj1.Eta() << " " 
	//   << wj1.Phi() << " " << wj1.Energy() << endl;
      }
      if (dR2 < rmax && dR2 <= dR1) {
	wj2 += j;
	wj2_nconst_++;
	//cout << "2" << wj2.Pt() << " " << wj2.Eta() << " " 
	//   << wj2.Phi() << " " << wj2.Energy() << endl;
      }
      if (dR2 < rmax && dR1 < rmax) {
	wj1wj2_nconst_++;
      }
      
    }
    
    // wide jet 1:
    wj1_pt_ = wj1.Pt();
    wj1_eta_ = wj1.Eta();
    wj1_phi_ = wj1.Phi();
    wj1_energy_ = wj1.Energy();
    wj1_mass_ = wj1.M();
    // wide jet 2:
    wj2_pt_ = wj2.Pt();
    wj2_eta_ = wj2.Eta();
    wj2_phi_ = wj2.Phi();
    wj2_energy_ = wj2.Energy();
    wj2_mass_ = wj2.M();

    // di-wide jet invariant mass:
    wj1wj2_invmass_ = sqrt(pow((wj1.Energy() + wj2.Energy()), 2) -
                            pow((wj1.Px() + wj2.Px()), 2) -
                            pow((wj1.Py() + wj2.Py()), 2) -
                            pow((wj1.Pz() + wj2.Pz()), 2)
                            );

    /*    
    cout << "wide jet 1: pt, eta, phi, energy, mass, nconst" << endl;
    cout << wj1_pt << " " <<wj1_eta << " " << wj1_phi << " " 
	 << wj1_energy << " " << wj1_mass << " " << wj1_nconst << endl;
    
    cout << "wide jet 2: pt, eta, phi, energy, mass, nconst" << endl;
    cout << wj2_pt << " " << wj2_eta << " " << wj2_phi << " "
	 << wj2_energy << " " << wj2_mass << " " << wj2_nconst << endl;
    cout << "Number of common constituents: " << wj1wj2_nconst << endl;
    */
    
  }



  // Wide jets for CHS jets:
  
  // Select the jets with pt>30
  // and put those jets in a selected patjets vector
  
  std::vector<const pat::Jet*> seljetsCHS;
  
  for (unsigned int i=0; i<jetsCHS->size(); i++) {
    const pat::Jet* j = &((*jetsCHS)[i]);
    if (!(j->pt() > 30) ) continue;
    seljetsCHS.push_back(j);
  }
  
  // Reconstruct wide jets only if njets > 2:
  if (seljetsCHS.size() >= 2) {
    
    TLorentzVector j1;
    j1.SetPtEtaPhiE(seljetsCHS[0]->pt(), seljetsCHS[0]->eta(), 
		    seljetsCHS[0]->phi(), seljetsCHS[0]->energy());
    //cout << j1.Pt() << " " << j1.Eta() << " " << j1.Phi() << " " << j1.Energy() << endl;
    TLorentzVector j2;
    j2.SetPtEtaPhiE(seljetsCHS[1]->pt(), seljetsCHS[1]->eta(), 
		    seljetsCHS[1]->phi(), seljetsCHS[1]->energy());
    //cout << j2.Pt() << " " << j2.Eta() << " " << j2.Phi() << " " << j2.Energy() << endl;
    TLorentzVector wj1 = j1;
    TLorentzVector wj2 = j2;
    
    wj1CHS_nconst_ = 0;
    wj2CHS_nconst_ = 0;
    wj1wj2CHS_nconst_ = 0;
    
    
    // Loop through the selected jets
    for (unsigned int i=2; i<seljetsCHS.size(); i++) {
      
      const pat::Jet* sj = seljetsCHS[i];
      if (!(fabs(sj->eta()) < 2.5) ) continue;
      // Loose jet ID:
      if (!(sj->neutralHadronEnergyFraction() < 0.99) ) continue;
      if (!(sj->neutralEmEnergyFraction() < 0.99) ) continue;
      if (!(sj->nConstituents() > 1) ) continue;
      if (fabs(sj->eta()) < 2.4) {
	if (!(sj->chargedHadronEnergyFraction() > 0) ) continue;
	if (!(sj->chargedMultiplicity() > 0) ) continue;
	if (!(sj->chargedEmEnergyFraction() < 0.99) ) continue;       
      }
      TLorentzVector j;
      j.SetPtEtaPhiE(sj->pt(), sj->eta(), sj->phi(), sj->energy());
      double dR1 = j.DeltaR(j1);
      double dR2 = j.DeltaR(j2);
      //cout << "dRs: " << dR1 << " " << dR2 << endl;
      if (dR1 < rmax && dR1 < dR2) {
	wj1 += j;
	wj1CHS_nconst_++;
	//cout << "1" << wj1.Pt() << " " << wj1.Eta() << " " 
	//   << wj1.Phi() << " " << wj1.Energy() << endl;
      }
      if (dR2 < rmax && dR2 <= dR1) {
	wj2 += j;
	wj2CHS_nconst_++;
	//cout << "2" << wj2.Pt() << " " << wj2.Eta() << " " 
	//   << wj2.Phi() << " " << wj2.Energy() << endl;
      }
      if (dR2 < rmax && dR1 < rmax) {
	wj1wj2CHS_nconst_++;
      }
      
    }
    
    // wide jet 1:
    wj1CHS_pt_ = wj1.Pt();
    wj1CHS_eta_ = wj1.Eta();
    wj1CHS_phi_ = wj1.Phi();
    wj1CHS_energy_ = wj1.Energy();
    wj1CHS_mass_ = wj1.M();
    // wide jet 2:
    wj2CHS_pt_ = wj2.Pt();
    wj2CHS_eta_ = wj2.Eta();
    wj2CHS_phi_ = wj2.Phi();
    wj2CHS_energy_ = wj2.Energy();
    wj2CHS_mass_ = wj2.M();

    // di-wide jet invariant mass:
    wj1wj2CHS_invmass_ = sqrt(pow((wj1.Energy() + wj2.Energy()), 2) -
                               pow((wj1.Px() + wj2.Px()), 2) -
                               pow((wj1.Py() + wj2.Py()), 2) -
                               pow((wj1.Pz() + wj2.Pz()), 2)
                               );

    /*    
    cout << "wide jet 1: pt, eta, phi, energy, mass, nconst" << endl;
    cout << wj1_pt << " " <<wj1_eta << " " << wj1_phi << " " 
	 << wj1_energy << " " << wj1_mass << " " << wj1_nconst << endl;
    
    cout << "wide jet 2: pt, eta, phi, energy, mass, nconst" << endl;
    cout << wj2_pt << " " << wj2_eta << " " << wj2_phi << " "
	 << wj2_energy << " " << wj2_mass << " " << wj2_nconst << endl;
    cout << "Number of common constituents: " << wj1wj2_nconst << endl;
    */
    
  }

  
  
}

// -- Called once per object
//void EventHelperExtra::analyzeObject()
//{
//
//}

// -- User access methods
//double EventHelperExtra::someMethod()  const
//{
//  return  //-- some-value --
//}
// dijet invariant mass:
double EventHelperExtra::dijet_invmass() const {
  return dijet_invmass_;
}
double EventHelperExtra::dijetCHS_invmass() const {
  return dijetCHS_invmass_;
}
double EventHelperExtra::dijetCHSpruned_invmass() const {
  return dijetCHSpruned_invmass_;
}

// Non-CHS jets:
// wide jet 1
double EventHelperExtra::wj1_pt() const {
  return wj1_pt_;
}
double EventHelperExtra::wj1_eta() const {
  return wj1_eta_;
}
double EventHelperExtra::wj1_phi() const {
  return wj1_phi_;
}
double EventHelperExtra::wj1_energy() const {
  return wj1_energy_;
}
double EventHelperExtra::wj1_mass() const {
  return wj1_mass_;
}
int EventHelperExtra::wj1_nconst() const {
  return wj1_nconst_;
}
// wide jet 2:
double EventHelperExtra::wj2_pt() const {
  return wj2_pt_;
}
double EventHelperExtra::wj2_eta() const {
  return wj2_eta_;
}
double EventHelperExtra::wj2_phi() const {
  return wj2_phi_;
}
double EventHelperExtra::wj2_energy() const {
  return wj2_energy_;
}
double EventHelperExtra::wj2_mass() const {
  return wj2_mass_;
}
int EventHelperExtra::wj2_nconst() const {
  return wj2_nconst_;
}
// Number of common constituents:
int EventHelperExtra::wj1wj2_nconst() const {
  return wj1wj2_nconst_;
}
double EventHelperExtra::wj1wj2_invmass() const {
  return wj1wj2_invmass_;
}
// CHS jets:
// wide jet 1
double EventHelperExtra::wj1CHS_pt() const {
  return wj1CHS_pt_;
}
double EventHelperExtra::wj1CHS_eta() const {
  return wj1CHS_eta_;
}
double EventHelperExtra::wj1CHS_phi() const {
  return wj1CHS_phi_;
}
double EventHelperExtra::wj1CHS_energy() const {
  return wj1CHS_energy_;
}
double EventHelperExtra::wj1CHS_mass() const {
  return wj1CHS_mass_;
}
int EventHelperExtra::wj1CHS_nconst() const {
  return wj1CHS_nconst_;
}
// wide jet 2:
double EventHelperExtra::wj2CHS_pt() const {
  return wj2CHS_pt_;
}
double EventHelperExtra::wj2CHS_eta() const {
  return wj2CHS_eta_;
}
double EventHelperExtra::wj2CHS_phi() const {
  return wj2CHS_phi_;
}
double EventHelperExtra::wj2CHS_energy() const {
  return wj2CHS_energy_;
}
double EventHelperExtra::wj2CHS_mass() const {
  return wj2CHS_mass_;
}
int EventHelperExtra::wj2CHS_nconst() const {
  return wj2CHS_nconst_;
}
// Number of common constituents:
int EventHelperExtra::wj1wj2CHS_nconst() const {
  return wj1wj2CHS_nconst_;
}
double EventHelperExtra::wj1wj2CHS_invmass() const {
  return wj1wj2CHS_invmass_;
}


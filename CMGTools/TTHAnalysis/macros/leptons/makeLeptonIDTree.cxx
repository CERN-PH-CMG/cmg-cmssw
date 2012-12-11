#include <TTree.h>
#include <TFile.h>
#include <math.h>

void makeLeptonIDTree() {
    Int_t pdgId[6], charge[6], mcMatchId[6], mcId[6], nGenLeps, nGenLepsFromTau, nGoodLeps;
    Double_t pt[6], eta[6], phi[6], sip3d[6], dxy[6], dz[6], relIso[6], jetPtRatio[6], jetBTagCSV[6], ptRelJet[6], jetDR[6];
    Double_t genpt[6], geneta[6], genphi[6], gentaupt[6], gentaueta[6], gentauphi[6];

    TTree *in = (TTree *) gFile->Get("ttHLepTreeProducerBase");
    in->SetBranchAddress("LepGood1_pt",&pt[0]);
    in->SetBranchAddress("LepGood1_eta",&eta[0]);
    in->SetBranchAddress("LepGood1_phi",&phi[0]);
    in->SetBranchAddress("LepGood1_sip3d",&sip3d[0]);
    in->SetBranchAddress("LepGood1_dxy",&dxy[0]);
    in->SetBranchAddress("LepGood1_dz",&dz[0]);
    in->SetBranchAddress("LepGood1_relIso",&relIso[0]);
    in->SetBranchAddress("LepGood1_jetPtRatio",&jetPtRatio[0]);
    in->SetBranchAddress("LepGood1_jetDR",&jetDR[0]);
    in->SetBranchAddress("LepGood1_jetBTagCSV",&jetBTagCSV[0]);
    in->SetBranchAddress("LepGood1_ptRelJet",&ptRelJet[0]);
    in->SetBranchAddress("LepGood1_pdgId",&pdgId[0]);
    in->SetBranchAddress("LepGood1_charge",&charge[0]);
    in->SetBranchAddress("LepGood1_mcMatchId",&mcMatchId[0]);
    in->SetBranchAddress("LepGood2_pt",&pt[1]);
    in->SetBranchAddress("LepGood2_eta",&eta[1]);
    in->SetBranchAddress("LepGood2_phi",&phi[1]);
    in->SetBranchAddress("LepGood2_sip3d",&sip3d[1]);
    in->SetBranchAddress("LepGood2_dxy",&dxy[1]);
    in->SetBranchAddress("LepGood2_dz",&dz[1]);
    in->SetBranchAddress("LepGood2_relIso",&relIso[1]);
    in->SetBranchAddress("LepGood2_jetPtRatio",&jetPtRatio[1]);
    in->SetBranchAddress("LepGood2_jetDR",&jetDR[1]);
    in->SetBranchAddress("LepGood2_jetBTagCSV",&jetBTagCSV[1]);
    in->SetBranchAddress("LepGood2_ptRelJet",&ptRelJet[1]);
    in->SetBranchAddress("LepGood2_pdgId",&pdgId[1]);
    in->SetBranchAddress("LepGood2_charge",&charge[1]);
    in->SetBranchAddress("LepGood2_mcMatchId",&mcMatchId[1]);
    in->SetBranchAddress("LepGood3_pt",&pt[2]);
    in->SetBranchAddress("LepGood3_eta",&eta[2]);
    in->SetBranchAddress("LepGood3_phi",&phi[2]);
    in->SetBranchAddress("LepGood3_sip3d",&sip3d[2]);
    in->SetBranchAddress("LepGood3_dxy",&dxy[2]);
    in->SetBranchAddress("LepGood3_dz",&dz[2]);
    in->SetBranchAddress("LepGood3_relIso",&relIso[2]);
    in->SetBranchAddress("LepGood3_jetPtRatio",&jetPtRatio[2]);
    in->SetBranchAddress("LepGood3_jetDR",&jetDR[2]);
    in->SetBranchAddress("LepGood3_jetBTagCSV",&jetBTagCSV[2]);
    in->SetBranchAddress("LepGood3_ptRelJet",&ptRelJet[2]);
    in->SetBranchAddress("LepGood3_pdgId",&pdgId[2]);
    in->SetBranchAddress("LepGood3_charge",&charge[2]);
    in->SetBranchAddress("LepGood3_mcMatchId",&mcMatchId[2]);
    in->SetBranchAddress("LepGood4_pt",&pt[3]);
    in->SetBranchAddress("LepGood4_eta",&eta[3]);
    in->SetBranchAddress("LepGood4_phi",&phi[3]);
    in->SetBranchAddress("LepGood4_sip3d",&sip3d[3]);
    in->SetBranchAddress("LepGood4_dxy",&dxy[3]);
    in->SetBranchAddress("LepGood4_dz",&dz[3]);
    in->SetBranchAddress("LepGood4_relIso",&relIso[3]);
    in->SetBranchAddress("LepGood4_jetPtRatio",&jetPtRatio[3]);
    in->SetBranchAddress("LepGood4_jetDR",&jetDR[3]);
    in->SetBranchAddress("LepGood4_jetBTagCSV",&jetBTagCSV[3]);
    in->SetBranchAddress("LepGood4_ptRelJet",&ptRelJet[3]);
    in->SetBranchAddress("LepGood4_pdgId",&pdgId[3]);
    in->SetBranchAddress("LepGood4_charge",&charge[3]);
    in->SetBranchAddress("LepGood4_mcMatchId",&mcMatchId[3]);


    in->SetBranchAddress("GenLep1_pt",&genpt[0]);
    in->SetBranchAddress("GenLep1_eta",&geneta[0]);
    in->SetBranchAddress("GenLep1_phi",&genphi[0]);
    in->SetBranchAddress("GenLepFromTau1_pt",&gentaupt[0]);
    in->SetBranchAddress("GenLepFromTau1_eta",&gentaueta[0]);
    in->SetBranchAddress("GenLepFromTau1_phi",&gentauphi[0]);
    in->SetBranchAddress("GenLep2_pt",&genpt[1]);
    in->SetBranchAddress("GenLep2_eta",&geneta[1]);
    in->SetBranchAddress("GenLep2_phi",&genphi[1]);
    in->SetBranchAddress("GenLepFromTau2_pt",&gentaupt[1]);
    in->SetBranchAddress("GenLepFromTau2_eta",&gentaueta[1]);
    in->SetBranchAddress("GenLepFromTau2_phi",&gentauphi[1]);
    in->SetBranchAddress("GenLep3_pt",&genpt[2]);
    in->SetBranchAddress("GenLep3_eta",&geneta[2]);
    in->SetBranchAddress("GenLep3_phi",&genphi[2]);
    in->SetBranchAddress("GenLepFromTau3_pt",&gentaupt[2]);
    in->SetBranchAddress("GenLepFromTau3_eta",&gentaueta[2]);
    in->SetBranchAddress("GenLepFromTau3_phi",&gentauphi[2]);
    in->SetBranchAddress("GenLep4_pt",&genpt[3]);
    in->SetBranchAddress("GenLep4_eta",&geneta[3]);
    in->SetBranchAddress("GenLep4_phi",&genphi[3]);
    in->SetBranchAddress("GenLepFromTau4_pt",&gentaupt[3]);
    in->SetBranchAddress("GenLepFromTau4_eta",&gentaueta[3]);
    in->SetBranchAddress("GenLepFromTau4_phi",&gentauphi[3]);

    in->SetBranchAddress("nLepGood", &nGoodLeps);
    in->SetBranchAddress("nGenLeps", &nGenLeps);
    in->SetBranchAddress("nGenLepsFromTau", &nGenLepsFromTau);

    TFile *fOut = new TFile("leptonTree.root","RECREATE");
    TTree *out = new TTree("leptonTree","");
    out->Branch("pt",&pt[0],'D');
    out->Branch("eta",&eta[0],'D');
    out->Branch("sip3d",&sip3d[0],'D');
    out->Branch("dxy",&dxy[0],'D');
    out->Branch("dz",&dz[0],'D');
    out->Branch("relIso",&relIso[0],'D');
    out->Branch("jetPtRatio",&jetPtRatio[0],'D');
    out->Branch("jetBTagCSV",&jetBTagCSV[0],'D');
    out->Branch("ptRelJet",&ptRelJet[0],'D');
    out->Branch("jetDR",&jetDR[0],'D');
    out->Branch("pdgId",&pdgId[0],'I');
    out->Branch("charge",&charge[0],'I');
    out->Branch("mcId",&mcId[0],'I');
   
    for (unsigned int i = 0, n = in->GetEntries(); i < n; ++i) {
        in->GetEntry(i);
        for (int j = 0; j < nGoodLeps; ++j) {
            mcId[0] = (mcMatchId[j] > 0);
            pt[0] = pt[j];
            eta[0] = eta[j];
            phi[0] = phi[j];
            sip3d[0] = sip3d[j];
            dxy[0] = dxy[j];
            dz[0] = dz[j];
            relIso[0] = relIso[j];
            jetPtRatio[0] = jetPtRatio[j];
            jetBTagCSV[0] = jetBTagCSV[j];
            jetDR[0] = jetDR[j];
            ptRelJet[0] = ptRelJet[j];
            pdgId[0] = pdgId[j];
            charge[0] = charge[j];
            if (mcId[0] > 0) {
                for (int k = 0; k < nGenLeps; ++k) {
                    double dphi = fabs(phi[0] - genphi[k]);
                    if (dphi > M_PI) dphi = 2*M_PI-dphi;
                    double dr = hypot(dphi, eta[0] - geneta[k]);
                    if (dr < 0.3) { mcId[0] = 2; break; }
                }
            }
            out->Fill();
        }
    }
    out->Write();  fOut->Close();
}

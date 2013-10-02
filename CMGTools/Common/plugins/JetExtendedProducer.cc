// system include files
#include <memory>
#include <iostream>
#include <string>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "TMath.h"
#include "TLorentzVector.h"
#define GENPTOLOR(a) TLorentzVector((a).px(), (a).py(), (a).pz(), (a).energy())

class JetExtendedProducer : public edm::EDProducer {
   public:
      explicit JetExtendedProducer(const edm::ParameterSet&);
      virtual ~JetExtendedProducer();

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data --------------------------
      edm::InputTag src_,vtx_;
      std::string payload_;
      bool isJecUncSet_,debug_;
      JetCorrectionUncertainty *jecUnc_;
};

JetExtendedProducer::JetExtendedProducer(const edm::ParameterSet& iConfig)
{
  src_     = iConfig.getParameter<edm::InputTag> ("jets");
  vtx_     = iConfig.getParameter<edm::InputTag> ("vertices");
  payload_ = iConfig.getParameter<std::string>   ("payload");
  debug_   = iConfig.getUntrackedParameter<bool> ("debug",false);

  produces<pat::JetCollection>();
  isJecUncSet_ = false;
}

JetExtendedProducer::~JetExtendedProducer()
{

}

// ------------ method called to produce the data  ------------
void JetExtendedProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace reco;
  
  edm::Handle<edm::View<pat::Jet> > jets;
  iEvent.getByLabel(src_,jets);
  edm::View<pat::Jet> pat_jets = *jets;  

  std::auto_ptr<pat::JetCollection> result (new pat::JetCollection); //Extended jets
  const int size = pat_jets.size();
  result->reserve(size);

  //-------------- Vertex Info -----------------------------------
  Handle<reco::VertexCollection> recVtxs;
  iEvent.getByLabel(vtx_,recVtxs);

  //-------------- Set the JEC uncertainty object ----------------
  if (!isJecUncSet_ && payload_ != "") {
    if (debug_) {
      std::cout<<"Initializing the JEC uncertainty object"<<std::endl;
    }
    edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
    iSetup.get<JetCorrectionsRecord>().get(payload_,JetCorParColl); 
    JetCorrectorParameters const& par = (*JetCorParColl)["Uncertainty"];
    jecUnc_ = new JetCorrectionUncertainty(par);
    isJecUncSet_ = true;
  }
    
  unsigned int jetIdx = 0;
  for(edm::View<pat::Jet>::const_iterator ijet = pat_jets.begin(); ijet != pat_jets.end(); ++ijet) {
    //Secondary vertex infos
    int nConstituents = ijet->getPFConstituents().size();
    float vtxMass(0),vtx3dL(0),vtx3deL(0);
    int vtxNTracks(0);
    TLorentzVector vtxP4;
    const reco::SecondaryVertexTagInfo * tf = ijet->tagInfoSecondaryVertex();
    if (tf) {
      math::XYZTLorentzVectorD vertexSum;
      for(size_t vi=0;vi<tf->nVertices();vi++) {
        vertexSum+=tf->secondaryVertex(vi).p4();
      }
      if (tf->nVertices() > 0) {
	vtxP4 = GENPTOLOR(vertexSum);
	vtxMass =  tf->secondaryVertex(0).p4().mass();
	vtxNTracks = tf->secondaryVertex(0).nTracks();
		
	Measurement1D m = tf->flightDistance(0);
	vtx3dL = m.value();
	vtx3deL = m.error();
      }
    }

    //----- calculations based on the constituents -----
    std::vector<PFCandidatePtr> pfConst(ijet->getPFConstituents());
    int n_pf = pfConst.size();                                                                                          
    float phiJet = ijet->phi();                                                                                         
    float etaJet = ijet->eta();                                                                                         
    float deta,dphi,dR,weight,weight2,sumW(0.0),sumW2(0.0),sum_deta(0.0),sum_dphi(0.0),sum_deta2(0.0),sum_dphi2(0.0),sum_detadphi(0.0);              
    float Teta(0),Tphi(0),Ttheta(-9),jetPtMax(0),axis1(-999),axis2(-999),tana(-999),ptD(-999),ptD_QC(-999);                      
    
    float sumW_QC(0.0),sumW2_QC(0.0),sum_deta_QC(0.0),sum_dphi_QC(0.0),sum_deta2_QC(0.0),sum_dphi2_QC(0.0),sum_detadphi_QC(0.0);
    float axis1_QC(-999),axis2_QC(-999);   

    float ave_deta(0.0),ave_dphi(0.0),ave_deta2(0.0),ave_dphi2(0.0);
    float ave_deta_QC(0.0),ave_dphi_QC(0.0),ave_deta2_QC(0.0),ave_dphi2_QC(0.0);
    float pull(0.0),pull_QC(0.0);

    float pTMax(0.0),pTMaxChg(0.0),pTMaxNeutral(0.0),pTMaxChg_QC(0.0);
    int nChg_QC(0),nChg_ptCut(0),nChg_ptCut_QC(0),nNeutral_ptCut(0);
    std::vector<float> jetPart_pt,jetPart_deta,jetPart_dphi;
    std::vector<bool> jetPart_forMult,jetPart_forAxis;
     
    for(int j=0;j<n_pf;j++) {                                                                 
      reco::TrackRef itrk ;
      reco::PFCandidatePtr  part = ijet->getPFConstituent(j);
      if (part.isNonnull())
        itrk = (*part).trackRef();
      if (part->pt() > pTMax) 
        pTMax = part->pt();
      if (itrk.isNonnull() && part->pt() > pTMaxChg) 
        pTMaxChg = part->pt();
      if (!itrk.isNonnull() && part->pt() > pTMaxNeutral) 
        pTMaxNeutral = part->pt();
      if (!itrk.isNonnull() && part->pt() > 1.0) 
        nNeutral_ptCut++;
 
      bool trkForAxis = false;
      bool trkForMult = false;

      //-----matching with vertex tracks-------
      if (!itrk.isNonnull()) { 
        trkForMult = true;
        trkForAxis = true;
      }
      else {
        if (part->pt() > 1.0)
          nChg_ptCut++;
        float dZmin = 999;
        int index_min = 999;
        reco::VertexCollection::const_iterator vtxClose;
        for(unsigned ivtx = 0;ivtx < recVtxs->size();ivtx++) {
          float dZ_cut = fabs(itrk->dz((*recVtxs)[ivtx].position()));
          float sumpT = 0;
          for(reco::Vertex::trackRef_iterator itk = (*recVtxs)[ivtx].tracks_begin();itk!=(*recVtxs)[ivtx].tracks_end(); ++itk) {
            sumpT = sumpT + ((*itk)->pt())*((*itk)->pt());
          }
          if (dZ_cut < dZmin) {
            dZmin = dZ_cut;
            index_min = ivtx;
              //  std::cout<<"dz=="<<dZ_cut<<std::endl;
          }
        }//Loop over vertices 
        if (index_min == 0) {
          float dz = itrk->dz((*recVtxs)[0].position());
          float d0 = itrk->dxy((*recVtxs)[0].position());
          float vtx_xError = (*recVtxs)[0].xError();
          float vtx_yError = (*recVtxs)[0].yError();
          float vtx_zError = (*recVtxs)[0].zError();
          float d0_sigma=sqrt(pow(itrk->d0Error(),2) + pow(vtx_xError,2) + pow(vtx_yError,2));
          float dz_sigma=sqrt(pow(itrk->dzError(),2) + pow(vtx_zError,2));
          if (itrk->quality(reco::TrackBase::qualityByName("highPurity")) && fabs(dz/dz_sigma) < 5.) {
            trkForAxis = true;
            if (fabs(d0/d0_sigma) < 5.)
              trkForMult = true;
          }//
        }
        if (trkForMult)
          nChg_QC++;
        if (itrk.isNonnull() && trkForMult && part->pt() > 1.0)
          nChg_ptCut_QC++;
        if (part->pt() > pTMaxChg_QC && trkForAxis) 
          pTMaxChg_QC = part->pt();
      }// for charged particles only

      //-----------Store part info-----------------------
      jetPart_pt.push_back(pfConst[j]->pt());
      jetPart_forMult.push_back(trkForMult);
      jetPart_forAxis.push_back(trkForAxis);
  
      deta = pfConst[j]->eta() - etaJet;                                                                                               
      dphi = 2*atan(tan((pfConst[j]->phi()-phiJet)/2));                                                                              
      dR = sqrt(deta*deta + dphi*dphi);                                                                                              
      weight = pfConst[j]->pt(); // used for the thrust and ptD variables
      weight2 = weight * weight; // used for the jet axis, ptD variables                                                          
      sumW += weight;
      sumW2 += weight2;                                                                                                   
      Teta += weight * dR * deta;
      Tphi += weight * dR * dphi;
      sum_deta += deta*weight2;
      sum_dphi += dphi*weight2;
      sum_deta2 += deta*deta*weight2;
      sum_detadphi += deta*dphi*weight2;
      sum_dphi2 += dphi*dphi*weight2;
      //-----Axis using charged tracks with quality cuts--- 
      if (trkForAxis) {
        sumW2_QC += weight2;
        sumW_QC += weight;
        sum_deta_QC += deta*weight2;
        sum_dphi_QC += dphi*weight2;
        sum_deta2_QC += deta*deta*weight2;
        sum_detadphi_QC += deta*dphi*weight2;
        sum_dphi2_QC += dphi*dphi*weight2;
      }
      jetPart_deta.push_back(deta);
      jetPart_dphi.push_back(dphi);
      //-----------------------------------      
      if (fabs(pfConst[j]->charge()) > 0) {
        jetPtMax = TMath::Max(jetPtMax,float(pfConst[j]->pt()));
      }
    }// loop over the constituents
    if (sumW > 0) {
      Teta = Teta/sumW;
      Tphi = Tphi/sumW;
      if (Teta != 0 && Tphi !=0 ) {
        Ttheta = atan2(Tphi,Teta);
      }
      ptD = sqrt(sumW2)/sumW;
      ave_deta = sum_deta/sumW2;
      ave_dphi = sum_dphi/sumW2;
      ave_deta2 = sum_deta2/sumW2;
      ave_dphi2 = sum_dphi2/sumW2;
      float a = ave_deta2-ave_deta*ave_deta;
      float b = ave_dphi2-ave_dphi*ave_dphi;
      float c = -(sum_detadphi/sumW2-ave_deta*ave_dphi);
      float delta = sqrt(fabs((a-b)*(a-b)+4*c*c));
      if (a+b+delta > 0) {
        axis1 = sqrt(0.5*(a+b+delta));
      }
      if (a+b-delta > 0) {  
        axis2 = sqrt(0.5*(a+b-delta));
      }
      if (c != 0) {
        tana = 0.5*(b-a+delta)/c;
      }
    }
    if (sumW_QC > 0) {
      ptD_QC = sqrt(sumW2_QC)/sumW_QC;
      ave_deta_QC = sum_deta_QC/sumW2_QC;
      ave_dphi_QC = sum_dphi_QC/sumW2_QC;
      ave_deta2_QC = sum_deta2_QC/sumW2_QC;
      ave_dphi2_QC = sum_dphi2_QC/sumW2_QC;

      float a_QC = ave_deta2_QC-ave_deta_QC*ave_deta_QC;
      float b_QC = ave_dphi2_QC-ave_dphi_QC*ave_dphi_QC;
      float c_QC = -(sum_detadphi_QC/sumW2_QC-ave_deta_QC*ave_dphi_QC);
      float delta_QC = sqrt(fabs((a_QC-b_QC)*(a_QC-b_QC)+4*c_QC*c_QC));

      if (a_QC+b_QC+delta_QC > 0) {
        axis1_QC = sqrt(0.5*(a_QC+b_QC+delta_QC));
      }
      if (a_QC+b_QC-delta_QC > 0) {
        axis2_QC = sqrt(0.5*(a_QC+b_QC-delta_QC));
      }
    }
    //-------calculate pull------
    float ddetaR_sum(0.0), ddphiR_sum(0.0),ddetaR_sum_QC(0.0), ddphiR_sum_QC(0.0);
    for(unsigned int i=0; i<jetPart_pt.size(); ++i) {
      float weight = jetPart_pt[i] * jetPart_pt[i];
      float ddeta, ddphi, ddeta_QC, ddphi_QC,ddR, ddR_QC;
      ddeta = jetPart_deta[i] - ave_deta ;
      ddphi = 2*atan(tan((jetPart_dphi[i] - ave_dphi)/2.)) ;
      ddR = sqrt(ddeta*ddeta + ddphi*ddphi);
      ddetaR_sum += ddR*ddeta*weight;
      ddphiR_sum += ddR*ddphi*weight;
      if (jetPart_forAxis[i]) {
        ddeta_QC = jetPart_deta[i] - ave_deta_QC ;
        ddphi_QC = 2*atan(tan((jetPart_dphi[i] - ave_dphi_QC)/2.)) ;
        ddR_QC = sqrt(ddeta_QC*ddeta_QC + ddphi_QC*ddphi_QC);
        ddetaR_sum_QC += ddR_QC *ddeta_QC *weight;
        ddphiR_sum_QC  += ddR_QC *ddphi_QC *weight;  
      }
    }//second loop over constituents  
    if (sumW2 > 0) {
      float ddetaR_ave = ddetaR_sum/sumW2;
      float ddphiR_ave = ddphiR_sum/sumW2;
      pull = sqrt(ddetaR_ave*ddetaR_ave+ddphiR_ave*ddphiR_ave);
    }
    if (sumW2_QC > 0) {
      float ddetaR_ave_QC = ddetaR_sum_QC/sumW2_QC;
      float ddphiR_ave_QC = ddphiR_sum_QC/sumW2_QC;
      pull_QC = sqrt(ddetaR_ave_QC*ddetaR_ave_QC+ddphiR_ave_QC*ddphiR_ave_QC);
    }
    int nChg  = ijet->chargedMultiplicity();
    int nNeutral  = ijet->neutralMultiplicity();
    float jetRchg = pTMaxChg/sumW;
    float jetRneutral = pTMaxNeutral/sumW;
    float jetR = pTMax/sumW;
    float jetRchg_QC = pTMaxChg_QC/sumW_QC;
   
    //---- vertex association -----------
    //---- get the vector of tracks -----
    const reco::PFJet& pfJet = dynamic_cast <const reco::PFJet&> (*(ijet->originalObject()));
    reco::TrackRefVector vTrks(pfJet.getTrackRefs());
    float sumTrkPt(0.0),sumTrkPtBeta(0.0),beta(0.0);
    float sumTrkPx(0.0),sumTrkPy(0.0),sumTrkP(0.0),leadTrkPt(0);
    //---- loop over the tracks of the jet ----
    for(reco::TrackRefVector::const_iterator i_trk = vTrks.begin(); i_trk != vTrks.end(); i_trk++) {
      sumTrkPt += (*i_trk)->pt();
      sumTrkPx+= (*i_trk)->px();
      sumTrkPy+= (*i_trk)->py();
      sumTrkP+=(*i_trk)->p();
      leadTrkPt=TMath::Max(leadTrkPt,float((*i_trk)->pt()));
      //---- loop over all vertices ----------------------------
      for(unsigned ivtx = 0;ivtx < recVtxs->size();ivtx++) {
        //---- loop over the tracks associated with the vertex ---
        for(reco::Vertex::trackRef_iterator i_vtxTrk = (*recVtxs)[ivtx].tracks_begin(); i_vtxTrk != (*recVtxs)[ivtx].tracks_end(); ++i_vtxTrk) {
          //---- match the jet track to the track from the vertex ----
          reco::TrackRef trkRef(i_vtxTrk->castTo<reco::TrackRef>());
          //---- check if the tracks match -------------------------
          if (trkRef == (*i_trk)) {
            if (ivtx > 0) {
              sumTrkPtBeta += (*i_trk)->pt();
            }   
            break;
          }
        }
      } 
    }
    if (sumTrkPt > 0) {
      beta = 1.-sumTrkPtBeta/sumTrkPt;  
    }
    pat::Jet extendedJet = *ijet; // copy original jet
    jecUnc_->setJetEta(ijet->eta());
    jecUnc_->setJetPt(ijet->pt());
    extendedJet.addUserFloat("jecUnc",jecUnc_->getUncertainty(true));
    extendedJet.addUserFloat("beta",beta);
    extendedJet.addUserFloat("ptMax",jetPtMax);
    extendedJet.addUserFloat("ptD",ptD);
    extendedJet.addUserFloat("ptD_QC",ptD_QC);
    extendedJet.addUserFloat("axis1",axis1);
    extendedJet.addUserFloat("axis2",axis2);
    extendedJet.addUserFloat("tana",tana);
    extendedJet.addUserFloat("ttheta",Ttheta);

    extendedJet.addUserFloat("axis1_QC",axis1_QC);
    extendedJet.addUserFloat("axis2_QC",axis2_QC);
    extendedJet.addUserFloat("pull",pull);
    extendedJet.addUserFloat("pull_QC",pull_QC);

    extendedJet.addUserInt("nChg",nChg);
    extendedJet.addUserInt("nNeutral",nNeutral);
    extendedJet.addUserInt("nChg_QC",nChg_QC);
    extendedJet.addUserInt("nChg_ptCut",nChg_ptCut);
    extendedJet.addUserInt("nChg_ptCut_QC",nChg_ptCut_QC);
    extendedJet.addUserInt("nNeutral_ptCut",nNeutral_ptCut);

    extendedJet.addUserFloat("jetRchg",jetRchg);
    extendedJet.addUserFloat("jetRneutral",jetRneutral);
    extendedJet.addUserFloat("jetR",jetR);
    extendedJet.addUserFloat("jetRchg_QC",jetRchg_QC);

    extendedJet.addUserFloat("vtxMass",vtxMass);
    extendedJet.addUserFloat("vtx3dL",vtx3dL);
    extendedJet.addUserFloat("vtx3deL",vtx3deL);
    extendedJet.addUserFloat("sumTrkPt",sumTrkPt);
    extendedJet.addUserFloat("sumTrkP",sumTrkP);
    float sumTrkPtV = sqrt(sumTrkPx*sumTrkPx+sumTrkPy*sumTrkPy);
    extendedJet.addUserFloat("sumTrkPtV",sumTrkPtV);
    extendedJet.addUserFloat("leadTrkPt",leadTrkPt);
    float vtxPt=vtxP4.Pt();
    extendedJet.addUserFloat("vtxPt",vtxPt);
    extendedJet.addUserInt("vtxNTracks",vtxNTracks);
    extendedJet.addUserInt("nConstituents",nConstituents);

    if (debug_) {
      std::cout<<"pt = "<<extendedJet.pt()<<", eta = "<<extendedJet.eta()<<", beta = "<<beta<<", ptMax = "<<jetPtMax<<", ptD = "<<ptD<<", axis1 = "<<axis1<<", axis2 = "<<axis2<<", tana = "<<tana<<", ttheta = "<<Ttheta<<std::endl;
    }
    result->push_back(extendedJet); 
    ++jetIdx;
  }

  iEvent.put(result);

  return;
}

// ------------ method called once each job just before starting event loop  ------------
void JetExtendedProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void JetExtendedProducer::endJob() {
  if (isJecUncSet_) {
    if (debug_) {
      std::cout<<"deleting jecUnc_"<<std::endl;
    }
    delete jecUnc_;
  }
}

//define this as a plug-in
DEFINE_FWK_MODULE(JetExtendedProducer);

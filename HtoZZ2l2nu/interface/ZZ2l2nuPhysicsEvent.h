#ifndef zz2l2nuphysicsevent_h
#define zz2l2nuphysicsevent_h

#include <vector>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"


class PhysicsObject : public LorentzVector
{
public :
  PhysicsObject(LorentzVector vec, Int_t id_):
    LorentzVector(vec), id(id_){ }
    Int_t id;
};


//
class PhysicsObject_Lepton : public LorentzVector
{
 public :
  PhysicsObject_Lepton(LorentzVector vec, Int_t id_,Int_t genid_=0, Float_t ptErr_=0, 
		       Float_t ecalIso_=0, Float_t hcalIso_=0, Float_t trkIso_=0, 
		       Float_t gIso_=0,     Float_t chIso_=0,    Float_t puchIso_=0, Float_t nhIso_=0,
		       Float_t pid_=0):
    LorentzVector(vec), id(id_), genid(genid_), ptErr(ptErr_), 
    ecalIso(ecalIso_), hcalIso(hcalIso_), trkIso(trkIso_), gIso(gIso_), chIso(chIso_), puchIso(puchIso_), nhIso(nhIso_),
    pid(pid_) { }

    Float_t relIso()                              { return (ecalIso+hcalIso+trkIso)/pt(); }
    Float_t relIsoRho(double rho,double cone=0.3) { return (TMath::Max(ecalIso+hcalIso-pow(cone,2)*3.1415*rho,0.)+trkIso)/pt(); }
    Float_t pfRelIsoDbeta()                       { return (TMath::Max(nhIso+gIso-0.5*puchIso,0.)+chIso)/pt(); }
    Float_t pfRelIso()                            { return (nhIso+gIso+chIso)/pt(); }
    Float_t relTkIso()                            { return trkIso/pt(); }
    Float_t ePFRelIsoCorrected2012(double rho)
    {
      //cf. https://twiki.cern.ch/twiki/bin/view/CMS/EgammaEARhoCorrection
      Float_t aeff(0.19);
      if(fabs(eta())>2.4)        aeff=0.52;
      else if(fabs(eta())>2.3)   aeff=0.44;
      else if(fabs(eta())>2.2)   aeff=0.27;
      else if(fabs(eta())>2.0)   aeff=0.21;
      else if(fabs(eta())>1.479) aeff=0.12;
      else if(fabs(eta())>1.0)   aeff=0.25;
      
      return (chIso+TMath::Max(gIso+nhIso-rho*aeff,0.))/pt();
    }
    Float_t muPFRelIsoCorrected2012(double rho)
    {
      //cf. page 9 https://indico.cern.ch/getFile.py/access?contribId=1&resId=0&materialId=slides&confId=188494
      Float_t aeff(0.674);
      if(fabs(eta())>2.3)        aeff=0.660;
      else if(fabs(eta())>2.2)   aeff=0.821;
      else if(fabs(eta())>2.0)   aeff=0.515;
      else if(fabs(eta())>1.5)   aeff=0.442;
      else if(fabs(eta())>1.0)   aeff=0.565;      
      return (chIso+TMath::Max(gIso+nhIso-rho*aeff,0.))/pt();
    }
    void setTrackInfo(Float_t d0_, Float_t dZ_, Float_t trkpt_, Float_t trketa_, Float_t trkphi_, Float_t trkchi2_, Float_t trkValidPixelHits_, Float_t trkValidTrackerHits_, Float_t trkLostInnerHits_)
    {
      d0=d0_; dZ=dZ_;
      trkpt=trkpt_; trketa=trketa_; trkphi=trkphi_; trkchi2=trkchi2_;
      trkValidPixelHits=trkValidPixelHits_; trkValidTrackerHits=trkValidTrackerHits_;trkLostInnerHits=trkLostInnerHits_;
    }
    void setEnergyCorrections(Float_t ensf_, Float_t ensferr_) { ensf=ensf_; ensferr=ensferr_; }
 
    Int_t id,genid;
    Float_t ensf, ensferr;
    Float_t d0, dZ, trkpt,trketa,trkphi,trkchi2,trkValidPixelHits,trkValidTrackerHits,trkLostInnerHits;
    Float_t ptErr, ecalIso, hcalIso, trkIso, gIso, chIso, puchIso, nhIso;
    Int_t  pid;
};


class PhysicsObject_Jet : public LorentzVector
{
public :
  PhysicsObject_Jet(LorentzVector vec, Int_t genid_=0, Float_t btag1_=0, Float_t btag2_=0, Float_t btag3_=0, Float_t neutHadFrac_=0, Float_t neutEmFrac_=0, Float_t chHadFrac_=0, Int_t pid_=0):
    LorentzVector(vec), genid(genid_), btag1(btag1_), btag2(btag2_), btag3(btag3_), neutHadFrac(neutHadFrac_), neutEmFrac(neutEmFrac_), chHadFrac(chHadFrac_), pid(pid_) 
    {
      genPt=0;
      pumva=-9999.;
    }
    void setGenFlavor(Int_t flavid_){ flavid=flavid_;}
    void setGenPt(float val) { if(val<0 || val>1000) genPt=0; else genPt=val; }
    void setPUmva(float val) { pumva=val; }
    void setShapeVariables(float beta_,  float betaStar_, float dRMean_, float ptD_, float ptRMS_)
    {
      beta=beta_; betaStar=betaStar_; dRMean=dRMean_; ptD=ptD_; ptRMS=ptRMS_;
    }
    Int_t genid,flavid;
    Float_t beta,betaStar,dRMean,ptD,ptRMS;
    Float_t btag1, btag2, btag3, neutHadFrac, neutEmFrac, chHadFrac, genPt,pumva;
    Int_t pid;
};
bool JetPtOrdering (PhysicsObject_Jet i,PhysicsObject_Jet j);


class PhysicsObject_Gamma : public LorentzVector
{ 
public : 
  PhysicsObject_Gamma(LorentzVector vec, Float_t ptErr_=0, Float_t iso1_=0, Float_t iso2_=0, Float_t iso3_=0, Float_t sihih_=0, Float_t r9_=0, Float_t hoe_=0):
    LorentzVector(vec), iso1(iso1_), iso2(iso2_), iso3(iso3_), sihih(sihih_), r9(r9_), hoe(hoe_) 
    { 
      hasCtfTrkVeto=false;
      scEnSF=1.0; scEnSFerr=0;
    }
    inline void setSCcorrections(float scEnCorrected,float scEnCorrectedError)
      {
	scEnSF    = scEnCorrected/this->energy();
	scEnSFerr = scEnCorrectedError/this->energy();
      }
    inline void setID(int id)
      {
	hasPixelSeed     = !(id && 0x1);
	hasElectronVeto  = ((id >> 1) & 0x1);
	hasCtfTrkVeto    = ((id >> 2) & 0x1);
	isConv           = ((id >> 3) & 0x1);
	isVtxConstrained = ((id>>4) & 0x1);
      }
    bool hasPixelSeed, hasElectronVeto, hasCtfTrkVeto, isConv,isVtxConstrained;
    LorentzVector convP4;
    Float_t iso1, iso2, iso3, sihih, r9, hoe;
    Float_t scEnSF,scEnSFerr;
};

typedef std::vector<PhysicsObject>        PhysicsObjectCollection;
typedef std::vector<PhysicsObject_Lepton> PhysicsObjectLeptonCollection;
typedef std::vector<PhysicsObject_Jet>    PhysicsObjectJetCollection;
typedef std::vector<PhysicsObject_Gamma>  PhysicsObjectGammaCollection;

//
struct PhysicsEvent_t
{
  int run,event,lumi;
  float puWeight;
  int cat;
  int nvtx;
  LorentzVectorCollection met;
  LorentzVector vtx;
  PhysicsObjectGammaCollection gammas;
  PhysicsObjectJetCollection jets, ajets;
  PhysicsObjectLeptonCollection leptons;

  PhysicsObjectCollection genjets, genleptons, genmet, genhiggs, gengammas;
};

//
PhysicsEvent_t getPhysicsEventFrom(ZZ2l2nuSummary_t &ev);
std::pair<int,int> getWZdecayMode(ZZ2l2nuSummary_t &ev);
int getNgenLeptons(int mcChannelCode, int pdgId);
int getGenProcess(int mcChannelCode);
bool isDYToLL(int mcChannelCode);
bool isDYToTauTau(int mcChannelCode);
bool isZZ2l2nu(int mcChannelCode);
bool isW(int mcChannelCode);
bool isWW(int mcChannelCode);
bool hasObjectId(int idcode,int arbitration);

#endif

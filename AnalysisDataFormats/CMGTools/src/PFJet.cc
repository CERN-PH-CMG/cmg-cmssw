#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include <unordered_map>

const unsigned cmg::PFJet::NCANDTYPES=8;

cmg::PFJetComponent cmg::PFJet::dummy_;
namespace {
    std::unordered_map<std::string,cmg::PFJet::JetID> _pfjet_id_map;
    const std::unordered_map<std::string,cmg::PFJet::JetID> & fill_pfjet_id_map() {
        if (_pfjet_id_map.empty()) {
            _pfjet_id_map["POG_PFID_Loose"] = cmg::PFJet::POG_PFID_Loose;
            _pfjet_id_map["POG_PFID_Medium"] = cmg::PFJet::POG_PFID_Medium;
            _pfjet_id_map["POG_PFID_Tight"] = cmg::PFJet::POG_PFID_Tight;
            _pfjet_id_map["VBFHBB_PFID_Loose"] = cmg::PFJet::VBFHBB_PFID_Loose;
            _pfjet_id_map["VBFHBB_PFID_Medium"] = cmg::PFJet::VBFHBB_PFID_Medium;
            _pfjet_id_map["VBFHBB_PFID_Tight"] = cmg::PFJet::VBFHBB_PFID_Tight;
        }
        return _pfjet_id_map;
    }
}

const cmg::PFJetComponent& cmg::PFJet::component(unsigned ci) const {
  if(ci<1 || ci>=NCANDTYPES) return dummy_;
  else return components_[ci];
}
   
const int cmg::PFJet::nConstituents() const{
  int nConstituents = 0;
  for (unsigned ci = 1; ci < NCANDTYPES; ci++) nConstituents += component(ci).number();
  return nConstituents;

}

float cmg::PFJet::puMva(const std::string & name) const
{
	int index = puIdIndex(name);
	if( index == -1 ) return UnSet(float);
	return puMvas_.at(index);
}

int   cmg::PFJet::puId(const std::string & name) const
{
	int index = puIdIndex(name);
	if( index == -1 ) return UnSet(int);
	return puIds_.at(index);
}

int cmg::PFJet::puIdIndex(const std::string & name) const
{
	const PuIdNames::const_iterator index = std::find( puIdNames_.begin(), puIdNames_.end(), name);
	if( index == puIdNames_.end() ) { return -1; }
	return index - puIdNames_.begin();
}

bool cmg::PFJet::jetID(JetID id) const {
    float chf = component(1).fraction();
    float nhf = component(5).fraction();
    float phf = component(4).fraction();
    float muf = component(3).fraction();
    float elf = component(2).fraction();
    int chm = component(1).number();
    int npr = nConstituents();
    float eta = std::abs(this->eta());
    switch(id) {
        case POG_PFID_Loose:    return (npr>1 && phf<0.99 && nhf<0.99) && (eta>2.4 || (elf<0.99 && chf>0 && chm>0));
        case POG_PFID_Medium:   return (npr>1 && phf<0.95 && nhf<0.95) && (eta>2.4 || (elf<0.99 && chf>0 && chm>0));
        case POG_PFID_Tight:    return (npr>1 && phf<0.90 && nhf<0.90) && (eta>2.4 || (elf<0.99 && chf>0 && chm>0));
        case VBFHBB_PFID_Loose:  return (npr>1 && phf<0.99 && nhf<0.99);
        case VBFHBB_PFID_Medium: return (npr>1 && phf<0.99 && nhf<0.99) && ((eta<=2.4 && nhf<0.9 && phf<0.9 && elf<0.99 && muf<0.99 && chf>0 && chm>0) || eta>2.4);
        case VBFHBB_PFID_Tight:  return (npr>1 && phf<0.99 && nhf<0.99) && ((eta<=2.4 && nhf<0.9 && phf<0.9 && elf<0.70 && muf<0.70 && chf>0 && chm>0) || eta>2.4);
    }
    throw cms::Exception("InvalidArgument", "Unrecognized Jet ID choice");
}

bool cmg::PFJet::jetID(const std::string &id) const {
    const std::unordered_map<std::string,cmg::PFJet::JetID> & id_map = fill_pfjet_id_map();
    auto match = id_map.find(id);
    if (match == id_map.end()) throw cms::Exception("InvalidArgument", id+" jet ID not known");
    return jetID(match->second);
}
bool cmg::PFJet::jetID(const char *id) const 
{
    return jetID(std::string(id));
}
double cmg::PFJet::quarkGluonID(double rho, QGTag::QG_Training training, QGTag::QG_Correction corr) const {
    return QGTag::getLD(*this, rho, training, corr);
}
double cmg::PFJet::quarkGluonID(double rho, const std::string &training, const std::string & correction) const {
    QGTag::QG_Training train; QGTag::QG_Correction corr;
    if (training == "LD") train = QGTag::LD;
    else if (training == "LD_CHS") train = QGTag::LD_CHS;
    else if (training == "LD_CMGVARS") train = QGTag::LD_CMGVARS;
    else if (training == "LD_CHS_CMGVARS") train = QGTag::LD_CHS_CMGVARS;
    else if (training == "VBFHBB") train = QGTag::VBFHBB;
    else throw cms::Exception("Training not implemented", training);
    
    if (correction == "None") corr = QGTag::None;
    else if (correction == "Data") corr = QGTag::Data;
    else if (correction == "Pythia") corr = QGTag::Pythia;
    else if (correction == "Herwig") corr = QGTag::Herwig;
    else throw cms::Exception("Correction not implemented", correction);

    return quarkGluonID(rho, train, corr);
}
double cmg::PFJet::quarkGluonID(double rho, const char *training, const char * correction) const {
    return quarkGluonID(rho, std::string(training), std::string(correction));
}

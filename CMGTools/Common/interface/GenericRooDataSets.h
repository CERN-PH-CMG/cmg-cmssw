#ifndef CMGTOOLS_COMMON_GENERICROODATASETS_H_
#define CMGTOOLS_COMMON_GENERICROODATASETS_H_

#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"
#include "CommonTools/Utils/interface/StringObjectFunction.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "CMGTools/Common/interface/GenericHistograms.h"

#include <cfloat>
#include <climits>
#include <list>
#include <string>
#include <map>
#include <memory>

#include "RooArgSet.h"
#include "RooCategory.h"
#include "RooDataSet.h"
#include "RooRealVar.h"

namespace cmg{
  
  template<class T>
  class GenericRooDataSets : public cmg::GenericHistograms<T>{
    protected:
        
        typedef StringObjectFunction<T> function;
        struct RealVar{
            public:
                /// used to parse the expression and get the value
                function fn_;
                /// the string expression to use
                std::string obs_;
                /// the index of the object in the ordered collection 
                unsigned int index_;
                /// the min value of the RooRealVar
                float low_;
                /// the max value of the RooRealVar
                float high_;
                /// The number of bins (for RooHist*). Set to -0 for no binning
                int nbins_;
                /// The name of the RooRealVar
                std::string name_;
                /// The title of the RooRealVar
                std::string title_;
                /// The unit of the RooRealVar
                std::string unit_;
                /// Whether or not to set a range
                bool setrange_;
                RealVar(const std::string& obs,unsigned int index,
                    const float low,const float high,const int nbins,
                    const std::string& name, const std::string& title, const std::string& unit,
                    const bool setrange):
                    fn_(obs),
                    obs_(obs),index_(index),
                    low_(low),high_(high),nbins_(nbins),
                    name_(name),title_(title),unit_(unit),
                    setrange_(setrange){
                    }
        };
        typedef std::list<RealVar> RealVars;
        
        typedef StringCutObjectSelector<T> selector;
        struct Category{
          public:
            /// selector used to decide cat
            selector sel_;
            /// name of the category
            std::string name_;
            /// the index of the object in the ordered collection
            unsigned int index_;
            Category(const std::string& cut, const std::string& name, const unsigned int index):
                sel_(cut),name_(name),index_(index){
                }
        };
        typedef std::map<std::string, std::list<Category> > Categories;

    public:
 
        GenericRooDataSets(const edm::ParameterSet& ps):
            cmg::GenericHistograms<T>::GenericHistograms(ps),
            vars_(new RealVars),
            cats_(new Categories),
            dataset_(0){
                initArgSet(ps.getUntrackedParameter<edm::ParameterSet>("argset",edm::ParameterSet()));
                initCategories(ps.getUntrackedParameter<edm::ParameterSet>("categories",edm::ParameterSet()));
                std::auto_ptr<RooArgSet> args = argSet();
                const TFileDirectory* myDir = cmg::HistogramCreator<T>::fs_.operator->();
                dataset_ = myDir->make<RooDataSet>("GenericDataSet","GenericDataSet",*args);
        }
        
        virtual void fill(const T& cand){
        }
        virtual void fill(const edm::Event& iEvent, const edm::EventSetup&);

        ///create a new RooArgSet from the ParameterSet
        std::auto_ptr<RooArgSet> argSet() const;
        
        static const std::string weightVarName_;

     protected:
        void initArgSet(const edm::ParameterSet& ps);
        void initCategories(const edm::ParameterSet& ps);
        const std::auto_ptr<RealVars> vars_;
        const std::auto_ptr<Categories> cats_;
        RooDataSet* dataset_;

};

}

template<class T>
const std::string cmg::GenericRooDataSets<T>::weightVarName_ = "weightVar";

template<class T>
void cmg::GenericRooDataSets<T>::initArgSet(const edm::ParameterSet& ps){
    
    std::vector<std::string> parameterNames = ps.getParameterNames();
    for(std::vector<std::string>::const_iterator n = parameterNames.begin(); n != parameterNames.end(); n++){
        std::string name = *n;
        
        edm::ParameterSet a = ps.getUntrackedParameter<edm::ParameterSet>(name);
        //parameters for the RooRealVar
        const std::string var = a.getUntrackedParameter<std::string>("var");
        const unsigned int index = a.getUntrackedParameter<unsigned int>("index",0);
        const double low = a.getUntrackedParameter<double>("low",DBL_MIN);
        const double high = a.getUntrackedParameter<double>("high",DBL_MAX);
        const int nbins = a.getUntrackedParameter<int>("nbins",-1);
        const std::string title = a.getUntrackedParameter<std::string>("title",var);
        const std::string unit = a.getUntrackedParameter<std::string>("unit","");
        const bool setrange = a.getUntrackedParameter<bool>("setrange",true);
        vars_->push_back(RealVar(var,index,low,high,nbins,name,title,unit,setrange));
    }    
}

template<class T>
void cmg::GenericRooDataSets<T>::initCategories(const edm::ParameterSet& ps){
    
    std::vector<std::string> parameterNames = ps.getParameterNames();
    for(std::vector<std::string>::const_iterator n = parameterNames.begin(); n != parameterNames.end(); n++){
        
        std::string name = *n;
        std::list<Category> categories;
        
        std::vector<edm::ParameterSet> cats = ps.getUntrackedParameter<std::vector<edm::ParameterSet> >(name);
        for(std::vector<edm::ParameterSet>::const_iterator a = cats.begin(); a != cats.end(); a++){
            //parameters for the RooCategory
            const std::string cut = a->getUntrackedParameter<std::string>("cut");
            const std::string catname = a->getUntrackedParameter<std::string>("name");
            const unsigned int index = a->getUntrackedParameter<unsigned int>("index",0);
            categories.push_back(Category(cut,catname,index));
        }
        (*cats_)[name] = categories;
    }    
}

template<class T>
std::auto_ptr<RooArgSet> cmg::GenericRooDataSets<T>::argSet() const{
    
    RooArgSet* set = new RooArgSet();
    //define the RooRealVars
    for(typename RealVars::const_iterator it = vars_->begin(); it != vars_->end(); ++it){
        RooRealVar v(it->name_.c_str(),it->title_.c_str(),UnSet(double));
        if(it->nbins_ > 0){
            v.setBins(it->nbins_);   
        }
        if(it->setrange_){
            v.setMin(it->low_); 
            v.setMax(it->high_);     
        }
        set->addClone(v);
    }
    
    //define the RooCategories
    for(typename Categories::const_iterator it = cats_->begin(); it != cats_->end(); ++it){
        RooCategory c(it->first.c_str(),it->first.c_str());    
        for(typename Categories::mapped_type::const_iterator jt = it->second.begin(); jt != it->second.end(); ++jt){
            c.defineType(jt->name_.c_str());
        }      
        set->addClone(c);
    }
        
    //also add a standard weight var
    RooRealVar w(cmg::GenericRooDataSets<T>::weightVarName_.c_str(),"Weight var",1);
    set->addClone(w);
    
    RooRealVar run("Run","Run Number",0);
    set->addClone(run);
    
    RooRealVar lumi("Lumi","Lumi Section",0);
    set->addClone(lumi);
    
    RooRealVar event("Event","Event Number",0);
    set->addClone(event);
    
    return std::auto_ptr<RooArgSet>(set);
}


template<class T>
void cmg::GenericRooDataSets<T>::fill(const edm::Event& iEvent, const edm::EventSetup&){

    edm::Handle<typename cmg::HistogramCreator<T>::view> cands;
    iEvent.getByLabel(cmg::HistogramCreator<T>::labelX_,cands);
    
    const unsigned int run = iEvent.id().run();
    const unsigned int lumi = iEvent.id().luminosityBlock();
    const unsigned int event = iEvent.id().event();

    std::auto_ptr<RooArgSet> args = argSet();
    for(typename RealVars::const_iterator it = vars_->begin(); it != vars_->end(); ++it){
        unsigned int index = it->index_;
        if(cands->size() > index){
            args->setRealValue(it->name_.c_str(),it->fn_(cands->at(index)));
        }
    }
    args->setRealValue("Run",run);
    args->setRealValue("Lumi",lumi);
    args->setRealValue("Event",event);
    
    for(typename Categories::const_iterator it = cats_->begin(); it != cats_->end(); ++it){
        for(typename Categories::mapped_type::const_iterator jt = it->second.begin(); jt != it->second.end(); ++jt){
            unsigned int index = jt->index_;
            //categories set according to index in collection
            if(cands->size() > index){
                //test the cut
                const bool set = jt->sel_(cands->at(index));
                //if its true, set the cat to this one and break. Categories should be exclusive
                if(set){
                    args->setCatLabel(it->first.c_str(),jt->name_.c_str());
                    break;
                }
            }
        }      
    }
    dataset_->add(*args);
}


#endif /*CMGTOOLS_COMMON_GENERICROODATASETS_H_*/

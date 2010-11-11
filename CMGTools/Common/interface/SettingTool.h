#ifndef SETTINGTOOL_H_
#define SETTINGTOOL_H_

#include "FWCore/Framework/interface/Event.h"

namespace cmg{

template <class InputType,class OutputType>
class SettingTool{
    public:
        /// set something without using the event
        virtual void set(const InputType& input, OutputType* const output) const{
        }
        /// set something using the event
        virtual void set(const InputType& input, OutputType* const output, const edm::Event&, const edm::EventSetup&) const{
            set(input,output);
        }
        
};

}

#endif /*SETTINGTOOL_H_*/

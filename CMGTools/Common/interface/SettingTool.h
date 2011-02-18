#ifndef SETTINGTOOL_H_
#define SETTINGTOOL_H_

#include "FWCore/Framework/interface/Event.h"

#include <functional>

namespace cmg{

template <class InputType,class OutputType>
class SettingTool : public std::binary_function<const InputType&, OutputType* const, OutputType* const>{
    public:
        /// set something without using the event
        virtual void set(const InputType& input, OutputType* const output) const{
        }
        /// set something using the event
        virtual void set(const InputType& input, OutputType* const output, const edm::Event&, const edm::EventSetup&) const{
            set(input,output);
        }
        /// an adapter for std::binary_function
        OutputType* const operator()(const InputType& input, OutputType* const output) const{
            set(input,output);
            return output;
        }
};

}

#endif /*SETTINGTOOL_H_*/

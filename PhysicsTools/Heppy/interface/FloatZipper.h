#ifndef PhysicsTools_Heppy_FloatZipper_h
#define PhysicsTools_Heppy_FloatZipper_h

#include <cstdint>
#include "DataFormats/Math/interface/libminifloat.h"

namespace heppy {
    class FloatZipper {
        public:
            FloatZipper() : worker(23) {}
            FloatZipper(int bits) : worker(bits) {}
            void zip(unsigned int n, float *arr) const ;
        private:
            MiniFloatConverter::ReduceMantissaToNbitsRounding worker;
    };
}

#endif

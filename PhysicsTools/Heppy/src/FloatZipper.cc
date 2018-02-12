#include "PhysicsTools/Heppy/interface/FloatZipper.h"
void heppy::FloatZipper::zip(unsigned int n, float *arr) const {
    for (unsigned int i = 0; i < n; ++i) {
        arr[i] = worker(arr[i]);
    }
}

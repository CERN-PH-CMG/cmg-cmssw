#include "CMGTools/Susy/interface/RazorBoxDef.h"
#include "DataFormats/Common/interface/Wrapper.h"

#include <vector>

namespace {
  struct CMGTools_Susy {

    static const razor::Boxes _em_ = razor::EleMu;
    static const razor::Boxes _mm_ = razor::MuMu;
    static const razor::Boxes _ee_ = razor::EleEle;
    static const razor::Boxes _m_ = razor::Mu;
    static const razor::Boxes _e_ = razor::Ele;
    static const razor::Boxes _h_ = razor::Had;
    
    razor::RazorBox b_;
    std::vector<razor::RazorBox> bv;
    edm::Wrapper<razor::RazorBox> bw;
    edm::Wrapper<std::vector<razor::RazorBox> > bwv;
    
  };
  

}

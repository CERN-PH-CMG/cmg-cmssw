#ifndef __PFAnalyses_RootTools_StyleAndOpt__
#define __PFAnalyses_RootTools_StyleAndOpt__

#include "PFAnalyses/RootTools/interface/Styles.h"

class StyleAndOpt {
 public: 
  StyleAndOpt( Style* style, const char* opt, bool addToStack) : 
    style_(style), opt_(opt), addToStack_(addToStack) {}
  StyleAndOpt() : style_(0), addToStack_(false)  {}
  const Style* GetStyle() const {return style_;}
  const std::string& GetOpt() const {return opt_;}
  bool GetAddToStack() const { return addToStack_; } 
 private:
  Style* style_; 
  std::string opt_;
  bool addToStack_;
};

#endif

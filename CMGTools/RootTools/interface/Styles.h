#ifndef __Validation_RecoParticleFlow_Styles__
#define __Validation_RecoParticleFlow_Styles__

#include <TH1.h>
#include <TPad.h>

class Style : public TH1 {
};


class Styles {
 public:
  Style *s1;
  Style *s2;
  Style *sg1;
  Style *spyellow;
  Style *spred;
  Style *spblue;
  Style *spgreen;
  Style *sppink1;
  Style *sppink2;
  Style *sgr1;
  Style *sgr2;
  Style *sdata1;
  Style *sdata2;
  Style *spgray;

  Styles();


  static void FormatHisto( TH1* h, const Style* s=0, bool solidFill=false);
  
  static void FormatPad( TPad* pad, bool grid = true, bool logy = false, bool logx = false);
  
  static void SavePlot(const char* name, const char* dir);

  static void SetMax( TVirtualPad* pad, float safetyFactor = 1.2);

  static void ScaleToOne( TPad* pad, float safetyFactor = 1.2);
};


#endif

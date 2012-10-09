#ifndef BTAGWEIGHT
#define BTAGWEIGHT
#include <math.h>
#include <iostream>
#include <vector>
using namespace std; 

class BTagWeight {
 public:
  struct JetInfo {
    JetInfo(float mceff,float datasf,float t=0.) : eff(mceff), sf(datasf) , tag(t){}
    float eff;
    float sf;
    int tag;
  };
  
  BTagWeight() : taggers(1) {}
  BTagWeight(int nTaggers) : taggers(nTaggers) {}
    
    virtual bool filter(vector<int> tags);
    float weight(vector<vector<JetInfo> > jets);
    int addJet(double eff, double sf);
    float weight();
    void cleanVector();

 private:
    int taggers;
    vector<vector<JetInfo> > jets_ ;
    
};

#endif

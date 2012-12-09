#include <vector>

class component {
public:
  component() : xsec(0.) {}
  component(string name_, float xsec_, string mode_="") : name(name_), mode(mode_), xsec(xsec_) {}    
  string name;
  string mode;
  float xsec;
};



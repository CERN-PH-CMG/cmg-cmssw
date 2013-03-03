#ifndef LeptonEfficiencySF_h
#define LeptonEfficiencySF_h

class LeptonEfficiencySF
{
 public:
  LeptonEfficiencySF(int era=2012) : era_(era) { }
    ~LeptonEfficiencySF() {}

    std::pair<float,float> getLeptonEfficiency(float pt, float eta, int id)
      {
	std::pair<float,float> eff(1.0,0.04);
	if(era_==2012)
	  {
	    //tight e-id+ISO efficiencies
	    if(id==11)
	      {
		if(fabs(eta)<0.8)
		  {
		    if(pt<30)      { eff.first=1.010;  eff.second=0.004; }
		    else if(pt<40) { eff.first=1.006;  eff.second=0.001; }
		    else if(pt<50) { eff.first=1.009;  eff.second=0.002; }
		    else           { eff.first=1.008;  eff.second=0.003; }

		  }
		else if(fabs(eta)<1.442)
		  {
		    if(pt<30)      { eff.first=0.981;  eff.second=0.02; }
		    else if(pt<40) { eff.first=0.987;  eff.second=0.002; }
		    else if(pt<50) { eff.first=0.993;  eff.second=0.02; }
		    else           { eff.first=0.995;  eff.second=0.003; }
		  }
		else if(fabs(eta)<1.556)
		  {
		    if(pt<30)      { eff.first=1.046;  eff.second=0.02; }
		    else if(pt<40) { eff.first=1.011;  eff.second=0.007; }
		    else if(pt<50) { eff.first=0.994;  eff.second=0.004; }
		    else           { eff.first=0.997;  eff.second=0.006; }
		  }
		else if(fabs(eta)<2.0)
		  {
		    if(pt<30)      { eff.first=0.992;  eff.second=0.09;  }
		    else if(pt<40) { eff.first=0.993;  eff.second=0.003; }
		    else if(pt<50) { eff.first=1.008;  eff.second=0.002; }
		    else           { eff.first=1.009;  eff.second=0.003; }
		  }
		else
		  {
		    if(pt<30)      { eff.first=1.045;  eff.second=0.007; }
		    else if(pt<40) { eff.first=1.031;  eff.second=0.003; }
		    else if(pt<50) { eff.first=1.019;  eff.second=0.001; }
		    else           { eff.first=1.014;  eff.second=0.003; }
		  }

	      }

	    //tight m-id+ISO efficiencies
	    if(id==13)
	      {
		if(fabs(eta)<0.9)
		  {
		    if(pt<25)       { eff.first=0.989*0.987; eff.second=0.002; }
		    else if(pt<30)  { eff.first=0.994*1.001; eff.second=0.002; }
		    else if(pt<35)  { eff.first=0.994*0.999; eff.second=0.002; }
		    else if(pt<40)  { eff.first=0.994*0.999; eff.second=0.002; }
		    else if(pt<50)  { eff.first=0.992*0.998; eff.second=0.002; }
		    else if(pt<60)  { eff.first=0.991*0.999; eff.second=0.002; }
		    else if(pt<90)  { eff.first=0.990*1.001; eff.second=0.002; }
		    else if(pt<140) { eff.first=1.004*1.001; eff.second=0.003; }
		    else if(pt<300) { eff.first=1.028*1.001; eff.second=0.020; }
		    else            { eff.first=1.000*1.011; eff.second=0.160; }
		  }
		else if(fabs(eta)<1.2)
		  {
		    if(pt<25)       { eff.first=0.987*0.992; eff.second=0.003; }
		    else if(pt<30)  { eff.first=0.994*1.004; eff.second=0.002; }
		    else if(pt<35)  { eff.first=0.991*1.002; eff.second=0.002; }
		    else if(pt<40)  { eff.first=0.990*1.001; eff.second=0.002; }
		    else if(pt<50)  { eff.first=0.990*1.000; eff.second=0.002; }
		    else if(pt<60)  { eff.first=0.991*1.000; eff.second=0.002; }
		    else if(pt<90)  { eff.first=0.986*1.000; eff.second=0.002; }
		    else if(pt<140) { eff.first=1.012*0.999; eff.second=0.007; }
		    else if(pt<300) { eff.first=0.956*1.000; eff.second=0.034; }
		    else            { eff.first=1.000*1.000; eff.second=0.255; }
		  }
		else if(fabs(eta)<2.1)
		  {
		    if(pt<25)       { eff.first=1.002*1.000; eff.second=0.002; }
		    else if(pt<30)  { eff.first=0.999*1.006; eff.second=0.002; }
		    else if(pt<35)  { eff.first=0.998*1.005; eff.second=0.002; }
		    else if(pt<40)  { eff.first=0.997*1.002; eff.second=0.002; }
		    else if(pt<50)  { eff.first=0.997*1.000; eff.second=0.002; }
		    else if(pt<60)  { eff.first=0.998*1.000; eff.second=0.002; }
		    else if(pt<90)  { eff.first=0.994*1.001; eff.second=0.002; }
		    else if(pt<140) { eff.first=1.019*1.000; eff.second=0.003; }
		    else if(pt<300) { eff.first=1.016*1.002; eff.second=0.02;  }
		    else            { eff.first=1.000*1.000; eff.second=2*0.415; }
		    //the last one should be 0.609 but is it really true or simply a problem with the fit
		    //instead I blew up the uncertanty by 2
		  }
		else
		  {
		    eff.first=0.994*1.028;
		    eff.second=0.1;
		  }
	      }
	  }

	return eff;
      }


 private:
    int era_;
};


#endif

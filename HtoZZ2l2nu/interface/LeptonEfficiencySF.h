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
		if(fabs(eta)<1.479)
		  {
		    if(pt<20)      { eff.first=1.04;  eff.second=0.45; }
		    else if(pt<30) { eff.first=0.93;  eff.second=0.03; }
		    else if(pt<40) { eff.first=0.966; eff.second=0.006; }
		    else if(pt<50) { eff.first=0.98;  eff.second=0.07; }
		    else           { eff.first=0.969; eff.second=0.008; }
		  }
		else
		  {
		    if(pt<20)      { eff.first=0.94; eff.second=0.08; }
		    else if(pt<30) { eff.first=0.97; eff.second=0.03; }
		    else if(pt<40) { eff.first=0.99; eff.second=0.01; }
		    else if(pt<50) { eff.first=0.96; eff.second=0.01; }
		    else           { eff.first=0.92; eff.second=0.02; }
		  }
	      }

	    //tight m-id+ISO efficiencies
	    if(id==13)
	      {

		if(fabs(eta)<1.2)
		  {
		    if(pt<20) { eff.first=0.98525784412145911; eff.second=0.01072726647303005; }
		    else if(pt<30) { eff.first=1.0120581013997967; eff.second=0.0022947686421935422; }
		    else if(pt<40) { eff.first=1.0066854; eff.second=0.00083886720920398083; }
		    else if(pt<60) { eff.first=1.002233456646902; eff.second=0.00039480243858272464; }
		    else           { eff.first=1.0027667810058651; eff.second=0.0011473000813821998; }
		  }
		else
		  {
		    if(pt<20)      { eff.first=1.0447124027795287; eff.second=0.0067351181069166969; }
		    else if(pt<30) { eff.first=1.0387128974732804; eff.second=0.0018263572124372204; }
		    else if(pt<40) { eff.first=1.0148770718095641; eff.second=0.00088220766155088707; }
		    else if(pt<60) { eff.first=1.0049826379556341; eff.second=0.00045095365695810654; }
		    else           { eff.first=1.0013015405490315; eff.second=0.0015982944034112468; }
		  }
	      }
	  }

	return eff;
      }


 private:
    int era_;
};


#endif

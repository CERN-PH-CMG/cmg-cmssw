#ifndef LeptonEfficiencySF_h
#define LeptonEfficiencySF_h

// cf.
// https://twiki.cern.ch/twiki/bin/view/Main/EGammaScaleFactors2012#2012_8_TeV_data_53X
//
class LeptonEfficiencySF
{
 public:

  //
  LeptonEfficiencySF() { }

  //
  ~LeptonEfficiencySF() {}

  //
  std::pair<float,float> getLeptonEfficiency(float pt, float eta, int id, std::string wp)
    {
      eta=fabs(eta);
      id=abs(id);
      
      std::pair<float,float> eff(1.0,0.04);
      switch(id){
      case 11:
	{
	  if(wp=="loose")
	    {
		if(fabs(eta)<0.8)
		  {
		    if(pt<30)      { eff.first=1.005;  eff.second=0.004; }
		    else if(pt<40) { eff.first=1.004;  eff.second=0.003; }
		    else if(pt<50) { eff.first=1.008;  eff.second=0.002; }
		    else           { eff.first=1.008;  eff.second=0.002; }

		  }
		else if(fabs(eta)<1.442)
		  {
		    if(pt<30)      { eff.first=0.981;  eff.second=0.006; }
		    else if(pt<40) { eff.first=0.991;  eff.second=0.001; }
		    else if(pt<50) { eff.first=0.994;  eff.second=0.002; }
		    else           { eff.first=0.999;  eff.second=0.003; }
		  }
		else if(fabs(eta)<1.556)
		  {
		    if(pt<30)      { eff.first=1.044;  eff.second=0.012; }
		    else if(pt<40) { eff.first=0.998;  eff.second=0.005; }
		    else if(pt<50) { eff.first=0.989;  eff.second=0.005; }
		    else           { eff.first=0.994;  eff.second=0.005; }
		  }
		else if(fabs(eta)<2.0)
		  {
		    if(pt<30)      { eff.first=0.980;  eff.second=0.007; }
		    else if(pt<40) { eff.first=0.992;  eff.second=0.003; }
		    else if(pt<50) { eff.first=1.004;  eff.second=0.003; }
		    else           { eff.first=1.006;  eff.second=0.004; }
		  }
		else
		  {
		    if(pt<30)      { eff.first=1.017;  eff.second=0.008; }
		    else if(pt<40) { eff.first=1.019;  eff.second=0.004; }
		    else if(pt<50) { eff.first=1.005;  eff.second=0.010; }
		    else           { eff.first=1.009;  eff.second=0.003; }
		  }

	      }
	  if(wp=="medium")
	      {
		if(fabs(eta)<0.8)
		  {
		    if(pt<30)      { eff.first=1.004;  eff.second=0.004; }
		    else if(pt<40) { eff.first=1.003;  eff.second=0.003; }
		    else if(pt<50) { eff.first=1.007;  eff.second=0.002; }
		    else           { eff.first=1.007;  eff.second=0.003; }

		  }
		else if(fabs(eta)<1.442)
		  {
		    if(pt<30)      { eff.first=0.975;  eff.second=0.012; }
		    else if(pt<40) { eff.first=0.984;  eff.second=0.002; }
		    else if(pt<50) { eff.first=0.992;  eff.second=0.002; }
		    else           { eff.first=0.995;  eff.second=0.003; }
		  }
		else if(fabs(eta)<1.556)
		  {
		    if(pt<30)      { eff.first=1.034;  eff.second=0.016; }
		    else if(pt<40) { eff.first=1.006;  eff.second=0.008; }
		    else if(pt<50) { eff.first=0.991;  eff.second=0.005; }
		    else           { eff.first=0.993;  eff.second=0.006; }
		  }
		else if(fabs(eta)<2.0)
		  {
		    if(pt<30)      { eff.first=0.983;  eff.second=0.010; }
		    else if(pt<40) { eff.first=0.990;  eff.second=0.004; }
		    else if(pt<50) { eff.first=1.006;  eff.second=0.003; }
		    else           { eff.first=1.007;  eff.second=0.003; }
		  }
		else
		  {
		    if(pt<30)      { eff.first=1.245;  eff.second=0.007; }
		    else if(pt<40) { eff.first=1.022;  eff.second=0.003; }
		    else if(pt<50) { eff.first=1.013;  eff.second=0.004; }
		    else           { eff.first=1.009;  eff.second=0.003; }
		  }

	      }
	  if(wp=="tight")
	      {
		if(fabs(eta)<0.8)
		  {
		    if(pt<30)      { eff.first=0.973;  eff.second=0.004; }
		    else if(pt<40) { eff.first=0.979;  eff.second=0.003; }
		    else if(pt<50) { eff.first=0.984;  eff.second=0.0021; }
		    else           { eff.first=0.983;  eff.second=0.001; }

		  }
		else if(fabs(eta)<1.442)
		  {
		    if(pt<30)      { eff.first=0.948;  eff.second=0.007; }
		    else if(pt<40) { eff.first=0.961;  eff.second=0.006; }
		    else if(pt<50) { eff.first=0.972;  eff.second=0.002; }
		    else           { eff.first=0.977;  eff.second=0.005; }
		  }
		else if(fabs(eta)<1.556)
		  {
		    if(pt<30)      { eff.first=0.983;  eff.second=0.020; }
		    else if(pt<40) { eff.first=0.983;  eff.second=0.010; }
		    else if(pt<50) { eff.first=0.957;  eff.second=0.004; }
		    else           { eff.first=0.978;  eff.second=0.008; }
		  }
		else if(fabs(eta)<2.0)
		  {
		    if(pt<30)      { eff.first=0.957;  eff.second=0.010;  }
		    else if(pt<40) { eff.first=0.962;  eff.second=0.004; }
		    else if(pt<50) { eff.first=0.985;  eff.second=0.002; }
		    else           { eff.first=0.986;  eff.second=0.005; }
		  }
		else
		  {
		    if(pt<30)      { eff.first=1.001;  eff.second=0.009;  }
		    else if(pt<40) { eff.first=1.002;  eff.second=0.005; }
		    else if(pt<50) { eff.first=0.999;  eff.second=0.004; }
		    else           { eff.first=0.995;  eff.second=0.004; }
		  }

	      }
	}
	break;
      case 13:
	{
	  if(wp=="loose")
	    {
	      if(fabs(eta)<0.9){
		if(pt<20) { eff.first=0.992655; eff.second=0.005766; }
		else if(pt<25) { eff.first=0.992198; eff.second=0.001499; }
		else if(pt<30) { eff.first=0.996142; eff.second=0.000501; }
		else if(pt<35) { eff.first=0.998442; eff.second=0.000275; }
		else if(pt<40) { eff.first=0.998523; eff.second=0.000175; }
		else if(pt<50) { eff.first=0.998694; eff.second=0.000099; }
		else if(pt<60) { eff.first=0.996988; eff.second=0.000319; }
		else if(pt<90) { eff.first=0.993659; eff.second=0.000684; }
		else if(pt<140) { eff.first=1.001510; eff.second=0.001408; }
		else { eff.first=1.013407; eff.second=0.012274; }
	      }
	      else if(fabs(eta)<1.2){
		if(pt<20) { eff.first=0.995661; eff.second=0.006692; }
		else if(pt<25) { eff.first=0.997587; eff.second=0.002157; }
		else if(pt<30) { eff.first=1.001378; eff.second=0.000919; }
		else if(pt<35) { eff.first=0.998584; eff.second=0.000561; }
		else if(pt<40) { eff.first=0.999539; eff.second=0.000336; }
		else if(pt<50) { eff.first=0.998602; eff.second=0.000182; }
		else if(pt<60) { eff.first=0.997953; eff.second=0.000596; }
		else if(pt<90) { eff.first=0.992331; eff.second=0.001302; }
		else if(pt<140) { eff.first=1.001732; eff.second=0.002126; }
		else { eff.first=0.964431; eff.second=0.032908; }
	      }
	      else if(fabs(eta)<2.1){
		if(pt<20) { eff.first=1.003393; eff.second=0.000571; }
		else if(pt<25) { eff.first=1.002725; eff.second=0.001084; }
		else if(pt<30) { eff.first=0.999066; eff.second=0.000535; }
		else if(pt<35) { eff.first=0.998504; eff.second=0.000365; }
		else if(pt<40) { eff.first=0.998502; eff.second=0.000258; }
		else if(pt<50) { eff.first=0.998536; eff.second=0.000144; }
		else if(pt<60) { eff.first=0.996718; eff.second=0.000470; }
		else if(pt<90) { eff.first=0.990389; eff.second=0.001082; }
		else if(pt<140) { eff.first=1.002093; eff.second=0.001776; }
		else { eff.first=1.000698; eff.second=0.014194; }
	      }
	      else{
		eff.first=0.998941; eff.second=0.01;
	      }
	    }
	  if(wp=="tight"){
	    if(fabs(eta)<0.9){
	      if(pt<20) { eff.first=0.947050; eff.second=0.002755; }
	      else if(pt<25) { eff.first=0.974978; eff.second=0.001536; }
	      else if(pt<30) { eff.first=0.997129; eff.second=0.000938; }
	      else if(pt<35) { eff.first=0.993863; eff.second=0.000621; }
	      else if(pt<40) { eff.first=0.993442; eff.second=0.000432; }
	      else if(pt<50) { eff.first=0.994101; eff.second=0.000222; }
	      else if(pt<60) { eff.first=0.995544; eff.second=0.000405; }
	      else if(pt<90) { eff.first=0.999036; eff.second=0.000468; }
	      else if(pt<140) { eff.first=1.001041; eff.second=0.000894; }
	      else { eff.first=1.000300; eff.second=0.001769; }
	    }
	    else if(fabs(eta)<1.2){
	      if(pt<20) { eff.first=0.951836; eff.second=0.003166; }
	      else if(pt<25) { eff.first=0.988368; eff.second=0.002317; }
	      else if(pt<30) { eff.first=1.000834; eff.second=0.001646; }
	      else if(pt<35) { eff.first=0.998546; eff.second=0.001165; }
	      else if(pt<40) { eff.first=0.999140; eff.second=0.000747; }
	      else if(pt<50) { eff.first=0.998176; eff.second=0.000352; }
	      else if(pt<60) { eff.first=0.998696; eff.second=0.000651; }
	      else if(pt<90) { eff.first=0.999132; eff.second=0.000778; }
	      else if(pt<140) { eff.first=0.999559; eff.second=0.001429; }
	      else { eff.first=0.996767; eff.second=0.003024; }
	    }
	    else if(fabs(eta)<2.1){
	      if(pt<20) { eff.first=0.980045; eff.second=0.001622; }
	      else if(pt<25) { eff.first=0.997342; eff.second=0.001208; }
	      else if(pt<30) { eff.first=1.007837; eff.second=0.000879; }
	      else if(pt<35) { eff.first=1.006848; eff.second=0.000672; }
	      else if(pt<40) { eff.first=1.003699; eff.second=0.000465; }
	      else if(pt<50) { eff.first=1.002093; eff.second=0.000208; }
	      else if(pt<60) { eff.first=1.001247; eff.second=0.000384; }
	      else if(pt<90) { eff.first=1.000654; eff.second=0.000474; }
	      else if(pt<140) { eff.first=0.999878; eff.second=0.000955; }
	      else { eff.first=0.999890; eff.second=0.002275; }
	    }
	    else{
	      eff.first=1.003090; eff.second=0.01;
	    }
	  }
	}
	break;
      }
      
      return eff;
    }

 private:

};


#endif

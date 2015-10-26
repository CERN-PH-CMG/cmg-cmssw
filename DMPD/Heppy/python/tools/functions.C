void functions(){
  cout<<"load some functions.."<<endl;
}

float deltaPhi(float phi1, float phi2) {
  float PHI = fabs(phi1-phi2);
  if (PHI<=3.14159265)
    return PHI;
  else
    return 2*3.14159265-PHI;
}

float deltaR(float phi1, float eta1, float phi2, float eta2) {
  return sqrt(pow((eta2-eta1),2)+pow(deltaPhi(phi1,phi2),2));
}


float vectorSumPhi(float px1, float py1, float px2, float py2){
  float phi = atan((py1+py2)/(px1+px2));
  if ((px1+px2)>0) return phi;
  else if ((py1+py2)>0) return phi + 3.14159265;
  else return phi - 3.14159265;
}


float vectorSumPt(float pt1, float phi1, float pt2, float phi2){
  return sqrt( pow(pt1*cos(phi1) + pt2*cos(phi2),2) +
	       pow(pt1*sin(phi1) + pt2*sin(phi2),2) );
}

float vectorSum3Pt(float pt1, float phi1, float pt2, float phi2,float pt3, float phi3){
  return sqrt( pow(pt1*cos(phi1) + pt2*cos(phi2) + pt3*cos(phi3),2) +
	       pow(pt1*sin(phi1) + pt2*sin(phi2) + pt3*sin(phi3),2) );
}

float vectorSumMass(float px1, float py1, float pz1, float px2, float py2, float pz2) {
  double E1 = sqrt(pow(px1,2) + pow(py1,2) + pow(pz1,2));
  double E2 = sqrt(pow(px2,2) + pow(py2,2) + pow(pz2,2));
  double cosTheta = (px1*px2 + py1*py2 + pz1*pz2)/ (E1*E2);
  return sqrt(2*E1*E2*(1-cosTheta));
}

float transverseMass(float lepPt, float lepPhi, float met,  float metPhi) {
  double cosDPhi = cos(deltaPhi(lepPhi,metPhi));
  return sqrt(2*lepPt*met*(1-cosDPhi));
}

float caloMet1l(float pt, float phi, float met, float metPhi){
  return sqrt( pow(pt*cos(phi) + met*cos(metPhi),2) +
	       pow(pt*sin(phi) + met*sin(metPhi),2));
}

float caloMet2l(float pt1, float phi1, float pt2, float phi2, float met, float metPhi){
  return sqrt( pow(pt1*cos(phi1) + pt2*cos(phi2) + met*cos(metPhi),2) +
	       pow(pt1*sin(phi1) + pt2*sin(phi2) + met*sin(metPhi),2));
}

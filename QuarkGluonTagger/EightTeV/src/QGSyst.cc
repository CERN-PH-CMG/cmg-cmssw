#include "../interface/QGSyst.h"
#include <iostream>
#include "TMath.h"

//
//-------- Constructor -------
QGSyst::QGSyst()
{
	tagger_="QGLHisto";
}
//-------- Destructor -------
QGSyst::~QGSyst()
{
}
//----------Read Database ----------
void QGSyst::ReadDatabase(std::string fileName)
{
        std::ifstream database_;
	database_.open(fileName.c_str(),std::ios::in);
	if( !database_.is_open() ) { std::cerr<<"ERROR: File "<<fileName<<" not open"<<std::endl; return;}
	std::string line;
	while ( database_.good() )
    		{
      		std::getline (database_,line);
      		//cout << line << endl;
		float pt1,pt2,eta1,eta2,rho1,rho2,a,b,lmin,lmax;
		char tag[1023],leadchar;
		sscanf(line.c_str(),"%c",&leadchar);
		if(  (leadchar=='#') || (leadchar=='!')) continue; 
		sscanf(line.c_str(),"%s %f %f %f %f %f %f %f %f %f %f",&tag[0],&pt1,&pt2,&rho1,&rho2,&eta1,&eta2,&a,&b,&lmin,&lmax);
		QGSystBin B;
			B.PtRange=std::pair<float,float>(pt1,pt2);
			B.EtaRange=std::pair<float,float>(eta1,eta2);
			B.RhoRange=std::pair<float,float>(rho1,rho2);
			B.tag=std::string(tag);
		QGSystParameters P;
			P.a=a;P.b=b;P.lmin=lmin;P.lmax=lmax;	
		corrections_[B]=P;
    		}	
	database_.close();
}



void QGSyst::ReadDatabaseDoubleMin(std::string fileName)
{
        std::ifstream database_;
	database_.open(fileName.c_str(),std::ios::in);
	if( !database_.is_open() ) { std::cerr<<"ERROR: File "<<fileName<<" not open"<<std::endl; return;}
	std::string line;
	while ( database_.good() )
    		{
      		std::getline (database_,line);
      		//cout << line << endl;
		float pt1,pt2,eta1,eta2,rho1,rho2,a_q,b_q,a_g,b_g,lmin,lmax;
		char tag[1023],leadchar;
		sscanf(line.c_str(),"%c",&leadchar);
		if(  (leadchar=='#') || (leadchar=='!')) continue; 
		sscanf(line.c_str(),"%s %f %f %f %f %f %f %f %f %f %f %f %f",&tag[0],&pt1,&pt2,&rho1,&rho2,&eta1,&eta2,&a_q,&b_q,&a_g,&b_g,&lmin,&lmax);
		QGSystBin thisBin;
			thisBin.PtRange=std::pair<float,float>(pt1,pt2);
			thisBin.EtaRange=std::pair<float,float>(eta1,eta2);
			thisBin.RhoRange=std::pair<float,float>(rho1,rho2);
			thisBin.tag=std::string(tag);
		QGSystParameters quarkParameters;
			quarkParameters.a = a_q;
			quarkParameters.b = b_q;
			quarkParameters.lmin = lmin;
			quarkParameters.lmax = lmax;
		QGSystParameters gluonParameters;
			gluonParameters.a = a_g;
			gluonParameters.b = b_g;
			gluonParameters.lmin = lmin;
			gluonParameters.lmax = lmax;
		corrections_quark_[thisBin]=quarkParameters;
		corrections_gluon_[thisBin]=gluonParameters;
    		}	
	database_.close();
}





float QGSyst::function(float x0, float a ,float b,float min,float max)
{
using namespace TMath;
float x=(x0-min)/(max-min); 
if(x<0.)x=0.;
if(x>1.)x=1.;

float x1= (TanH( a* ATanH(2.*x-1.)+b )/2.+.5 ) ;
if(x<=0.)x1=0.; //prevent overflow and underflow bins
if(x>=1.)x1=1.;

return x1*(max-min)+min;
 
}

float QGSyst::Smear(float pt,float eta, float rho, float x, const std::string& type){

	std::map<QGSystBin,QGSystParameters>* corrections = &corrections_; //default: all
	if( type=="quark" ) corrections = &corrections_quark_;
	else if( type=="gluon" ) corrections = &corrections_gluon_;
	else if( type!="all" ) {
		std::cout << "[QGSyst::Smear] Unkown jet type: '" << type << "'. Exiting." << std::endl;
		exit(199);
	}

	for(std::map<QGSystBin,QGSystParameters>::iterator it= corrections->begin();it!=corrections->end();it++)
		{
		if(it->first.tag.find(tagger_) == std::string::npos)continue;
		if(it->first.isInside(pt,fabs(eta),rho) )
			return function(x,it->second.a,it->second.b,it->second.lmin,it->second.lmax);
		else continue;
		}
	return -99;
}


bool QGSystBin::operator<(const QGSystBin& rhs) const {

	if( !(this->PtRange.first   == rhs.PtRange.first) )   return (this->PtRange.first   < rhs.PtRange.first);
	if( !(this->PtRange.second  == rhs.PtRange.second) )  return (this->PtRange.second  < rhs.PtRange.second);
	if( !(this->EtaRange.first  == rhs.EtaRange.first) )  return (this->EtaRange.first  < rhs.EtaRange.first);
	if( !(this->EtaRange.second == rhs.EtaRange.second) ) return (this->EtaRange.second < rhs.EtaRange.second);
	if( !(this->RhoRange.first  == rhs.RhoRange.first) )  return (this->RhoRange.first  < rhs.RhoRange.first);
	if( !(this->RhoRange.second == rhs.RhoRange.second) ) return (this->RhoRange.second < rhs.RhoRange.second);
	return (this->tag < rhs.tag) ;
	return false; //never here
}


bool QGSystBin::isInside(float pt,float eta,float rho) const {
	if( ( pt<=PtRange.second) && (PtRange.first<pt) && 
	    (RhoRange.first<=rho) && (rho<RhoRange.second) && 
	    (EtaRange.first <= eta) && (eta<EtaRange.second) ) return true;
	else { 
		//printf("pt eta rho (%.0f,%.1f,%.1f) outside of (%.0f-%.0f),(%.1f,%.1f),(%.1f,%.1f\n)\n",pt,eta,rho,PtRange.first,PtRange.second,EtaRange.first,EtaRange.second,RhoRange.first,RhoRange.second); //debug
		return false;
	} 
}

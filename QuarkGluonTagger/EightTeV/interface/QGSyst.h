
/*************************************
 * C++ Based class for syst. smearing
 * Original author: A.C. Marini
 * Date: 01/06/2013
 *
 */

#ifndef QGSYST_H
#define QGSYST_H

#include <cstdio>
#include <vector>
#include <map>
#include <string>
#include <fstream>



class QGSystBin{
public:
	QGSystBin(){};
	~QGSystBin(){};
	std::pair<float,float> PtRange;	
	std::pair<float,float> EtaRange;	
	std::pair<float,float> RhoRange;	
	std::string tag;
	bool isInside(float pt,float eta,float rho) const;
	bool operator<(const QGSystBin& rhs) const;
};

struct QGSystParameters
{
	float a;
	float b;
	float lmin;
	float lmax;
}; 


class QGSyst {
public:
	QGSyst();
	~QGSyst();
	float Smear(float pt,float eta, float rho, float x, const std::string& type="all");
	void SetTagger(std::string tag){tagger_=tag;};
	std::string GetTagger(){ return tagger_;};
	void ReadDatabase(std::string fileName);
	void ReadDatabaseDoubleMin(std::string fileName);
private:
	std::string tagger_;
	//FILE *database_;
	std::ifstream database_;
	std::map<QGSystBin,QGSystParameters> corrections_;
	std::map<QGSystBin,QGSystParameters> corrections_quark_;
	std::map<QGSystBin,QGSystParameters> corrections_gluon_;
	float function(float x0, float a ,float b,float min,float max);

};
#endif

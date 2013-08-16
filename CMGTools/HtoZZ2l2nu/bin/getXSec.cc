#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>

#include "TGraph.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TGraphErrors.h"

#include "../src/tdrstyle.C"

using namespace std;


struct stData{
   double mass; 
   double xsec;
   double br;
   double eff;
   double err;
};

bool dataOrdering (stData i,stData j){ return (i.mass<j.mass);}



//
int main(int argc, char *argv[])
{
   setTDRStyle();
   gStyle->SetPadTopMargin   (0.04);
   gStyle->SetPadBottomMargin(0.12);
   gStyle->SetPadRightMargin (0.05);
   gStyle->SetPadLeftMargin  (0.12);
   gStyle->SetTitleSize(0.04, "XYZ");
   gStyle->SetTitleXOffset(1.1);
   gStyle->SetTitleYOffset(1.45);
   gStyle->SetPalette(1);
   gStyle->SetNdivisions(505);

   std::map<string, std::map<string, std::vector<stData> >   > map;
   std::map<double, stData > mapMass;
   std::map<string,  TGraph* > graphs;

   std::string summaryFile = string(argv[1]);
   
   for(int i=2;i<argc;i++){
     string arg(argv[i]);

     FILE* pFile = fopen(arg.c_str(),"r");
     if(!pFile){printf("Can't open %s\n",arg.c_str()); return 0;}      
     while(true){
        char bin[1024]; char proc[1024]; stData data;
        if(fscanf(pFile,"%s %s %lf %lf %lf %lf %lf\n",bin, proc, &data.mass, &data.xsec, &data.br, &data.eff, &data.err)==EOF)break;
        string procS = proc;
        procS = procS.substr(0,procS.find('('));

        std::map<string, std::vector<stData> >& tmp = map[procS];
        std::vector<stData>& tmp2 = tmp[bin];
        tmp2.push_back(data);

        mapMass[data.mass] = data;
     }
     fclose(pFile);     
   }


   FILE* pFile = fopen(summaryFile.c_str(),"w");
   for(std::map<double, stData>::iterator it=mapMass.begin(); it!=mapMass.end(); it++){
      fprintf(pFile, "%4f %E %E\n", it->second.mass, it->second.xsec, it->second.br);
   }
   fclose(pFile);      
}

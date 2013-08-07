#ifndef HIGGSCSANDWIDTH_CC
#define HIGGSCSANDWIDTH_CC


#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <fstream>

#include "TROOT.h"
#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TSpline.h"


#include "HiggsCSandWidth.h"

using namespace std;

HiggsCSandWidth::HiggsCSandWidth()
{

  N_BR = 217;
  N_CS = 197;
  N_CSggToH_8tev = 223;
  N_CSvbf_8tev = 223;
  N_CSttH_8tev = 178;
  N_CSZH_8tev = 178;
  N_CSWH_8tev = 178;

  ifstream file;
 
  // Read Widths into memory
  file.open("../txtFiles/HiggsBR_7TeV_Official.txt");
  for(int k = 0; k < N_BR; k++){

    file >> mass_BR[k] >> BR[0][k] >> BR[1][k] >> BR[2][k] >> BR[3][k] >> BR[4][k] >> BR[5][k] >> BR[6][k] >> BR[7][k] >> BR[8][k] >> BR[9][k]
	 >> BR[10][k] >> BR[11][k] >> BR[12][k] >> BR[13][k] >> BR[14][k] >> BR[15][k] >> BR[16][k] >> BR[17][k] >> BR[18][k] >> BR[19][k] >> BR[20][k]
	 >> BR[21][k] >> BR[22][k] >> BR[23][k] >> BR[24][k] >> BR[25][k];


  }
  file.close();

  // ---------------- Read 7 TeV CS into memory ------------------ //         
  file.open("../txtFiles/HiggsCS_Official.txt");//directory of input file
  for(int k = 0; k < N_CS; k++){

    file >> mass_XS[k] >> CS[ID_ggToH][k] >> CS[ID_VBF][k] >> CS[ID_WH][k] >> CS[ID_ZH][k] >> CS[ID_ttH][k] >> CS[ID_Total][k];

  }
  file.close();

  file.open("../txtFiles/HiggsCS_ErrorPlus_Official.txt");//directory of input file                       
  for(int k = 0; k < N_CS; k++){

    file >> scratchMass >> CSerrPlus[ID_ggToH][k] >> CSerrPlus[ID_VBF][k] >> CSerrPlus[ID_WH][k] >> CSerrPlus[ID_ZH][k] >> CSerrPlus[ID_ttH][k];

  }
  file.close();

  file.open("../txtFiles/HiggsCS_ErrorMinus_Official.txt");//directory of input file                                                
  for(int k = 0; k < N_CS; k++){

    file >> scratchMass >> CSerrMinus[ID_ggToH][k] >> CSerrMinus[ID_VBF][k] >> CSerrMinus[ID_WH][k] >> CSerrMinus[ID_ZH][k] >> CSerrMinus[ID_ttH][k];

  }
  file.close();

  file.open("../txtFiles/HiggsCS_ScaleErrorPlus_Official.txt");//directory of input file                                                
  for(int k = 0; k < N_CS; k++){

    file >> scratchMass >> CSscaleErrPlus[ID_ggToH][k] >> CSscaleErrPlus[ID_VBF][k] >> CSscaleErrPlus[ID_WH][k] >> CSscaleErrPlus[ID_ZH][k] >> CSscaleErrPlus[ID_ttH][k];

  }
  file.close();

  file.open("../txtFiles/HiggsCS_ScaleErrorMinus_Official.txt");//directory of input file                                     
  for(int k = 0; k < N_CS; k++){

    file >> scratchMass >> CSscaleErrMinus[ID_ggToH][k] >> CSscaleErrMinus[ID_VBF][k] >> CSscaleErrMinus[ID_WH][k] >> CSscaleErrMinus[ID_ZH][k] >> CSscaleErrMinus[ID_ttH][k];

  }
  file.close();

  file.open("../txtFiles/HiggsCS_PdfErrorPlus_Official.txt");//directory of input file                  
  for(int k = 0; k < N_CS; k++){

    file >> scratchMass >> CSpdfErrPlus[ID_ggToH][k] >> CSpdfErrPlus[ID_VBF][k] >> CSpdfErrPlus[ID_WH][k] >> CSpdfErrPlus[ID_ZH][k] >> CSpdfErrPlus[ID_ttH][k];

  }
  file.close();

  file.open("../txtFiles/HiggsCS_PdfErrorMinus_Official.txt");//directory of input file                           
  for(int k = 0; k < N_CS; k++){

    file >> scratchMass >> CSpdfErrMinus[ID_ggToH][k] >> CSpdfErrMinus[ID_VBF][k] >> CSpdfErrMinus[ID_WH][k] >> CSpdfErrMinus[ID_ZH][k] >> CSpdfErrMinus[ID_ttH][k];

  }
  file.close();


  // ---------------- Read 8 TeV CS into memory ------------------ //         
  file.open("../txtFiles/8TeV-ggH.txt");//directory of input file
  for(int k = 0; k < N_CSggToH_8tev; k++){

    file >> mass_XS_8tev[ID_ggToH][k] >> CS_8tev[ID_ggToH][k] >> CSerrPlus_8tev[ID_ggToH][k] >> CSerrMinus_8tev[ID_ggToH][k] 
	 >> CSscaleErrPlus_8tev[ID_ggToH][k] >> CSscaleErrMinus_8tev[ID_ggToH][k] >> CSpdfErrPlus_8tev[ID_ggToH][k] >> CSpdfErrMinus_8tev[ID_ggToH][k];
  
  }
  file.close();

  file.open("../txtFiles/8TeV-vbfH.txt");//directory of input file
  for(int k = 0; k < N_CSvbf_8tev; k++){

    file >> mass_XS_8tev[ID_VBF][k] >> CS_8tev[ID_VBF][k] >> CSerrPlus_8tev[ID_VBF][k] >> CSerrMinus_8tev[ID_VBF][k] >> CSscaleErrPlus_8tev[ID_VBF][k]
	 >> CSscaleErrMinus_8tev[ID_VBF][k] >> CSpdfErrPlus_8tev[ID_VBF][k] >> CSpdfErrMinus_8tev[ID_VBF][k];

  }
  file.close();

  file.open("../txtFiles/8TeV-ttH.txt");//directory of input file
  for(int k = 0; k < N_CSttH_8tev; k++){

    file >> mass_XS_8tev[ID_ttH][k] >> CS_8tev[ID_ttH][k] >> CSerrPlus_8tev[ID_ttH][k] >> CSerrMinus_8tev[ID_ttH][k] >> CSscaleErrPlus_8tev[ID_ttH][k]
	 >> CSscaleErrMinus_8tev[ID_ttH][k] >> CSpdfErrPlus_8tev[ID_ttH][k] >> CSpdfErrMinus_8tev[ID_ttH][k];

  }
  file.close();

  file.open("../txtFiles/8TeV-ZH.txt");//directory of input file
  for(int k = 0; k < N_CSZH_8tev; k++){

    file >> mass_XS_8tev[ID_ZH][k] >> CS_8tev[ID_ZH][k] >> CSerrPlus_8tev[ID_ZH][k] >> CSerrMinus_8tev[ID_ZH][k] >> CSscaleErrPlus_8tev[ID_ZH][k]
	 >> CSscaleErrMinus_8tev[ID_ZH][k] >> CSpdfErrPlus_8tev[ID_ZH][k] >> CSpdfErrMinus_8tev[ID_ZH][k];
  }
  file.close();

  file.open("../txtFiles/8TeV-WH.txt");//directory of input file
  for(int k = 0; k < N_CSWH_8tev; k++){

    file >> mass_XS_8tev[ID_WH][k] >> CS_8tev[ID_WH][k] >> CSerrPlus_8tev[ID_WH][k] >> CSerrMinus_8tev[ID_WH][k] >> CSscaleErrPlus_8tev[ID_WH][k]
	 >> CSscaleErrMinus_8tev[ID_WH][k] >> CSpdfErrPlus_8tev[ID_WH][k] >> CSpdfErrMinus_8tev[ID_WH][k];
  }
  file.close();

}


HiggsCSandWidth::~HiggsCSandWidth()
{
  //destructor

}


//Higgs CS takes process ID, higgs mass mH, and COM energy sqrts in TeV 
double HiggsCSandWidth::HiggsCS(int ID, double mH, double sqrts){

  /**********IDs*************/ 
  /*     ggToH = 1          */
  /*       VBF = 2          */ 
  /*        WH = 3          */ 
  /*        ZH = 4          */
  /*       ttH = 5          */
  /*     Total = 0          */
  /**************************/
 
  int i = 0;
  double reqCS = 0;
  double step = 0;
  int index = 4;
  double xmh[index], sig[index];


  // If ID is unavailable return -1                                                                                                
  if(ID > ID_ttH || ID < ID_Total) return -1;
  // If Ecm is not 7 or 8 TeV return -1
  if(sqrts != 7 && sqrts != 8) return -1;
  //Don't interpolate btw 0 and numbers for mH300
  if(ID > ID_VBF && mH > 300) return 0;

  // If mH is out of range return -1                                           
  // else find what array number to read         
  if( mH < 90 || mH > 1000){ return -1;}
  else{
    
    if(sqrts == 7)
      {
	
	if(mH <= 110 ){step = 5; i = (int)((mH - 90)/step); }
	if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(4 + (mH - 110)/step); }
	if(mH > 140 && mH <= 160 ){step = 1; i = (int)(64 + (mH - 140)/step); }
	if(mH > 160 && mH <= 290 ){step = 2; i = (int)(84 + (mH - 160)/step); }
	if(mH > 290 && mH <= 350 ){step = 5; i = (int)(149 + (mH - 290)/step); }
	if(mH > 350 && mH <= 400 ){step = 10; i = (int)(161 + (mH-350)/step); }
	if(mH > 400){step = 20; i = (int)(166 + (mH-400)/step); }
	
        
	if(i < 1){i = 1;}
	if(i+2 >= N_CS){i = N_CS - 3;}
	xmh[0]=mass_XS[i-1];xmh[1]=mass_XS[i];xmh[2]=mass_XS[i+1];xmh[3]=mass_XS[i+2];
	sig[0]=CS[ID][i-1]; sig[1]=CS[ID][i]; sig[2]=CS[ID][i+1]; sig[3]=CS[ID][i+2];
	
      }
    else if (sqrts == 8)
      {
	
	if(ID == ID_ggToH)
	  {

	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
	    if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
	    if(mH > 290 && mH <= 350 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 350 && mH <= 400 ){step = 10; i = (int)(187 + (mH - 350)/step); }
	    if(mH > 400 && mH <= 1000 ){step = 20; i = (int)(192 + (mH - 400)/step); }
	    	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSggToH_8tev){i = N_CSggToH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CS_8tev[ID][i-1]; sig[1]=CS_8tev[ID][i]; sig[2]=CS_8tev[ID][i+1]; sig[3]=CS_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_VBF)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
	    if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
	    if(mH > 290 && mH <= 350 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 350 && mH <= 400 ){step = 10; i = (int)(187 + (mH-350)/step); }
	    if(mH > 400){step = 20; i = (int)(192 + (mH-400)/step); }
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSvbf_8tev){i = N_CSvbf_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CS_8tev[ID][i-1]; sig[1]=CS_8tev[ID][i]; sig[2]=CS_8tev[ID][i+1]; sig[3]=CS_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_WH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step);}
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSWH_8tev){i = N_CSWH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CS_8tev[ID][i-1]; sig[1]=CS_8tev[ID][i]; sig[2]=CS_8tev[ID][i+1]; sig[3]=CS_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_ZH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step);}
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSZH_8tev){i = N_CSZH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CS_8tev[ID][i-1]; sig[1]=CS_8tev[ID][i]; sig[2]=CS_8tev[ID][i+1]; sig[3]=CS_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_ttH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSttH_8tev){i = N_CSttH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CS_8tev[ID][i-1]; sig[1]=CS_8tev[ID][i]; sig[2]=CS_8tev[ID][i+1]; sig[3]=CS_8tev[ID][i+2];
	    
	  }
	else{ return 0;}
	
      }
    else{cout << "HiggsCSandWidth::HiggsCS --- unknown sqrts! Choose 7 or 8." << endl; return -1;}
  }  
  
  TGraph *graph = new TGraph(index, xmh, sig);
  TSpline3 *gs = new TSpline3("gs",graph);
  gs->Draw();
  reqCS = gs->Eval(mH);
  delete gs;
  delete graph;
  
  return reqCS;
}
  
  

//Higgs CS takes process ID, higgs mass mH, and COM energy sqrts in TeV 
double HiggsCSandWidth::HiggsCSErrPlus(int ID, double mH, double sqrts){

  /**********IDs*************/
  /*     ggToH = 1          */
  /*       VBF = 2          */
  /*        WH = 3          */
  /*        ZH = 4          */
  /*       ttH = 5          */
  /**************************/

  int i = 0;
  double reqCSerr = 0;
  double step = 0;
  int index = 4;
  double xmh[index], sig[index];


  // If ID is unavailable return -1                                                                                    
  if(ID > ID_ttH || ID < ID_Total){return -1;}
  if(ID == ID_Total){return 0;}
  // If Ecm is not 7 or 8 TeV return -1                                                                                                
  if(sqrts != 7 && sqrts != 8){return -1;}
  //Don't interpolate btw 0 and numbers for mH300                        
  if(ID > ID_VBF && mH > 300){return 0;}

  // If mH is out of range return -1                                                                        
  // else find what array number to read                                          
  if( mH < 90 || mH > 1000){return -1;}
  else{

    if(sqrts == 7)
      {
	if(mH <= 110 ){step = 5; i = (int)((mH - 90)/step); }
	if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(4 + (mH - 110)/step); }
	if(mH > 140 && mH <= 160 ){step = 1; i = (int)(64 + (mH - 140)/step); }
	if(mH > 160 && mH <= 290 ){step = 2; i = (int)(84 + (mH - 160)/step);}
	if(mH > 290 && mH <= 350 ){step = 5; i = (int)(149 + (mH - 290)/step); }
	if(mH > 350 && mH <= 400 ){step = 10; i = (int)(161 + (mH-350)/step); }
	if(mH > 400){step = 20; i = (int)(166 + (mH-400)/step); }
	
	
	if(i < 1){i = 1;}
	if(i+2 >= N_CS){i = N_CS - 3;}
	xmh[0]=mass_XS[i-1];xmh[1]=mass_XS[i];xmh[2]=mass_XS[i+1];xmh[3]=mass_XS[i+2];
	sig[0]=CSerrPlus[ID][i-1]; sig[1]=CSerrPlus[ID][i]; sig[2]=CSerrPlus[ID][i+1]; sig[3]=CSerrPlus[ID][i+2];
	
      }
    else if (sqrts == 8)
      {
	
	if(ID == ID_ggToH)
	  {
	    
            if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
            if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
            if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 350 ){step = 5; i = (int)(175 + (mH - 290)/step); }
            if(mH > 350 && mH <= 400 ){step = 10; i = (int)(187 + (mH - 350)/step); }
            if(mH > 400 && mH <= 1000 ){step = 20; i = (int)(192 + (mH - 400)/step); }

	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSggToH_8tev){i = N_CSggToH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSerrPlus_8tev[ID][i-1]; sig[1]=CSerrPlus_8tev[ID][i]; sig[2]=CSerrPlus_8tev[ID][i+1]; sig[3]=CSerrPlus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_VBF)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
	    if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
	    if(mH > 290 && mH <= 350 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 350 && mH <= 400 ){step = 10; i = (int)(187 + (mH-350)/step); }
	    if(mH > 400){step = 20; i = (int)(192 + (mH-400)/step); }
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSvbf_8tev){i = N_CSvbf_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSerrPlus_8tev[ID][i-1]; sig[1]=CSerrPlus_8tev[ID][i]; sig[2]=CSerrPlus_8tev[ID][i+1]; sig[3]=CSerrPlus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_WH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSWH_8tev){i = N_CSWH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSerrPlus_8tev[ID][i-1]; sig[1]=CSerrPlus_8tev[ID][i]; sig[2]=CSerrPlus_8tev[ID][i+1]; sig[3]=CSerrPlus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_ZH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSZH_8tev){i = N_CSZH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSerrPlus_8tev[ID][i-1]; sig[1]=CSerrPlus_8tev[ID][i]; sig[2]=CSerrPlus_8tev[ID][i+1]; sig[3]=CSerrPlus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_ttH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;

	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSttH_8tev){i = N_CSttH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSerrPlus_8tev[ID][i-1]; sig[1]=CSerrPlus_8tev[ID][i]; sig[2]=CSerrPlus_8tev[ID][i+1]; sig[3]=CSerrPlus_8tev[ID][i+2];
	    
	  }
	else{ return 0;}
      }
    else{cout << "HiggsCSandWidth::HiggsCSErrPlus --- unknown sqrts! Choose 7 or 8." << endl; return -1;}
  }
  TGraph *graph = new TGraph(index, xmh, sig);
  TSpline3 *gs = new TSpline3("gs",graph);
  gs->Draw();
  reqCSerr = gs->Eval(mH);
  delete gs;
  delete graph;
  
  reqCSerr *= .01; //Account for percentage  
  
  return reqCSerr;
  
}


//Higgs CS takes process ID, higgs mass mH, and COM energy sqrts in TeV
double HiggsCSandWidth::HiggsCSErrMinus(int ID, double mH, double sqrts){

  /**********IDs*************/
  /*     ggToH = 1          */
  /*       VBF = 2          */
  /*        WH = 3          */
  /*        ZH = 4          */
  /*       ttH = 5          */
  /**************************/

  int i = 0;
  double reqCSerr = 0;
  double step = 0;
  int index = 4;
  double xmh[index], sig[index];


  // If ID is unavailable return -1                                                                                       
  if(ID > ID_ttH || ID < ID_Total){return -1;}
  if(ID == ID_Total){return 0;}
  // If Ecm is not 7 or 8 TeV return -1                                                                                           
  if(sqrts != 7 && sqrts != 8){return -1;}
  //Don't interpolate btw 0 and numbers for mH300                                                        
  if(ID > ID_VBF && mH > 300){return 0;}


  // If mH is out of range return -1                                                                           
  // else find what array number to read                                                                 
  if( mH < 90 || mH > 1000){return -1;}
  else{

    if(sqrts == 7)
      {
	if(mH <= 110 ){step = 5; i = (int)((mH - 90)/step); }
	if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(4 + (mH - 110)/step); }
	if(mH > 140 && mH <= 160 ){step = 1; i = (int)(64 + (mH - 140)/step); }
	if(mH > 160 && mH <= 290 ){step = 2; i = (int)(84 + (mH - 160)/step); }
	if(mH > 290 && mH <= 350 ){step = 5; i = (int)(149 + (mH - 290)/step); }
	if(mH > 350 && mH <= 400 ){step = 10; i = (int)(161 + (mH-350)/step); }
	if(mH > 400){step = 20; i = (int)(166 + (mH-400)/step); }
	
	if(i < 1){i = 1;}
	if(i+2 >= N_CS){i = N_CS - 3;}
	xmh[0]=mass_XS[i-1];xmh[1]=mass_XS[i];xmh[2]=mass_XS[i+1];xmh[3]=mass_XS[i+2];
	sig[0]=CSerrMinus[ID][i-1]; sig[1]=CSerrMinus[ID][i]; sig[2]=CSerrMinus[ID][i+1]; sig[3]=CSerrMinus[ID][i+2];
	
      }
    else if (sqrts == 8)
      {
	
	if(ID == ID_ggToH)
	  {
	    
            if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
            if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
            if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 350 ){step = 5; i = (int)(175 + (mH - 290)/step); }
            if(mH > 350 && mH <= 400 ){step = 10; i = (int)(187 + (mH - 350)/step); }
            if(mH > 400 && mH <= 1000 ){step = 20; i = (int)(192 + (mH - 400)/step); }

	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSggToH_8tev){i = N_CSggToH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSerrMinus_8tev[ID][i-1]; sig[1]=CSerrMinus_8tev[ID][i]; sig[2]=CSerrMinus_8tev[ID][i+1]; sig[3]=CSerrMinus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_VBF)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step);}
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
	    if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
	    if(mH > 290 && mH <= 350 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 350 && mH <= 400 ){step = 10; i = (int)(187 + (mH-350)/step); }
	    if(mH > 400){step = 20; i = (int)(192 + (mH-400)/step); }
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSvbf_8tev){i = N_CSvbf_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSerrMinus_8tev[ID][i-1]; sig[1]=CSerrMinus_8tev[ID][i]; sig[2]=CSerrMinus_8tev[ID][i+1]; sig[3]=CSerrMinus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_WH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSWH_8tev){i = N_CSWH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSerrMinus_8tev[ID][i-1]; sig[1]=CSerrMinus_8tev[ID][i]; sig[2]=CSerrMinus_8tev[ID][i+1]; sig[3]=CSerrMinus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_ZH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step);}
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSZH_8tev){i = N_CSZH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSerrMinus_8tev[ID][i-1]; sig[1]=CSerrMinus_8tev[ID][i]; sig[2]=CSerrMinus_8tev[ID][i+1]; sig[3]=CSerrMinus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_ttH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSttH_8tev){i = N_CSttH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSerrMinus_8tev[ID][i-1]; sig[1]=CSerrMinus_8tev[ID][i]; sig[2]=CSerrMinus_8tev[ID][i+1]; sig[3]=CSerrMinus_8tev[ID][i+2];
	    
	  }
	else{ return 0;}
      }
    else{cout << "HiggsCSandWidth::HiggsCSErrMinus --- unknown sqrts! Choose 7 or 8." << endl; return -1;}

  }
  TGraph *graph = new TGraph(index, xmh, sig);
  TSpline3 *gs = new TSpline3("gs",graph);
  gs->Draw();
  reqCSerr = gs->Eval(mH);
  delete gs;
  delete graph;
  
  reqCSerr *= .01; //Account for percentage  
  
  return reqCSerr;

}

//Higgs CS takes process ID, higgs mass mH, and COM energy sqrts in TeV
double HiggsCSandWidth::HiggsCSscaleErrPlus(int ID, double mH, double sqrts){

  /**********IDs*************/
  /*     ggToH = 1          */
  /*       VBF = 2          */
  /*        WH = 3          */
  /*        ZH = 4          */
  /*       ttH = 5          */
  /**************************/

  int i = 0;
  double reqCSerr = 0;
  double step = 0;
  int index = 4;
  double xmh[index], sig[index];


  // If ID is unavailable return -1                                                         
  if(ID > ID_ttH || ID < ID_Total){return -1;}
  if(ID == ID_Total){return 0;}
  // If Ecm is not 7 or 8 TeV return -1                                                
  if(sqrts != 7 && sqrts != 8){return -1;}
  //Don't interpolate btw 0 and numbers for mH300                                           
  if(ID > ID_VBF && mH > 300){return 0;}

  // If mH is out of range return -1                                                         
  // else find what array number to read                                                      
  if( mH < 90 || mH > 1000){return -1;}
  else{
    
    
    if(sqrts == 7)
      {
	if(mH <= 110 ){step = 5; i = (int)((mH - 90)/step); }
	if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(4 + (mH - 110)/step); }
	if(mH > 140 && mH <= 160 ){step = 1; i = (int)(64 + (mH - 140)/step); }
	if(mH > 160 && mH <= 290 ){step = 2; i = (int)(84 + (mH - 160)/step); }
	if(mH > 290 && mH <= 350 ){step = 5; i = (int)(149 + (mH - 290)/step); }
	if(mH > 350 && mH <= 400 ){step = 10; i = (int)(161 + (mH-350)/step); }
	if(mH > 400){step = 20; i = (int)(166 + (mH-400)/step); }
	
	if(i < 1){i = 1;}
	if(i+2 >= N_CS){i = N_CS - 3;}
	xmh[0]=mass_XS[i-1];xmh[1]=mass_XS[i];xmh[2]=mass_XS[i+1];xmh[3]=mass_XS[i+2];
	sig[0]=CSscaleErrPlus[ID][i-1]; sig[1]=CSscaleErrPlus[ID][i]; sig[2]=CSscaleErrPlus[ID][i+1]; sig[3]=CSscaleErrPlus[ID][i+2];
	
      }
    else if (sqrts == 8)
      {
	
	if(ID == ID_ggToH)
	  {
	    
            if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
            if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
            if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 350 ){step = 5; i = (int)(175 + (mH - 290)/step); }
            if(mH > 350 && mH <= 400 ){step = 10; i = (int)(187 + (mH - 350)/step); }
            if(mH > 400 && mH <= 1000 ){step = 20; i = (int)(192 + (mH - 400)/step); }

	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSggToH_8tev){i = N_CSggToH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSscaleErrPlus_8tev[ID][i-1]; sig[1]=CSscaleErrPlus_8tev[ID][i]; sig[2]=CSscaleErrPlus_8tev[ID][i+1]; sig[3]=CSscaleErrPlus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_VBF)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
	    if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
	    if(mH > 290 && mH <= 350 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 350 && mH <= 400 ){step = 10; i = (int)(187 + (mH-350)/step); }
	    if(mH > 400){step = 20; i = (int)(192 + (mH-400)/step); }
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSvbf_8tev){i = N_CSvbf_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSscaleErrPlus_8tev[ID][i-1]; sig[1]=CSscaleErrPlus_8tev[ID][i]; sig[2]=CSscaleErrPlus_8tev[ID][i+1]; sig[3]=CSscaleErrPlus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_WH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSWH_8tev){i = N_CSWH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSscaleErrPlus_8tev[ID][i-1]; sig[1]=CSscaleErrPlus_8tev[ID][i]; sig[2]=CSscaleErrPlus_8tev[ID][i+1]; sig[3]=CSscaleErrPlus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_ZH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step);}
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSZH_8tev){i = N_CSZH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSscaleErrPlus_8tev[ID][i-1]; sig[1]=CSscaleErrPlus_8tev[ID][i]; sig[2]=CSscaleErrPlus_8tev[ID][i+1]; sig[3]=CSscaleErrPlus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_ttH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;

	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSttH_8tev){i = N_CSttH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSscaleErrPlus_8tev[ID][i-1]; sig[1]=CSscaleErrPlus_8tev[ID][i]; sig[2]=CSscaleErrPlus_8tev[ID][i+1]; sig[3]=CSscaleErrPlus_8tev[ID][i+2];
	    
	  }
	else{ return 0;}
      }
    else{cout << "HiggsCSandWidth::HiggsCSscaleErrPlus --- unknown sqrts! Choose 7 or 8." << endl; return -1;}

  }
  
  TGraph *graph = new TGraph(index, xmh, sig);
  TSpline3 *gs = new TSpline3("gs",graph);
  gs->Draw();
  reqCSerr = gs->Eval(mH);
  delete gs;
  delete graph;
  
  reqCSerr *= .01; //Account for percentage  
  
  return reqCSerr;
  
}

//Higgs CS takes process ID, higgs mass mH, and COM energy sqrts in TeV
double HiggsCSandWidth::HiggsCSscaleErrMinus(int ID, double mH, double sqrts){

  /**********IDs*************/
  /*     ggToH = 1          */
  /*       VBF = 2          */
  /*        WH = 3          */
  /*        ZH = 4          */
  /*       ttH = 5          */
  /**************************/

  int i = 0;
  double reqCSerr = 0;
  double step = 0;
  int index = 4;
  double xmh[index], sig[index];


  // If ID is unavailable return -1                     
  if(ID > ID_ttH || ID < ID_Total){return -1;}
  if(ID == ID_Total){return 0;}
  // If Ecm is not 7 or 8 TeV return -1                                                               
  if(sqrts != 7 && sqrts != 8){return -1;}
  //Don't interpolate btw 0 and numbers for mH300                                   
  if(ID > ID_VBF && mH > 300){return 0;}


  // If mH is out of range return -1                        
  // else find what array number to read                              
  if( mH < 90 || mH > 1000){return -1;}
  else{

    if(sqrts == 7)
      {
	if(mH <= 110 ){step = 5; i = (int)((mH - 90)/step); }
	if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(4 + (mH - 110)/step); }
	if(mH > 140 && mH <= 160 ){step = 1; i = (int)(64 + (mH - 140)/step); }
	if(mH > 160 && mH <= 290 ){step = 2; i = (int)(84 + (mH - 160)/step); }
	if(mH > 290 && mH <= 350 ){step = 5; i = (int)(149 + (mH - 290)/step); }
	if(mH > 350 && mH <= 400 ){step = 10; i = (int)(161 + (mH-350)/step); }
	if(mH > 400){step = 20; i = (int)(166 + (mH-400)/step); }
	
	if(i < 1){i = 1;}
	if(i+2 >= N_CS){i = N_CS - 3;}
	xmh[0]=mass_XS[i-1];xmh[1]=mass_XS[i];xmh[2]=mass_XS[i+1];xmh[3]=mass_XS[i+2];
	sig[0]=CSscaleErrMinus[ID][i-1]; sig[1]=CSscaleErrMinus[ID][i]; sig[2]=CSscaleErrMinus[ID][i+1]; sig[3]=CSscaleErrMinus[ID][i+2];
	
      }
    else if (sqrts == 8)
      {
	
	if(ID == ID_ggToH)
	  {
	    
            if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
            if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
            if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 350 ){step = 5; i = (int)(175 + (mH - 290)/step); }
            if(mH > 350 && mH <= 400 ){step = 10; i = (int)(187 + (mH - 350)/step); }
            if(mH > 400 && mH <= 1000 ){step = 20; i = (int)(192 + (mH - 400)/step); }

	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSggToH_8tev){i = N_CSggToH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSscaleErrMinus_8tev[ID][i-1]; sig[1]=CSscaleErrMinus_8tev[ID][i]; sig[2]=CSscaleErrMinus_8tev[ID][i+1]; sig[3]=CSscaleErrMinus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_WH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSWH_8tev){i = N_CSWH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSscaleErrMinus_8tev[ID][i-1]; sig[1]=CSscaleErrMinus_8tev[ID][i]; sig[2]=CSscaleErrMinus_8tev[ID][i+1]; sig[3]=CSscaleErrMinus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_ZH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSZH_8tev){i = N_CSZH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSscaleErrMinus_8tev[ID][i-1]; sig[1]=CSscaleErrMinus_8tev[ID][i]; sig[2]=CSscaleErrMinus_8tev[ID][i+1]; sig[3]=CSscaleErrMinus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_ttH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;

	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSttH_8tev){i = N_CSttH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSscaleErrMinus_8tev[ID][i-1]; sig[1]=CSscaleErrMinus_8tev[ID][i]; sig[2]=CSscaleErrMinus_8tev[ID][i+1]; sig[3]=CSscaleErrMinus_8tev[ID][i+2];
	    
	  }
	else{ return 0;}
      }
    else{cout << "HiggsCSandWidth::HiggsCSscaleErrMinus --- unknown sqrts! Choose 7 or 8." << endl; return -1;}
  }
  
  TGraph *graph = new TGraph(index, xmh, sig);
  TSpline3 *gs = new TSpline3("gs",graph);
  gs->Draw();
  reqCSerr = gs->Eval(mH);
  delete gs;
  delete graph;
  
  reqCSerr *= .01; //Account for percentage  
  
  return reqCSerr;

}


//Higgs CS takes process ID, higgs mass mH, and COM energy sqrts in TeV
double HiggsCSandWidth::HiggsCSpdfErrPlus(int ID, double mH, double sqrts){

  /**********IDs*************/
  /*     ggToH = 1          */
  /*       VBF = 2          */
  /*        WH = 3          */
  /*        ZH = 4          */
  /*       ttH = 5          */
  /**************************/

  int i = 0;
  double reqCSerr = 0;
  double step = 0;
  int index = 4;
  double xmh[index], sig[index];


  // If ID is unavailable return -1                                                                           
  if(ID > ID_ttH || ID < ID_Total){return -1;}
  if(ID == ID_Total){return 0;}
  // If Ecm is not 7 or 8 TeV return -1                                                                                         
  if(sqrts != 7 && sqrts != 8){return -1;}
  //Don't interpolate btw 0 and numbers for mH300                                                  
  if(ID > ID_VBF && mH > 300){return 0;}

  // If mH is out of range return -1                                                                                  
  // else find what array number to read                                                              
  if( mH < 90 || mH > 1000){return -1;}
  else{

    if(sqrts == 7)
      {
	if(mH <= 110 ){step = 5; i = (int)((mH - 90)/step); }
	if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(4 + (mH - 110)/step); }
	if(mH > 140 && mH <= 160 ){step = 1; i = (int)(64 + (mH - 140)/step); }
	if(mH > 160 && mH <= 290 ){step = 2; i = (int)(84 + (mH - 160)/step); }
	if(mH > 290 && mH <= 350 ){step = 5; i = (int)(149 + (mH - 290)/step); }
	if(mH > 350 && mH <= 400 ){step = 10; i = (int)(161 + (mH-350)/step); }
	if(mH > 400){step = 20; i = (int)(166 + (mH-400)/step); }
	
	if(i < 1){i = 1;}
	if(i+2 >= N_CS){i = N_CS - 3;}
	xmh[0]=mass_XS[i-1];xmh[1]=mass_XS[i];xmh[2]=mass_XS[i+1];xmh[3]=mass_XS[i+2];
	sig[0]=CSpdfErrPlus[ID][i-1]; sig[1]=CSpdfErrPlus[ID][i]; sig[2]=CSpdfErrPlus[ID][i+1]; sig[3]=CSpdfErrPlus[ID][i+2];
	
      }
    else if (sqrts == 8)
      {
	
	if(ID == ID_ggToH)
	  {
	    
            if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
            if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
            if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 350 ){step = 5; i = (int)(175 + (mH - 290)/step); }
            if(mH > 350 && mH <= 400 ){step = 10; i = (int)(187 + (mH - 350)/step); }
            if(mH > 400 && mH <= 1000 ){step = 20; i = (int)(192 + (mH - 400)/step); }

	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSggToH_8tev){i = N_CSggToH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSpdfErrPlus_8tev[ID][i-1]; sig[1]=CSpdfErrPlus_8tev[ID][i]; sig[2]=CSpdfErrPlus_8tev[ID][i+1]; sig[3]=CSpdfErrPlus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_VBF)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
	    if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
	    if(mH > 290 && mH <= 350 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 350 && mH <= 400 ){step = 10; i = (int)(187 + (mH-350)/step); }
	    if(mH > 400){step = 20; i = (int)(192 + (mH-400)/step); }
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSvbf_8tev){i = N_CSvbf_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSpdfErrPlus_8tev[ID][i-1]; sig[1]=CSpdfErrPlus_8tev[ID][i]; sig[2]=CSpdfErrPlus_8tev[ID][i+1]; sig[3]=CSpdfErrPlus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_WH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSWH_8tev){i = N_CSWH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSpdfErrPlus_8tev[ID][i-1]; sig[1]=CSpdfErrPlus_8tev[ID][i]; sig[2]=CSpdfErrPlus_8tev[ID][i+1]; sig[3]=CSpdfErrPlus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_ZH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSZH_8tev){i = N_CSZH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSpdfErrPlus_8tev[ID][i-1]; sig[1]=CSpdfErrPlus_8tev[ID][i]; sig[2]=CSpdfErrPlus_8tev[ID][i+1]; sig[3]=CSpdfErrPlus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_ttH)
	  { 
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;

	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSttH_8tev){i = N_CSttH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSpdfErrPlus_8tev[ID][i-1]; sig[1]=CSpdfErrPlus_8tev[ID][i]; sig[2]=CSpdfErrPlus_8tev[ID][i+1]; sig[3]=CSpdfErrPlus_8tev[ID][i+2];
	    
	  }
	else{ return 0;}
      }
    else{cout << "HiggsCSandWidth::HiggsCSpdfErrPlus --- unknown sqrts! Choose 7 or 8." << endl; return -1;}

  }
  
  TGraph *graph = new TGraph(index, xmh, sig);
  TSpline3 *gs = new TSpline3("gs",graph);
  gs->Draw();
  reqCSerr = gs->Eval(mH);
  delete gs;
  delete graph;
  
  reqCSerr *= .01; //Account for percentage  
  
  return reqCSerr;


}


//Higgs CS takes process ID, higgs mass mH, and COM energy sqrts in TeV
double HiggsCSandWidth::HiggsCSpdfErrMinus(int ID, double mH, double sqrts){

  /**********IDs*************/
  /*     ggToH = 1          */
  /*       VBF = 2          */
  /*        WH = 3          */
  /*        ZH = 4          */
  /*       ttH = 5          */
  /**************************/

  int i = 0;
  double reqCSerr = 0;
  double step = 0;
  int index = 4;
  double xmh[index], sig[index];


  // If ID is unavailable return -1                           
  if(ID > ID_ttH || ID < ID_Total){return -1;}
  if(ID == ID_Total){return 0;}
  // If Ecm is not 7 or 8 TeV return -1                                                                 
  if(sqrts != 7 && sqrts != 8){return -1;}
  //Don't interpolate btw 0 and numbers for mH300             
  if(ID > ID_VBF && mH > 300){return 0;}


  // If mH is out of range return -1                                                              
  // else find what array number to read                            
  if( mH < 90 || mH > 1000){return -1;}
  else{

    if(sqrts == 7)
      {
	if(mH <= 110 ){step = 5; i = (int)((mH - 90)/step); }
	if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(4 + (mH - 110)/step); }
	if(mH > 140 && mH <= 160 ){step = 1; i = (int)(64 + (mH - 140)/step); }
	if(mH > 160 && mH <= 290 ){step = 2; i = (int)(84 + (mH - 160)/step); }
	if(mH > 290 && mH <= 350 ){step = 5; i = (int)(149 + (mH - 290)/step);}
	if(mH > 350 && mH <= 400 ){step = 10; i = (int)(161 + (mH-350)/step); }
	if(mH > 400){step = 20; i = (int)(166 + (mH-400)/step); }
	
	if(i < 1){i = 1;}
	if(i+2 >= N_CS){i = N_CS - 3;}
	xmh[0]=mass_XS[i-1];xmh[1]=mass_XS[i];xmh[2]=mass_XS[i+1];xmh[3]=mass_XS[i+2];
	sig[0]=CSpdfErrMinus[ID][i-1]; sig[1]=CSpdfErrMinus[ID][i]; sig[2]=CSpdfErrMinus[ID][i+1]; sig[3]=CSpdfErrMinus[ID][i+2];
	
      }
    else if (sqrts == 8)
      {
	
	if(ID == ID_ggToH)
	  {
	    
            if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
            if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step); }
            if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step);}
            if(mH > 290 && mH <= 350 ){step = 5; i = (int)(175 + (mH - 290)/step); }
            if(mH > 350 && mH <= 400 ){step = 10; i = (int)(187 + (mH - 350)/step); }
            if(mH > 400 && mH <= 1000 ){step = 20; i = (int)(192 + (mH - 400)/step); }

	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSggToH_8tev){i = N_CSggToH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSpdfErrMinus_8tev[ID][i-1]; sig[1]=CSpdfErrMinus_8tev[ID][i]; sig[2]=CSpdfErrMinus_8tev[ID][i+1]; sig[3]=CSpdfErrMinus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_WH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step);}
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step);}
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step);}
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSWH_8tev){i = N_CSWH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSpdfErrMinus_8tev[ID][i-1]; sig[1]=CSpdfErrMinus_8tev[ID][i]; sig[2]=CSpdfErrMinus_8tev[ID][i+1]; sig[3]=CSpdfErrMinus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_ZH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step);}
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSZH_8tev){i = N_CSZH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSpdfErrMinus_8tev[ID][i-1]; sig[1]=CSpdfErrMinus_8tev[ID][i]; sig[2]=CSpdfErrMinus_8tev[ID][i+1]; sig[3]=CSpdfErrMinus_8tev[ID][i+2];
	    
	  }
	else if(ID == ID_ttH)
	  {
	    if(mH <= 110 ){step = 1; i = (int)((mH - 80)/step); }
	    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(30 + (mH - 110)/step);}
	    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(90 + (mH - 140)/step); }
            if(mH > 160 && mH <= 290 ){step = 2; i = (int)(110 + (mH - 160)/step); }
            if(mH > 290 && mH <= 300 ){step = 5; i = (int)(175 + (mH - 290)/step); }
	    if(mH > 300) return 0;
	    
	    if(i < 1){i = 1;}
	    if(i+2 >= N_CSttH_8tev){i = N_CSttH_8tev - 3;}
	    xmh[0]=mass_XS_8tev[ID][i-1];xmh[1]=mass_XS_8tev[ID][i];xmh[2]=mass_XS_8tev[ID][i+1];xmh[3]=mass_XS_8tev[ID][i+2];
	    sig[0]=CSpdfErrMinus_8tev[ID][i-1]; sig[1]=CSpdfErrMinus_8tev[ID][i]; sig[2]=CSpdfErrMinus_8tev[ID][i+1]; sig[3]=CSpdfErrMinus_8tev[ID][i+2];
	    
	  }
	else{ return 0;}
      }
    else{cout << "HiggsCSandWidth::HiggsCSpdfErrMinus --- unknown sqrts! Choose 7 or 8." << endl; return -1;}

  }
  
  TGraph *graph = new TGraph(index, xmh, sig);
  TSpline3 *gs = new TSpline3("gs",graph);
  gs->Draw();
  reqCSerr = gs->Eval(mH);
  delete gs;
  delete graph;
  
  reqCSerr *= .01; //Account for percentage  
  
  return reqCSerr;

}



// HiggsWidth takes process ID and higgs mass mH
double HiggsCSandWidth::HiggsWidth(int ID, double mH){


  /***********************IDs************************/
  /*                       Total = 0                */
  /*                       H->bb = 1                */
  /*                   H->tautau = 2                */
  /*                     H->mumu = 3                */
  /*                       H->ss = 4                */
  /*                       H->cc = 5                */
  /*                       H->tt = 6                */
  /*                       H->gg = 7                */
  /*                   H->gamgam = 8                */
  /*                     H->gamZ = 9                */
  /*                       H->WW = 10               */
  /*                       H->ZZ = 11               */
  /*                       H->4e = 12               */
  /*                    H->2e2mu = 13               */
  /*              H->4lep (e,mu) = 14               */
  /*          H->4lep (e,mu,tau) = 15               */
  /*                H->e+nu e-nu = 16               */
  /*               H->e+nu mu-nu = 17               */
  /*    H->2l2nu(l=e,mu)(nu=any) = 18               */
  /* H->2l2nu(l=e,mu,tau)(nu=any) = 19              */  
  /*    H->2l2q (l=e,mu)(q=udcsb) = 20              */
  /* H->2l2q(l=e,mu,tau)(q=udcsb) = 21              */
  /* H->l+nu qq(*) (l=e,mu)(q=udcsb) = 22           */
  /*  H->2nu2q (nu=any)(q=udcsb) = 23               */
  /*            H->4q (q=udcsb) = 24                */
  /*      H->4f (f=any fermion) = 25                */
  /**************************************************/



  double TotalWidth = 0;
  double PartialWidth = 0;
  double Width = 0;
  int i = 0;
  double step;

  // If ID is unavailable return -1                                           
  if(ID > 25 || ID < 0){return -1;}


  // If mH is out of range return -1                                            
  // else find what array number to read                                        
  if( mH < 90 || mH > 1000){return -1;}
  else{

    //Find index and closest higgs mass for which we have numbers
    if(mH <=110 ){step = 5; i = (int)((mH - 90)/step);}
    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(4 + (mH-110)/step);}
    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(64 + (mH-140)/step); }
    if(mH > 160 && mH <= 290 ){step = 2; i = (int)(84 + (mH-160)/step); }
    if(mH > 290 && mH <= 350 ){step = 5; i = (int)(149 + (mH-290)/step); }
    if(mH > 350 && mH <= 400 ){step = 10; i = (int)(161 + (mH-350)/step); }
    if(mH > 400 && mH <= 600 ){step = 20; i = (int)(166 + (mH-400)/step); }
    if(mH > 600){step = 10; i = (int)(176 + (mH-600)/step); }


    if( ID == 0 )
      {
	if(i < 1){i = 1;}
	if(i+2 >= N_BR){i = N_BR - 3;}
	int indexW = 4;
	double xmhW[indexW], sigW[indexW];
	xmhW[0]=mass_BR[i-1];xmhW[1]=mass_BR[i];xmhW[2]=mass_BR[i+1];xmhW[3]=mass_BR[i+2];
	sigW[0]=BR[ID][i-1]; sigW[1]=BR[ID][i]; sigW[2]=BR[ID][i+1]; sigW[3]=BR[ID][i+2];
	
	TGraph *graphW = new TGraph(indexW, xmhW, sigW);
	TSpline3 *gsW = new TSpline3("gsW",graphW);
	gsW->Draw();
	Width = gsW->Eval(mH);
	delete gsW;
	delete graphW;
      }
    else{
      if(i < 1){i = 1;}
      if(i+2 >= N_BR){i = N_BR - 3;}
      
      int indexW = 4;
      double xmhW[indexW], sigW[indexW];
      xmhW[0]=mass_BR[i-1];xmhW[1]=mass_BR[i];xmhW[2]=mass_BR[i+1];xmhW[3]=mass_BR[i+2];
      sigW[0]=BR[0][i-1]; sigW[1]=BR[0][i]; sigW[2]=BR[0][i+1]; sigW[3]=BR[0][i+2];
      
      TGraph *graphW = new TGraph(indexW, xmhW, sigW);
      TSpline3 *gsW = new TSpline3("gsW",graphW);
      gsW->Draw();
      PartialWidth = gsW->Eval(mH);
      delete gsW;
      delete graphW;
      
      int indexPW = 4;
      double xmhPW[indexPW], sigPW[indexPW];
      xmhPW[0]=mass_BR[i-1];xmhPW[1]=mass_BR[i];xmhPW[2]=mass_BR[i+1];xmhPW[3]=mass_BR[i+2];
      sigPW[0]=BR[ID][i-1]; sigPW[1]=BR[ID][i]; sigPW[2]=BR[ID][i+1]; sigPW[3]=BR[ID][i+2];
      
      TGraph *graphPW = new TGraph(indexPW, xmhPW, sigPW);
      TSpline3 *gsPW = new TSpline3("gsPW",graphPW);
      gsPW->Draw();
      PartialWidth *= gsPW->Eval(mH);
      delete gsPW;
      delete graphPW;
      
      Width = PartialWidth;
      
    }
    
  }
  
  return Width;
  
} 


// HiggsWidth takes process ID and higgs mass mH
double HiggsCSandWidth::HiggsBR(int ID, double mH){


  /***********************IDs************************/
  /*                       Total = 0                */
  /*                       H->bb = 1                */
  /*                   H->tautau = 2                */
  /*                     H->mumu = 3                */
  /*                       H->ss = 4                */
  /*                       H->cc = 5                */
  /*                       H->tt = 6                */
  /*                       H->gg = 7                */
  /*                   H->gamgam = 8                */
  /*                     H->gamZ = 9                */
  /*                       H->WW = 10               */
  /*                       H->ZZ = 11               */
  /*                       H->4e = 12               */
  /*                    H->2e2mu = 13               */
  /*              H->4lep (e,mu) = 14               */
  /*          H->4lep (e,mu,tau) = 15               */
  /*                H->e+nu e-nu = 16               */
  /*               H->e+nu mu-nu = 17               */
  /*    H->2l2nu(l=e,mu)(nu=any) = 18               */
  /* H->2l2nu(l=e,mu,tau)(nu=any) = 19              */  
  /*    H->2l2q (l=e,mu)(q=udcsb) = 20              */
  /* H->2l2q(l=e,mu,tau)(q=udcsb) = 21              */
  /* H->l+nu qq(*) (l=e,mu)(q=udcsb) = 22           */
  /*  H->2nu2q (nu=any)(q=udcsb) = 23               */
  /*            H->4q (q=udcsb) = 24                */
  /*      H->4f (f=any fermion) = 25                */
  /**************************************************/



  double PartialBR = 0;
  double BranchRatio = 0;
  int i = 0;
  double step;

  // If ID is unavailable return -1                                           
  if(ID > 25 || ID < 1){return -1;}


  // If mH is out of range return -1                                            
  // else find what array number to read                                        
  if( mH < 90 || mH > 1000){return -1;}
  else{

    //Find index and closest higgs mass for which we have numbers
    if(mH <=110 ){step = 5; i = (int)((mH - 90)/step); }
    if(mH > 110 && mH <= 140 ){step = 0.5; i = (int)(4 + (mH-110)/step); }
    if(mH > 140 && mH <= 160 ){step = 1; i = (int)(64 + (mH-140)/step); }
    if(mH > 160 && mH <= 290 ){step = 2; i = (int)(84 + (mH-160)/step); }
    if(mH > 290 && mH <= 350 ){step = 5; i = (int)(149 + (mH-290)/step);}
    if(mH > 350 && mH <= 400 ){step = 10; i = (int)(161 + (mH-350)/step);}
    if(mH > 400 && mH <= 600 ){step = 20; i = (int)(166 + (mH-400)/step); }
    if(mH > 600){step = 10; i = (int)(176 + (mH-600)/step); }

    
    if(i < 1){i = 1;}
    if(i+2 >= N_BR){i = N_BR - 3;}
    int indexBR = 4;
    double xmhBR[indexBR], sigBR[indexBR];
    xmhBR[0]=mass_BR[i-1];xmhBR[1]=mass_BR[i];xmhBR[2]=mass_BR[i+1];xmhBR[3]=mass_BR[i+2];
    sigBR[0]=BR[ID][i-1]; sigBR[1]=BR[ID][i]; sigBR[2]=BR[ID][i+1]; sigBR[3]=BR[ID][i+2];
    
    TGraph *graphBR = new TGraph(indexBR, xmhBR, sigBR);
    TSpline3 *gsBR = new TSpline3("gsBR",graphBR);
    gsBR->Draw();
    PartialBR = gsBR->Eval(mH);
    delete gsBR;
    delete graphBR;
    
    BranchRatio = PartialBR;
    
  }

  return BranchRatio;

} 





#endif

void chi2_beta_maxime(){
  
  const Char_t* fDataBetaCen = "UnfoldNormDataBetaCen.dat";
  const Char_t* fDataBetaFor = "UnfoldNormDataBetaFor.dat";

  const Char_t* fPythia6BetaCen = "NormUnfoldPythia6BetaCen.dat";
  const Char_t* fPythia6BetaFor = "NormUnfoldPythia6BetaFor.dat";
  
  const Char_t* fPythia8BetaCen = "NormUnfoldPythia8BetaCen.dat";
  const Char_t* fPythia8BetaFor = "NormUnfoldPythia8BetaFor.dat";
  
  const Char_t* fHerwigBetaCen = "NormUnfoldHerwigBetaCen.dat";
  const Char_t* fHerwigBetaFor = "NormUnfoldHerwigBetaFor.dat";

  const Char_t* fMadgraphBetaCen = "NormUnfoldMadgraphBetaCen.dat";
  const Char_t* fMadgraphBetaFor = "NormUnfoldMadgraphBetaFor.dat";

  
  //Data
  double vDCen[18]={0.};
  double vDFor[18]={0.};
  //Pythia6
  double vP6Cen[18]={0.};
  double vP6For[18]={0.};
  //Pythia8
  double vP8Cen[18]={0.};
  double vP8For[18]={0.};
  //Herwig++
  double vHCen[18]={0.};
  double vHFor[18]={0.};
 //Madgraph
  double vMCen[18]={0.};
  double vMFor[18]={0.};


  ifstream in0;
  ifstream in2;

  ifstream in4;
  ifstream in6;

  ifstream in8;
  ifstream in10;

  ifstream in12;
  ifstream in14;

  ifstream in16;
  ifstream in18;

  in0.open( fPythia6BetaCen, ios::in);
  in2.open( fPythia6BetaFor, ios::in); 
  
  in4.open( fPythia8BetaCen, ios::in); 
  in6.open( fPythia8BetaFor, ios::in);
  
  in8.open( fHerwigBetaCen, ios::in);
  in10.open( fHerwigBetaFor, ios::in); 

  in12.open( fDataBetaCen, ios::in); 
  in14.open( fDataBetaFor, ios::in); 

  in16.open( fMadgraphBetaCen, ios::in);
  in18.open( fMadgraphBetaFor, ios::in); 
  
  for(int i=0;i<18;i++){
    //Pythia6
    in0 >> vP6Cen[i];
    in2 >> vP6For[i];
    //Pythia8
    in4 >> vP8Cen[i];
    in6 >> vP8For[i];
    //Herwig++
    in8 >> vHCen[i];
    in10 >> vHFor[i];
    //Data
    in12 >> vDCen[i];
    in14 >> vDFor[i];
    //Madgraph
    in16 >> vMCen[i];
    in18 >> vMFor[i];
  }
   
  ///// Start by building the M Matrix and his Transpose
  ///// First build the array with beta_data - beta_MC
  
  double beta_data[18]={0.};  //Fill here the 18 beta values DATA !!!!
  double beta_MC[18]={0.};    //Fill here the 18 beta values MC !!!!

#if 1
  //Central
  for(int i=0;i<18;i++){
    beta_data[i] = vDCen[i];
    beta_MC[i] = vP6Cen[i];
    //    beta_MC[i] = vMCen[i];
    //beta_MC[i] = vP8Cen[i];
    //beta_MC[i] = vHCen[i];
    std::cout << "beta_data[" << i << "]: " << beta_data[i] << std::endl;
    std::cout << "beta_MC[" << i << "]: " << beta_MC[i] << std::endl;
  }
#else
  //Forward
  for(int i=0;i<18;i++){
    beta_data[i] = vDFor[i];
    //    beta_MC[i] = vP6For[i];
    //    beta_MC[i] = vMFor[i];
    //    beta_MC[i] = vP8For[i];
    beta_MC[i] = vHFor[i];

    //    std::cout << "beta_data[" << i << "]: " << beta_data[i] << std::endl;
    //    std::cout << "beta_MC[" << i << "]: " << beta_MC[i] << std::endl;
  }
#endif  

  double data_MC[18];
  for(int j=0;j<18;++j){data_MC[j]=beta_data[j]-beta_MC[j];}
  
  
  TMatrixD M_data_MC(18,1); 
  for(int j=0;j<18;++j){M_data_MC(j,0)=data_MC[j];}
  
  
  TMatrixD M_data_MC_Transpose(1,18); 
  for(int j=0;j<18;++j){M_data_MC_Transpose(0,j)=data_MC[j];}
  
  
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////// Build Stat cov matrix 
#if 1
  //// Assume no correlation
  double stat_uncertainty=0.040;  //Use here value of table 2 of paper 
  double sigma_data_cen[18] = {0.0019, 0.0021, 0.0023, 0.0022, 0.0020, 0.0023, 0.0022, 0.0021, 0.0022, 0.0022, 
			       0.0022, 0.0022, 0.0022, 0.0025, 0.0024, 0.0022, 0.0024, 0.0020};
  double sigma_data_for[18] = {0.0013, 0.0016, 0.0015, 0.0017, 0.0017, 0.0017, 0.0019, 0.0021, 0.0022, 0.0022, 
			       0.0023, 0.0024, 0.0023, 0.0025, 0.0025, 0.0025, 0.0026, 0.0023};
  TMatrixD Cov_STAT(18,18);
  TMatrixD Cov_STAT(18,18);
  for(int i=0;i<18;++i){
    for(int j=0;j<18;++j){
      if(i==j){
	//	Cov_STAT(i,j) = (stat_uncertainty*beta_data[i])*(stat_uncertainty*beta_data[j]);
	Cov_STAT(i,j) = (sigma_data_cen[i])*(sigma_data_cen[j]);
	cout << "Pull data_MC[" << j << "] = "  << data_MC[i]*data_MC[j] / (sigma_data_cen[i]*sigma_data_cen[j]) << endl;
	//	Cov_STAT(i,j) = (sigma_data_for[i])*(sigma_data_for[j]);
      }
      else{
	Cov_STAT(i,j) = 0.;
      }
    }
  }
#endif

  //// Using correlation matrix from unfolding. This is the proper way!
#if 0
  //Herwig++ Central
  double cor_coef[18][18]=
{{0.0019,  -0.0008,  0.0001,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{-0.0008,  0.0021,  -0.0008,  0.0001,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0001,  -0.0008,  0.0023,  -0.0009,  -0.0001,  0.000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0001,  0.0001,  -0.0009,  0.0022,  -0.0008,  -0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0001,  -0.0001,  -0.0008,  0.0021,  -0.0008,  0.0001,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0001,  -0.0001,  -0.0008,  0.0023,  -0.0009,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000, 0.0000,  0.0000, 0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0001,  0.0001,  -0.0009,  0.0022,  -0.0008,  0.0001,  0.0000,  0.0000,   0.0000,  0.0000,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  0.0001,  -0.0008,  0.0022,  -0.0008,  0.0000,  -0.0000,  -0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  0.0001,  -0.0008,  0.0022,  -0.0008,  -0.0001,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  0.0000,  -0.0008,  0.0022,  -0.0008,  0.0000,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  -0.0002,  -0.0001,  -0.0008,  0.0023,  -0.0008,  0.0000,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  -0.0001,  0.0001,  0.0000,  -0.0008,  0.0023,  -0.0008,  0.0000,  0.0001,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  0.0000,  -0.0008,  0.0022,  -0.0009,  0.0001,  0.0001,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  0.0000,  -0.0009,  0.0025,  -0.0009,  0.0001,  0.0001,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  0.0001,  -0.0009,  0.0024,  -0.0008,  0.0001,  0.0001,  } 
{0.0000,  0.0000,  0.0000, 0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  0.0001,  -0.0008,  0.0023,  -0.0009,  0.0001,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  0.0001,  -0.0009,  0.0024,  -0.0009,  } 
{0.0000, 0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  0.0001,  -0.0009,  0.0021,  } 
}  
  ;

#else
  //Forward//////////////////////////////////////
  //Herwig++ 
  double cor_coef[18][18]= 
{{0.0013,  -0.0006,  0.0001,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{-0.0006,  0.0016,  -0.0006,  -0.0001,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0001,  -0.0006,  0.0015,  -0.0006,  -0.0002,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0001,  -0.0001,  -0.0006,  0.0017,  -0.0007,  -0.0001,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0001,  -0.0002,  -0.0007,  0.0017,  -0.0007,  -0.0002,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0001,  -0.0001,  -0.0007,  0.0017,  -0.0007,  -0.0001,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0001,  -0.0002,  -0.0007,  0.0020,  -0.0008,  -0.0001,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  -0.0001,  -0.0008,  0.0021,  -0.0008,  -0.0001,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  -0.0001,  -0.0008,  0.0022,  -0.0008,  -0.0001,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  -0.0001,  -0.0008,  0.0023,  -0.0009,  -0.0001,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  -0.0001,  -0.0009,  0.0023,  -0.0009,  0.0000,  0.0001,  0.0000,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  -0.0001,  -0.0009,  0.0024,  -0.0009,  -0.0001,  0.0001,  0.0000,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  0.0000,  -0.0009,  0.0023,  -0.0009,  -0.0001,  0.0001,  0.0000,  0.0000,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  -0.0001,  -0.0009,  0.0025,  -0.0010,  0.0000,  0.0001,  -0.0001,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  -0.0001,  -0.0010,  0.0026,  -0.0010,  -0.0001,  0.0001,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  0.0000,  -0.0010,  0.0025,  -0.0010,  0.0002,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0001,  -0.0001,  -0.0010,  0.0026,  -0.0010,  } 
{0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  0.0000,  -0.0001,  0.0001,  0.0002,  -0.0010,  0.0023,  }}  ;
#endif

#if 0
  ///// Fill here with correlation coefficient matrix from unfolding !!!!
  //  double stat_uncertainty=0.040;  //Use here value of table 2 of paper 
  //
  double sigma_data_cen[18] = {0.0019, 0.0021, 0.0023, 0.0022, 0.0020, 0.0023, 0.0022, 0.0021, 0.0022, 0.0022, 
			       0.0022, 0.0022, 0.0022, 0.0025, 0.0024, 0.0022, 0.0024, 0.0020};
  double sigma_data_for[18] = {0.0013, 0.0016, 0.0015, 0.0017, 0.0017, 0.0017, 0.0019, 0.0021, 0.0022, 0.0022, 
			       0.0023, 0.0024, 0.0023, 0.0025, 0.0025, 0.0025, 0.0026, 0.0023};
  TMatrixD Cov_STAT(18,18);
  for(int i=0;i<18;++i){
    for(int j=0;j<18;++j){
      //      Cov_STAT(i,j) = cor_coef[i][j]*(stat_uncertainty*beta_data[i])*(stat_uncertainty*beta_data[j]);
      //      Cov_STAT(i,j) = cor_coef[i][j]*(beta_data[i])*(beta_data[j]);
      Cov_STAT(i,j) = cor_coef[i][j]*cor_coef[i][j];
      //      Cov_STAT(i,j) = cor_coef[i][j]*sigma_data_cen[j];
      //      Cov_STAT(i,j) = cor_coef[i][j]*sigma_data_for[j];
    }
  }
#endif  

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////// Build JES cov matrix assuming full correlation
  ////// Also treat uncertainty as multiplicative
  
  double jes_uncertainty=0.01;  //Use here value of table 2 of paper
  
  TMatrixD Cov_JES(18,18);
  for(int i=0;i<18;++i){
    for(int j=0;j<18;++j){
      Cov_JES(i,j) =  (jes_uncertainty*beta_MC[i])*(jes_uncertainty*beta_MC[j]);
    }
  }
  
  

  ////// Build JER cov matrix assuming full correlation
  ////// Also treat uncertainty as multiplicative
  double jer_uncertainty=0.004;  //Use here value of table 2 of paper
  
  TMatrixD Cov_JER(18,18);
  for(int i=0;i<18;++i){
    for(int j=0;j<18;++j){
      Cov_JER(i,j) =  (jer_uncertainty*beta_MC[i])*(jer_uncertainty*beta_MC[j]);
    }
  }
  
  
  ////// Build JAR cov matrix assuming full correlation
  ////// Also treat uncertainty as multiplicative
  double jar_uncertainty=0.005;  //Use here value of table 2 of paper
  
  TMatrixD Cov_JAR(18,18);
  for(int i=0;i<18;++i){
    for(int j=0;j<18;++j){
      Cov_JAR(i,j) =  (jar_uncertainty*beta_MC[i])*(jar_uncertainty*beta_MC[j]);
    }
  }
  
  ////// Build Unfolding cov matrix assuming full correlation
  ////// Also treat uncertainty as multiplicative
  double unf_uncertainty=0.006;  //Use here value of table 2 of paper
  //  double unf_uncertainty=6.0;  //Use here value of table 2 of paper

#if 1  
  TMatrixD Cov_UNF(18,18);
   for(int i=0;i<18;++i){
     for(int j=0;j<18;++j){
       Cov_UNF(i,j) =  (unf_uncertainty*beta_MC[i])*(unf_uncertainty*beta_MC[j]);
     }
   }
#else
  TMatrixD Cov_UNF(18,18);
   for(int i=0;i<18;++i){
     for(int j=0;j<18;++j){
       if (i ==j) {
	 Cov_UNF(i,j) =  (unf_uncertainty*beta_MC[i])*(unf_uncertainty*beta_MC[j]);
       }
       else {
	 Cov_UNF(i,j) = 0.;
       }
     }
   }
#endif
   

   //// Building the total Cov matrix and invert
   TMatrixD Cov_ALL(18,18); 
   //   Cov_ALL = Cov_STAT + Cov_JES + Cov_JER + Cov_JAR + Cov_UNF;
   //   Cov_ALL = Cov_JES + Cov_JER + Cov_JAR + Cov_UNF;
   Cov_ALL = Cov_STAT ;
   //Cov_ALL = Cov_STAT + Cov_UNF ;
   Cov_ALL.Invert();
   
   
   //// evaluating chi^2 
   TMatrixD chi_square(1,1);
   chi_square = M_data_MC_Transpose*Cov_ALL*M_data_MC;

   
   printf("chi_square=%f\n",chi_square(0,0));
   
}

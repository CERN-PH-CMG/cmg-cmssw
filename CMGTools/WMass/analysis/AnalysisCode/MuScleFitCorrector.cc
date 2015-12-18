/**
 * MuScleFitCorrector class
 * Author M. De Mattia - 18/11/2008
 * Author S. Casasso   - 25/10/2012
 * Author E. Migliore  - 25/10/2012
 */


#include "MuScleFitCorrector.h"

template <class T>
void MuScleFitCorrector::convertToArrays(T *& function_)
{
  //  bool isScale(true);

  if (resolFunctBase<double *> * checkFunction_ = dynamic_cast<resolFunctBase<double *> *>(function_)){
    // Check for consistency of number of passed parameters and number of required parameters.
    const int parNum = function_->parNum();
    int parVecSize = resolParVec_.size();
    if( 2*parNum != parVecSize ) {
      std::cout << "Error: inconsistent total number of requested parameters("<<2*parNum<<") and parameters read("<<parVecSize<<")" << std::endl;
      exit(1);
    }
    
    std::vector<double>::const_iterator parIt = resolParVec_.begin();
    
    // Loop on the parameters size for each function and create corresponding parameter arrays
    resolDataParArray_ = new double[parNum];
    resolMCParArray_ = new double[parNum];
    
    for ( int par = 0; par < parNum; ++par){
      resolDataParArray_[par]=*parIt;
      ++parIt;
    }

    for ( int par = parNum; par < 2*parNum; ++par){
      resolMCParArray_[par]=*parIt;
      ++parIt;
    }
    
  }// is resolution function
  else{
    // Check for consistency of number of passed parameters and number of required parameters.
    const int parNum = function_->parNum();
    int parVecSize = scaleParVec_.size();
    if( parNum != parVecSize ) {
      std::cout << "Error: inconsistent total number of requested parameters("<<parNum<<") and parameters read("<<parVecSize<<")" << std::endl;
      exit(1);
    }
    
    std::vector<double>::const_iterator parIt = scaleParVec_.begin();
    
    // Loop on the parameters size for each function and create corresponding parameter arrays
    scaleParArray_ = new double[parNum];
    
    for ( int par = 0; par < parNum; ++par){
      scaleParArray_[par]=*parIt;
      ++parIt;
    }

  }//is scale function

  
}


void MuScleFitCorrector::readParameters( TString fileName )
{
  scaleParArray_ = 0;
  resolDataParArray_ = 0;
  resolMCParArray_ = 0;

  // Read the parameters file
  ifstream parametersFile(fileName.Data());

  if( !parametersFile.is_open() ) {
    std::cout << "Error: file " << fileName << " not found. Aborting." << std::endl;
    abort();
  }
  std::string line;
  int nReadPar = 0;
  std::string iteration("Iteration ");
  // Loop on the file lines
  while (parametersFile) {
    getline( parametersFile, line );
    size_t lineInt = line.find("value");
    size_t iterationSubStr = line.find(iteration);

    if( iterationSubStr != std::string::npos ) {
      std::stringstream sLine(line);
      std::string num;
      int scaleFunctNum = 0;
      int resolFunctNum = 0;
      int wordCounter = 0;

      // Warning: this strongly depends on the parameters file structure.                                                                                   
      while( sLine >> num ) {
	++wordCounter;
	if( wordCounter == 8 ) {
	  std::stringstream in(num);
	  in >> resolFunctNum;
	}
	
	if( wordCounter == 9 ) {
	  std::stringstream in(num);
	  in >> scaleFunctNum;
	}
      }
            
      scaleFunctId_ = scaleFunctNum;
      scaleFunct_ = scaleFunctService(scaleFunctNum);
      resolFunctId_ = resolFunctNum;
      resolFunct_ = resolutionFunctService(resolFunctNum);

//       std::cout<<"Function IDs: "<<std::endl;
//       std::cout<<"     scale function number "<<scaleFunctId_<<std::endl;
//       std::cout<<"     resolution function number "<<resolFunctId_<<std::endl;

    }
        
    int nScalePar = scaleFunct_->parNum();
    int nResolPar = resolFunct_->parNum();

    if ( (lineInt != std::string::npos) ) {
      size_t subStr1 = line.find("value");
      std::stringstream paramStr;
      double param = 0;
      paramStr << line.substr(subStr1+5);
      paramStr >> param;
      // Fill the last vector of parameters, which corresponds to this iteration.
      if (nReadPar<nScalePar) scaleParVec_.push_back(param);
      else if (nReadPar < (nScalePar+2*nResolPar) ) resolParVec_.push_back(param);
      ++nReadPar;
      //      std::cout << "param n."<< nReadPar << " = " << param << std::endl;
    }
  }

  convertToArrays( scaleFunct_ );
  convertToArrays( resolFunct_ );

}

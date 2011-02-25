#ifndef __PFAnalyses_RootTools_Samples__
#define __PFAnalyses_RootTools_Samples__


/*! 
  \class Samples

  Works like a TTree. 
  Handles a set of TTrees, to make data/MC comparisons in one command. 
 
  \author Colin Bernet, september 2010
*/

#include "PFAnalyses/RootTools/interface/Plot.h"
#include "PFAnalyses/RootTools/interface/DataSet.h"
#include "PFAnalyses/RootTools/interface/StyleAndOpt.h"


#include "TTree.h"
#include <map>
#include <string>

#include <iostream>

class Style;

class Samples {
 public:
  /// default constructor **GENERAL
  Samples() : lxmin_(0.5), lymin_(0.75), lxmax_(1), lymax_(1) {}
  
  /// destructor **GENERAL
  virtual ~Samples() {} 
  
  /// add a TTree. make sure your TTree is correctly weighted. 
  void AddTree( TTree* tree, const char* role);

  /// add a DataSet 
  void AddDataSet( DataSet* dataSet, const char* role); 

  /// set the style and draw option for a given role **GENERAL
  void SetStyle( const char* role, bool addToStack=false, 
		 Style* style=0, const char* opt="");  

  /// set legend corners for all plots **GENERAL
  void SetLegendCorners( float xmin, float ymin, float xmax, float ymax);
  
  /// \brief draw var, knowing that cut, for all TTrees. 
  /// 
  /// The function will create and draw a Plot. The Plot 
  /// is kept in memory, indexed by the string "var_cut".
  /// If the same var and cut arguments are used later on, 
  /// the plot is re-drawn without reprocessing the TTrees. 
  /// \arg \c fracStat fraction of the statistics to be used
  /// \arg \c stack draw in stack or not
  void Draw(const char* var, const char* cut="", float fracStat=1, 
	    bool stack=true); 

  void DrawHist(const char* histName, bool stack=true); 


  
  /// use this function to set the main title, the x title, and the y title in the 
  /// usual root way: SetTitle("Main title;x title;y title") **GENERAL
  void SetTitle(const char* title);

  /// print internal information: TTrees, Plots in memory, ... **GENERAL
  void Print(std::ostream& out=std::cout) const;

  /// get a plot, using its identifier. Call Print first to find out 
  /// which identifiers are available.  **GENERAL
  Plot* GetPlot(const char* plotId);


 private:
  /// returns a string copied from var, with _number removed at the end
  /// needed when drawing trees
  std::string StripHistInfo( const char* var ) const;

  /// find the style corresponding to a given role. If not found, returns 
  /// a dummy object **GENERAL
  StyleAndOpt GetStyleAndOpt( const std::string& role ) const;


  typedef std::multimap<std::string, TTree*> Trees;
  typedef Trees::const_iterator CTI;
  typedef Trees::iterator TI;

  typedef std::multimap<std::string, DataSet*> DataSets;
  typedef DataSets::const_iterator CDI;
  typedef DataSets::iterator DI;


  /// unused
  TH1* BookHistogram( const std::string& name, int bins, float min, float max ) ;
  
  /// create the identifier, var_cut **GENERAL
  std::string plotId( const char* var, const char* cut ) const;

  /// multimap containing the TTrees added to the Samples, indexed by role. 
  /// several trees can contribute to a given role (e.g. QCD)
  Trees trees_;

  DataSets dataSets_;

  /// map containing the style and draw option corresponding to a given role **GENERAL
  typedef std::map< std::string, StyleAndOpt > StyleAndOpts;
  StyleAndOpts  styleAndOpts_;

  /// unused
  Plot histograms_; 

  /// plots will be indexed by var_cut string **GENERAL
  typedef std::map<std::string, Plot*> Plots;
  typedef Plots::const_iterator CPI; 
  typedef Plots::iterator PI; 

  /// map of plots, indexed by their identifier
  Plots     plots_;

  /// legend corner for all plots **GENERAL
  float     lxmin_;

  /// legend corner for all plots **GENERAL 
  float     lymin_;

  /// legend corner for all plots **GENERAL
  float     lxmax_;

  /// legend corner for all plots **GENERAL
  float     lymax_; 

  /// title **GENERAL
  std::string title_;

 };
#endif


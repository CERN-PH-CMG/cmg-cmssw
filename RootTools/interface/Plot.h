#ifndef __PFAnalyses_RootTools_Plot__
#define __PFAnalyses_RootTools_Plot__

/*! 
  \class Plot

  Handles a set of histograms for MC samples in a stack,
  an histogram for the data sample, and the legend.  
 
  Plots are built in the Samples class.
 
  \author Colin Bernet, september 2010
*/

#include "CMGTools/RootTools/interface/StyleAndOpt.h"


#include <map>
#include <string>
#include <vector>

#include <iostream>

#include <TLatex.h>

class TH1;
class Style;
class THStack;
class TLegend;

class Plot  {

 public: 
  /// default constructor
  Plot();
    
  /// constructor with a string identifier
  Plot( const char* id );

  /// Set the corner position of the TLegend box
  void SetLegendCorners( float xmin, float ymin, float xmax, float ymax) {
    lxmin_ = xmin; lymin_ = ymin;
    lxmax_ = xmax;  lymax_ = ymax;
  }

  /// add an histogram. All histograms in the plot should have the same 
  /// number of bins, min, and max. 
  void AddHistogram(TH1* histo, const StyleAndOpt& styleAndOpt );

  /// set the style for a given histo 
  void SetStyle(const char* histName, const StyleAndOpt& styleAndOpt );

  /// draw the Plot, in stack mode or not 
  void Draw( bool stack = false);

  /// get the stack 
  THStack* GetStack() {return stack_;}

  /// print internal information 
  void Print(std::ostream& out=std::cout) const;

  /// get one of the histograms. 
  /// \return 0 if not found
  TH1* GetHistogram(const char* histName);

  /// static function, for all Plots
  /// add a style. styles are used one after the other when drawing the plot.
  /// when no more styles are available, the default histogram style is used
  static void AddStyle(const Style* style); 

  /// static function, for all Plots
  /// clear all styles, in case you want to change them.
  static void ClearStyles();

/*   /// write id info for all plots? */
/*   static void SetWriteIdInfo( bool writeIt ) {writeIdInfo_ = writeIt;} */

 private:
  
  /// check the name of the histogram for the string DATA_ / function not used
  bool IsData(const std::string& histName) const;

  /// strip off _n extension from the histogram name, for the legend
  std::string StripOffExtension( const std::string& name) const;

  typedef  std::pair< TH1*, StyleAndOpt > HistogramExt;
  typedef  std::map<std::string, HistogramExt > Histograms;
  typedef  Histograms::iterator HI;
  typedef  Histograms::const_iterator CHI;

  /// multimap of histograms, indexed by the histogram name. 
  /// the MC histograms will be put in alphabetical order in the stack
  Histograms histograms_;

  /// unique id
  std::string id_;
  
  /// to be written on the plot
  TLatex      idInfo_;

  /// current style in use. 
  unsigned styleIndex_;

  /// get next available style
  const Style* NextStyle(); 

  /// styles available for all Plots
  static std::vector<const Style*> styles_;
  
  /// write inInfo_ on the plots? 
/*   static bool  writeIdInfo_;  */

  /// stack for MC histograms
  THStack  *stack_;

  /// legend 
  TLegend  *legend_;

  /// legend corner
  float     lxmin_;

  /// legend corner
  float     lymin_;

  /// legend corner
  float     lxmax_;

  /// legend corner
  float     lymax_;
  
};

#endif

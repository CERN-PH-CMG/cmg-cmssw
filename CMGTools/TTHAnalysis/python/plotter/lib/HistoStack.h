#ifndef HistoStack_H
#define HistoStack_H

/** \class HistoStack
 *  Code to create histogram stacks.
 *
 *  $Date: 2012/05/18 22:58:07 $
 *  $Revision: 1.2 $
 *  \author G. Cerminara - NEU Boston & INFN Torino
 */

#include <map>
#include <set>
#include <vector>
#include <string>
// #include <utility>

#include "Number.h"

#include "TString.h"

class THStack;
class TLegend;
class TH1F;
class TH2F;
class TH1;
class TStyle;

class HistoStack {
public:
  /// Constructor
  HistoStack();

  /// Destructor
  virtual ~HistoStack();

  // Declare a group of samples
  void setGroup(const TString& sampleName, bool isSignal, int color, int marker, int orderInLegend);
  
  // Assign samples to a group
  void assignToGroup(const TString& sampleName, const TString& groupName);

  // Add a prefilled histos to the stack
  Number add(const TString& sampleName, const TString& varName, TH1F* histo, float ScaleFactor);
  Number add(const TString& sampleName, const TString& varName, TH2F* histo);

  // Set the axis titles for a given stack
  //  void setAxisTitles(const TString& varName, const TString& xTitle, const TString& yUnits);

  THStack * getStack(const TString& varName, TString groupList = "");
  TH1F * getData(const TString& varName);
  TH2F * get2D(const TString& varName, TString& sampleName);

//   // Draw the given stack

//   THStack * draw(const TString& histoName, const TString& option = "");

//   // Set the name of the ntuple to be used to when filling histos from an ntuple
//   void setNtupleName(const TString& newName);

//   // get the histo stored for a particular variable and for a particular sample
//   TH1F *getHisto(const TString& sampleName, const TString& varName);

//   // set the rebinning option for each variable
//   void setRebin(const TString& varName, int reb);

//   // se the order for the legend of samples or groups
//   void setLegendOrder(int order, const TString& sampleName);

//   // Se the y range of the stack
//   void setYRange(const TString& varName, double yLow, double yHigh);

//   // Add a label in the top right corner
//   void setLabel(const TString& varName, const TString& label);

//   void setFillColor(const TString& sampleName, int color);

private:
  TString getGroupName(const TString& sampleName);

  // map between sample name and group name
  std::map<TString, TString> group;

  // Map of colors to be used for each sample
  std::map<TString, int> groupColor;

  // Map of markers to be used for each sample
  std::map<TString, int> groupMarker;

  // Sample is signal or bg
  std::map<TString, bool> isSignal;

  //  std::map<int, TString> samplesOrderInLegend;

  // Map of the histos inserted in the stack per sample and per variable 
  std::map<TString, std::map<TString, TH1F *> >histMapPerSample;
  std::map<TString, std::map<TString, TH2F *> >hist2DMapPerSample;


  // the order of groups of samples in the stack
  std::map<TString, std::vector<TString> > orderedGroups;

  // The map of the stack for each variable
  std::map<TString, THStack *> stackMap;


//   TString getPitchString(TH1 *histo, int prec) const;
//   void setStyle(TH1 *histo) const;

//   // create the stack "on-demand" when it's needed and stores it in the map
//   THStack * createStack(const TString& varName, bool normUnity=false);
//   void buildLegend(const TString& varName);

//  std::map<string, int> fillStyleMap;


//   // The map of the data hist for each sample.
//   // If the histo is present it is superimposed to the stack
  std::map<TString, TH1F *> dataHistMap;
  std::map<TString, TH2F *> data2DHistMap;
//   // Map of the sum of the histos for each variable: it is used to draw the MC error band
//   std::map<TString, TH1F *> sumForErrMap;
//   // Pointer to the legend (this is cloned for each stack actually drawn)
//   TLegend *leg;
//   std::set<TString> legSet;

//   // Map of units and labels to be used for each variable
//   std::map<TString, TString> xAxisTitle;
//   std::map<TString, TString> yAxisUnit;
//   std::map<TString, TString> yAxisTitle;
//   // Associate a label for the legend to each sample name
//   std::map<TString, TString> legLabel;
//   // keep track of samples already inserted in the stack and therefore in the legend
//   std::set<TString> alreadyInLeg; 
//   // Map of numbers for the rebinning of particular variables
//   std::map<TString, int> rebinMap; 

//   // the order of the samples or groups to be displayed in the stack
//   std::map<TString, std::vector<TString> > theSamplesInOrderPerVar;

//   std::map<int, TString> samplesOrderInLegend;

//   std::map<TString, std::pair<double, double> > rangeMap;

//   std::map<TString, TString> labelMap;

};
#endif


/*
 *  See header file for a description of this class.
 *
 *  $Date: 2012/05/18 22:58:06 $
 *  $Revision: 1.2 $
 *  \author G. Cerminara - NEU Boston & INFN Torino
 */

#include "HistoStack.h"
#include "THStack.h"
#include "TLegend.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TNtuple.h"
#include "TStyle.h"
#include "TH1.h"
#include "TSQLResult.h"
#include "TSQLRow.h"
#include "TPaveText.h"
#include "TMath.h"
#include "TROOT.h"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;


HistoStack::HistoStack(){
}


HistoStack::~HistoStack(){
  // Clean up some memory
//   for(map<TString, THStack *>::const_iterator stack =  stackMap.begin();
//       stack != stackMap.end(); ++stack) {
//     delete (*stack).second;
//   }
}


TString HistoStack::getGroupName(const TString& sampleName) {
  if(group.find(sampleName) !=  group.end()) {
    return group[sampleName];
  } else if (sampleName.BeginsWith("DoubleMu") ||
	     sampleName.BeginsWith("DoubleEle") ||
	     sampleName.BeginsWith("DoubleOr") ||
	     sampleName.BeginsWith("data",TString::kIgnoreCase)) {
    return "data";
  } else {
    return sampleName;
  }
}




void HistoStack::setGroup(const TString& sampleName, bool isSignal_, int color, int marker, int orderInLegend) {
  isSignal[sampleName] = isSignal_;
  groupColor[sampleName]    = color;
  groupMarker[sampleName]   = marker;

  //  samplesOrderInLegend[order] = sampleName;
}


void HistoStack::assignToGroup(const TString& sampleName, const TString& groupName) {
  group[sampleName] = groupName;
}



Number HistoStack::add(const TString& sampleName, const TString& varName, TH2F* histo) {
  if(histo == 0) { // check that this is a valid pointer
    cout << "[HistoStack]Warning: histo for variable: " << varName << " has non-valid pointer" << endl;
    return Number(-1,-1);
  }

  // check if the sample is assigned to any group
  TString groupName = getGroupName(sampleName);   

  int color = groupColor.find(groupName)->second;
  histo->SetFillColor(color);
  histo->SetLineColor(color);
  histo->SetMarkerColor(color);
  histo->SetMarkerStyle(groupMarker.find(groupName)->second);
  
  // Set the fill color for MC histos and marker style for data histos
  if(groupName == "data") {
//     histo->SetMarkerStyle(theStyle->GetMarkerStyle());
//     histo->SetMarkerSize(theStyle->GetMarkerSize());
//    histo->SetLineWidth(2);
  } else {
    int fillStyle = 1001;
    histo->SetFillStyle(fillStyle);
  }


  // Store the data histo
  if(groupName == "data") {
    data2DHistMap[varName] = histo;
  } 

  // Add the histo to the map of histograms per sample.
  // if the sample is in a group the histos are added
  map<TString, map<TString, TH2F *> >::iterator sampleAndHistoMap = hist2DMapPerSample.find(groupName);
  if(sampleAndHistoMap != hist2DMapPerSample.end()) {
    map<TString, TH2F *> histMp = sampleAndHistoMap->second;
    if(histMp.find(varName) != histMp.end()) {
      hist2DMapPerSample[groupName][varName]->Add(histo);
    } else {
      hist2DMapPerSample[groupName][varName] = histo;
      //keep track of the order of groups
      orderedGroups[varName].push_back(groupName);
    }
  } else {
    hist2DMapPerSample[groupName][varName] = histo;
    //keep track of the order of groups
    orderedGroups[varName].push_back(groupName);
  }

  return  Number(0,0);


}

TH2F * HistoStack::get2D(const TString& varName, TString& sampleName) {
  TH2F * hs =0;
  TString option = "hist";

   // Get the list of samples for this variable in the correct order
  vector<TString> groups = orderedGroups[varName];

  // loop over the samples and add histos
  for(vector<TString>::const_iterator g = groups.begin();      
    g != groups.end(); ++g) {
    if((*g)!= sampleName) continue;
    if (hs==0) {
      hs = (TH2F*) hist2DMapPerSample[*g][varName]->Clone();
      hs->SetTitle(g->Data());
    } else {
      hs->Add(histMapPerSample[*g][varName]);
    }
  }
  return hs;
}



Number HistoStack::add(const TString& sampleName, const TString& varName, TH1F* histo, float scaleFactor) {
  if(histo == 0) { // check that this is a valid pointer
    cout << "[HistoStack]Warning: histo for variable: " << varName << " has non-valid pointer" << endl;
    return Number(-1,-1);
  }

  histo->Scale(scaleFactor);

//   // If a rebin option has been set witht the methos setRebin then we rebin the histo
//   map<TString, int>::const_iterator rebin = rebinMap.find(varName);
//   if(rebin != rebinMap.end()) {
//     histo->Rebin((*rebin).second);
//   }


//   // Add the MC histo to the histo sum used to display the stack error
//   if(!sampleName.Contains("data")) {
//      if(sumForErrMap.find(varName) == sumForErrMap.end()) {
//        TString hSumName = varName+"_sum";
//        TH1F *hSum = (TH1F *)histo->Clone(hSumName.Data());
//        sumForErrMap[varName] = hSum;
//      } else {
//        sumForErrMap[varName]->Add(histo);
//      }
//    }

  // check if the sample is assigned to any group
  TString groupName = getGroupName(sampleName);   

  int color = groupColor.find(groupName)->second;
  histo->SetFillColor(color);
  histo->SetLineColor(color);
  histo->SetMarkerColor(color);
  histo->SetMarkerStyle(groupMarker.find(groupName)->second);
  
  // Set the fill color for MC histos and marker style for data histos
  if(groupName == "data") {
//     histo->SetMarkerStyle(theStyle->GetMarkerStyle());
//     histo->SetMarkerSize(theStyle->GetMarkerSize());
//    histo->SetLineWidth(2);
  } else {
    int fillStyle = 1001;
    histo->SetFillStyle(fillStyle);
  }


  // Store the data histo
  if(groupName == "data") {
    dataHistMap[varName] = histo;
  } 

  // Add the histo to the map of histograms per sample.
  // if the sample is in a group the histos are added
  map<TString, map<TString, TH1F *> >::iterator sampleAndHistoMap = histMapPerSample.find(groupName);
  if(sampleAndHistoMap != histMapPerSample.end()) {
    map<TString, TH1F *> histMp = sampleAndHistoMap->second;
    if(histMp.find(varName) != histMp.end()) {
      histMapPerSample[groupName][varName]->Add(histo);
    } else {
      histMapPerSample[groupName][varName] = histo;
      //keep track of the order of groups
      orderedGroups[varName].push_back(groupName);
    }
  } else {
    histMapPerSample[groupName][varName] = histo;
    //keep track of the order of groups
    orderedGroups[varName].push_back(groupName);
  }

// //   // Add an entry to the legend
// //   if(legSet.find(groupName) == legSet.end()) {
// //     TString legendLabel = legLabel[groupName];
// //     if(groupName == "data") {
// //       if(alreadyInLeg.find(legendLabel) == alreadyInLeg.end()) {
// // 	leg->AddEntry(histo,legendLabel.Data(),"pl");
// // 	alreadyInLeg.insert(legendLabel);
// //       }
// //     } else {
// //       if(alreadyInLeg.find(legendLabel) == alreadyInLeg.end()) {
// // 	leg->AddEntry(histo,legendLabel.Data(),"F"); 
// // 	alreadyInLeg.insert(legendLabel);
// //       }
// //     }
// //     legSet.insert(groupName);
// //   }
  
//   // Build the yAxis title
//   if(yAxisUnit.find(varName) != yAxisUnit.end() || yAxisTitle.find(varName) == yAxisTitle.end()) {
//     TString pitch = getPitchString(histo,2);
//     TString yTitle = "Events/"+pitch+" "+yAxisUnit[varName];
//     yAxisTitle[varName] = yTitle;
//   }
  

  // Compute the error on the histo integral (under and overflow are NOT included) 
  double sq_integral_error = 0;
  for(int i = 1; i <= histo->GetNbinsX(); ++i) {
     sq_integral_error += TMath::Power(histo->GetBinError(i),2.);
   }
  double integ_error = sqrt(sq_integral_error);

//   // Print the histo integral
//  int prec = cout.precision();
//   cout.setf(ios::fixed);
//   cout << sampleName << " " << varName
//        << " # of weighted ev. (integral): " << setprecision(4) << histo->Integral()
//        << " +/- " <<  integ_error << endl;
//   cout << setprecision(prec);
//   cout << " # entries: " << histo->GetEntries() << endl;
  return  Number(histo->Integral(),integ_error);
}


// create the stack "on-demand" when it's needed and stores it in the map
THStack * HistoStack::getStack(const TString& varName, TString groupList) {
//   buildLegend(varName);
  THStack * hs =0;
  TString option = "hist";
  bool cacheStacks = false;
  
  vector<TString> groups;
  set<TString> skipGroups;
  if (groupList!="") {
    TObjArray *strobj = groupList.Tokenize(";");
    for (int i=0; i<strobj->GetEntries(); ++i) { 
      TString name = ((TObjString*) strobj->At(i))->GetString();
      if (name[0] != '-') {
	groups.push_back(name);
      } else {
	skipGroups.insert(name.Strip(TString::kLeading,'-'));
	//	cout << " skipping " << name.Strip(TString::kLeading,'-') << endl;
      }
    }
  } 

  if (groups.size()==0) {
    groups = orderedGroups[varName];
  }

  int count = 0;
  if(cacheStacks==false || stackMap.find(varName) == stackMap.end()) {
    hs = new THStack(varName.Data(), varName.Data());    
    //    hs->SetMinimum(0.001);
   // Get the list of samples for this variable in the correct order
    // loop over the samples and add them to the stack
    for(vector<TString>::const_iterator g = groups.begin();
 	g != groups.end(); ++g) {
      if((*g).Contains("data")) continue;
      //      if (normUnity) histMapPerSample[*samp][varName]->Scale(1./histMapPerSample[*samp][varName]->Integral());

      //      cout << (*g) << " " << skipGroups.count(*g) << endl;
      if (skipGroups.count(*g)) continue;
      
      
      // Check that group exists
      map<TString, map<TString, TH1F *> >::iterator sampleAndHistoMap = histMapPerSample.find(*g);
      if(sampleAndHistoMap == histMapPerSample.end()) continue;

      histMapPerSample[*g][varName]->SetTitle(g->Data());
      hs->Add(histMapPerSample[*g][varName],option.Data());
      ++count;
    }
    if (count==0) {
      cout << "getStack: no histograms " << varName << endl;
      return 0;
    }
    stackMap[varName] = hs;

  } else {
    hs = stackMap[varName];
  }

  
  return hs;
}

TH1F* HistoStack::getData(const TString& varName) {
  if(dataHistMap.find(varName) != dataHistMap.end()) 
    return (dataHistMap.find(varName))->second;
  else return 0;
}




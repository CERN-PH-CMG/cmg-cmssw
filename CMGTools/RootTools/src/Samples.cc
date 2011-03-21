#include "CMGTools/RootTools/interface/Samples.h"
#include "CMGTools/RootTools/interface/TmpHistName.h"

#include <TH1F.h> 
#include <sstream>

using namespace std;


void Samples::AddTree( TTree* tree, const char* role ) {

  trees_.insert( make_pair<string, TTree*>(role, tree) );
}


void Samples::AddDataSet( DataSet* dataSet, const char* role ) {

  dataSets_.insert(  make_pair<string, DataSet*>(role, dataSet ) );
}


void Samples::SetStyle( const char* role, bool addToStack, Style* style, const char* opt ) {

  StyleAndOpt stopt( style, opt, addToStack );
  styleAndOpts_[ role ] = stopt;  

  for( PI pi = plots_.begin(); pi!=plots_.end(); ++pi) {
    pi->second->SetStyle( role, stopt );
  }
}


void Samples::Print( ostream& out) const {
  
  out<<"Samples"<<endl;
  out<<"--------------"<<endl;

  for (CTI ti=trees_.begin(); ti!=trees_.end(); ++ti) {
    
    TTree* tree = ti->second;
    
    out<<ti->first
       <<" "<<tree->GetTitle()
       <<" "<<tree->GetEntries()
       <<" "<<tree->GetWeight()<<endl;
  }

  out<<"DataSets:"<<endl;
  for (CDI di=dataSets_.begin(); di!=dataSets_.end(); ++di) {
    
    DataSet* dataSet = di->second;
    
    out<<di->first<<endl;
    dataSet->Print();
  }  

  out<<"Plots:"<<endl;
  for(CPI pi=plots_.begin(); pi!=plots_.end(); ++pi) {
    out<<"plot: "<<pi->first<<endl;
    pi->second->Print( out );
  }
}


void Samples::SetLegendCorners( float xmin, float ymin, float xmax, float ymax) {
  
  // need to keep track of these values in case no plot exist yet
  lxmin_ = xmin; lymin_ = ymin;
  lxmax_ = xmax; lymax_ = ymax;
  
  // in case some plots exist, set their legend corners
  for( PI pi = plots_.begin(); pi!=plots_.end(); ++pi) {
    pi->second->SetLegendCorners( xmin, ymin, xmax, ymax );
  }
}
 

void Samples::SetTitle( const char* title) {
  title_ = title;
}

void Samples::Draw( const char* var, const char* cut, float fracStat, bool stack ) {

  if(fracStat<=0 || fracStat>1 ) {
    cerr<<"fracStat argument must be > 0 and <= 1"<<endl;
    return;
  }

  string prevRole = "";

  string plusHistName; 

  // if plot already exists, plot and return, 
  // otherwise create the plot and continue
  string id = plotId( var, cut );
  PI pi = plots_.find( id );

  // here, extract histogram information from var
  // and strip it off from var
  
  Plot* currentPlot = 0;
  if( pi != plots_.end() ) {
    pi->second->Draw( stack );
    return;
  }
  else {
    currentPlot = new Plot( id.c_str() );
    currentPlot->SetLegendCorners( lxmin_, lymin_, lxmax_, lymax_ );
    plots_.insert( make_pair<string, Plot*>(id, currentPlot ) );
  }

  string svar = StripHistInfo( var ); 

  TH1F* firstHist = 0;
  for (CTI ti=trees_.begin(); ti!=trees_.end(); ++ti) {

    TTree* tree = ti->second;

    string role = ti->first; 
   
    cout<<"processing "<<role<<" "<<tree->GetTitle()
	<<" NEntries = "<<tree->GetEntries()<<endl;

    if(role != prevRole ) {
      
      // changing role, need to book a new histogram 
      prevRole = role; 
      TH1F* hist = 0;

      if( ti!= trees_.begin() ) {
	// not the first tree. 
	// book a new histogram taking the first one as a model

	hist = (TH1F*) firstHist->Clone( TmpHistName::Name( prevRole.c_str() ).c_str() );
	cout<<"booking "<<hist->GetName()<<endl;
	hist->Reset();	
      }
      else {
	// first tree
	// keeping track of the first histogram, which will serve as a model.
	tree->Draw( var, cut, "goff", 
		    static_cast<int>(fracStat*tree->GetEntries()) );
	
	//TODO must make sure we are using only TH1F...
	firstHist = (TH1F*) tree->GetHistogram()->Clone( TmpHistName::Name( role.c_str() ).c_str() );

	cout<<"setting title "<<title_<<endl;
	firstHist->SetTitle( title_.c_str() );
	hist = firstHist; 

	cout<<"adding first histogram: name="<<firstHist->GetName()<<" , title="<<firstHist->GetTitle()<<endl;
      }
      
      // adding the new histogram to the current plot
      currentPlot->AddHistogram(hist, GetStyleAndOpt( role ) );

      // will now project into this histogram. 
      plusHistName = "+";
      plusHistName += hist->GetName();
    }
    
    if( ti != trees_.begin() ) {
      // first the first tree, the histogram is filled in the Draw function, see above.
      // for the next trees, need to project in the current histogram
      cout<<"projecting into "<<plusHistName<<endl;
      tree->Project( plusHistName.c_str(), svar.c_str(), cut, "", 
		     static_cast<int>(fracStat*tree->GetEntries()) );
    }
  } 
  
  currentPlot->Draw( stack );
}



void Samples::DrawHist( const char* histName, bool stack ) {



  string prevRole = "";
  TH1* prevHist = 0;
  
  string plusHistName; 

  // if plot already exists, plot and return, 
  // otherwise create the plot and continue
  string id = plotId( histName, "histName" );
  PI pi = plots_.find( id );

  // here, extract histogram information from var
  // and strip it off from var
  
  Plot* currentPlot = 0;
  if( pi != plots_.end() ) {
    pi->second->Draw( stack );
    return;
  }
  else {
    currentPlot = new Plot( id.c_str() );
    currentPlot->SetLegendCorners( lxmin_, lymin_, lxmax_, lymax_ );
    plots_.insert( make_pair<string, Plot*>(id, currentPlot ) );
  }

  //   string svar = StripHistInfo( var ); 

  //   TH1F* firstHist = 0;
  for (CDI di=dataSets_.begin(); di!=dataSets_.end(); ++di) {

    DataSet* dataSet = di->second;
    string role = di->first; 
   
    cout<<"processing "<<role<<" "<<dataSet->GetFileHistos()->GetName()<<endl;

    TH1* histTmp = dataSet->GetHist( histName );
    TH1* hist = (TH1*) histTmp->Clone( TmpHistName::Name( prevRole.c_str() ).c_str() );

    if(!hist) {
      continue;
    }
    
    if(role != prevRole ) {
      prevRole = role; 
      prevHist = hist; 
       
      // adding the new histogram to the current plot
      currentPlot->AddHistogram(hist, GetStyleAndOpt( role ) );

//       // will now project into this histogram. 
//       plusHistName = "+";
      plusHistName = hist->GetName();
    }
    else {
      cout<<"adding to "<<prevHist->GetName()<<endl;
      prevHist->Add( hist );
    }
    
    //   if( di != dataSets_.begin() ) {
    //       // For the first dataSet, the histogram is filled in the Draw function, see above.
    //       // for the next dataSets, need to project in the current histogram
    //       cout<<"adding to "<<plusHistName<<endl;
    //       dataSet->Project( plusHistName.c_str(), svar.c_str(), cut, "", 
    // 		     static_cast<int>(fracStat*dataSet->GetEntries()) );
    //     }
  } 
  
  currentPlot->Draw( stack );
}



TH1* Samples::BookHistogram( const string& name, int bins, float min, float max )  {
  // will need to delete these guys? 
  // or let root do the garbage collection>.
  // keep track of all histograms in the Samples class? 
  cout<<"booking histogram "<<name<<endl;
  //   histograms_.push_back( new TH1F( name.c_str(), name.c_str(), bins, min, max) );
  return new TH1F( name.c_str(), name.c_str(), bins, min, max);
}


std::string Samples::plotId( const char* var, const char* cut ) const {
  string id = var;
  id += "_";
  id += cut;
  return id;
}


Plot* Samples::GetPlot(const char* plotId) {
  PI pi = plots_.find( plotId );
  if(pi != plots_.end() ) {
    return pi->second;
  }
  else return 0;
}


std::string Samples::StripHistInfo( const char* var) const {
  
  string svar = var; 

  const std::string::size_type pos = svar.find(">>"); 
  if(pos==string::npos )  // search failed, values not provided
    return svar;
  else 
    return svar.substr(0,pos);
}


StyleAndOpt Samples::GetStyleAndOpt( const std::string& role ) const {
  typedef StyleAndOpts::const_iterator SOI; 
  
  SOI soi = styleAndOpts_.find( role );
  if(soi!=styleAndOpts_.end() ) return soi->second;
  else return StyleAndOpt();
}

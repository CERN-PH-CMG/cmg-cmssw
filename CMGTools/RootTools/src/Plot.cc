#include "CMGTools/RootTools/interface/Plot.h"
#include "CMGTools/RootTools/interface/TmpHistName.h"
#include "CMGTools/RootTools/interface/Styles.h"

#include <TH1F.h> 
#include <THStack.h> 
#include <TLegend.h> 

using namespace std;

Styles allStyles;

std::vector<const Style*> Plot::styles_;

// bool Plot::writeIdInfo_ = true;


Plot::Plot() : id_(""), styleIndex_(0), stack_(0), legend_(0), lxmin_(0.5), lymin_(0.75), lxmax_(1), lymax_(1) {}

Plot::Plot( const char* id ) : id_(id), styleIndex_(0), stack_(0), legend_(0), lxmin_(0.5), lymin_(0.75), lxmax_(1), lymax_(1)  {}

void Plot::AddStyle( const Style* style) {
  styles_.push_back( style );
}

void Plot::ClearStyles() {
  styles_.clear();
}

void Plot::AddHistogram(TH1* histo, const StyleAndOpt& styleAndOpt ) {

  cout<<"Plot: AddHistogram : name="<<histo->GetName()<<" , title="<<histo->GetTitle()<<endl;
  
  string name = StripOffExtension( string( histo->GetName() )  );
  histograms_[ name ] = make_pair< TH1*, StyleAndOpt >(histo, styleAndOpt ) ;
}


void Plot::SetStyle(const char* histName, const StyleAndOpt& styleAndOpt ) {

  HI hi = histograms_.find( histName );
  if( hi!=histograms_.end() ) {
    hi->second.second = styleAndOpt;
  }
  else {
    cout<<"Plot::SetStyle : cannot find histogram "<<histName<<endl;
    cout<<"available histograms are: "<<endl;
    Print( cout );
  }
}




void Plot::Draw( bool stack ) {
  
  if(stack_) delete stack_;
  stack_ = new THStack("stack", "");
  
  if(legend_) delete legend_;
  legend_ = new TLegend(lxmin_, lymin_, lxmax_, lymax_ );

  vector< HI > unStackedHistos; 

  //TODO leave freedom to define the style from outside, for all plots

  std::string stackTitle; 
  std::string stackXTitle; 
  std::string stackYTitle; 
  
  TH1* histInStack = 0;
  
  for( HI hi = histograms_.begin(); hi!=histograms_.end(); ++hi) {


    TH1* histo = hi->second.first;
    StyleAndOpt styleAndOpt = hi->second.second;

    cout<<histo->GetName()<<" "<<styleAndOpt.GetAddToStack()<<endl;

    Styles::FormatHisto(histo, styleAndOpt.GetStyle() );
    
    // in fact, should create the legend at the very end. 
    legend_->AddEntry( histo, 
		       hi->first.c_str() );

    if( !styleAndOpt.GetAddToStack() ) {
      // will be drawn at the end
      unStackedHistos.push_back(hi);
    }
    else {
      cout<<"adding to stack histo: name="<<histo->GetName()
	  <<" , title="<<histo->GetTitle()
	  <<" , xtitle="<<histo->GetXaxis()->GetTitle()<<endl;
      stack_->Add( histo );
      
      histInStack = histo;
      
      stackTitle = histo->GetTitle();
      stackXTitle = histo->GetXaxis()->GetTitle();
      stackYTitle = histo->GetYaxis()->GetTitle();
      
      //       stack_->SetTitle( histo->GetTitle() );
      //       cout<<"stack axis "<<stack_->GetXaxis()<<endl;
      //       stack_->GetXaxis()->SetTitle( histo->GetXaxis()->GetTitle() );
    }
  }

  // drawing the first unstacked histogram as a support histo
  // if the stack is drawn first, a graphics bug makes the results 
  // ugly (problem with the axes)

  string opt = "";
  if( !stack ) opt += "NOSTACK";
  
  if( !unStackedHistos.empty() ) {
//     if( writeIdInfo_ )
//       unStackedHistos[0]->second.first->SetTitle(id_.c_str() );
    unStackedHistos[0]->second.first->Draw( unStackedHistos[0]->second.second.GetOpt().c_str() );
    opt += ",A,HIST,SAME";
  }
  else {
    histInStack->Draw("NOAXIS");
    opt += ",A,HIST,SAME";    
  }

//   else{
//     cout<<"stack title: "<<stack_->GetTitle()<<endl;
//     if( writeIdInfo_ && std::string(stack_->GetTitle()).empty() )
//       stack_->SetTitle(id_.c_str() );
//   } 

  stack_->Draw(opt.c_str() );
  stack_->SetTitle( stackTitle.c_str() );
  stack_->GetXaxis()->SetTitle( stackXTitle.c_str() );
  stack_->GetYaxis()->SetTitle( stackYTitle.c_str() );
  

  // superimposing unstacked histograms

  for( unsigned id=0; id<unStackedHistos.size(); ++id ) {
    
    HI& unStacked = unStackedHistos[id];
    
//     const Style* style = NextStyle(); 
//     if(style)
//       Styles::FormatHisto(unStacked->second.first, style);    
//     legend_->AddEntry( unStacked->second.first, 
// 		       StripOffExtension( unStacked->first ).c_str() );
    unStacked->second.first->Sumw2();
    string usOpt = unStacked->second.second.GetOpt();
    usOpt += ",same";
    unStacked->second.first->Draw(usOpt.c_str());
  }
  
  // superimposing the legend
  
  legend_->Draw("same");
  
//   if(writeIdInfo_) {
//     //     idInfo_.SetNDC( true );
//     idInfo_.DrawLatex(0.02,0.95, id_.c_str() );
//   }
}


bool Plot::IsData(const string& histName) const {
  const std::string::size_type pos = histName.find("DATA"); 
  if(pos!=string::npos ) 
    return true;
  else return false;
}

std::string Plot::StripOffExtension( const string& histName) const {
  const std::string::size_type pos = histName.find("_"); 
  if(pos!=string::npos ) 
    return histName.substr(0, pos) ;
  else return histName;
  
}

void Plot::Print(std::ostream& out) const {
  for( CHI hi = histograms_.begin(); hi!=histograms_.end(); ++hi) {
    out<<hi->first<<endl;
  }
}


TH1* Plot::GetHistogram(const char* histName) {
  HI hi = histograms_.find( histName );
  if( hi!=histograms_.end() ) 
    return hi->second.first;
  else 
    return 0;
}

const Style* Plot::NextStyle() {
  if( styleIndex_ >= styles_.size() ) return 0;
  else {
    const Style* style = styles_[styleIndex_];
    styleIndex_++;
    return style;
  }
}

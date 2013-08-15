
TTree* InitChain( const std::string& pathToSample, float intLumi, const std::string& pathToRia ) {

  string stree = pathToSample;
  stree += "/*tree*root";

  string sria = pathToSample;
  if( !pathToRia.empty() ) {
    sria = pathToRia;
  }
  sria += "/*_histograms*root";

  chain = new Chain("Events", stree.c_str() ); 

  cout<<"chain ok"<<endl;

  ria = new Chain("runInfoAccounting/RunInfoAccounting", sria.c_str() ); 

  cout<<"ria ok, setting weight"<<endl;

  chain->SetWeight( RiaInfo( ria )*intLumi,"global"); 
  initCMG("ANA", chain);

  return chain;
}

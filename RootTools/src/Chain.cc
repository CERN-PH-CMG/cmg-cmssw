#include "CMGTools/RootTools/interface/Chain.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TCollection.h"
#include "TChainElement.h"

#include <iostream>
#include <cstdlib>

using namespace std;

Chain::Chain(const char* chainname,
             const char* pattern,
             const char* legendEntry,
             bool const testFiles)
  : TChain(chainname), legendEntry_(legendEntry) {

  cout << "Creating Chain with file pattern '" << pattern << "'" << endl;
  this->SetTitle(pattern);
  this->Add(pattern);

  size_t nFiles = this->GetListOfFiles()->GetEntries();
  if (nFiles == 0) {
    cout << "WARNING No files found matching pattern --> empty Chain!!!" << endl;
    // Argh! Scary, but for some reason this is needed?
    exit(1);
  } else {
    cout << "  Added " << nFiles;
    if (nFiles > 1)
      cout << " files:" << endl;
    else
      cout << " file:" << endl;
    std::vector<std::string> files = this->files();
    for (std::vector<std::string>::const_iterator it = files.begin();
         it != files.end(); ++it)
      cout << "    " << *it << endl;
  }

  // If requested, open all files and make sure the tree exists (by
  // asking for the total number of entries in the chain).
  if (testFiles) {
    cout << "  Testing existence of tree '" << chainname
         << "' in all files..." << endl;
    this->GetEntries();
  } else
    cout << "WARNING Skipping test for tree existence!" << endl;
}

std::vector<std::string> Chain::files() const {
  size_t nFiles = this->GetListOfFiles()->GetEntries();
  std::vector<std::string> files;
  files.reserve(nFiles);
  TObjArray* fileElements = this->GetListOfFiles();
  TIter next(fileElements);
  TChainElement* chEl = 0;
  while ((chEl = (TChainElement*)next()))
    files.push_back(chEl->GetTitle());
  return files;
}

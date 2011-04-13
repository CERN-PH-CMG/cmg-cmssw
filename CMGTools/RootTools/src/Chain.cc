#include "CMGTools/RootTools/interface/Chain.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TCollection.h"
#include "TChainElement.h"

#include <iostream>
#include <cstdlib>
#include <glob.h>

using namespace std;

//--------------------------------------------------------------------

std::vector<std::string>
Chain::Expand(std::string const& pattern)
{
  std::vector<std::string> result;

  glob_t res;
  int err = glob(pattern.c_str(), 0, 0, &res);
  if (err) {
    std::cerr << "Error expanding pattern '"
              << pattern << "'" << std::endl;
  } else {
    for (size_t i = 0, end = res.gl_pathc; i != end; ++i) {
      result.push_back(res.gl_pathv[i]);
    }
  }
  globfree(&res);

  return result;
}

//--------------------------------------------------------------------

Chain::Chain(const char* chainname,
             const char* pattern,
             const char* legendEntry,
             bool const testFiles)
  : TChain(chainname), legendEntry_(legendEntry) {

  cout << "Creating Chain with file pattern '" << pattern << "'" << endl;
  this->SetTitle(pattern);

  // Figure out if the pattern points to a CASTOR area or not.
  TString patternString(pattern);
  bool isCastor = (TString(patternString(0, 4)) == "rfio");

  if (isCastor) {
    // In this case we just feed the pattern to TChain without
    // expanding wildcards ourselves.
    // But, we have to make sure there are no wildcards in the path.
    TString name(pattern);
    int slashPos = -1;
    int nextSlash = name.Index("/");
    while (nextSlash >= 0) {
      slashPos = nextSlash;
      nextSlash = name.Index("/", slashPos + 1);
    }
    TString pathName(name(slashPos + 1, name.Length()));
    if (!pathName.MaybeWildcard()) {
      this->Add(pattern);
    } else {
      std::cerr << "CASTOR paths do not support wildcards!" << std::endl;
    }
  } else {
    // In this case we do the pattern expansion ourselves and feed the
    // results to TChain::Add().
    std::vector<std::string> files = Expand(pattern);
    for (std::vector<std::string>::const_iterator it = files.begin();
         it != files.end(); ++it) {
      this->Add(it->c_str());
    }
  }

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

//--------------------------------------------------------------------

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

//--------------------------------------------------------------------

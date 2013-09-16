//--------------------------------------------------------------------
// Purpose: Split a multi-object event into a di-object event using
// the brute force approach documented in e.g. SUS-09-001.
// NOTE: This can take _very_ long to cluster many objects.
//--------------------------------------------------------------------

#include "CMGTools/Common/interface/HemiFactory.h"
#include "CMGTools/Common/interface/next_combination.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/View.h"

#include <limits>
#include <map>
#include <utility>

typedef edm::View<reco::Candidate>::size_type size_type;


cmg::HemisphereFactory::event_ptr
cmg::HemisphereFactory::create(const edm::Event& iEvent,
                               const edm::EventSetup& iSetup)
{
  typedef std::vector<cmg::Hemisphere> collection;
  cmg::HemisphereFactory::event_ptr result(new collection);

  typedef std::map<double, std::pair<cmg::Hemisphere, cmg::Hemisphere>, std::less<double> > sorted_map;
  sorted_map all_hemi;

  // Step one is to take all candidates from all input collections and
  // store them into a single vector.
  std::vector<edm::Ptr<reco::Candidate> > candidates;
  for (VInputTag::const_iterator it = hemisphereLabel_.begin();
       it != hemisphereLabel_.end(); ++it) {
    edm::Handle<edm::View<reco::Candidate> > cands;
    iEvent.getByLabel(*it, cands);
    for (size_t i = 0, n = cands->size(); i < n; ++i) {
      candidates.push_back(edm::Ptr<reco::Candidate>(cands, i));
    }
  }

  // Problem at hand: divide all N candidates into two groups, and
  // generate all possible combinations. First group contains n
  // candidates, second group contains m = N - n candidates.
  size_type numCand = candidates.size();

  // If there are too many or too few candidates, skip the event.
  if ((numCand < 2) || (numCand > maxNCand_)) {
    // Only warn for the 'too many' case.
    if (numCand > maxNCand_) {
      edm::LogWarning("HemiFactory")
        << "Skipping event with " << numCand << " candidate objects "
        << "(max = " << maxNCand_ << ")";
    }
  } else {
    size_type numSteps = (numCand / 2);

    std::vector<size_t> indVec;
    for (size_t i = 0; i != numCand; ++i) {
      indVec.push_back(i);
    }

    double minImbalance = std::numeric_limits<double>::max();
    std::vector<size_t> bestCombination;

    // Groupings of n, m are the same as groupings of m, n -> only
    // need to iterate over 'half the number of candidates.'
    for (size_type i = 1; i <= numSteps; ++i) {
      std::sort(indVec.begin(), indVec.end());

      // For each step loop over all combinations of i candidates
      // picked out of N.
      do {
        std::ostringstream msg;
        for (std::vector<size_t>::const_iterator it = indVec.begin();
             it != indVec.begin() + i; ++it) {
          msg << *it << ", ";
        }
        
        //create tempory hemispheres
        std::vector<edm::Ptr<reco::Candidate> > tmp0;
        std::vector<edm::Ptr<reco::Candidate> > tmp1;
        
        //first hemisphere
        for (std::vector<size_t>::const_iterator it = indVec.begin();
             it != indVec.begin() + i; ++it) {
                tmp0.push_back(candidates[*it]);
        }
        //second hemisphere
        for (std::vector<size_t>::const_iterator it = indVec.begin() + i;
             it != indVec.end(); ++it) {
                tmp1.push_back(candidates[*it]);
        }        
        assert( (tmp0.size() + tmp1.size()) == indVec.size() );

	double diff = std::numeric_limits<double>::max();
	if( (minObjectsPerHemi0_ == 0 && minObjectsPerHemi1_ == 0) || //the hemi count cut is turned off
	    (candidates.size() < (minObjectsPerHemi0_ + minObjectsPerHemi1_) ) || //we don't have enough candidates to ever pass the cut
	    ( ( (tmp0.size() >= minObjectsPerHemi0_) && (tmp1.size() >= minObjectsPerHemi1_) ) ||  //at least one hemi passes the cut
	      ( (tmp0.size() >= minObjectsPerHemi1_) && (tmp1.size() >= minObjectsPerHemi0_) ) )
	    ){
	  diff = balance_->balance(tmp0,tmp1);
	  if(keepAll_){
	    cmg::Hemisphere hemi0(tmp0);
	    cmg::Hemisphere hemi1(tmp1);

	    // Set all internal hemisphere variables.
	    set(tmp0, hemi0);
	    set(tmp1, hemi1);
	    
	    all_hemi.insert( std::make_pair( diff, std::make_pair(hemi0, hemi1) ) );

	  }
	}else{
	  continue;
	}

        if (diff < minImbalance) {
          minImbalance = diff;
          bestCombination.resize(i);
          std::copy(indVec.begin(), indVec.begin() + i,
                    bestCombination.begin());
          std::ostringstream msg;
          for (std::vector<size_t>::const_iterator it = indVec.begin();
               it != indVec.begin() + i; ++it) {
            msg << *it << ", ";
          }
        }
      }
      while (next_combination(indVec.begin(),
                              indVec.begin() + i,
                              indVec.end()));
    }

    //----------

    assert(bestCombination.size() > 0);

    if(keepAll_){//keep all valid combinations
      collection all;
      for(sorted_map::const_iterator it = all_hemi.begin(); it != all_hemi.end(); ++it){
	//pt order
	if(it->second.first.pt() > it->second.second.pt()){
	  result->push_back(it->second.first);
	  result->push_back(it->second.second);
	}else{
	  result->push_back(it->second.second);
	  result->push_back(it->second.first);
	}
      }
    }else{ //just return the best hemi
      std::vector<edm::Ptr<reco::Candidate> > tmp0;
      std::vector<edm::Ptr<reco::Candidate> > tmp1;
      
      for (size_t i = 0; i != numCand; ++i) {
	if (std::binary_search(bestCombination.begin(),
			       bestCombination.end(), i)) {
	  tmp0.push_back(candidates.at(i));
	} else {
	  tmp1.push_back(candidates.at(i));
	}
      }

      assert((tmp0.size() + tmp1.size()) == numCand);

      cmg::Hemisphere hemi0(tmp0);
      cmg::Hemisphere hemi1(tmp1);

      // Set all internal hemisphere variables.
      set(tmp0, hemi0);
      set(tmp1, hemi1);

      // Don't forget to return pT ordered results.
      if (hemi0.pt() > hemi1.pt()) {
	result->push_back(hemi0);
	result->push_back(hemi1);
      } else {
	result->push_back(hemi1);
	result->push_back(hemi0);
      }
    }
    


  } // if (numCand)


  return result;
}

//--------------------------------------------------------------------

void
cmg::HemisphereFactory::set(std::vector<edm::Ptr<reco::Candidate> > const& input,
                            cmg::Hemisphere& object) const
{
  multiObjectFactory_.set(input, &object);
}

//--------------------------------------------------------------------

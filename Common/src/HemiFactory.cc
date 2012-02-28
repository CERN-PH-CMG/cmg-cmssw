//--------------------------------------------------------------------
// Purpose: Split a multi-object event into a di-object event using
// the brute force approach documented in e.g. SUS-09-001.
// NOTE: This can take _very_ long to cluster many objects.
//--------------------------------------------------------------------

#include "CMGTools/Common/interface/HemiFactory.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/View.h"

#include <limits>
#include <utility>

typedef edm::View<reco::Candidate>::size_type size_type;

//--------------------------------------------------------------------

// Happily copy-pasted from here:
// http://photon.poly.edu/~hbr/boost/combinations.html
// until this is accepted into the BOOST library.
template<class BidirectionalIterator>
bool next_combination(BidirectionalIterator first1,
                      BidirectionalIterator last1,
                      BidirectionalIterator first2,
                      BidirectionalIterator last2)
{
  if ((first1 == last1) || (first2 == last2)) {
    return false;
  }

  BidirectionalIterator m1 = last1;
  BidirectionalIterator m2 = last2; --m2;

  // Find first m1 not less than *m2 (i.e., lower_bound(first1, last1, *m2)).
  // Actually, this loop stops at one position before that, except perhaps
  // if m1 == first1 (in which case one must compare *first1 with *m2).
  while (--m1 != first1 && !(*m1 < *m2)) {
  }

  // Test if all elements in [first1, last1) not less than *m2.
  bool result = (m1 == first1) && !(*first1 < *m2);

  if (!result) {

    // Find first first2 greater than *m1 (since *m1 < *m2, we know it
    // can't pass m2 and there's no need to test m2).
    while (first2 != m2 && !(*m1 < *first2)) {
      ++first2;
    }

    first1 = m1;
    std::iter_swap (first1, first2);
    ++first1;
    ++first2;
  }

  // Merge [first1, last1) with [first2, last2), given that the rest of the
  // original ranges are sorted and compare appropriately.
  if ((first1 != last1) && (first2 != last2)) {
    for (m1 = last1, m2 = first2;  (m1 != first1) && (m2 != last2); ++m2) {
      std::iter_swap (--m1, m2);
    }

    std::reverse (first1, m1);
    std::reverse (first1, last1);

    std::reverse (m2, last2);
    std::reverse (first2, last2);
  }

  return !result;
}

//--------------------------------------------------------------------

template<class BidirectionalIterator>
bool next_combination(BidirectionalIterator first,
                      BidirectionalIterator middle,
                      BidirectionalIterator last)
{
  return next_combination(first, middle, middle, last);
}

//--------------------------------------------------------------------

cmg::HemisphereFactory::event_ptr
cmg::HemisphereFactory::create(const edm::Event& iEvent,
                               const edm::EventSetup& iSetup)
{
  typedef std::vector<cmg::Hemisphere> collection;
  cmg::HemisphereFactory::event_ptr result(new collection);

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
        
        double diff = balance_->balance(tmp0,tmp1);
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


import FWCore.ParameterSet.Config as cms

source = cms.Source(
	"PoolSource",

	noEventSort = cms.untracked.bool(True),
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
	fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
 		'root://castorcms//castor/cern.ch/user/s/sakoay/mht_geq300_skim//hotskim_QCD170to300-pythia6.root',
 		'root://castorcms//castor/cern.ch/user/s/sakoay/mht_geq300_skim//hotskim_QCD300to470-pythia6.root',
		'root://castorcms//castor/cern.ch/user/s/sakoay/mht_geq300_skim//hotskim_QCD470to600-pythia6.root',
		'root://castorcms//castor/cern.ch/user/s/sakoay/mht_geq300_skim//hotskim_QCD600to800-pythia6.root',
		'root://castorcms//castor/cern.ch/user/s/sakoay/mht_geq300_skim//hotskim_QCD800to1000-pythia6.root',
		'root://castorcms//castor/cern.ch/user/s/sakoay/mht_geq300_skim//hotskim_QCD1000to1400-pythia6.root',
		'root://castorcms//castor/cern.ch/user/s/sakoay/mht_geq300_skim//hotskim_QCD1400to1800-pythia6.root',
		'root://castorcms//castor/cern.ch/user/s/sakoay/mht_geq300_skim//hotskim_QCD1800toInf-pythia6.root'
])

# Auto generated configuration file
# using: 
# Revision: 1.372.2.3 
# Source: /local/reps/CMSSW.admin/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/EightTeV/POWHEG_PYTHIA6_Tauola_H_ZZ_2l2nu_8TeV_cff.py --filein=unweighted_events-dec.lhe.root --fileout=events.root --filetype=EDM -s GEN,SIM,DIGI,L1,DIGI2RAW,HLT:GRun,RAW2DIGI,L1Reco,RECO --pileup=2012_Startup_50ns_PoissonOOTPU --mc --geometry DB --conditions=auto:startup --eventcontent=AODSIM --datatier AODSIM --number=-1 --no_exec --python_filename=cmsRun_FullSim.py
import FWCore.ParameterSet.Config as cms

process = cms.Process('HLT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_2012_Startup_50ns_PoissonOOTPU_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic8TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_GRun_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(XXX_NEVENTS_XXX)
)

process.RandomNumberGeneratorService.generator.initialSeed = cms.untracked.uint32(XXX_SEED_XXX)
process.RandomNumberGeneratorService.mix.initialSeed = cms.untracked.uint32(XXX_SEED_XXX)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('/store/cmst3/user/psilva/Madgraph/VBFZ_interf/unweighted_events.lhe.root'),
    skipEvents = cms.untracked.uint32(XXX_SKIP_XXX) 
)

process.mix.input.fileNames = cms.untracked.vstring(
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/003EEBD4-8061-E111-9A23-003048D437F2.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/005825F1-F260-E111-BD97-003048C692DA.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0065594C-B35E-E111-8B8C-003048C693EA.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0091FFD0-6B5E-E111-92FE-003048C693DA.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/00C69AE3-FE60-E111-BC48-0030487D8633.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/02079692-AC61-E111-97BB-0025901D4D54.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/021BD915-2D61-E111-8BAD-002481E76052.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/02386E4D-DC5E-E111-9413-00266CF1074C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/02446A08-515E-E111-82C7-00266CF330B8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/024BBC60-CD5D-E111-A41A-0025901D4C32.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/025AB53D-F15E-E111-A7F4-003048C66BBE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0274007A-A05E-E111-9227-003048D4DFB8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/02AC6853-E65D-E111-A515-0025901D4B04.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/02B17485-B861-E111-84FC-003048C6930E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/02CFF160-B85E-E111-B547-003048D3CD92.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0407D67D-F360-E111-A737-0030487F1A73.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0407DB67-2F5E-E111-8115-003048D47976.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0464DA8A-4D61-E111-95F5-003048C64787.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/04670075-3561-E111-AD81-002481E0D50C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/046FF351-6261-E111-AAE8-003048F0E84E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/04BC6125-4261-E111-9776-0030487E4EBF.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/04C21348-E85D-E111-BE80-003048F02CB2.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/04C50AAD-C45E-E111-A3DA-0030487F1A3F.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/04F2E342-3A61-E111-9739-003048F02CB2.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/062411FC-135E-E111-8063-0030487D8121.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/063B1518-FD60-E111-B98F-003048C69416.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/06461FC4-BD61-E111-A169-002481E0E450.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/064EC577-B661-E111-AFCD-003048D43788.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/067C711D-E55D-E111-B456-00266CF422D8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/068BCE53-BA5E-E111-894F-003048D439AA.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0697F430-1761-E111-B589-0030487F1655.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/069CF9AF-F45E-E111-A372-003048C66BBE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/069EDF54-DB5E-E111-B63D-00266CF32F14.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/06A7E2E0-8E5E-E111-BB25-0030487FA625.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/06B34711-DA5D-E111-8803-00266CF33100.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0804BFCE-795E-E111-A825-002481E94BCA.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/082EBD22-BE61-E111-90E3-003048F02CB8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/083D4C03-4F61-E111-A02D-0030487F1661.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/086104F5-CA5D-E111-B11C-0030487D5EBB.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0884F4C3-105E-E111-8107-0030487E4ED5.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/08B41562-765E-E111-8A11-003048D43980.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/08BE4523-4B61-E111-AA20-003048C64787.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0A00DE40-F45E-E111-BDFE-0030487F9351.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0A0AAEAC-4761-E111-B311-0030487E4EBF.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0A3E4134-795E-E111-BBFA-003048D43656.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0A40E6FF-0B5E-E111-A85B-0030487E4B8D.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0A414C31-E75D-E111-B672-0025901D4B04.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0A453645-945E-E111-A061-0030487F92A5.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0A574507-CB5D-E111-9D0F-0030487D5EB5.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0A692ACC-3361-E111-91C0-002481E0D678.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0A83A19A-F760-E111-927B-0030487E4EB5.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0A97F725-F860-E111-8676-0030487F929B.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0A983B14-8B5E-E111-9E7F-0030487F1BD7.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0C0AEF6A-7B5E-E111-A62F-002481E1026A.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0C2AB7AB-3161-E111-B25C-00215AD4D6C8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0C4AD0E3-315E-E111-A96F-0030487F929F.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0C51FA43-F55E-E111-9C58-003048C66BBE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0C6B3BEE-315E-E111-8BC3-003048C69408.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0C7D3AFD-E45D-E111-BC91-003048C693FA.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0C7D7F55-F960-E111-9E5F-0030487D5D93.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0C9A90AF-2F61-E111-846A-002481E0DC4E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0C9F1ECE-EA5D-E111-9731-00266CF27430.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0CA0F591-3261-E111-8F14-00215AD4D6C8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0CEDEC95-4E61-E111-AB6F-003048C64787.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0E189982-D25D-E111-9841-003048C693E2.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0E3FA0A2-4C5E-E111-BAA1-0030487E9881.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0E5FA39E-C95D-E111-900B-002481E0D144.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0E7FFD23-C95E-E111-B018-0030487F1A3F.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0E88F23D-205E-E111-A0A7-003048C692A4.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0E8CCDFB-6661-E111-86F4-0030487F1A3F.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0E9CE827-545E-E111-B33C-0030487E9881.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/0EF77082-165E-E111-827C-003048D45FCE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/105F888E-B55E-E111-81D0-003048D4DCD8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/10778551-235F-E111-8B85-0030487CF3F7.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/120CDB81-4061-E111-9053-003048C64787.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/12247C7A-135F-E111-9776-0030487F9351.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1238792E-CD5D-E111-A2E4-003048C662B8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/125C1796-6E60-E111-A538-0030487E4D11.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1295650F-CB5D-E111-9CB4-0030487D5EBD.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/129C9250-CE5D-E111-92B8-0025901D4C32.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/12B568FA-6B5E-E111-B525-003048C690A0.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/12C10516-B65E-E111-A00E-003048C692B4.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/14125F35-ED60-E111-ACDB-003048D43996.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/14665CBA-9361-E111-80DA-002481E0DE14.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/14717158-295E-E111-AB91-00237DDCBEA4.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1492A2D5-E05E-E111-B1D7-0030487F1C57.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/149BBF2E-0A61-E111-B1E3-0030487FA627.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/149C0B04-0B61-E111-A84A-003048F0E1EC.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/14FCFD73-DD5D-E111-825A-00266CFB8D74.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1614E6CF-E35D-E111-B633-002481E0D5E2.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/16C9E605-335F-E111-ABB0-0030487F91D7.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/16D479DD-685E-E111-AC8D-003048C69412.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/16F86836-7261-E111-A915-00237DE0BED6.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/16FA5994-3D5E-E111-BF21-0030487F1A55.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1818B843-6261-E111-B43A-0030487F1775.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/181DC917-B961-E111-8A74-003048CF6346.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/183C5CB9-BD5E-E111-892D-0030487F9331.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/185E44E2-E05D-E111-B6D8-003048C69292.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1877531C-B261-E111-B368-0025901D4D64.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/18CE8E87-085E-E111-9F00-0030487D5DA9.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/18FC8B1F-4F5E-E111-B305-0030487E9881.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1A302CDF-4761-E111-859A-003048C64787.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1A591FA5-3861-E111-A621-0030487D5E45.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1A9DF0B3-4B61-E111-AA81-003048C64787.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1AB3F3E7-475E-E111-BD07-0030487E5399.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1ADA5BA1-2961-E111-8419-003048D43958.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1C145192-5261-E111-BE56-003048C64787.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1C706EED-4B61-E111-B378-0030487F1661.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1C8292E8-0E5E-E111-9CA2-0030487D811F.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1C87B1B1-EF5D-E111-AA7E-003048F0E3BC.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1C91D343-1061-E111-9796-003048C69424.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1CAC0DA9-6E61-E111-8E1F-002481E94B4E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1CD1030D-CF5E-E111-A79D-0030487F1A3F.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1CFB5284-2A5E-E111-9E97-003048D479C0.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1E35BEC9-7E5E-E111-9569-003048D43996.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1E3FD85E-E660-E111-9BE1-0030487D5DA3.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1E55849D-F65E-E111-B57C-0030487F9351.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1E813A2F-D95D-E111-8B91-003048D436BE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/1E83E795-BA61-E111-B745-003048D4797E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2022B36B-F660-E111-8656-00215AD4D670.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/20378226-E15E-E111-B89C-003048C68F6A.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/206553A7-E85E-E111-9FFC-003048C66BBE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2069830A-AE61-E111-968C-002481E0DDBE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/208358B5-AD5E-E111-AE18-0030487F910D.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/20AED6DE-215E-E111-8D75-002481E101DC.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/20B90B95-F95D-E111-B26F-002481E0D678.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/20D48D60-685E-E111-9106-003048C6928C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/20DB039F-E45D-E111-A8BE-0030487FA629.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2204CB86-3961-E111-BFD1-002481E946EE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2268FF00-B361-E111-906D-00266CF327C0.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2276DF49-CB5D-E111-8278-003048C6903E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/228840B5-CD5E-E111-9A80-0030487F1A3F.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/22C99365-525E-E111-A636-00266CF330B8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/22C9A79E-C95D-E111-886E-002481E10CC2.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/22DE8370-835E-E111-B231-003048D439C0.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/22EBD507-245E-E111-BE6A-003048F02CBA.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/22F80F65-115E-E111-BAA3-003048C68A9C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/22FC0DB4-0861-E111-AF9E-0030487F6A8B.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/24229057-DE60-E111-9F9A-002481E946EE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/243C811E-0E5F-E111-BF39-0030487F9351.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/247AE3E7-B561-E111-97C2-002481E15184.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/248BE3B2-C061-E111-94DC-003048C662C8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2491EB60-D05E-E111-9322-0030487F1665.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/24B10871-6F5E-E111-B114-003048D37570.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/24C4E066-E35E-E111-81A9-0030487F1309.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/24F1A44C-3761-E111-BAED-003048D43996.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/260B9D9F-7A61-E111-8A33-0030487F933F.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/262282ED-F960-E111-927B-0030487D5EB3.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/267EB6A4-1E5E-E111-9B75-003048D436F2.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/26ACF792-705E-E111-BCA3-003048D3739C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/26B24EEC-3E61-E111-9386-003048D43996.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/26C50791-6961-E111-8CD5-0025901D4124.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/26F31B60-565E-E111-8C2B-D8D385FF6C5E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2822EF23-EB60-E111-AA14-0030487E4B8F.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2869BFE0-505E-E111-B801-0030487E9881.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2876991A-3761-E111-B226-003048D47912.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/28A22C57-AE61-E111-81A6-0030487D5DBF.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/28A80651-E460-E111-85AC-0030487E510B.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/28C6F031-2861-E111-812B-003048C69318.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2A286FC8-665E-E111-83AC-0030487D5E67.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2A383EED-DB5D-E111-AD95-003048D437D2.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2A6B5BDF-2A5E-E111-BB10-003048C68A98.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2AB538F0-AB5E-E111-9D82-003048D47776.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2AEA4173-CD5D-E111-857A-003048C66184.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2AF6964D-735E-E111-9202-003048C6931E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2C39E694-ED5D-E111-88F8-003048F0E826.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2C538F97-B85E-E111-A2DA-003048D439AA.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2C618672-D35E-E111-A3D4-0030487F1665.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2C6A30D9-4861-E111-9CC9-003048C64787.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2C761BF4-DF5D-E111-821A-003048D437A0.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2C859CF3-175F-E111-92AD-0030487F9351.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2C8EF400-E65E-E111-B44D-003048C66BBE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2CF9A662-765E-E111-8493-002481E7636A.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2CFC54EA-2161-E111-BEC5-0030487E5179.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2E1CE6EA-FC5E-E111-BA00-0030487F9351.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2E337309-CC5E-E111-9591-0030487F1A3F.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2E554DB7-815E-E111-9B74-003048D436B4.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2E5597D5-555E-E111-83EF-0030487F910D.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2E9D9081-BF61-E111-9662-003048C69296.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2EBA4923-2F61-E111-B947-003048CF6332.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2EBB01EA-845E-E111-889B-002481E0D6A6.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/2EEDAC55-0261-E111-9890-003048D4364C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3022D0AD-F75E-E111-9496-0030487F9351.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/30BE98F3-4561-E111-BA36-0030487E4EBF.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/30CBBF53-CE5D-E111-9A2D-003048C662B8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/30F4E48D-8361-E111-8E30-003048D47912.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/30FFE650-205E-E111-9FAF-003048C6617E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/32017150-655E-E111-8941-003048C692D4.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/32098764-6F5E-E111-A80D-003048C69026.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/32252204-785E-E111-A06A-0030487D7B79.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/322B53AE-875E-E111-927A-0030487FA629.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/32461670-AD61-E111-90B8-00266CF32E2C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3267AAE4-B561-E111-BE7B-002481E0D480.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/328797F2-DF5D-E111-8E9C-003048D3C8FC.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/32D67CC3-615E-E111-8D25-003048F0E84E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/32DA2006-C25E-E111-B17D-002481E0D69C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/32E093E8-DA5D-E111-B438-003048D437DA.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/347C419E-BC61-E111-8E32-0030487F1665.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/34BDE995-6E60-E111-8289-0030487D7103.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/34DBF5C8-185E-E111-8051-00266CF3338C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/34E4B9E2-E860-E111-A1FD-003048C68A90.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/34F3F161-115E-E111-81D0-003048C69414.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3605E55B-1261-E111-9A86-0030487F0EDD.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/364E8A57-C361-E111-9142-003048D439C0.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/36514B29-A161-E111-84B6-0025901D4C32.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/365ED14A-9F5E-E111-A1C4-0030487E55B9.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3661F107-F960-E111-8A46-00237DDCBEA4.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/368726FF-C65E-E111-88C3-0030487F1665.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/36B9C2C2-DA5E-E111-A934-0030487F1A3F.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/36BAB7C8-5461-E111-9303-003048C64787.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/36BB8E96-6E60-E111-892D-0030487E510B.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/36C6C57A-275E-E111-88D1-003048C662D4.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/36DA081F-D65E-E111-A6E1-0030487F1665.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/382D0E39-3F5E-E111-B9D8-0030487F91D9.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3838AC57-775E-E111-875D-002481E0DC82.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/383BD39E-8D5E-E111-BC5D-0030487F1BD7.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/384DEC95-6E60-E111-A647-0030487D857D.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/38B9E840-3A5E-E111-9ED1-002481E0DC64.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/38E019B7-115E-E111-8E6A-003048C693D8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3A29678C-7B5E-E111-8184-003048C68A9E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3A3A72D5-F560-E111-A34E-0030487E5179.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3A559BC7-C15E-E111-8DCE-003048C6903E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3A82D175-D45D-E111-85D7-003048D437DA.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3A9D72C3-2E5E-E111-BA2E-002481E0DC7C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3AC43E06-AA61-E111-92B3-00266CF32FA0.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3AD36C0B-F060-E111-B8F5-0030487D43E1.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3AEE7188-B161-E111-8819-0025901D484C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3C11E174-755E-E111-A69F-003048D439AC.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3C1464DF-345E-E111-81A1-0030487F88EB.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3C2791B8-EB60-E111-A69E-0030487F929B.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3C58A641-605E-E111-8EA5-0030487D5E45.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3C822D4A-1F5E-E111-87DB-0025901D4C32.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3CAB4EFF-F460-E111-AD84-003048C692B4.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3CCECDD1-1161-E111-A928-0030487F0EDD.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3CD651E0-295E-E111-A2CB-003048D437EC.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3CEDB5EE-4C61-E111-99DA-0030487F1661.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3E1EB217-C05E-E111-9DC2-0030487F1A55.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3E36BB9A-1461-E111-82A2-003048F0E18A.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3E4F32C0-615E-E111-B979-003048F0E188.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3E861446-6D5E-E111-8595-003048D43942.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3E9615D9-0F5E-E111-97F4-003048D462C8.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3EAB4479-F160-E111-B121-003048C6903E.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3EAF28F6-AE61-E111-99A7-003048C6903C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3EC4F946-7D61-E111-B2FF-0030487D811F.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3EC95EA2-5861-E111-BC74-0030487E4EBF.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3ED5D69F-B361-E111-BEA1-0025901D4D6C.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/3EFE6209-CB5D-E111-BFAA-0030487E52A3.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/400E6EA3-5661-E111-90A1-003048C64787.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/404E21BB-EE5E-E111-8208-003048C66BBE.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/404E3D70-4D61-E111-B897-0030487E4EBF.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/404FC36A-4961-E111-855C-003048C64787.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/404FD6A7-0D61-E111-907D-0030487F9297.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/405EB7B7-B261-E111-8237-0025901D4B00.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/406A8053-B561-E111-8EAD-003048D439B4.root',
'/store/mc/Summer12/MinBias_TuneZ2star_8TeV-pythia6/GEN-SIM/START50_V13-v3/0000/407E670F-CB5D-E111-AC93-0030487D5EBD.root'
	)
import random
random.seed(process.RandomNumberGeneratorService.generator.initialSeed.value())
random.shuffle(process.mix.input.fileNames)


process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    annotation = cms.untracked.string('Madgraph ZZ -> 2l2nu at 8TeV'),
    name = cms.untracked.string('$Source: /local/reps/CMSSW/UserCode/CMG/CMGTools/HtoZZ2l2nu/test/gen/cmsRun_FullSim.py,v $')
)

# Output definition

process.AODSIMoutput = cms.OutputModule("PoolOutputModule",
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    outputCommands = process.AODSIMEventContent.outputCommands,
    fileName = cms.untracked.string('/afs/cern.ch/user/p/psilva/work/FullSim/VBFZ_interf/Events_XXX_I_XXX.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('AODSIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'START52_V9::All'

process.generator = cms.EDFilter("Pythia6HadronizerFilter",
    ExternalDecays = cms.PSet(
        Tauola = cms.untracked.PSet(
            UseTauolaPolarization = cms.bool(True),
            InputCards = cms.PSet(
                mdtau = cms.int32(0),
                pjak2 = cms.int32(0),
                pjak1 = cms.int32(0)
            )
        ),
        parameterSets = cms.vstring('Tauola')
    ),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    filterEfficiency = cms.untracked.double(1.0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(8000.0),
    maxEventsToPrint = cms.untracked.int32(0),
    PythiaParameters = cms.PSet(
        pythiaUESettings = cms.vstring('MSTU(21)=1     ! Check on possible errors during program execution', 
            'MSTJ(22)=2     ! Decay those unstable particles', 
            'PARJ(71)=10 .  ! for which ctau  10 mm', 
            'MSTP(33)=0     ! no K factors in hard cross sections', 
            'MSTP(2)=1      ! which order running alphaS', 
            'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)', 
            'MSTP(52)=2     ! work with LHAPDF', 
            'PARP(82)=1.921 ! pt cutoff for multiparton interactions', 
            'PARP(89)=1800. ! sqrts for which PARP82 is set', 
            'PARP(90)=0.227 ! Multiple interactions: rescaling power', 
            'MSTP(95)=6     ! CR (color reconnection parameters)', 
            'PARP(77)=1.016 ! CR', 
            'PARP(78)=0.538 ! CR', 
            'PARP(80)=0.1   ! Prob. colored parton from BBR', 
            'PARP(83)=0.356 ! Multiple interactions: matter distribution parameter', 
            'PARP(84)=0.651 ! Multiple interactions: matter distribution parameter', 
            'PARP(62)=1.025 ! ISR cutoff', 
            'MSTP(91)=1     ! Gaussian primordial kT', 
            'PARP(93)=10.0  ! primordial kT-max', 
            'MSTP(81)=21    ! multiple parton interactions 1 is Pythia default', 
            'MSTP(82)=4     ! Defines the multi-parton model'),
         parameterSets = cms.vstring('pythiaUESettings') 
    )
)


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.AODSIMoutput_step])
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 


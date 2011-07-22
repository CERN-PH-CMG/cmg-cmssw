import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY")

pickRelVal = False

import datetime,os
d = datetime.datetime.today()
tag = os.getenv('USER') + '_' + d.strftime('%d%h%y-%Hh%Mm%Ss')



# process.source.fileNames = ['file:2011/Loukas/HT_PromptReco_42X_pfMETgt150_AND_pfMETovCaloMETgt05.root']
# High MHT cuts only
# process.source.eventsToProcess  = cms.untracked.VEventRange("166408:887:1111839383", "167807:1085:1360573388", "167807:237:292279224", "166049:257:339177021", "166554:242:285251939", 
#    "167281:252:321568659", "166512:1029:1195756956", "165415:1346:1594777376", "167675:245:268687499", "167675:854:901296707", 
#    "167913:210:223858414", "167898:233:235117681", "166380:880:969589698", "166763:343:382348162", "166950:110:127740977", 
#    "165570:174:231241493", "165570:157:206640799", "165570:363:485628899", "166923:161:177551447", "166462:186:178633172", 
#    "166563:137:147841995", "166841:137:149460393", "166841:888:929214969", "166033:777:1061889100", "166859:70:48357129", 
#    "166699:354:380002939", "166699:699:745620764", "166699:883:927560823", "166890:284:314687547", "167098:289:303387081", 
#    "166922:10:11181386", "165633:271:348405804", "166960:24:20016067")


process.load("CMGTools.Common.sources.HT.Run2011A_HighMET_PromptSkim_v4.RAW_RECO.source_160404_167913_cff")

# rejected by beam halo, but not by PBNR
# process.source.eventsToProcess = cms.untracked.VEventRange("166960:75:67598316", "165993:1609:1671377126", "167740:82:53357625", "166699:498:539558074")

# rejected by had PBNR, but not by beam halo:
# process.source.eventsToProcess = cms.untracked.VEventRange("166429:38:16903548", "165487:125:160814947", "167282:188:191950444", "167676:139:126636158", "165567:390:485635245", 
#    "166554:474:566681262", "166554:499:596554731", "166033:947:1270957216", "165993:154:157383124")

#process.source.fileNames = ['/store/relval/CMSSW_4_2_3/RelValLM1_sfts/GEN-SIM-RECO/MC_42_V12-v2/0062/84905E5D-0E7B-E011-8EF2-003048D15E14.root', '/store/relval/CMSSW_4_2_3/RelValLM1_sfts/GEN-SIM-RECO/MC_42_V12-v2/0062/CA07E500-137B-E011-B9BA-0026189438AA.root', '/store/relval/CMSSW_4_2_3/RelValLM1_sfts/GEN-SIM-RECO/MC_42_V12-v2/0062/ECC06B4B-0C7B-E011-AC75-002618943916.root', '/store/relval/CMSSW_4_2_3/RelValLM1_sfts/GEN-SIM-RECO/MC_42_V12-v2/0063/2EA656A0-B37B-E011-BCD8-003048678DA2.root', '/store/relval/CMSSW_4_2_3/RelValLM1_sfts/GEN-SIM-RECO/MC_42_V12-v2/0063/BA3F879F-8B7B-E011-93CE-001A928116E0.root']

process.source.eventsToProcess = cms.untracked.VEventRange("165993:1281:1373910616", "166049:377:505460338", "166554:499:596554731", "166512:273:327982246", "166429:43:23864655", 
    "165548:152:208888550", "166888:318:375457039", "166462:420:434270834", "165617:142:195827080", "166841:445:482355083", 
    "166839:93:94055132", "166839:150:164327895")

print process.source

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.load("Configuration.EventContent.EventContent_cff")
process.out = cms.OutputModule(
    "PoolOutputModule",
    # process.RECOSIMEventContent,
    outputCommands =  cms.untracked.vstring( 'keep *',
                                             # the following is much, much overhead
                                             'drop *_lumiProducer_*_*'
                                             ),
    fileName = cms.untracked.string('selectVEventRange_'+tag+'_out.root'),
    )

process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1


outFile = open('selectVEventRange_' + tag + '_source.py','w')
outFile.write('import FWCore.ParameterSet.Config as cms\n')
outFile.write(process.source.dumpPython() )
outFile.close()

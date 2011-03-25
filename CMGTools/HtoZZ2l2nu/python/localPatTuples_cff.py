import FWCore.ParameterSet.Config as cms
import os

def fillFromCastor(dir):
    os.system('rfdir ' + dir + ' | awk \'{print $9}\' > /tmp/castordump')
    inFile = open('/tmp/castordump', 'r')
    localdataset=cms.untracked.vstring()
    for line in inFile.readlines():
        if(len(line)==0) : continue
        if(line.find('histograms')>0 or line.find('monitor')>0): continue
        sline=str('rfio://' + dir + '/' + line.split()[0])
        localdataset.extend( [ sline ] )
    os.system('rm /tmp/castordump')
    return localdataset


"""
selects from a local source
"""
def getLocalSourceFor( theLocalSrc='GluGluToH200ToZZTo2L2Nu') :
    if( theLocalSrc=='GluGluToHToWWTo2L2NuM200' ) : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToWWTo2L2Nu_M-200/GluGluToHToWWTo2L2Nu_M-200') 
    if( theLocalSrc=='GluGluToHToWWTo2L2NuM400' ) : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToWWTo2L2Nu_M-400/GluGluToHToWWTo2L2Nu_M-400')
    if( theLocalSrc=='GluGluToHToZZTo2L2NuM200' ) : return  fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToZZTo2L2Nu_M-200/GluGluToHToZZTo2L2Nu_M-200')
    if( theLocalSrc=='GluGluToHToZZTo2L2NuM400' ) : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToZZTo2L2Nu_M-400/GluGluToHToZZTo2L2Nu_M-400')
    if( theLocalSrc=='GluGluToHToZZTo4LM200' )    : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToZZTo4L_M-200/GluGluToHToZZTo4L_M-200')
    if( theLocalSrc=='GluGluToHToZZTo4LM400' )    : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToZZTo4L_M-400/GluGluToHToZZTo4L_M-400') 
    if( theLocalSrc=='VBF_HToWWTo2L2NuM200' )     : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/VBF_HToWWTo2L2Nu_M-200/VBF_HToWWTo2L2Nu_M-200')
    if( theLocalSrc=='VBF_HToWWTo2L2NuM400' )     : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/VBF_HToWWTo2L2Nu_M-400/VBF_HToWWTo2L2Nu_M-400')
    if( theLocalSrc=='VBF_ToHToZZTo4LM200' )      : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/VBF_ToHToZZTo4L_M-200/VBF_ToHToZZTo4L_M-200')
    if( theLocalSrc=='DYToEEM20' )                : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/DYToEE_M-20/DYToEE_M-20')
    if( theLocalSrc=='DYToMuMuM20' )              : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/DYToMuMu_M-20/DYToMuMu_M-20')
    if( theLocalSrc=='DYToTauTauM20' )            : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/DYToTauTau_M-20/DYToTauTau_M-20')
    if( theLocalSrc=='TTJetsmadgraph' )           : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/TTJets_madgraph/TTJets_madgraph')
    if( theLocalSrc=='TToBLNuschannel' )          : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/TToBLNu_s-channel/TToBLNu_s-channel')
    if( theLocalSrc=='TToBLNutchannel' )          : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/TToBLNu_t-channel/TToBLNu_t-channel')
    if( theLocalSrc=='TToBLNutWchannel' )         : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/TToBLNu_tW-channel/TToBLNu_tW-channel')
    if( theLocalSrc=='WJetsToLNu' )               : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/WJetsToLNu/WJetsToLNu')
    if( theLocalSrc=='WWtoAnything' )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/WWtoAnything/WWtoAnything')
    if( theLocalSrc=='WZtoAnything' )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/WZtoAnything/WZtoAnything')
    if( theLocalSrc=='ZZtoAnything' )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/ZZtoAnything/ZZtoAnything')


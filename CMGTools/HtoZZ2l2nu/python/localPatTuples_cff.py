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

    dtag=theLocalSrc.split(':')
    theLocalSrc=dtag[0]
    prod=''
    if(len(dtag)>1):prod='_'+dtag[1]
             
    #Fall10/Spring11
    if( theLocalSrc=='GluGluToHToWWTo2L2NuM200'+prod ) : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToWWTo2L2Nu_M-200/GluGluToHToWWTo2L2Nu_M-200'+prod) 
    if( theLocalSrc=='GluGluToHToWWTo2L2NuM400'+prod ) : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToWWTo2L2Nu_M-400/GluGluToHToWWTo2L2Nu_M-400'+prod)
    if( theLocalSrc=='GluGluToHToZZTo2L2NuM200'+prod ) : return  fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToZZTo2L2Nu_M-200/GluGluToHToZZTo2L2Nu_M-200'+prod)
    if( theLocalSrc=='GluGluToHToZZTo2L2NuM400'+prod ) : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToZZTo2L2Nu_M-400/GluGluToHToZZTo2L2Nu_M-400'+prod)
    if( theLocalSrc=='GluGluToHToZZTo4LM200'+prod )    : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToZZTo4L_M-200/GluGluToHToZZTo4L_M-200'+prod)
    if( theLocalSrc=='GluGluToHToZZTo4LM400'+prod )    : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToZZTo4L_M-400/GluGluToHToZZTo4L_M-400'+prod) 
    if( theLocalSrc=='VBF_HToWWTo2L2NuM200'+prod )     : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/VBF_HToWWTo2L2Nu_M-200/VBF_HToWWTo2L2Nu_M-200'+prod)
    if( theLocalSrc=='VBF_HToWWTo2L2NuM400'+prod )     : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/VBF_HToWWTo2L2Nu_M-400/VBF_HToWWTo2L2Nu_M-400'+prod)
    if( theLocalSrc=='VBF_ToHToZZTo4LM200'+prod )      : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/VBF_ToHToZZTo4L_M-200/VBF_ToHToZZTo4L_M-200'+prod)
    if( theLocalSrc=='DYToEEM20'+prod )                : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/DYToEE_M-20/DYToEE_M-20'+prod)
    if( theLocalSrc=='DYToMuMuM20'+prod )              : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/DYToMuMu_M-20/DYToMuMu_M-20'+prod)
    if( theLocalSrc=='DYToTauTauM20'+prod )            : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/DYToTauTau_M-20/DYToTauTau_M-20'+prod)
    if( theLocalSrc=='TTJetsmadgraph'+prod )           : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/TTJets_madgraph/TTJets_madgraph'+prod)
    if( theLocalSrc=='TToBLNuschannel'+prod )          : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/TToBLNu_s-channel/TToBLNu_s-channel'+prod)
    if( theLocalSrc=='TToBLNutchannel'+prod )          : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/TToBLNu_t-channel/TToBLNu_t-channel'+prod)
    if( theLocalSrc=='TToBLNutWchannel'+prod )         : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/TToBLNu_tW-channel/TToBLNu_tW-channel'+prod)
    if( theLocalSrc=='WJetsToLNu'+prod )               : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/WJetsToLNu/WJetsToLNu'+prod)
    if( theLocalSrc=='WWtoAnything'+prod )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/WWtoAnything/WWtoAnything'+prod)
    if( theLocalSrc=='WZtoAnything'+prod )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/WZtoAnything/WZtoAnything'+prod)
    if( theLocalSrc=='ZZtoAnything'+prod )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/ZZtoAnything/ZZtoAnything'+prod)

    #Winter10
    if( theLocalSrc=='DYJetsToLL_PU2010' )        : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/DYJetsToLL_PU2010') 
    if( theLocalSrc=='DYToEE_M-20_FlatPU')        : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/DYToEE_M-20_FlatPU') 
    if( theLocalSrc=='TTJets_madgraph_FlatPU')    : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/TTJets_madgraph_FlatPU')
    if( theLocalSrc=='TToBLNu_s-channel_FlatPU')  : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/TToBLNu_s-channel_FlatPU')
    if( theLocalSrc=='TToBLNu_t-channel_FlatPU')  : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/TToBLNu_t-channel_FlatPU')
    if( theLocalSrc=='TToBLNu_tW-channel_FlatPU') : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/TToBLNu_tW-channel_FlatPU')
    if( theLocalSrc=='WJetsToLNu_PU2010')         : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/WJetsToLNu_PU2010')
    if( theLocalSrc=='WWtoAnything_PU2010')       : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/WWtoAnything_PU2010')
    if( theLocalSrc=='WZtoAnything_FlatPU')       : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/WZtoAnything_FlatPU')
    if( theLocalSrc=='WZtoAnything_PU2010')       : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/WZtoAnything_PU2010')
    if( theLocalSrc=='ZZtoAnything_FlatPU')       : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/ZZtoAnything_FlatPU')

    #data
    if( theLocalSrc=='Collisions2011_DoubleMuon' ) : return fillFromCastor('/castor/cern.ch/cms/store/cmst3/user/psilva/Collisions2011/DoubleMuon_v2')
    if( theLocalSrc=='Collisions2011_DoubleElectron' ) : return fillFromCastor('/castor/cern.ch/cms/store/cmst3/user/psilva/Collisions2011/DoubleElectron_v2')
    if( theLocalSrc=='Collisions2011_MuEG' ) : return fillFromCastor('/castor/cern.ch/cms/store/cmst3/user/psilva/Collisions2011/MuEG_v2')


        
                             

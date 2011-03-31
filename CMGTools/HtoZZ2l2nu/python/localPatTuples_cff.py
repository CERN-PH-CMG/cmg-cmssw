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

    prod=''    
    if( theLocalSrc.find('_')>0 ):
        dtag = theLocalSrc.rsplit('_',1)
        if(len(dtag)>1):
            prod='_'+dtag[1]
            if(prod.find('Spring11')<0) : prod=''

    #Fall10/Spring11
    if( theLocalSrc.find('GluGluToHToWWTo2L2NuM200')>=0 ) : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/GluGluToHToWWTo2L2Nu_M-200/GluGluToHToWWTo2L2Nu_M-200'+prod) 
    if( theLocalSrc.find('GluGluToHToWWTo2L2NuM400')>=0 ) : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/GluGluToHToWWTo2L2Nu_M-400/GluGluToHToWWTo2L2Nu_M-400'+prod)
    if( theLocalSrc.find('GluGluToHToZZTo2L2NuM200')>=0 ) : return  fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/GluGluToHToZZTo2L2Nu_M-200/GluGluToHToZZTo2L2Nu_M-200'+prod)
    if( theLocalSrc.find('GluGluToHToZZTo2L2NuM400')>=0 ) : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/GluGluToHToZZTo2L2Nu_M-400/GluGluToHToZZTo2L2Nu_M-400'+prod)
    if( theLocalSrc.find('GluGluToHToZZTo4LM200')>=0 )    : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/GluGluToHToZZTo4L_M-200/GluGluToHToZZTo4L_M-200'+prod)
    if( theLocalSrc.find('GluGluToHToZZTo4LM400')>=0 )    : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/GluGluToHToZZTo4L_M-400/GluGluToHToZZTo4L_M-400'+prod) 
    if( theLocalSrc.find('VBF_HToWWTo2L2NuM200')>=0 )     : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/VBF_HToWWTo2L2Nu_M-200/VBF_HToWWTo2L2Nu_M-200'+prod)
    if( theLocalSrc.find('VBF_HToWWTo2L2NuM400')>=0 )     : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/VBF_HToWWTo2L2Nu_M-400/VBF_HToWWTo2L2Nu_M-400'+prod)
    if( theLocalSrc.find('VBF_ToHToZZTo4LM200')>=0 )      : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/VBF_ToHToZZTo4L_M-200/VBF_ToHToZZTo4L_M-200'+prod)
    if( theLocalSrc.find('DYToEEM20')>=0 )                : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/DYToEE_M-20/DYToEE_M-20'+prod)
    if( theLocalSrc.find('DYToMuMuM20')>=0 )              : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/DYToMuMu_M-20/DYToMuMu_M-20'+prod)
    if( theLocalSrc.find('DYToTauTauM20')>=0 )            : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/DYToTauTau_M-20/DYToTauTau_M-20'+prod)
    if( theLocalSrc.find('TTJetsmadgraph')>=0 )           : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/TTJets_madgraph/TTJets_madgraph'+prod)
    if( theLocalSrc.find('TToBLNuschannel')>=0 )          : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/TToBLNu_s-channel/TToBLNu_s-channel'+prod)
    if( theLocalSrc.find('TToBLNutchannel')>=0 )          : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/TToBLNu_t-channel/TToBLNu_t-channel'+prod)
    if( theLocalSrc.find('TToBLNutWchannel')>=0 )         : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/TToBLNu_tW-channel/TToBLNu_tW-channel'+prod)
    if( theLocalSrc.find('WJetsToLNu')>=0 )               : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/WJetsToLNu/WJetsToLNu'+prod)
    if( theLocalSrc.find('WWtoAnything')>=0 )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/WWtoAnything/WWtoAnything'+prod)
    if( theLocalSrc.find('WZtoAnything')>=0 )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/WZtoAnything/WZtoAnything'+prod)
    if( theLocalSrc.find('ZZtoAnything')>=0 )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/ZZtoAnything/ZZtoAnything'+prod)

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
    if( theLocalSrc=='Collisions2011_DoubleMuon' ) : return fillFromCastor('/castor/cern.ch/cms/store/cmst3/user/psilva/Dileptons/Collisions2011/DoubleMuon_v2')
    if( theLocalSrc=='Collisions2011_DoubleElectron' ) : return fillFromCastor('/castor/cern.ch/cms/store/cmst3/user/psilva/Dileptons/Collisions2011/DoubleElectron_v2')
    if( theLocalSrc=='Collisions2011_MuEG' ) : return fillFromCastor('/castor/cern.ch/cms/store/cmst3/user/psilva/Dileptons/Collisions2011/MuEG_v2')


        
                             

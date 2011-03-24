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

GluGluToHToZZTo2L2NuM400 = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToZZTo2L2Nu_M-400/GluGluToHToZZTo2L2Nu_M-400')
GluGluToHToWWTo2L2NuM200 = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToWWTo2L2Nu_M-200/GluGluToHToWWTo2L2Nu_M-200')
GluGluToHToWWTo2L2NuM400 = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToWWTo2L2Nu_M-400/GluGluToHToWWTo2L2Nu_M-400')
GluGluToHToZZTo2L2NuM200 = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToZZTo2L2Nu_M-200/GluGluToHToZZTo2L2Nu_M-200')
GluGluToHToZZTo2L2NuM400 = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToZZTo2L2Nu_M-400/GluGluToHToZZTo2L2Nu_M-400')
GluGluToHToZZTo4LM200 = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToZZTo4L_M-200/GluGluToHToZZTo4L_M-200')
GluGluToHToZZTo4LM400 = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/GluGluToHToZZTo4L_M-400/GluGluToHToZZTo4L_M-400')
VBF_HToWWTo2L2NuM200 = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/VBF_HToWWTo2L2Nu_M-200/VBF_HToWWTo2L2Nu_M-200')
VBF_HToWWTo2L2NuM400 = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/VBF_HToWWTo2L2Nu_M-400/VBF_HToWWTo2L2Nu_M-400')
VBF_ToHToZZTo4LM200 = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/VBF_ToHToZZTo4L_M-200/VBF_ToHToZZTo4L_M-200')
DYToEEM20 = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/DYToEE_M-20/DYToEE_M-20')
DYToMuMuM20 = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/DYToMuMu_M-20/DYToMuMu_M-20')
DYToTauTauM20 = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/DYToTauTau_M-20/DYToTauTau_M-20')
TTJetsmadgraph = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/TTJets_madgraph/TTJets_madgraph')
TToBLNuschannel = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/TToBLNu_s-channel/TToBLNu_s-channel')
TToBLNutchannel = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/TToBLNu_t-channel/TToBLNu_t-channel')
TToBLNutWchannel = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/TToBLNu_tW-channel/TToBLNu_tW-channel')
WJetsToLNu = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/WJetsToLNu/WJetsToLNu')
WWtoAnything = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/WWtoAnything/WWtoAnything')
WZtoAnything = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/WZtoAnything/WZtoAnything')
ZZtoAnything = fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/ZZtoAnything/ZZtoAnything')



"""
defines a local source from a given tag
"""
def defineLocalSourceFor( theLocalSrc='GluGluToH200ToZZTo2L2Nu' ) :
    baseCastorDir='/castor/cern.ch/cms/store/cmst3/user/psilva/'
    return fillFromCastor(baseCastorDir + '/' + theLocalSrc)

"""
selects partial data from the all available
"""
def getLocalSourceFor( theLocalSrc='GluGluToH200ToZZTo2L2Nu', ffile=0, fstep=-1  ) :
    localSrc = defineLocalSourceFor( theLocalSrc )
    prunedLocalSrc = cms.untracked.vstring()
    if(fstep<0): fstep = len(localSrc)
    lfile=ffile+fstep
    if(ffile<lfile) :
        fctr=0
        for i in localSrc :
            if(fctr>=ffile and fctr<lfile) :
                prunedLocalSrc.append( i )
            fctr+=1
    return prunedLocalSrc

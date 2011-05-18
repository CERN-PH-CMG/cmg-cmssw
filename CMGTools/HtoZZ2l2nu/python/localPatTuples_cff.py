import FWCore.ParameterSet.Config as cms
import os,sys

"""
lists the files available in castor
"""
def fillFromCastor(dir,ffile=0,step=-1):
    os.system('rfdir ' + dir + ' | awk \'{print $9}\' > /tmp/castordump')
    inFile = open('/tmp/castordump', 'r')
    localdataset=cms.untracked.vstring()
    ifile=0
    for line in inFile.readlines():
        if(len(line)==0) : continue
        if(line.find('histograms')>0 or line.find('monitor')>0): continue
        sline=str('rfio://' + dir + '/' + line.split()[0])
        if(ifile>=ffile):
            if( (step<0) or  (step>0 and ifile<ffile+step) ):
                localdataset.extend( [ sline ] )
        ifile=ifile+1
    os.system('rm /tmp/castordump')
    return localdataset


"""
wrapper to read the configuration from comand line
args: castor_directory output_file first_file step
"""
def configureFromCommandLine(processSource) :
    castorDir='/castor/cern.ch/cms/store/cmst3/user/psilva/Dileptons/GluGluToHToWWTo2L2Nu_M-200/GluGluToHToWWTo2L2Nu_M-200'
    if(len(sys.argv)>2 ): castorDir=sys.argv[2]
    dtag=os.path.basename(castorDir)
    outputFile=dtag+'.root'
    if(len(sys.argv)>3 ): outputFile=sys.argv[3]
    ffile=0
    step=-1
    if(len(sys.argv)>4 ):
        if(sys.argv[4].isdigit()) : ffile=int(sys.argv[4])
    if(len(sys.argv)>5 ):
        if(sys.argv[5].isdigit()) : step=int(sys.argv[5])

    return dtag, fillFromCastor(castorDir,ffile,step), outputFile



"""
selects from a local source
"""
def getLocalSourceFor( theLocalSrc='GluGluToH200ToZZTo2L2Nu',ffile=0,step=-1) :

    prod=''    
    if( theLocalSrc.find('_')>0 ):
        dtag = theLocalSrc.rsplit('_',1)
        print dtag
        if(len(dtag)>1):
            prod='_'+dtag[1]
            if(prod.find('Spring11')<0) : prod=''

    #data
    if( theLocalSrc=='Collisions2011_DoubleMuon' ) : return fillFromCastor('/castor/cern.ch/cms/store/cmst3/user/psilva/Dileptons/DoubleMuon/DoubleMuon-v3',ffile,step)
    if( theLocalSrc=='Collisions2011_DoubleElectron' ) : return fillFromCastor('/castor/cern.ch/cms/store/cmst3/user/psilva/Dileptons/DoubleElectron/DoubleElectron-v3',ffile,step)
    if( theLocalSrc=='Collisions2011_MuEG' ) : return fillFromCastor('/castor/cern.ch/cms/store/cmst3/user/psilva/Dileptons/MuEG/MuEG-v3',ffile,step)

    #Fall10/Spring11
    if( theLocalSrc.find('GluGluToHToWWTo2L2NuM200')>=0 ) : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/GluGluToHToWWTo2L2Nu_M-200/GluGluToHToWWTo2L2Nu_M-200'+prod,ffile,step) 
    if( theLocalSrc.find('GluGluToHToWWTo2L2NuM400')>=0 ) : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/GluGluToHToWWTo2L2Nu_M-400/GluGluToHToWWTo2L2Nu_M-400'+prod,ffile,step)
    if( theLocalSrc.find('GluGluToHToZZTo2L2NuM200')>=0 ) : return  fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/GluGluToHToZZTo2L2Nu_M-200/GluGluToHToZZTo2L2Nu_M-200'+prod,ffile,step)
    if( theLocalSrc.find('GluGluToHToZZTo2L2NuM400')>=0 ) : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/GluGluToHToZZTo2L2Nu_M-400/GluGluToHToZZTo2L2Nu_M-400'+prod,ffile,step)
    if( theLocalSrc.find('GluGluToHToZZTo4LM200')>=0 )    : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/GluGluToHToZZTo4L_M-200/GluGluToHToZZTo4L_M-200'+prod,ffile,step)
    if( theLocalSrc.find('GluGluToHToZZTo4LM400')>=0 )    : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/GluGluToHToZZTo4L_M-400/GluGluToHToZZTo4L_M-400'+prod,ffile,step) 
    if( theLocalSrc.find('VBF_HToWWTo2L2NuM200')>=0 )     : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/VBF_HToWWTo2L2Nu_M-200/VBF_HToWWTo2L2Nu_M-200'+prod,ffile,step)
    if( theLocalSrc.find('VBF_HToWWTo2L2NuM400')>=0 )     : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/VBF_HToWWTo2L2Nu_M-400/VBF_HToWWTo2L2Nu_M-400'+prod,ffile,step)
    if( theLocalSrc.find('VBF_ToHToZZTo4LM200')>=0 )      : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/VBF_ToHToZZTo4L_M-200/VBF_ToHToZZTo4L_M-200'+prod,ffile,step)
    if( theLocalSrc.find('DYToEEM20')>=0 )                : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/DYToEE_M-20/DYToEE_M-20'+prod,ffile,step)
    if( theLocalSrc.find('DYToMuMuM20')>=0 )              : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/DYToMuMu_M-20/DYToMuMu_M-20'+prod,ffile,step)
    if( theLocalSrc.find('DYToTauTauM20')>=0 )            : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/DYToTauTau_M-20/DYToTauTau_M-20'+prod,ffile,step)
    if( theLocalSrc.find('TTJetsmadgraph')>=0 )           : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/TTJets_madgraph/TTJets_madgraph'+prod,ffile,step)
    if( theLocalSrc.find('TToBLNuschannel')>=0 )          : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/TToBLNu_s-channel/TToBLNu_s-channel'+prod,ffile,step)
    if( theLocalSrc.find('TToBLNutchannel')>=0 )          : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/TToBLNu_t-channel/TToBLNu_t-channel'+prod,ffile,step)
    if( theLocalSrc.find('TToBLNutWchannel')>=0 )         : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/TToBLNu_tW-channel/TToBLNu_tW-channel'+prod,ffile,step)
    if( theLocalSrc.find('WJetsToLNu')>=0 )               : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/WJetsToLNu/WJetsToLNu'+prod,ffile,step)
    if( theLocalSrc.find('WWtoAnything')>=0 )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/WWtoAnything/WWtoAnything'+prod,ffile,step)
    if( theLocalSrc.find('WZtoAnything')>=0 )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/WZtoAnything/WZtoAnything'+prod,ffile,step)
    if( theLocalSrc.find('ZZtoAnything')>=0 )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/ZZtoAnything/ZZtoAnything'+prod,ffile,step)
    if( theLocalSrc.find('QCD_Pt-30to50')>=0 )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/QCD_Pt-30to50/QCD_Pt-30to50'+prod,ffile,step)
    if( theLocalSrc.find('QCD_Pt-50to80')>=0 )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/QCD_Pt-50to80/QCD_Pt-50to80'+prod,ffile,step)
    if( theLocalSrc.find('QCD_Pt-80to120')>=0 )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/QCD_Pt-80to120/QCD_Pt-80to120'+prod,ffile,step)
    if( theLocalSrc.find('QCD_Pt-120to170')>=0 )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/QCD_Pt-120to170/QCD_Pt-120to170'+prod,ffile,step)
    if( theLocalSrc.find('QCD_Pt-170to300')>=0 )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/QCD_Pt-170to300/QCD_Pt-170to300'+prod,ffile,step)
    if( theLocalSrc.find('QCD_Pt-300to470')>=0 )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/QCD_Pt-300to470/QCD_Pt-300to470'+prod,ffile,step)
    if( theLocalSrc.find('QCD_Pt-470to600')>=0 )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/QCD_Pt-470to600/QCD_Pt-470to600'+prod,ffile,step)
    if( theLocalSrc.find('QCD_Pt-600to800')>=0 )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/QCD_Pt-600to800/QCD_Pt-600to800'+prod,ffile,step)
    if( theLocalSrc.find('QCD_Pt-800to1000')>=0 )             : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/psilva/Dileptons/QCD_Pt-800to1000/QCD_Pt-800to1000'+prod,ffile,step)

    #Winter10
    if( theLocalSrc=='DYJetsToLL_PU2010' )        : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/DYJetsToLL_PU2010',ffile,step) 
    if( theLocalSrc=='DYToEE_M-20_FlatPU')        : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/DYToEE_M-20_FlatPU',ffile,step) 
    if( theLocalSrc=='TTJets_madgraph_FlatPU')    : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/TTJets_madgraph_FlatPU',ffile,step)
    if( theLocalSrc=='TToBLNu_s-channel_FlatPU')  : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/TToBLNu_s-channel_FlatPU',ffile,step)
    if( theLocalSrc=='TToBLNu_t-channel_FlatPU')  : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/TToBLNu_t-channel_FlatPU',ffile,step)
    if( theLocalSrc=='TToBLNu_tW-channel_FlatPU') : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/TToBLNu_tW-channel_FlatPU',ffile,step)
    if( theLocalSrc=='WJetsToLNu_PU2010')         : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/WJetsToLNu_PU2010',ffile,step)
    if( theLocalSrc=='WWtoAnything_PU2010')       : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/WWtoAnything_PU2010',ffile,step)
    if( theLocalSrc=='WZtoAnything_FlatPU')       : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/WZtoAnything_FlatPU',ffile,step)
    if( theLocalSrc=='WZtoAnything_PU2010')       : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/WZtoAnything_PU2010',ffile,step)
    if( theLocalSrc=='ZZtoAnything_FlatPU')       : return fillFromCastor('/castor/cern.ch//cms/store/cmst3/user/cerminar/ZZllvv_sel3/ZZtoAnything_FlatPU',ffile,step)


        
                             

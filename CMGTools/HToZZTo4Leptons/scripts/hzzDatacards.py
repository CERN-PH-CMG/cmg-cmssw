import ROOT
from CMGTools.HToZZTo4Leptons.macros.stat.DataCardMaker import *
from CMGTools.HToZZTo4Leptons.macros.hzzSetup import * 

higgsMass=125

finalStates=['MuMu','EleEle','MuEle']
periods =['7TeV','8TeV']

#finalStates=['MuMu']
#periods =['8TeV']


categories=['inc']

cuts=dict()
cuts['inc'] ='H_Mass>100&&H_Z2_Mass>12&&H_MinOSPairMass>4' 
cuts['vbftag'] ='H_Mass>100&&H_NJets==2&&abs(H_DEta)>3&&H_MJJ>300'
cuts['novbflow'] ='H_Mass>100&&H_NJets!=2&&H_Pt<50'
cuts['novbfhigh'] ='H_Mass>100&&H_NJets!=2&&H_Pt>50'
 
variables=dict()
variables['inc']='H_MELA:H_Mass'
variables['novbflow']='H_MELA:H_Mass'
variables['novbfhigh']='H_MELA:H_Mass'
variables['vbftag']='H_MELA:H_Mass'

model=dict()
model['novbflow']='param2D_MELA'
model['novbfhigh']='param2D_MELA'
model['vbftag']='param2D_MELA'
model['inc']='param2D_MELA'


hzzSetup = HZZSetup()
#############################################################################
for category in categories:
    for period in periods:
        for finalstate in finalStates:
             #create plotters
            ggH  = hzzSetup.getHiggsParamPlotter(finalstate,'ggH',period,higgsMass,category)
            qqH  = hzzSetup.getHiggsParamPlotter(finalstate,'qqH',period,higgsMass,category)
            WH   = hzzSetup.getHiggsParamPlotter(finalstate,'WH',period,higgsMass,category)
            ZH   = hzzSetup.getHiggsParamPlotter(finalstate,'ZH',period,higgsMass,category)
            ttH  = hzzSetup.getHiggsParamPlotter(finalstate,'ttH',period,higgsMass,category)
            QQZZ = hzzSetup.getZZParamPlotter(finalstate,'QQZZ',period,category)
            GGZZ = hzzSetup.getZZParamPlotter(finalstate,'GGZZ',period,category)
            FAKES = hzzSetup.getZZParamPlotter(finalstate,'FAKES',period,category)

            data = TreePlotter("All_"+period+"/DATA.root","FourLeptonTreeProducer/tree","1")
            data.name='data_obs'


            floatingParams='CMS_scale_m,CMS_scale_e,CMS_res_m,CMS_res_e,melascale'
            
            #create datacard maker and link Plotters
            d = DataCardMaker(finalstate,period,category,'hzz4l_PARAM_')

            mod=model[category]


            d.addShape('ggH',ggH,'signal',mod,floatingParams)
            d.addShape('qqH',qqH,'signal',mod,floatingParams)
            d.addShape('WH',WH,'signal',mod,floatingParams)
            d.addShape('ZH',ZH,'signal',mod,floatingParams)
            d.addShape('ttH',ttH,'signal',mod,floatingParams)
            mod=model[category]+'_bkg'

            d.addShape('QQZZ',QQZZ,'background',mod,floatingParams)
            d.addShape('GGZZ',GGZZ,'background',mod,floatingParams)
            d.addShape('FAKES',FAKES,'background',mod,floatingParams)
            d.addData(data,'unbinned')

            
            d.setParamError('CMS_scale_m',1.0,0.004)
            d.setParamError('CMS_scale_e',1.0,0.004)
            d.setParamError('CMS_res_m',0.0,0.2)
            d.setParamError('CMS_res_e',0.0,0.2)
            d.setParamError('bkg',0.0,1)


            #define final state separation here
            finalStateCut=''
            if finalstate=='MuMu':
                finalStateCut='&&abs(H_Z1_leg1_PdgId)==13&&abs(H_Z2_leg1_PdgId)==13'
            if finalstate=='EleEle':
                finalStateCut='&&abs(H_Z1_leg1_PdgId)==11&&abs(H_Z2_leg1_PdgId)==11'
            if finalstate=='MuEle':
                finalStateCut='&&abs(H_Z1_leg1_PdgId)!=abs(H_Z2_leg1_PdgId)'
            
        
            #make card
#            if category !='vbf':
#                d.writeCard(variables[category],cuts[category]+finalStateCut,'1',20,100,160,10,0,1)
#            else:    
            d.writeCard(variables[category],cuts[category]+finalStateCut,'1',20,104,140,10,0,1)



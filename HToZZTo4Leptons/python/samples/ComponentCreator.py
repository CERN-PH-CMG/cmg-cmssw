import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.Production.datasetToSource import datasetToSource
from CMGTools.Production.datasetInformation import DatasetInformation

class ComponentCreator(object):
    def makeMCComponent(self,name,dataset,user,pattern):
        
         component = cfg.MCComponent(
             dataset=dataset,
             name = name,
             files = self.getFiles(dataset,user,pattern),
             xSection = 1,
             nGenEvents = 1,
             triggers = [],
             effCorrFactor = 1,
#             skimEfficiency = self.getSkimEfficiency(dataset,user)
         )

 #        print 'Skim Efficiency for ',name,'=', component.skimEfficiency
         return component


    def makeDataComponent(self,name,dataset,user,pattern,json):
         component = cfg.DataComponent(
             name = name,
             files = self.getFiles(dataset,user,pattern),
             intLumi=1,
             triggers = [],
             json=json
         )

         return component


    def getFiles(self,dataset, user, pattern):
        # print 'getting files for', dataset,user,pattern
        ds = datasetToSource( user, dataset, pattern, True )
        files = ds.fileNames
        return ['root://eoscms//eos/cms%s' % f for f in files]


    def makeMCComponentFromList(self,filename,suffix,user,pattern,triggers):
        list=[]
        f = open(filename)
        for line in f:
            sample = (line.split('%')[1]+'/'+suffix).replace('\t','').replace('\n','')
            name=sample.split('/')[1]
            component = self.makeMCComponent(name,sample,user,pattern)
            component.triggers=triggers
            list.append(component)
        f.close()    
        return list                

    def makeDataComponentFromList(self,filename,suffix,user,pattern,json,triggersMuMu,triggersEE,triggersMuE):
        list=[]
        f = open(filename)

        for line in f:
            sample = (line.split('%')[1]+'/'+suffix).replace('\t','').replace('\n','')

            sampleT = sample.split('/')
            name=sampleT[1]+'_'+sampleT[2]
            component = self.makeDataComponent(name,sample,user,pattern,json)
            
            if line.find('DoubleElectron') !=-1:
                component.triggers=triggersEE
            elif line.find('DoubleMu') !=-1:   
                component.triggers=triggersMuMu
                component.vetoTriggers=triggersEE
            elif line.find('MuEG') !=-1:   
                component.triggers=triggersMuE
                component.vetoTriggers=triggersEE+triggersMuMu
            list.append(component)
        f.close()
        return list                
                        

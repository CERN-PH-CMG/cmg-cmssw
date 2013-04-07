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

    def makePrivateMCComponent(self,name,dataset,files):
        
         component = cfg.MCComponent(
             dataset=dataset,
             name = name,
             files = ['root://eoscms//eos/cms%s/%s' % (dataset,f) for f in files],
             xSection = 1,
             nGenEvents = 1,
             triggers = [],
             effCorrFactor = 1,
#             skimEfficiency = self.getSkimEfficiency(dataset,user)
         )

 #        print 'Skim Efficiency for ',name,'=', component.skimEfficiency
         return component

    def makeDataComponent(self,name,datasets,user,pattern):
         files=[]

         for dataset in datasets:
             files=files+self.getFiles(dataset,user,pattern)
        
         component = cfgDataComponent(
             dataset=dataset,
             name = name,
             files = files,
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


    def getSkimEfficiency(self,dataset,user):
       info=DatasetInformation(dataset,user,'',False,False,'','','')
       fraction=info.dataset_details['PrimaryDatasetFraction']
       if fraction<0.001:
           print 'ERROR FRACTION IS ONLY ',fraction
       return fraction 

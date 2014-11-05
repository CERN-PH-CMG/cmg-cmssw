import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.Production.datasetToSource import datasetToSource, myDatasetToSource
from CMGTools.Production.datasetInformation import DatasetInformation
from CMGTools.Production import eostools
import re

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
         if len(files) == 0:
            raise RuntimeError, "Trying to make a component %s with no files" % name
         # prefix filenames with dataset unless they start with "/"
         dprefix = dataset +"/" if files[0][0] != "/" else ""
         component = cfg.MCComponent(
             dataset=dataset,
             name = name,
             files = ['root://eoscms.cern.ch//eos/cms%s%s' % (dprefix,f) for f in files],
             xSection = 1,
             nGenEvents = 1,
             triggers = [],
             effCorrFactor = 1,
#             skimEfficiency = self.getSkimEfficiency(dataset,user)
         )

 #        print 'Skim Efficiency for ',name,'=', component.skimEfficiency
         return component
    
    ### MM
    def makeMyPrivateMCComponent(self,name,dataset,user,pattern,dbsInstance):

        component = cfg.MCComponent(
            dataset=dataset,
            name = name,
            files = self.getMyFiles(dataset, user, pattern, dbsInstance),
            xSection = 1,
            nGenEvents = 1,
            triggers = [],
            effCorrFactor = 1,
        )

        return component
    ### MM

    def makeMCComponentFromEOS(self,name,dataset,path,pattern=".*root"):
        from CMGTools.Production.dataset import getDatasetFromCache, writeDatasetToCache
        if "%" in path: path = path % dataset;
        try:
            files = getDatasetFromCache('EOS%{path}%{pattern}.pck'.format(path = path.replace('/','_'), pattern = pattern))
        except IOError:
            files = [ 'root://eoscms.cern.ch/'+x for x in eostools.listFiles('/eos/cms'+path) if re.match(pattern,x) ] 
            if len(files) == 0:
                raise RuntimeError, "ERROR making component %s: no files found under %s matching '%s'" % (name,path,pattern)
            writeDatasetToCache('EOS%{path}%{pattern}.pck'.format(path = path.replace('/','_'), pattern = pattern), files)
        component = cfg.MCComponent(
            dataset=dataset,
            name = name,
            files = files,
            xSection = 1,
            nGenEvents = 1,
            triggers = [],
            effCorrFactor = 1,
        )
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


    def getFiles(self, dataset, user, pattern):
        # print 'getting files for', dataset,user,pattern
        ds = datasetToSource( user, dataset, pattern, True )
        files = ds.fileNames
        return ['root://eoscms.cern.ch//eos/cms%s' % f for f in files]


    def getMyFiles(self, dataset, user, pattern, dbsInstance):
        # print 'getting files for', dataset,user,pattern
        ds = myDatasetToSource( user, dataset, pattern, dbsInstance, True )
        files = ds.fileNames
        return ['root://eoscms.cern.ch//eos/cms%s' % f for f in files]

    def getSkimEfficiency(self,dataset,user):
        info=DatasetInformation(dataset,user,'',False,False,'','','')
        fraction=info.dataset_details['PrimaryDatasetFraction']
        if fraction<0.001:
            print 'ERROR FRACTION IS ONLY ',fraction
        return fraction 
        

#!/usr/bin/env python
import optparse
import os
import ROOT
from CMGTools.HToZZTo4Leptons.macros.modifiers.SuperMELAEmbedder import *
from CMGTools.HToZZTo4Leptons.macros.modifiers.MergedModifier import *

if __name__ == '__main__':

    parser = optparse.OptionParser()
    parser.add_option("-m","--mass",dest="mass",default='125',help="runonly")
    parser.add_option("-M","--maxMass",dest="maxMass",default='130',help="runonly")
    (options,args) = parser.parse_args()


    for period in ['7TeV','8TeV']:
        files=[]
        for filename in os.listdir('All_'+period):
            name,extention=filename.split('.')
            if extention =='root':
                if name.find('GGH')>-1 or name.find('VBF')>-1: 
                    for prod in ['GGH','VBF']:
                        if name.find(prod)>-1:
                            s=name.split(prod)
                            if s[1]<int(options.maxMass):
                                files.append('All_'+period+'/'+filename)
                else:            
                    files.append('All_'+period+'/'+filename)

        for file in files:
            print 'DOING FILE ',file
            F=ROOT.TFile(file,'UPDATE')
            sm=SuperMELADefaultEmbedder(float(options.mass),period,'H')
            smL=SuperMELADefaultEmbedder(float(options.mass),period,'HLoose')
            both=MergedModifier([sm,smL])
            both.loop(F)
            F.Close()

    

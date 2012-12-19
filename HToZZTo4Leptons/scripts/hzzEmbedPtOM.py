#!/usr/bin/env python
import optparse
import os
import ROOT
from CMGTools.HToZZTo4Leptons.macros.modifiers.TreeVariableEmbedder import *
from CMGTools.HToZZTo4Leptons.macros.modifiers.MergedModifier import *

if __name__ == '__main__':

    parser = optparse.OptionParser()
    (options,args) = parser.parse_args()


    for period in ['7TeV','8TeV']:
        files=[]
        for filename in os.listdir('All_'+period):
            name,extention=filename.split('.')
            if extention =='root':
                files.append('All_'+period+'/'+filename)

        for file in files:
            print 'DOING FILE ',file
            F=ROOT.TFile(file,'UPDATE')
            fisher=TreeVariableEmbedder('H_PtOM',lambda x: x.H_Pt/x.H_Mass)
            fisherLoose=TreeVariableEmbedder('HLoose_PtOM',lambda x: x.HLoose_Pt/x.HLoose_Mass)
            both=MergedModifier([fisher,fisherLoose])
            both.loop(F)
            F.Close()

    

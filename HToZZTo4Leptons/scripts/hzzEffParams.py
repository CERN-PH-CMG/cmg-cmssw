#!/usr/bin/env python
import optparse
import os
from CMGTools.HToZZTo4Leptons.macros.param.HiggsEfficiency import *
from CMGTools.HToZZTo4Leptons.macros.param.BackgroundRates import *

if __name__ == '__main__':

    parser = optparse.OptionParser()
    parser.add_option("-c","--cut",dest="cut",default='H_Mass>0',help="cut to apply")
    parser.add_option("-o","--output",dest="out",default='inc',help="suffix for file")
    parser.add_option("-m","--min",dest="minMass",default='100',help="max mass")
    parser.add_option("-M","--max",dest="maxMass",default='2000',help="max mass")


    (options,args) = parser.parse_args()

    fname = {'ggH':'ggH','qqH':'qqH','ZH':'VH','WH':'VH','ttH':'VH'}

    for prod in ['ggH','qqH','WH','ZH','ttH']:

        

        masses7TeV =[]
        masses8TeV=[]


        for filename in os.listdir('All_7TeV'):
            name,extention=filename.split('.')
            if name.find(fname[prod])>-1 and extention == 'root':
                s=name.split(fname[prod])
                if int(s[1])<int(options.maxMass) and int(s[1])>int(options.minMass):
                    masses7TeV.append(int(s[1]))

        for filename in os.listdir('All_8TeV'):
            name,extention=filename.split('.')
            if name.find(fname[prod])>-1 and extention == 'root':
                s=name.split(fname[prod])
                if int(s[1])<int(options.maxMass) and int(s[1])>int(options.minMass):
                    masses8TeV.append(int(s[1]))



        mumu7TeV = HiggsEfficiency('MuMu',options.cut,masses7TeV,prod,'7TeV',options.out)
        mumu7TeV.fit()
        mumu7TeV.save(options.out)

    
        mue7TeV = HiggsEfficiency('MuEle',options.cut,masses7TeV,prod,'7TeV',options.out)
        mue7TeV.fit()
        mue7TeV.save(options.out)

        ee7TeV = HiggsEfficiency('EleEle',options.cut,masses7TeV,prod,'7TeV',options.out)
        ee7TeV.fit()
        ee7TeV.save(options.out)

        mumu8TeV = HiggsEfficiency('MuMu',options.cut,masses8TeV,prod,'8TeV',options.out)
        mumu8TeV.fit()
        mumu8TeV.save(options.out)

    
        mue8TeV = HiggsEfficiency('MuEle',options.cut,masses8TeV,prod,'8TeV',options.out)
        mue8TeV.fit()
        mue8TeV.save(options.out)

        ee8TeV = HiggsEfficiency('EleEle',options.cut,masses8TeV,prod,'8TeV',options.out)
        ee8TeV.fit()
        ee8TeV.save(options.out)




##################################################################################
######Now backgrounds#############################################################
##################################################################################
    
    for prod in ['QQZZ','GGZZ','FAKES']:
        mumu7TeV = BackgroundRates('MuMu',options.cut,prod,'7TeV',options.out)
        mumu7TeV.save(options.out)
        mumu8TeV = BackgroundRates('MuMu',options.cut,prod,'8TeV',options.out)
        mumu8TeV.save(options.out)

        mue7TeV = BackgroundRates('MuEle',options.cut,prod,'7TeV',options.out)
        mue7TeV.save(options.out)
        mue8TeV = BackgroundRates('MuEle',options.cut,prod,'8TeV',options.out)
        mue8TeV.save(options.out)


        ee7TeV = BackgroundRates('EleEle',options.cut,prod,'7TeV',options.out)
        ee7TeV.save(options.out)
        ee8TeV = BackgroundRates('EleEle',options.cut,prod,'8TeV',options.out)
        ee8TeV.save(options.out)

    

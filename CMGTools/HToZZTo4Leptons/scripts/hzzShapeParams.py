#!/usr/bin/env python
import optparse
import os
from CMGTools.HToZZTo4Leptons.macros.param.HiggsShapes import *

if __name__ == '__main__':

    parser = optparse.OptionParser()
    parser.add_option("-c","--cut",dest="cut",default='H_Mass>0',help="cut to apply")
    parser.add_option("-p","--prod",dest="prod",default='ggH',help="sub-sample")
    parser.add_option("-m","--max",dest="maxMass",default='160',help="max mass")
    parser.add_option("-r","--runOnly",dest="runOnly",default='',help="runonly")
    (options,args) = parser.parse_args()


    masses7TeV =[]
    masses8TeV=[]


    for filename in os.listdir('All_7TeV'):
        name,extention=filename.split('.')
        if name.find(options.prod)>-1 and extention == 'root':
            s=name.split(options.prod)
            if int(s[1])<int(options.maxMass):
                masses7TeV.append(int(s[1]))

    for filename in os.listdir('All_8TeV'):
        name,extention=filename.split('.')
        if name.find(options.prod)>-1 and extention == 'root':
            s=name.split(options.prod)
            if int(s[1])<int(options.maxMass):
                masses8TeV.append(int(s[1]))

    if len(options.runOnly)>1:
        masses7TeV=[]
        masses8TeV=[]
        masses=options.runOnly.split(',')
        for m in masses:
            masses7TeV.append(int(m))
            masses8TeV.append(int(m))
            

    mumu7TeV =HiggsShapes ('MuMu',options.cut,masses7TeV,options.prod,'7TeV')
    mue7TeV = HiggsShapes('MuEle',options.cut,masses7TeV,options.prod,'7TeV')
    ee7TeV = HiggsShapes('EleEle',options.cut,masses7TeV,options.prod,'7TeV')
    mumu8TeV = HiggsShapes('MuMu',options.cut,masses8TeV,options.prod,'8TeV')
    mue8TeV = HiggsShapes('MuEle',options.cut,masses8TeV,options.prod,'8TeV')
    ee8TeV = HiggsShapes('EleEle',options.cut,masses8TeV,options.prod,'8TeV')





    

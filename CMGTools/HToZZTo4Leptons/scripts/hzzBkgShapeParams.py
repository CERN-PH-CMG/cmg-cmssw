#!/usr/bin/env python
import optparse
import os
from CMGTools.HToZZTo4Leptons.macros.param.BackgroundShapes import *

if __name__ == '__main__':

    parser = optparse.OptionParser()
    parser.add_option("-c","--cut",dest="cut",default='H_Mass>0',help="cut to apply")
    (options,args) = parser.parse_args()


            
    for prod in ['QQZZ','GGZZ','FAKES']:
#    for prod in ['FAKES']:
        mumu7TeV =BackgroundShapes ('MuMu',options.cut,prod,'7TeV')
        mue7TeV = BackgroundShapes('MuEle',options.cut,prod,'7TeV')
        ee7TeV = BackgroundShapes('EleEle',options.cut,prod,'7TeV')
        mumu8TeV = BackgroundShapes('MuMu',options.cut,prod,'8TeV')
        mue8TeV = BackgroundShapes('MuEle',options.cut,prod,'8TeV')
        ee8TeV = BackgroundShapes('EleEle',options.cut,prod,'8TeV')





    

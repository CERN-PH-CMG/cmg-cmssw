#!/usr/bin/env python
import os, sys, re, optparse,pickle,shutil



if __name__ == '__main__':
    parser = optparse.OptionParser()



    parser.add_option("-t","--trigger",dest="trigcounter",default='TriggerAnalyzer/Trigger.pck',help="trigger counter")
    parser.add_option("-s","--sigma",dest="sigma",type=float,help="cross section")
    parser.add_option("-r","--rootFile",dest="rootFile",default='FourLeptonTreeProducer_MuMuFourLeptonAnalyzer/FourLeptonTreeProducer_MuMuFourLeptonAnalyzer_tree.root',help="output root file ")

    (options,args) = parser.parse_args()
    #define output dictionary
    output=dict()

    directory=args[0]
    
    #First unpack tyhe counter and get the events
    counterFile=open(directory+'/'+options.trigcounter)
    counter=pickle.load(counterFile)
    events=counter[0][1]

    output['events'] = events
    output['weight'] = options.sigma/float(events)
    output['sigma']  = options.sigma

    f=open(directory+".pck","w")
    pickle.dump(output,f)
    f.close()

    #next copy the main tree
    shutil.copyfile(directory+'/'+options.rootFile,directory+'.root')
    

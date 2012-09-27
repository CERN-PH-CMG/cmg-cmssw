#!/usr/bin/env python
import os, sys, re, optparse,pickle,shutil



if __name__ == '__main__':
    parser = optparse.OptionParser()



    parser.add_option("-c","--counter",dest="trigcounter",default='SkimCountAnalyzer/SkimReport.pck',help="counter")
    parser.add_option("-s","--sigma",dest="sigma",type=float,help="cross section")
    parser.add_option("-f","--finalState",dest="finalState",default='All',help="final state")

    (options,args) = parser.parse_args()
    #define output dictionary
    output=dict()

    rootFile='FourLeptonTreeProducer/FourLeptonTreeProducer_tree.root'

    directory=args[0]
    
    #First unpack tyhe counter and get the events
    if os.path.exists(directory+'/'+options.trigcounter):
        counterFile=open(directory+'/'+options.trigcounter)
        counter=pickle.load(counterFile)
        events=counter[0][1]
    else:
        print 'problem in file/ cannot count events in ',directory
        print 'If is data you can ignore'
        events=-1
        

    output['events'] = events
    output['weight'] = options.sigma/float(events)
    output['sigma']  = options.sigma

    f=open(directory+".pck","w")
    pickle.dump(output,f)
    f.close()

    #next copy the main tree
    shutil.copyfile(directory+'/'+rootFile,directory+'.root')
    

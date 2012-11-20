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
        print counter
        events=counter[0][1]
        events4e=counter[1][1]
        events4mu=counter[2][1]
        events2e2mu=counter[3][1]
        eventsZH=counter[4][1]
        eventsWH=counter[5][1]
        eventsttH=counter[6][1]
    else:
        print 'problem in file/ cannot count events in ',directory
        print 'If is data you can ignore'
        events=-1
        events4e=-1
        events4mu=-1
        events2e2mu=-1
        eventsZH=-1
        eventsWH=-1
        eventsttH=-1
        

    output['events'] = events
    output['4e'] = events4e
    output['4mu'] = events4mu
    output['2e2mu'] = events2e2mu
    output['ZH'] = eventsZH
    output['WH'] = eventsWH  
    output['ttH'] = eventsttH
  
    output['weight'] = options.sigma/float(events)
    output['sigma']  = options.sigma

    f=open(directory+".pck","w")
    pickle.dump(output,f)
    f.close()

    #next copy the main tree
    shutil.copyfile(directory+'/'+rootFile,directory+'.root')
    

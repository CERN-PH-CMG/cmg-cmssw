#!/usr/bin/env python
import os, sys, re, optparse,pickle,shutil



if __name__ == '__main__':
    parser = optparse.OptionParser()

    parser.add_option("-c","--counter",dest="trigcounter",default='skimAnalyzerCount/SkimReport.pck',help="counter")
    parser.add_option("-s","--sigma",dest="sigma",type=float,help="cross section",default=1.0)

    (options,args) = parser.parse_args()
    #define output dictionary
    output=dict()
    rootFile='vvTreeProducer/tree.root'


    for directory in os.listdir(args[0]):
        if directory.find("Chunk")!=-1:
            continue
    
        #First unpack tyhe counter and get the events
        if os.path.exists(directory+'/'+options.trigcounter):
            counterFile=open(directory+'/'+options.trigcounter)
            counter=pickle.load(counterFile)
            if len(counter)>1 and counter[1][0]=='Sum Weights':
                events=counter[1][1]
            else:    
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
    

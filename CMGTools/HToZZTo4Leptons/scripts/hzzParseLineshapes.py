#!/usr/bin/env python
import os
import json
if __name__ == '__main__':

    data=dict()

    for filename in os.listdir('.'):
        #read the mass
        if filename.find('.txt')>-1 and filename.find('Higgs')>-1:
            print filename
            mass=int(filename.split('_')[1].split('Higgs')[1])
            period = filename.split('_')[2]
            data[period]=dict()
            data[period][mass]=dict()
            print 'Reading mass ',mass
            f=open(filename)
            for line in f:
                cols=line.split()
                print cols
                data[period][mass][float(cols[1])]={'final':float(cols[5]),'finalUp':float(cols[6]),'finalDown':float(cols[7]) }
            f.close()
    print data
                 

    f=open('lineshapeWeights.json','w')
    json.dump(data,f)
    f.close()

                 
                 
            
        


    

#!/usr/bin/env python

import os,sys
import json
import getopt
import glob

#print usage
def usage() :
    print ' '
    print 'generateJSONplotterFromList.py [options]'
    print '  -i : input file with the plot names'
    print '  -o : output file (plotter.json by default'
    print ' '
                                        
#parse the options 
try:
     # retrive command line options
     shortopts  = "i:o:h?"
     opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
     # print help information and exit:
     print "ERROR: unknown options in argument %s" % sys.argv[1:]
     usage()
     sys.exit(1)


input=''
output='plotter.json'
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(1)
    elif o in('-i'): input = a 
    elif o in('-o'): output=a

if(len(input)==0) :
    usage()
    sys.exit(1)

#generate the json for the plot browser
categories=[]
subcategories=[]
finalplots=[]

os.system('rm -rf ' + output)
inFile = open(input, 'r')

searchFor=['ee','mumu','emu','gamma','photon',
           'eq0jets', 'geq1jets', 'eq1jets','geq2jets','eq2jets','geq3jets','eq3jets','vbf',
           'eq0btags','geq1btags', 'eq1btags', 'geq2btags','eq2btags']
for p in inFile.readlines():
   pTokens=p.split('/')
   pBase=pTokens[len(pTokens)-1].rstrip()
   if(len(pBase)==0): continue

   pBaseTokens=pBase.split('_')

   cat=''
   subcat=''
   if(len(pBaseTokens)>1) :
       prefix=pBaseTokens[0]
       for i in searchFor :
           if(prefix.find(i)==0 and len(cat)==0)   : cat=i
           elif(prefix.find(i) and len(subcat)>0) : subcat=i
       if(len(cat)==0) : cat = prefix

   categories.append(cat)
   subcategories.append(subcat)
   if(len(cat)>0) : finalplots.append( pBase.replace(cat+subcat+'_','') )
   else           : finalplots.append( pBase )
      
#remove duplicates
categories=list(set(categories))
subcategories=list(set(subcategories))
finalplots=list(set(finalplots))

#write in json format
fileObj = open(output,'w')
fileObj.write(json.dumps( { 'categories':categories,
                            'subcategories':subcategories,
                            'plots':finalplots,
                            'ploturl':''},
#                          sort_keys=True,
                          indent=4 ) )
fileObj.close()

    








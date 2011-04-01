#!/usr/bin/env python

import os,sys
import json

import ROOT
ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libCMGToolsHtoZZ2l2nu.so')
from ROOT import formatPlot

"""
Converts ROOT constant to int
"""
def rootConst(k):
    if( isinstance(k, int) ) :  return int(k)
    kattr=k
    kmodif=0

    # first split if a modifier is present
    if '-' in k :
        kattr = k.split('-')[0]
        kmodif = -int(k.split('-')[1])
    elif '+' in k:
        kattr = k.split('+')[0]
        kmodif = int(k.split('+')[1])

    # convert the const and add the modifier
    if( kattr.startswith("k") ) :
        kmodif = ROOT.__getattr__(str(kattr)) + kmodif
    else:
        kmodif = int(kattr)
            
    #return the result
    return int(kmodif)


"""
Gets the value of a given item
(if not available a default value is returned)
"""
def getByLabel(desc,key,defaultVal=None) :
    try :
        return desc[key]
    except KeyError:
        return defaultVal

"""
Parses a json file and retrieves the necessary info
"""
def runOverSamples(samplesDB,func,debugFunc=None,integratedLumi=1.0) :

    #open the file which describes the sample
    jsonFile = open(samplesDB,'r')
    procList=json.load(jsonFile,encoding='utf-8').items()
    stackplots=[]
    spimposeplots=[]
    dataplots=[]

    generalLabel='CMS preliminary,#sqrt{s}=7 TeV, #int L=' +str(integratedLumi)+' pb^{-1}'

    from CMGTools.HtoZZ2l2nu.localPatTuples_cff import fillFromCastor

    #run over sample
    for proc in procList :


        #run over processes
        for desc in proc[1] :

            procplots=[]
            isdata = getByLabel(desc,'isdata',False)
            spimpose = getByLabel(desc,'spimpose',False)
            color = rootConst(getByLabel(desc,'color',1))
            line = getByLabel(desc,'line', 1)
            lwidth = getByLabel(desc,'lwidth', 1)
            fill  = getByLabel(desc,'fill',1001)
            marker = getByLabel(desc,'marker',20)
            lcolor = rootConst(getByLabel(desc,'lcolor',color))
            fcolor = rootConst(getByLabel(desc,'fcolor',color))
            mcolor = rootConst(getByLabel(desc,'fcolor',color))
            tag = getByLabel(desc,'tag','')
            
            #run over items in process
            data = getByLabel(desc,'data')
            for d in data :

                #get the plots
                plots=func(d,isdata)
                dtag=getByLabel(d,'dtag')
                if(plots==None):continue
                if(len(plots)==0): continue

                #compute the normalization
                weight=1
                absNorm=False
                if( not isdata ) :
                    sfactor = getByLabel(d,'sfactor',1)
                    xsec = getByLabel(d,'xsec',-1)
                    br = getByLabel(d,'br',1)
                    normto = getByLabel(d,'normto',-1)
                    if(xsec>0) :
                        weight = integratedLumi*sfactor*xsec*br
                    elif(normto>0) :
                        weight = sfactor*normto
                        absNorm=True

                #book keep the result
                iplot=0
                for p in plots.items():
                    
                    #create the base plot for this sample if non existing
                    if(len(procplots)<=iplot):
                        newname=p[1].GetName()+'_'+str(len(stackplots))
                        newplot=p[1].Clone( newname )
                        newplot.Reset('ICE')
                        newplot.SetTitle(tag)
                        formatPlot(newplot, color,line,lwidth,marker,fill,True,True,lcolor,fcolor,mcolor)
                        procplots.append( newplot )

                    #apply new normalization
                    if(absNorm) :
                        total=newplot.Integral()
                        if(total>0): p[1].Scale(weight/newplot.Integral())
                    else : p[1].Scale(weight)

                    #add to base plot
                    print ' Normalizing plots from ' + dtag + ' with abs=' + str(absNorm) + ' and weight=' + str(weight) 
                    procplots[iplot].Add(p[1])
                    iplot=iplot+1

            #store the final result
            if(len(procplots)==0): continue
            if(not isdata) :
                if(not spimpose) : stackplots.append(procplots)
                else : spimposeplots.append(procplots)
            else : dataplots.append(procplots)

    if(debugFunc != None) : debugFunc(stackplots,spimposeplots,dataplots,generalLabel)


# steer script
if(len(sys.argv)<2):
    print 'runOverSamples.py analysisScript.py samples.json integratedLumi=1'
    exit()

#import the module
script = sys.argv[1]
scriptdir =  os.path.dirname(script)
modname = os.path.splitext(os.path.basename(script))[0]
import pkgutil
i = pkgutil.ImpImporter(scriptdir)
l = i.find_module(modname)

mysource = l.load_module(modname)
if (mysource is None):
    print 'Unable to find ' + modname + ' in ' +scriptdir
    exit();


#configure 
samplesDB = sys.argv[2]
integratedLumi=1.0
if(len(sys.argv)>3): integratedLumi = float(sys.argv[3])

#run the script
print ' Integrated lumi is:' + str(integratedLumi) + ' /pb'
print ' Running the following module: ' + modname
print ' Samples defined in: ' +  samplesDB
runOverSamples( samplesDB, mysource.getControlPlots, mysource.showControlPlots, integratedLumi )

#!/usr/bin/env python

import os,sys
import json
from rounding import PDGRoundSym

import ROOT
ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libCMGToolsHtoZZ2l2nu.so')
from ROOT import formatPlot, setStyle, showPlots, formatForCmsPublic, getNewCanvas, showMCtoDataComparison, getPlotAsTable

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
loops over files in directory and fills control histograms
"""
def getControlPlots(descriptor,isData,inputDir='data') :

    results={}
    
    tag=getByLabel(descriptor,'dtag')
    if(len(tag)==0) : return results
    noNorm=getByLabel(descriptor,'nonorm',False)
    
    #open the file
    url=inputDir+'/'+tag+'.root'
    file = ROOT.TFile(url)

    if(file is None): return results
    if(file.IsZombie()) : return results
    dir = file.GetDirectory('evAnalyzer/'+tag)
    if( dir == 0 ) :
        cresults={}
        plots = file.GetListOfKeys();
        for p in plots:
            pname=p.GetName()
            h = file.Get(pname)
            h.SetDirectory(0)
            cresults[h.GetName()]=h
        results.update(cresults)
    else :
        #get plots from file
        categs = dir.GetListOfKeys();
        cnorm=1
        centralresults={}
        for c in categs:
            cname=c.GetName()
            path='evAnalyzer/'+tag+'/'+cname
            subdir = file.GetDirectory(path)
            try :
                subdir.ClassName()
                cresults={}
                plots = subdir.GetListOfKeys()
                for p in plots:
                    hname=path+'/'+p.GetName()
                    h = file.Get(hname)
                    h.SetDirectory(0)
                    cresults[h.GetName()]=h
                    if(hname.find('cutflow')<0):continue
                    cnorm=h.GetBinContent(1)
                    
                #rescale if not data
                if( not isData and cnorm!=0 and not noNorm) :
                    for p in cresults.items():
                        p[1].Scale(1./float(cnorm))
                results.update(cresults)
            except :
                h = file.Get(path)
                h.SetDirectory(0)
                centralresults[h.GetName()]=h

        #rescale central results (assume latest normalization    
        if( not isData and cnorm!=0 and not noNorm) :
            for p in centralresults.items():
                p[1].Scale(1./float(cnorm))
        results.update(centralresults)
        
    file.Close()

    return results

"""
dumps the plots to a table 
"""
def savePlotAsTable(stackplots=None,spimposeplots=None,dataplots=None,outUrl='table.tex') :
    tabtex=getPlotAsTable(stackplots,spimposeplots,dataplots)
    fileObj = open(outUrl,"w")
    fileObj.write(tabtex.Data())
    fileObj.close()


"""
shows the control plots
"""
def showControlPlots(stackplots=None,spimposeplots=None,dataplots=None,generalLabel='CMS preliminary',outputDir='data/plots', samplehtml='') :

    if(len(stackplots)==0 and len(spimposeplots)==0 and len(dataplots)==0) : return

    #convert to lists
    stackLists=[]
    for p in stackplots:
        for i in xrange(0,len(p)):
            if(len(stackLists)<=i):
                newlist=ROOT.TList()
                stackLists.append(newlist)
            if( p[i].IsA().InheritsFrom("TH1") ):
                if(p[i].GetEntries()==0) : continue
            stackLists[i].Add(p[i])
            
    spimposeLists=[]
    for p in spimposeplots:
        for i in xrange(0,len(p)):
            if(len(spimposeLists)<=i):
                newlist=ROOT.TList()
                spimposeLists.append(newlist)
            if( p[i].IsA().InheritsFrom("TH1") ):
                if(p[i].GetEntries()==0) : continue
            spimposeLists[i].Add(p[i])

    dataLists=[]
    for p in dataplots:
        for i in xrange(0,len(p)):
            if(len(dataLists)<=i):
                newlist=ROOT.TList()
                dataLists.append(newlist)
            if( p[i].IsA().InheritsFrom("TH1") ):
                print p[i].GetEntries()
                if(p[i].GetEntries()==0) : continue
            dataLists[i].Add(p[i])

    #draw result
    nplots=max([len(stackLists), len(dataLists), len(spimposeLists)])
    if(nplots==0) : return

    #output dir
    import getpass
    os.system('mkdir -p ' + outputDir)

    plotsToDisplay={}
    plotsToDisplay['ee events']=[]
    plotsToDisplay['#mu#mu events']=[]
    plotsToDisplay['e#mu events']=[]
    plotsToDisplay['All events']=[]
    setStyle()
    c = getNewCanvas("recolevelc","recolevelc",False)
    c.SetWindowSize(600,800)
    for iplot in xrange(0,nplots):
        stack=ROOT.TList()
        if(len(stackLists)>iplot): stack=stackLists[iplot]
        spimpose=ROOT.TList()
        if(len(spimposeLists)>iplot): spimpose=spimposeLists[iplot]
        data=ROOT.TList()
        if(len(dataLists)>iplot): data=dataLists[iplot]
        pname=''
        if(stack.At(0) != None) :      pname=stack.At(0).GetName()
        elif(spimpose.At(0) != None) : pname=spimpose.At(0).GetName()
        elif(data.At(0) != None) :     pname=data.At(0).GetName()
        if(len(pname)<=0): continue

        chtag='All events'
        if(pname.find('mumu')==0): chtag='#mu#mu events'
        if(pname.find('emu')==0):  chtag='e#mu events'
        if(pname.find('ee')==0):  chtag='ee events'
        plotsToDisplay[chtag].append(pname)
        
        if(pname.find('eq0jets')>0):  chtag += ' (= 0 jets)'
        if(pname.find('eq1jets')>0):  chtag += ' (= 1 jets)'
        if(pname.find('geq2jets')>0): chtag += ' (#geq 2 jets)'

        plotLabel = generalLabel + '\\' + chtag

        c.Clear()
        c.Divide(1,2)
        pad=c.cd(1)
        pad.SetPad(0,0.3,1.0,1.0);
        leg=showPlots(pad,stack,spimpose,data)
        formatForCmsPublic(pad,leg,plotLabel,5)
        pad.SetLogy()
        
        pad=c.cd(2)
        pad.SetPad(0,0.0,1.0,0.25);
        pad.SetTopMargin(0);
        pad.SetBottomMargin(0.3);
        yscale = (1.0-0.3)/(0.25-0.0);
        leg=showMCtoDataComparison(pad,stack,data,False,yscale)
                                                 
        c.SaveAs(outputDir+'/'+pname+'.png')
        c.SaveAs(outputDir+'/'+pname+'.C')

        if(pname.find('cutflow')<0):continue
        savePlotAsTable(stack,spimpose,data,outputDir+'/'+pname+'.tex')
        
        #raw_input('Any key to continue...')

    #create navigator files
    HTMLSTART="<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"tableFormat.css\"></link></head><body>"
    HTMLEND="</body></html>"
    titlehtml=HTMLSTART
    titlehtml+="<table class=\"sample\"><tr><th colspan=\"3\">Event selection categories</th></tr>" 
    itag=0
    for tag in plotsToDisplay.items() :
        thtml=HTMLSTART
        thtml+="<table class=\"sample\"><tr><th colspan=\"2\">" + tag[0] + " channel </th></tr>"
        for pname in tag[1] :
            thtml+="<tr>"
            thtml+="<td><img src=\"" + pname + ".png\" width=\"500\"></img></td>"
            thtml+="<td><small>Available in: <a href=\"" + pname + ".png\">.png</a> &nbsp;&nbsp; <a href=\"" + pname + ".C\">.C</a>"
            if(pname.find('cutflow')>=0):
                thtml+=" &nbsp;&nbsp; <a href=\"" + pname + ".tex\">.tex</a>"
            thtml += "</small></td></tr>"
        thtml+="</table>"
        thtml+=HTMLEND

        titlehtml += "<tr><td><a href=\"cat_" + str(itag) + ".html\">" + tag[0] + "</a></td></tr>" 
        fileObj = open(outputDir+"/cat_" + str(itag) + ".html","w")
        fileObj.write(thtml)
        fileObj.close()
        itag=itag+1

    titlehtml+=samplehtml        
    titlehtml+="</table>"
    titlehtml+=HTMLEND
    fileObj = open(outputDir+"/index.html","w")
    fileObj.write(titlehtml)
    fileObj.close()



"""
Parses a json file and retrieves the necessary info
"""
def runOverSamples(samplesDB, integratedLumi=1.0, inputDir='data', outputDir='data/plots') :

    #open the file which describes the sample
    jsonFile = open(samplesDB,'r')
    procList=json.load(jsonFile,encoding='utf-8').items()
    stackplots=[]
    spimposeplots=[]
    dataplots=[]

    generalLabel='CMS preliminary,#sqrt{s}=7 TeV, #int L=' +str(integratedLumi)+' pb^{-1}'

    samplehtml="<tr><th colspan=\"3\"><large>Samples used in analysis</large></th></tr>"
    
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

            samplehtml+="<tr><th colspan=\"3\">"+tag+" ("
            if(isdata) : samplehtml += "data"
            else : samplehtml +="mc"
            samplehtml+=") </th></tr>\n"
            
            #run over items in process
            data = getByLabel(desc,'data')
            for d in data :

                #get the plots
                plots=getControlPlots(d,isdata,inputDir)
                dtag=getByLabel(d,'dtag')
                if(plots==None):continue
                if(len(plots)==0): continue

                samplehtml+="<tr><td>"+dtag+"</td>"
                samplehtml+="<td>"+str(getByLabel(d,"xsec",1)) + "x" + str(getByLabel(d,"br",1)) + str(getByLabel(d,"sfactor",1)) + "</td>"
                samplehtml+="<td><small>" + getByLabel(d,'dset','n/a') + "</small></td></tr>\n" 

                #compute the normalization
                weight=1
                absNorm=False
                sfactor = getByLabel(d,'sfactor',1)
                xsec = getByLabel(d,'xsec',-1)
                br = getByLabel(d,'br',1)
                normto = getByLabel(d,'normto',-1)
                if(xsec>0 and not isdata) :
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
                        total=p[1].Integral()
                        if(total>0): p[1].Scale(weight/total)
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

    showControlPlots(stackplots,spimposeplots,dataplots,generalLabel,outputDir,samplehtml)


# steer script
if(len(sys.argv)<2):
    print 'runPlotterOverSamples.py samples.json integratedLumi=1 inputDir=data outputDir=data/plots'
    exit()

#import the module
#script = sys.argv[1]
#scriptdir =  os.path.dirname(script)
#modname = os.path.splitext(os.path.basename(script))[0]
#import pkgutil
#i = pkgutil.ImpImporter(scriptdir)
#l = i.find_module(modname)
#mysource = l.load_module(modname)
#if (mysource is None):
#    print 'Unable to find ' + modname + ' in ' +scriptdir
#    exit();


#configure 
samplesDB = sys.argv[1]
integratedLumi=1.0
if(len(sys.argv)>2): integratedLumi = float(sys.argv[2])
inputDir='data'
if(len(sys.argv)>3): inputDir = sys.argv[3]
outputDir='data/plots'
if(len(sys.argv)>4): outputDir = sys.argv[4]

#run the script
print ' Integrated lumi is:' + str(integratedLumi) + ' /pb'
print ' Samples defined in: ' +  samplesDB
print ' Plots in: ' + inputDir
runOverSamples( samplesDB, integratedLumi,inputDir, outputDir )
print ' Output stored in: ' + outputDir
print ' Can browse the results using: ' + outputDir + '/index.html'

import sys,os
import commands
import ROOT
ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libCMGToolsHtoZZ2l2nu.so')
from ROOT import setStyle, getNewCanvas, formatPlot, formatForCmsPublic, showPlots


"""
loops over files in directory and fills control histograms
"""
def getControlPlots(url) :

    cea = CleanEventAnalysis()

    #build the list of files
    import os
    dirList=[url]
    if(url.find('castor')>0):
        sc, flist = commands.getstatusoutput('nsls %s' % (url,) )
        flist = flist.split()
        for f in flist:
            dirList.append(f)
    else :
        dirList=os.listdir(url)

    #loop over results in files
    for fname in dirList:
        if(fname.find("root")<0) : continue
        absUrl='file:'+ url + '/' + fname
        if(url.find('castor')>0) : absUrl = 'rfio://' + url + '/' + fname
        file = ROOT.TFile.Open(absUrl)
        if(file==None) :
            continue
        if(file.IsZombie()):
            file.Close()
            continue
        cea.addToAnalysis(file)

    resultslist=cea.finalizePlots(True)
    results={}
    for r in resultslist : results[r.GetName()]=r
    return results

"""
shows the control plots
"""
def showControlPlots(stackplots=None,spimposeplots=None,dataplots=None,generalLabel='CMS preliminary',outdir='plots') :

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
    outdir='/tmp/'+getpass.getuser()+'/'

    plotsToDisplay={}
    plotsToDisplay['ee events']=[]
    plotsToDisplay['#mu#mu events']=[]
    plotsToDisplay['e#mu events']=[]
    setStyle()
    c = getNewCanvas("recolevelc","recolevelc",False)
    c.SetWindowSize(600,600)
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

        chtag='ee events'
        if(pname.find('mumu')==0): chtag='#mu#mu events'
        if(pname.find('emu')==0):  chtag='e#mu events'
        plotsToDisplay[chtag].append(pname)
        
        if(pname.find('eq0jets')>0):  chtag += '(= 0 jets)'
        if(pname.find('eq1jets')>0):  chtag +=' (= 1 jets)'
        if(pname.find('geq2jets')>0): chtag +=' (#geq 2 jets)'

        plotLabel = generalLabel + '\\' + chtag
        leg=showPlots(c,stack,spimpose,data)
        formatForCmsPublic(c,leg,plotLabel,5)
        
        c.SaveAs(outdir+'/'+pname+'.png')
        c.SaveAs(outdir+'/'+pname+'.C')
        #raw_input('Any key to continue...')

    #create navigator files
    HTMLSTART="<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"tableFormat.css\"></link></head><body>"
    HTMLEND="</body></html>"
    titlehtml=HTMLSTART
    titlehtml+="<table class=\"sample\"><tr><th>Event selection categories</th></tr>" 
    itag=0
    for tag in plotsToDisplay.items() :
        thtml=HTMLSTART
        thtml+="<table class=\"sample\"><tr><th>" + tag[0] + " channel </th></tr>"
        for pname in tag[1] :
            thtml+="<tr><td><img src=\"" + pname + ".png\" width=\"500\"></img></td></tr>"
            thtml+="<tr><td><a href=\"" + pname + ".png\">.png</img> <a href=\"" + pname + ".C\">.C</img></td></tr>"
        thtml+="</table>"
        thtml+=HTMLEND

        titlehtml += "<tr><td><a href=\"cat_" + str(itag) + ".html\">" + tag[0] + "</a></td></tr>" 
        fileObj = open(outdir+"/cat_" + str(itag) + ".html","w")
        fileObj.write(thtml)
        fileObj.close()
        itag=itag+1
        
    titlehtml+="</table>"
    titlehtml+=HTMLEND
    fileObj = open(outdir+"/index.html","w")
    fileObj.write(titlehtml)
    fileObj.close()
                    
    
    

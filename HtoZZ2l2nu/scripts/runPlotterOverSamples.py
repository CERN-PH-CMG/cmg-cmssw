#!/usr/bin/env python

import os,sys
import json
from rounding import toLatexRounded
import getopt
import glob
    
import ROOT
ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libCMGToolsHtoZZ2l2nu.so')
from ROOT import formatPlot, setStyle, showPlots, formatForCmsPublic, getNewCanvas, showMCtoDataComparison, getPlotAsTable, showPlotsAndMCtoDataComparison

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
def getControlPlots(descriptor,isData,inputDir='data',getFromDir='') :

    results={}
    
    tag=getByLabel(descriptor,'dtag')
    if(len(tag)==0) : return results
    noNorm=getByLabel(descriptor,'nonorm',False)
    
    #open the file
    urls=glob.glob(inputDir+'/'+tag+'*.root')
    if(len(urls)==0) : return results

    #fix me, in order to have more files
    url=urls[0]
    file = ROOT.TFile(url)

    if(file is None): return results
    if(file.IsZombie()) : return results
    baseDirName='evAnalyzer/'
    if(len(getFromDir)>0): baseDirName=getFromDir
    dir = file.GetDirectory(baseDirName)
    if( dir == 0 ) :
        cresults={}
        plots = file.GetListOfKeys();
        for p in plots:
            pname=p.GetName()
            h = file.Get(pname)
            h.SetDirectory(0)
            h.Sumw2()
            cresults[h.GetName()]=h
        results.update(cresults)
    else :
        #get plots from file
        categs = dir.GetListOfKeys();
        cnorm=1
        centralresults={}
        for c in categs:
            cname=c.GetName()
            path=baseDirName+'/'+cname
            subdir = file.GetDirectory(path)
            try :
                subdir.ClassName()
                cresults={}
                plots = subdir.GetListOfKeys()
                for p in plots:
                    hname=path+'/'+p.GetName()
                    h = file.Get(hname)
                    h.SetDirectory(0)
                    h.Sumw2()
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
                h.Sumw2()
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
    #tabtex=getPlotAsTable(stackplots,spimposeplots,dataplots)
    
    href=None
    if(stackplots is not None) : href=stackplots.At(0)
    if(href is None  and spimposeplots is not None) : href=spimposeplots.At(0)
    if(href is None  and dataplots is not None) : href=dataplots.At(0)
    if(href is None) : return

    colfmt='l'
    colnames=''
    for ibin in xrange(1,href.GetXaxis().GetNbins()+1) :
        colfmt += 'c'
        colnames += ' & ' + href.GetXaxis().GetBinLabel(ibin)
    
    tabtex =  '\\begin{table}[htp]\n'
    tabtex += '\\begin{center}\n'
    tabtex += '\\caption{}\n'
    tabtex += '\\label{tab:table}\n'
    tabtex += '\\begin{tabular}{'+colfmt+'} \\hline\n'
    tabtex += 'Process ' + colnames + '\\\\ \\hline\\hline\n'    

    #create the sum
    if(stackplots.At(0) is not None) :
        stackSum = stackplots.At(0).Clone("totalstack")
        stackSum.SetTitle('Total expected')
        stackSum.Reset('ICE')
        for p in stackplots: stackSum.Add(p)
        stackplots.Add(stackSum)

    alllists = [stackplots, spimposeplots,dataplots ]
    for ll in alllists :
        for p in ll :
            tabtex += p.GetTitle() 
            for ibin in xrange(1,p.GetXaxis().GetNbins()+1) :
                val = p.GetBinContent(ibin)
                valerr= p.GetBinError(ibin)
                try :
                    tabtex += ' & '
                    fmtValue = toLatexRounded(val,valerr)
                    fmtValue = fmtValue.replace("[","(")
                    fmtValue = fmtValue.replace("]",")")
                    tabtex += fmtValue
                except :
                    tabtex += ' & ' 
            tabtex += '\\\\\n'
        tabtex += '\\hline\n'
    
    tabtex += '\\end{tabular}\n'
    tabtex += '\\end{center}\n'
    tabtex += '\\end{table}\n'
    
    fileObj = open(outUrl,"w")
    #fileObj.write(tabtex.Data())
    fileObj.write(tabtex)
    fileObj.close()


"""
shows the control plots
"""
def showControlPlots(stackplots=None,spimposeplots=None,dataplots=None,plottitles=None,generalLabel='CMS preliminary',outputDir='data/plots', samplehtml='') :

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
                #print p[i].GetEntries()
                if(p[i].GetEntries()==0) : continue
            dataLists[i].Add(p[i])

    #draw result
    nplots=max([len(stackLists), len(dataLists), len(spimposeLists)])
    if(nplots==0) : return

    #output dir
    import getpass
    os.system('mkdir -p ' + outputDir)

    plotsToDisplay={}
    setStyle()
    c = getNewCanvas("recolevelc","recolevelc",False)
    c.SetWindowSize(800,800)
    c.SetCanvasSize(800,800)
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
        tagname = pname.split('_')[0]
        #print tagname
        if(tagname.find('all')==0) :   chtag='All events'
        elif(tagname.find('mumu')==0): chtag='#mu#mu events'
        elif(tagname.find('emu')==0):  chtag='e#mu events'
        elif(tagname.find('ee')==0):   chtag='ee events'
        elif(tagname.find('etau')==0):   chtag='e#tau events'
        elif(tagname.find('mutau')==0):   chtag='#mu#tau events'
        else :                         chtag=tagname

        #add the plot
        try :
            plotsToDisplay[chtag].append(pname)
        except :
            plotsToDisplay[chtag]=[]
            plotsToDisplay[chtag].append(pname)
            
        plotLabel = generalLabel + '\\' #+ chtag
        if(pname.find('eq0jets')>0): plotLabel += '=0 jets' 
        elif(pname.find('eq1jets')>0): plotLabel += '=1 jet'
        elif(pname.find('geq2jets')>0): plotLabel += '#geq 2 jets'
        elif(pname.find('eq0btags')>0): plotLabel += '=0 b-tags'
        elif(pname.find('eq1btags')>0): plotLabel += '=1 b-tags'
        elif(pname.find('geq2btags')>0): plotLabel += '#geq 2 b-tags'
        else : plotLabel += 'All events'
        
        c.Clear()
        leg=showPlotsAndMCtoDataComparison(c,stack,spimpose,data)
        #leg=showPlots(c,stack,spimpose,data)
        #        leg.SetHeader(plottitles[iplot])
        formatForCmsPublic(c.cd(1),leg,plotLabel,5)
        
#        c.Clear()
#        c.Divide(1,2)
#        pad=c.cd(1)
#        pad.SetPad(0,0.3,1.0,1.0);
#        leg=showPlots(pad,stack,spimpose,data)
#        formatForCmsPublic(pad,leg,plotLabel,5)
#        pad.SetLogy()
        
#        pad=c.cd(2)
#        pad.SetPad(0,0.0,1.0,0.25);
#        pad.SetTopMargin(0);
#        pad.SetBottomMargin(0.3);
#        yscale = (1.0-0.3)/(0.25-0.0);
#        leg=showMCtoDataComparison(pad,stack,data,False,yscale)
                                                 
        c.SaveAs(outputDir+'/'+pname+'.png')
        c.SaveAs(outputDir+'/'+pname+'.C')

        if(pname.find('cutflow')>=0 or pname.find('eventflow')>=0 or pname.find('evtflow')>=0) :
            try :
                savePlotAsTable(stack,spimpose,data,outputDir+'/'+pname+'.tex')
            except :
                continue
        #raw_input('Any key to continue...')
        #c.Delete()

    #create navigator files
    HTMLSTART="<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"tableFormat.css\"></link></head><body>"
    HTMLEND="</body></html>"
    titlehtml=HTMLSTART
    titlehtml+="<table class=\"sample\"><tr><th colspan=\"3\">Event selection categories</th></tr>" 

    #check if subcategories are present
    varitems=[]
    for tag in plotsToDisplay.items() :
        for pname in tag[1] :
            if(pname.find('eq0jets')>=0) : subcat='eq0jets'
            elif(pname.find('eq1jets')>=0) : subcat='eq1jets'
            elif(pname.find('geq2jets')>=0) : subcat='geq2jets'
            elif(pname.find('eq0btags')>=0) : subcat='eq0btags'
            elif(pname.find('eq1btags')>=0) : subcat='eq1btags'
            elif(pname.find('geq2btags')>=0) : subcat='geq2btags'
            else : continue
            if(subcat in varitems) :continue
            varitems.append(subcat)

    varitems=sorted(set(varitems))
    vars={}
    vars['']=True
    for v in varitems :  vars[v]=True
    #print vars

    #create the table
    itag=0
    for tag in plotsToDisplay.items() :
       
        if( tag[0]=='All events')  : tagch='all'
        elif( tag[0]=='ee events') : tagch='ee'
        elif( tag[0]=='#mu#mu events') : tagch='mumu'
        elif( tag[0]=='e#mu events') : tagch='emu'
        elif( tag[0]=='e#tau events') : tagch='etau'
        elif( tag[0]=='#mu#tau events') : tagch='mutau'
        else : tagch=tag[0]

        thtml=HTMLSTART
        thtml+="<table class=\"sample\" border=\"1\" cellspacing=\"3\"><tr><th colspan=\""+str(len(vars))+"\">" + tag[0] + " channel </th></tr>"
        
        for pname in tag[1] :

            if(pname.find('eq0jets')>=0 or pname.find('eq1jets')>=0 or pname.find('geq2jets')>=0
               or pname.find('eq0btags')>=0 or pname.find('eq1btags')>=0 or pname.find('geq2btags')>=0) : continue
        
            thtml+="<tr>"
            for v in vars.items() :
                theplotName = pname.replace(tagch,tagch+v[0])
                thtml+="<td><img src=\"" + theplotName + ".png\"></img></td>"
            thtml+="</tr>"
            
            thtml+="<tr>"
            for v in vars.items() :
                theplotName = pname.replace(tagch,tagch+v[0])
                thtml+="<td><small>Available in: <a href=\"" + theplotName + ".png\">.png</a> &nbsp;&nbsp; <a href=\"" + theplotName + ".C\">.C</a>"
                if(pname.find('cutflow')>=0):
                    thtml+=" &nbsp;&nbsp; <a href=\"" + theplotName + ".tex\">.tex</a>"
                thtml += "</small></td>"
            thtml+="</tr>"
            
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
def runOverSamples(samplesDB, integratedLumi=1.0, inputDir='data', outputDir='data/plots', getFromDir='') :

    #open the file which describes the sample
    jsonFile = open(samplesDB,'r')
    procList=json.load(jsonFile,encoding='utf-8').items()
    stackplots=[]
    spimposeplots=[]
    dataplots=[]
    plottitles=[]
    generalLabel='CMS preliminary,#sqrt{s}=7 TeV, #int L=' +str(integratedLumi)+' pb^{-1}'

    samplehtml="<tr><th colspan=\"3\"><large>Samples used in analysis</large></th></tr>"
    
    #run over sample
    for proc in procList :

        #run over processes
        for desc in proc[1] :

            procplots=[]
            procplotstitles=[]
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
            normto = getByLabel(desc,'normto',-1)
            for d in data :

                #get the plots
                plots=getControlPlots(d,isdata,inputDir,getFromDir)
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
                br = getByLabel(d,'br',[])
                if(xsec>0 and not isdata) :
                    brprod=1.0
                    for ibr in br :  brprod = brprod*ibr
                    weight = integratedLumi*sfactor*xsec*brprod

                #book keep the result
                iplot=0
                for p in plots.items():
                   
                    #create the base plot for this sample if non existing
                    if(len(procplots)<=iplot):
                        procplotstitles.append( p[1].GetTitle() )
                        newname=p[1].GetName()+'_'+str(len(stackplots))
                        newplot=p[1].Clone( newname )
                        newplot.Reset('ICE')
                        newplot.SetTitle(tag)
                        formatPlot(newplot, color,line,lwidth,marker,fill,True,True,lcolor,fcolor,mcolor)
                        procplots.append( newplot )

                    #apply new normalization
                    p[1].Scale(weight)

                    #add to base plot
                    #print ' Normalizing plots from ' + dtag + ' with abs=' + str(absNorm) + ' and weight=' + str(weight) 
                    procplots[iplot].Add(p[1])
                    iplot=iplot+1

            #overall normalization
            if(normto>0) :
                #print ' Normalizing final yields to: ' + str(normto)
                for p in procplots:
                    total=p.Integral()
                    if(total>0): p.Scale(normto/total)
                    
            #store the final result
            if(len(procplots)==0): continue
            plottitles=procplotstitles
            if(not isdata) :
                if(not spimpose) : stackplots.append(procplots)
                else : spimposeplots.append(procplots)
            else : dataplots.append(procplots)

    showControlPlots(stackplots,spimposeplots,dataplots,plottitles,generalLabel,outputDir,samplehtml)


#print usage
def usage() :
    print ' '
    print 'runPlotterOverSamples.py [options]'
    print '  -j : json file containing the samples'
    print '  -l : integrated lumi (used to rescale MC)'
    print '  -i : input directory'
    print '  -o : output directory'
    print '  -d : root directory'
    print '  -f : output format (csv, e.g. C,png,root,pdf)'
    print ' '
                                        

#parse the options 
try:
     # retrive command line options
     shortopts  = "l:j:i:o:d:h?"
     opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
     # print help information and exit:
     print "ERROR: unknown options in argument %s" % sys.argv[1:]
     usage()
     sys.exit(1)


integratedLumi=1.0
inputDir='data'
outputDir='data/plots'
getFromDir=''
samplesDB=''
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(1)
    elif o in('-j'): samplesDB = a 
    elif o in('-i'): inputDir = a
    elif o in('-d'): getFromDir = a
    elif o in('-l'): integratedLumi=float(a)
    elif o in('-o'): outputDir=a


#run the script
print ' Integrated lumi is:' + str(integratedLumi) + ' /pb'
print ' Samples defined in: ' +  samplesDB
print ' Plots in: ' + inputDir
runOverSamples( samplesDB, integratedLumi,inputDir, outputDir, getFromDir )
print ' Output stored in: ' + outputDir
print ' Can browse the results using: ' + outputDir + '/index.html'

#  LocalWords:  subcat

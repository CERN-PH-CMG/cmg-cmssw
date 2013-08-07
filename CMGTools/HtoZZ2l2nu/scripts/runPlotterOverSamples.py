#!/usr/bin/env python

import os,sys
import json
from rounding import toLatexRounded
import getopt
import glob
    
import ROOT
ROOT.gSystem.Load('${CMSSW_BASE}/lib/${SCRAM_ARCH}/libCMGToolsHtoZZ2l2nu.so')
from ROOT import TFile, formatPlot, setStyle, showPlots, formatForCmsPublic, getNewCanvas, showMCtoDataComparison, getPlotAsTable, showPlotsAndMCtoDataComparison

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
    urls=glob.glob(inputDir+'/'+tag+'.root')
    if(len(urls)==0) : return results

    #fix me, in order to have more files
    url=urls[0]
    file = ROOT.TFile(url)

    if(file is None): return results
    if(file.IsZombie()) : return results
    baseDirName='evAnalyzer/'
    if(len(getFromDir)>0): baseDirName=getFromDir
    dir = file.GetDirectory(baseDirName)
    try:
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
                    if(h.GetSumw2N()<=h.GetNbinsX()): h.Sumw2()
                    cresults[h.GetName()]=h
                    if(hname.find('cutflow')<0): continue
                    if(hname.find('presel')>=0): continue
                    cnorm=h.GetBinContent(1)
                    
                #rescale if not data
                if( not isData and cnorm!=0 and not noNorm) :
                    for p in cresults.items():
                        p[1].Scale(1./float(cnorm))
                results.update(cresults)
            except :
                h = file.Get(path)
                h.SetDirectory(0)
                if(h.GetSumw2N()<=h.GetNbinsX()): h.Sumw2()
                centralresults[h.GetName()]=h

        #rescale central results (assume latest normalization found)    
        if( not isData and cnorm!=0 and not noNorm) :
            for p in centralresults.items():
                p[1].Scale(1./float(cnorm))
        results.update(centralresults)

    except :
        cresults={}
        plots = file.GetListOfKeys();
        for p in plots:
            pname=p.GetName()
            h = file.Get(pname)
            h.SetDirectory(0)
            if(h.GetSumw2N()<=h.GetNbinsX()): h.Sumw2()
            cresults[h.GetName()]=h
        results.update(cresults)
        
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
def showControlPlots(stackplots=None,spimposeplots=None,dataplots=None,plottitles=None,generalLabel='CMS preliminary',outputDir='data/plots', samplehtml='',forceNormalization=True) :

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


        isamp=0
        for iplot in stack : isamp = isamp+1
        for iplot in spimpose : isamp = isamp+1
        for iplot in data: isamp = isamp+1
            
        plotLabel = generalLabel
        c.Clear()
        leg=showPlotsAndMCtoDataComparison(c,stack,spimpose,data,True,forceNormalization)#,forceNormalization)
        formatForCmsPublic(c.cd(1),leg,plotLabel,isamp)
        c.SaveAs(outputDir+'/'+pname+'.png')
        c.SaveAs(outputDir+'/'+pname+'.C')
        
        if(pname.find('cutflow')>=0 or pname.find('eventflow')>=0 or pname.find('evtflow')>=0) :
            try :
                savePlotAsTable(stack,spimpose,data,outputDir+'/'+pname+'.tex')
            except :
                continue
        
    #build a summary for the plot browser
    categories=[]
    subcategories=[]
    finalplots=[]
    for tag in plotsToDisplay.items() :

        tagch=''
        if( tag[0]=='All events')  : tagch='all'
        elif( tag[0]=='ee events') : tagch='ee'
        elif( tag[0]=='#mu#mu events') : tagch='mumu'
        elif( tag[0]=='e#mu events') : tagch='emu'
        elif( tag[0]=='e#tau events') : tagch='etau'
        elif( tag[0]=='#mu#tau events') : tagch='mutau'
        #else : tagch=tag[0]
        categories.append(tagch)
        
        for pname in tag[1] :
            ptokens=pname.split('_')
            if(len(ptokens)>1):
                prefix=ptokens[0]
                subcat=''
                if(prefix.find('eq0jets')>=0) :     subcat='eq0jets'
                elif(prefix.find('geq1jets')>=0) :  subcat='geq1jets'
                elif(prefix.find('eq1jets')>=0) :   subcat='eq1jets'
                elif(prefix.find('geq2jets')>=0) :  subcat='geq2jets'
                elif(prefix.find('eq2jets')>=0) :   subcat='eq2jets'
                elif(prefix.find('geq3jets')>=0) :  subcat='geq3jets'
                elif(prefix.find('eq3jets')>=0) :  subcat='eq3jets'
                elif(prefix.find('eq0btags')>=0) :  subcat='eq0btags'
                elif(prefix.find('geq1btags')>=0) : subcat='geq1btags'
                elif(prefix.find('eq1btags')>=0) :  subcat='eq1btags'
                elif(prefix.find('geq2btags')>=0) : subcat='geq2btags'
                elif(prefix.find('eq2btags')>=0) : subcat='eq2btags'
                elif(prefix.find('vbf')>=0) :       subcat='vbf'
                else :
                    subcat=prefix
                    subcat=subcat.replace(tagch,'')
                subcategories.append(subcat)
                finalplots.append( ptokens[1] ) 
            
    #remove duplicates
    categories=list(set(categories))
    subcategories=list(set(subcategories))
    finalplots=list(set(finalplots))

    #write in json format
    fileObj = open(outputDir+'/plotter.json','w')
    fileObj.write(json.dumps( { 'categories':categories,
                                'subcategories':subcategories,
                                'plots':finalplots,
                                'ploturl':''},
                              sort_keys=True,
                              indent=4 ) )
    fileObj.close()
    
    

"""
Parses a json file and retrieves the necessary info
"""
def runOverSamples(samplesDB, integratedLumi=1.0, inputDir='data', outputDir='data/plots', getFromDir='', forceNormalization=False ) :
  
    #open the file which describes the sample
    jsonFile = open(samplesDB,'r')
    procList=json.load(jsonFile,encoding='utf-8').items()
    stackplots=[]
    spimposeplots=[]
    dataplots=[]
    plottitles=[]
    generalLabel='CMS preliminary, #sqrt{s}=7 TeV, #int L=%3.1f fb^{-1}' % (integratedLumi/1000)

    samplehtml="<tr><th colspan=\"3\"><large>Samples used in analysis</large></th></tr>"
    
    #run over sample
    for proc in procList :

        #run over processes
        iprocess=0
        for desc in proc[1] :
            iprocess=iprocess+1
            
            procplots=[]
            procplotstitles=[]
            isdata = getByLabel(desc,'isdata',False)
            forceDataNorm = getByLabel(desc,'forceDataNorm',False)
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

                #compute the normalization
                weight=1
                absNorm=False
                sfactor = getByLabel(d,'sfactor',1)
                xsec = getByLabel(d,'xsec',-1)
                br = getByLabel(d,'br',[])
                brprod=1.0
                if(xsec>0 or forceDataNorm) :
                    for ibr in br :  brprod = brprod*ibr
                    weight = integratedLumi*sfactor*xsec*brprod
                samplehtml+="<tr><td>"+dtag+"</td>"
                samplehtml+="<td>"+str(xsec) + "x" +str(brprod) + "x" + str(sfactor) + "</td>"
                samplehtml+="<td><small>" + getByLabel(d,'dset','n/a') + "</small></td></tr>\n" 

                #book keep the result
                iplot=0
                for p in plots.items():
                   
                    #create the base plot for this sample if non existing
                    if(len(procplots)<=iplot):
                        procplotstitles.append( p[1].GetTitle() )
#                        newname=p[1].GetName()+'_'+str(iprocess)
                        newname=p[1].GetName()
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

            #save copy in plotter.root
            pOut = TFile.Open(outputDir+"/plotter.root","UPDATE")
            pOut.cd()
            #pDir = pOut.mkdir( 'proc_'+str(iprocess) )
            pDir = pOut.mkdir( tag.replace('/','-') )
            pDir.cd()
            for i in xrange(0,len(procplots)): procplots[i].Write()
            pOut.Close()

    print 'Output plots available @ ' + outputDir + '/plotter.root'
    showControlPlots(stackplots,spimposeplots,dataplots,plottitles,generalLabel,outputDir,samplehtml,forceNormalization)


#print usage
def usage() :
    print ' '
    print 'runPlotterOverSamples.py [options]'
    print '  -j : json file containing the samples'
    print '  -l : integrated lumi (used to rescale MC)'
    print '  -i : input directory'
    print '  -o : output directory'
    print '  -d : root directory'
    print '  -n : force normalization'
    print '  -f : output format (csv, e.g. C,png,root,pdf)'
    print ' '
                                        

#parse the options 
try:
     # retrive command line options
     shortopts  = "l:n:j:i:o:d:f:h?"
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
forceNormalization=False
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(1)
    elif o in('-j'): samplesDB = a 
    elif o in('-i'): inputDir = a
    elif o in('-d'): getFromDir = a
    elif o in('-l'): integratedLumi=float(a)
    elif o in('-o'): outputDir=a
    elif o in('-n'): forceNormalization=True
    
os.system('rm -rf plotter.root')


#run the script
print ' Integrated lumi is: %3.1f fb' % (integratedLumi/1000)
print ' Samples defined in: ' +  samplesDB
print ' Plots in: ' + inputDir
runOverSamples( samplesDB, integratedLumi,inputDir, outputDir, getFromDir, forceNormalization )
print ' Output stored in: ' + outputDir
os.system('cp ${CMSSW_BASE}/src/CMGTools/HtoZZ2l2nu/data/html/index.html ' + outputDir)
print ' Can browse the results using: ' + outputDir + '/index.html'

#  LocalWords:  subcat

#!/usr/bin/env python
import os,sys
import json
import ROOT
import getopt
import commands

#print usage
def usage() :
    print ' '
    print 'getThXsec.py [options]'
    print '  -j : json file containing the samples'
    print ' '
    exit(-1)

"""
Gets the value of a given item
(if not available a default value is returned)
"""
def getByLabel(desc,key,defaultVal=None) :
    try :
        return desc[key]
    except KeyError:
        return defaultVal


#parse the options 
try:
     # retrive command line options
     shortopts  = "j:h?"
     opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
     # print help information and exit:
     print "ERROR: unknown options in argument %s" % sys.argv[1:]
     usage()
     sys.exit(1)


samplesDB=''
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(0)
    elif o in('-j'): samplesDB = a
                                        
#open the file which describes the sample
jsonFile = open(samplesDB,'r')
procList=json.load(jsonFile,encoding='utf-8').items()

from ROOT import TGraphAsymmErrors
gluglu={}
vbf={}
#run over sample
for proc in procList :

    #run over processes
    for desc in proc[1] :

        data = desc['data']
        for d in data :
            dtag = getByLabel(d,'dtag','')
            if(dtag.find('toH')<0): continue 
            xsec = getByLabel(d,'xsec',-1)
            xsecunc = getByLabel(d,'xsecunc',[])
            xsecup=abs(xsecunc[0]/100*xsec)
            xsecdown=abs(xsecunc[1]/100*xsec)
            splittag=dtag.split('to')
            mass=float((splittag[1].split('H'))[1])
            if(dtag.find('VBF')>0):  vbf[mass]=[xsec,xsecup,xsecdown]
            if(dtag.find('GG')>0): gluglu[mass]=[xsec,xsecup,xsecdown]


glugluGr=TGraphAsymmErrors()
vbfGr=TGraphAsymmErrors()
totalGr=TGraphAsymmErrors()

ipt=0
import math
for imass in gluglu.items() :

    ggxsec=imass[1]
    glugluGr.SetPoint(ipt,imass[0],ggxsec[0])
    glugluGr.SetPointError(ipt,0,0,ggxsec[1],ggxsec[2])

    vbfxsec=vbf[imass[0]]
    vbfGr.SetPoint(ipt,imass[0],vbfxsec[0])
    vbfGr.SetPointError(ipt,0,0,vbfxsec[1],vbfxsec[2])

    total=ggxsec[0]+vbfxsec[0]
    errUp=math.sqrt(pow(ggxsec[1],2)+pow(vbfxsec[1],2))
    errDown=math.sqrt(pow(ggxsec[2],2)+pow(vbfxsec[2],2))
    totalGr.SetPoint(ipt,imass[0],total)
    totalGr.SetPointError(ipt,0,0,errUp,errDown)

    ipt=ipt+1
    
c=ROOT.TCanvas("c","c",600,600)

totalGr.SetName("total")
totalGr.SetTitle("gg+VBF")
totalGr.SetMarkerStyle(20)
totalGr.SetMarkerColor(1)
totalGr.SetFillColor(0)
totalGr.SetFillStyle(0)
totalGr.Draw("ae1p")
totalGr.GetXaxis().SetTitle("Higgs mass [GeV/c^{2}]")
totalGr.GetYaxis().SetTitle("Cross section [pb]")

glugluGr.SetName("gg")
glugluGr.SetTitle("gg")
glugluGr.SetMarkerStyle(21)
glugluGr.SetMarkerColor(2)
glugluGr.SetFillColor(0)
glugluGr.SetFillStyle(0)
glugluGr.Draw("e1p")

vbfGr.SetName("vbf")
vbfGr.SetTitle("VBF")
vbfGr.SetMarkerStyle(22)
vbfGr.SetMarkerColor(4)
vbfGr.SetFillColor(0)
vbfGr.SetFillStyle(0)
vbfGr.Draw("e1p")

leg=c.BuildLegend()
leg.SetFillStyle(0)
leg.SetBorderSize(0)
leg.SetTextFont(42)

fout=ROOT.TFile.Open('thxsec.root','RECREATE')
fout.cd()
totalGr.Write()
glugluGr.Write()
vbfGr.Write()
fout.Close()




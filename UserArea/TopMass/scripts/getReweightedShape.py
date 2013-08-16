#!/usr/bin/env python


## ./scripts/getReweightedShape.py -i bla/


import os
import sys
import optparse
import subprocess
from pprint import pprint
from array import array
import re

import ROOT
from ROOT import *

sys.path.append('../scripts/')
sys.path.append('scripts/')
import myRootFunctions as mRF




def getFiles(path):
    l = os.listdir(path)
    files = [f for f in l if f.endswith('.root')]
    return files

def getSamples(path):
    files = getFiles(path)
    ## FIXME use regular expressions
    # samples = list(set([re.sub(r'_', '', f) for f in files]))
    samples = []
    for f in files:
        li = f.split('_')
        li.pop(-1)
        name = '_'.join(li)
        samples.append(name)
    # samples = list(set(['_'.join([f.split('_').pop()]) for f in files]))
    return list(set(samples))

def getChannels():
    pass


def mergeFiles(inputdir, outputdir, samples, force):
    print 'merging the following samples:'
    for sample in samples:
        ## hadd the files
        if force:
            cmd = 'hadd -f '+outputdir+'/'+sample+'.root '+' '.join([inputdir+'/'+f for f in samples[sample]])
        else:
            cmd = 'hadd '+outputdir+'/'+sample+'.root '+' '.join([inputdir+'/'+f for f in samples[sample]])
        print cmd
        os.system(cmd)


def makeDirectory(path):
    if not os.path.exists(path):
        print 'creating output directory '+path
        os.makedirs(path)
    else:
        print 'directory '+path+' already exists'    


def getSummedShapes(path, tag, chan=''):

    ## FIXME get the following numbers from the file
    lxy_max = 5.
    lxy_bins = 50

    if chan != '':
        chan = '_'+chan


    ## prepare a summed histogram
    summed_cat = []
    ## open the corresponfing file
    tfile = mRF.openTFile(path)
    # nevents = mRF.getHist(tfile,'map_counter').Integral(0,2)

    nevents = 0.
    for i in xrange(ybin-1):
        map_cat = mRF.getHist(tfile,'map'+chan+'_cat'+str(i))
        # n = map_cat.Integral(0,map_cat.GetNbinsX()+1)
        n = map_cat.Integral()
        nevents+=n

    checksum = 0.
    checkweight = 0.
    ## loop over categories
    for i in xrange(ybin-1):
        summed = TH1F(tag+'_summed_lxy'+chan+'_cat'+str(i),tag+'_summed_lxy'+chan+'_cat'+str(i),lxy_bins, 0., lxy_max).Clone()
        summed.Sumw2()
        ## get the map histo
        map_cat = mRF.getHist(tfile,'map'+chan+'_cat'+str(i))
        map_cat.Sumw2()
        checksum+=map_cat.GetEntries()
        ## get the gamma binning per category
        nbins = map_cat.GetNbinsX()

        for j in xrange(nbins):
            w = float(map_cat.GetBinContent(j+1))
            checkweight+=w
            if tag=='templ':
                hist = mRF.getHist(tfile,tag+'_lxy_cat'+str(i)+'_bin'+str(j))
            else:
                hist = mRF.getHist(tfile,tag+'_lxy'+chan+'_cat'+str(i)+'_bin'+str(j))

            hist.Sumw2()
            ## scale the histogram
            integr = float(hist.Integral(0, hist.GetNbinsX()+1))
            if integr > 0 and tag=='templ':
                hist.Scale(w/integr)
            ## sum the histograms
            summed.Add(hist.Clone())
        summed_cat.append(summed)
    print 'checksum (map): ',checksum
    print 'checksum: ',sum([h.GetEntries() for h in summed_cat])
    print 'checkweight: ',checkweight
    return summed_cat


def getFinalShape(histos,tag,chan=''):
    ## FIXME get the following numbers from the file
    lxy_max = 5.
    lxy_bins = 50

    if chan != '':
        chan = '_'+chan

    summed = TH1F(tag+'_final_lxy'+chan,tag+'_final_lxy'+chan,lxy_bins, 0., lxy_max).Clone()
    checksum=0.
    for hist in histos:
        h = hist.Clone('')
        checksum+=h.GetEntries()
        summed.Add(h)
    print 'checksum final: ',checksum
    return summed.Clone()


def getResults(new, old):
    ## get the mean
    new_mean = new.GetMean()
    new_mean_error = new.GetMeanError()
    old_mean = old.GetMean()
    old_mean_error = old.GetMeanError()
    delta_mean = old_mean-new_mean

    ## get the quantiles
    old_xq=array('d',[0.25,0.5,0.75,1])
    old_yq=array('d',[0,   0,  0,   0])
    old.GetQuantiles(4,old_yq,old_xq)
    old_q25 = old_yq[0]
    old_q50 = old_yq[1]
    
    xq=array('d',[0.25,0.5,0.75,1])
    yq=array('d',[0,   0,  0,   0])
    new.GetQuantiles(4,yq,xq)
    new_q25 = yq[0]
    new_q50 = yq[1]
    # gr_q25.SetPoint(ix,ix+1,old_25q-new_25q)
    # gr_q50.SetPoint(ix,ix+1,old_50q-new_50q)

    delta_q25 = old_q25-new_q25
    delta_q50 = old_q50-new_q50

    
    chi = old.Chi2Test(new,"CHI2/NDF")
    # gr_chi2.SetPoint(ix,ix+1,chi)


    print '************************************'
    print 'entries: ',old.GetEntries(), new.GetEntries()
    print 'integral:', old.Integral(), new.Integral()
    print 'Mean (old,new: )',old_mean,',',new_mean
    print 'Delta mean: ',delta_mean
    print 'Delta q25:  ',delta_q25
    print 'Delta q50:  ',delta_q50
    print 'ChiSquare:  ',chi
    print '************************************'

    results = {'old_mean':old_mean,
               'new_mean':new_mean,
               'old_mean_error':old_mean_error,
               'delta_mean':delta_mean,
               'delta_q25':delta_q25,
               'delta_q50':delta_q50,
               'chi_square':chi
               }

    name = new.GetTitle()
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetOptTitle(0)

    c = TCanvas(name,name,600,600)
    old.SetLineColor(2)
    old.GetXaxis().SetTitle('L_{xy} [cm]')
    old.GetYaxis().SetTitle('events/bin')
    old.GetYaxis().SetTitleOffset(2.)

    new.GetXaxis().SetTitle('L_{xy} [cm]')
    new.GetYaxis().SetTitle('events/bin')
    new.GetYaxis().SetTitleOffset(2.)

    old.Draw('e1 hist')
    new.Draw('e1 same')
    rpave = TPaveText(0.5, 0.5, 0.8, 0.8, 'NDC')
    rpave.SetFillColor(0)
    rpave.SetLineColor(0)
    rpave.SetShadowColor(0)
    rpave.AddText('#Delta #mu: '+str("%.3f" % delta_mean)+' cm')
    rpave.AddText('#Delta q25: '+str("%.3f" % delta_q25)+' cm')
    rpave.AddText('#Delta q50: '+str("%.3f" % delta_q50)+' cm')
    rpave.AddText('Stat. error: '+str("%.3f" % old_mean_error)+' cm')
    rpave.AddText('#chi^{2}/N_{dof}: '+str("%.3f" % chi))

    old.Draw('e1 hist')
    new.Draw('e1 same')
    new.Draw('axis same')
    rpave.Draw()
    c.Update()
    name = new.GetHistName()
    print name
    # c.Write()

    return c,results


    
def makeSimplePlot(old,new,name):
    c = TCanvas(name,name,600,600)
    old.SetLineColor(2)
    old.GetXaxis().SetTitle('L_{xy} [cm]')
    old.GetXaxis().SetTitle('events/bin')

    new.GetXaxis().SetTitle('L_{xy} [cm]')
    new.GetXaxis().SetTitle('events/bin')

    old.Draw('e1 hist')
    new.Draw('e1 same')
    return c


def makeSummaryPlot(results,samplelist,tag):
    c_summary = TCanvas('summary_'+tag,'summary_'+tag,900,600)
    c_summary.Divide(1,4)
    nbins = len(results)
    hist_mean = TH1F('hist_mean','hist_mean', nbins,0,nbins)
    hist_mean.SetMaximum(0.01)
    hist_mean.SetMinimum(-0.01)
    hist_mean.SetMarkerStyle(20)
    hist_mean.GetYaxis().SetTitle('#Delta <L_{xy}>')
    hist_mean.GetYaxis().SetTitleSize(0.1)
    hist_mean.GetYaxis().SetTitleOffset(0.4)
    hist_mean.GetXaxis().SetLabelSize(0.1)
    hist_mean.GetYaxis().SetLabelSize(0.1)
    # hist_mean_error_up = TH1F('hist_mean_error_up','hist_mean_error_up', nbins,0,nbins)
    # hist_mean_error_up.SetFillColor(5)
    # hist_mean_error_up.SetFillStyle(3004)
    # hist_mean_error_up.SetMaximum(0.01)
    # hist_mean_error_up.SetMinimum(-0.01)
    # hist_mean_error_up.SetMarkerStyle(20)
    # hist_mean_error_down = TH1F('hist_mean_error_down','hist_mean_error_down', nbins,0,nbins)
    # hist_mean_error_up.SetFillColor(10)
    # hist_mean_error_up.SetFillStyle(1)
    # hist_mean_error_down.SetMaximum(0.01)
    # hist_mean_error_down.SetMinimum(-0.01)
    # hist_mean_error_down.SetMarkerStyle(20)
    bin=0
    print results
    #for sample in all_final_results:
    for sample in samplelist:    
        print sample
        bin+=1
        print results[sample]['delta_mean']
        hist_mean.SetBinContent(bin,results[sample]['delta_mean'])
        # hist_mean_error_up.SetBinContent(bin,0.+all_final_results[sample]['old_mean_error'])
        # hist_mean_error_down.SetBinContent(bin,0.-all_final_results[sample]['old_mean_error'])
        hist_mean.GetXaxis().SetBinLabel(bin,sampledict[sample])
    c_summary.cd(1)
    # hist_mean_error_up.Draw()
    # hist_mean_error_down.Draw('same')
    hist_mean.Draw('p')
    hist_mean.Draw('axis same')
    line = TGraph()
    line.SetPoint(0,-1,0.)
    line.SetPoint(1,999,0.)
    line.SetLineWidth(2)
    line.SetLineColor(2)
    line.SetLineStyle(2)
    line.Draw()

    hist_q25 = TH1F('hist_q25','hist_q25', nbins,0,nbins)
    hist_q25.SetMaximum(0.01)
    hist_q25.SetMinimum(-0.01)
    hist_q25.SetMarkerStyle(21)
    hist_q25.GetYaxis().SetTitle('#Delta q_{0.25}(L_{xy})')
    hist_q25.GetYaxis().SetTitleSize(0.1)
    hist_q25.GetYaxis().SetTitleOffset(0.4)
    hist_q25.GetXaxis().SetLabelSize(0.1)
    hist_q25.GetYaxis().SetLabelSize(0.1)
    bin=0
    print results
    # for sample in all_final_results:
    for sample in samplelist:    
        print sample
        bin+=1
        print results[sample]['delta_q25']
        hist_q25.SetBinContent(bin,results[sample]['delta_q25'])
        hist_q25.GetXaxis().SetBinLabel(bin,sampledict[sample])
    c_summary.cd(2)
    hist_q25.Draw('p')
    hist_q25.Draw('axis same')
    line.Draw()

    hist_q50 = TH1F('hist_q50','hist_q50', nbins,0,nbins)
    hist_q50.SetMaximum(0.01)
    hist_q50.SetMinimum(-0.01)
    hist_q50.SetMarkerStyle(22)
    hist_q50.GetYaxis().SetTitle('#Delta q_{0.50}(L_{xy})')
    hist_q50.GetYaxis().SetTitleSize(0.1)
    hist_q50.GetYaxis().SetTitleOffset(0.4)
    hist_q50.GetXaxis().SetLabelSize(0.1)
    hist_q50.GetYaxis().SetLabelSize(0.1)
    bin=0
    print results
    # for sample in all_final_results:
    for sample in samplelist:    
        print sample
        bin+=1
        print results[sample]['delta_q50']
        hist_q50.SetBinContent(bin,results[sample]['delta_q50'])
        hist_q50.GetXaxis().SetBinLabel(bin,sampledict[sample])
    c_summary.cd(3)
    hist_q50.Draw('p')
    hist_q50.Draw('axis same')
    line.Draw()

    hist_chi_square = TH1F('hist_chi_square','hist_chi_square', nbins,0,nbins)
    hist_chi_square.SetMaximum(2.)
    hist_chi_square.SetMinimum(0.)
    hist_chi_square.SetMarkerStyle(23)
    hist_chi_square.GetYaxis().SetTitle('#chi^{2}/N_{dof}')
    hist_chi_square.GetYaxis().SetTitleSize(0.1)
    hist_chi_square.GetYaxis().SetTitleOffset(0.4)
    hist_chi_square.GetXaxis().SetLabelSize(0.1)
    hist_chi_square.GetYaxis().SetLabelSize(0.1)
    bin=0
    print results
    # for sample in all_final_results:
    for sample in samplelist:    
        print sample
        bin+=1
        print results[sample]['chi_square']
        hist_chi_square.SetBinContent(bin,results[sample]['chi_square'])
        hist_chi_square.GetXaxis().SetBinLabel(bin,sampledict[sample])
    c_summary.cd(4)
    hist_chi_square.Draw('p')
    hist_chi_square.Draw('axis same')
    line2 = TGraph()
    line2.SetPoint(0,-1,1.)
    line2.SetPoint(1,999,1.)
    line2.SetLineWidth(2)
    line2.SetLineColor(2)
    line2.SetLineStyle(2)
    line2.Draw()




    c_summary.Print('summary_'+tag+'.png')
    c_summary.Print('summary_'+tag+'.C')
    c_summary.Print('summary_'+tag+'.eps')







# def makeComparisonPlots():
    

## FIXME define this in the topinfo file
ybins = [0., 0.45, 0.9, 1.1, 1.5, 2.5, 5.]
ybin = len(ybins)-1
ybins_array = array( 'f', ybins )

sampledict = {
    'TTJets_mass178_5':'178.5 GeV',
    'TTJets_mass169_5':'169.5 GeV',
    'TTJets_mass175_5':'175.5 GeV',
    'TTJets_mass181_5':'181.5 GeV',
    'TTJets_scaleup':'Q^{2} up',
    'TTJets_mass166_5':'166.5 GeV',
    'TTJets_mass161_5':'161.5 GeV',
    'TTJets_MassiveBinDECAY':'172.5 GeV',
    'TTJets_scaledown':'Q^{2} down',
    'TTJets_matchingup':'matching up',
    'TTJets_matchingdown':'matching down',
    'TTJets_mass184_5':'184.5 GeV',
    'TTJets_mass163_5':'163.5 GeV',
    }

samplelist = [
    'TTJets_mass161_5',
    'TTJets_mass163_5',
    'TTJets_mass166_5',
    'TTJets_mass169_5',
    'TTJets_MassiveBinDECAY',
    'TTJets_mass175_5',
    'TTJets_mass178_5',
    'TTJets_mass181_5',
    'TTJets_mass184_5',
    'TTJets_scaleup',
    'TTJets_scaledown',
    'TTJets_matchingup',
    'TTJets_matchingdown',
    ]



def main():
    
    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    parser.add_option('-i', '--inputdir'  ,    dest='inputdir'       , help='Name of the input directory containing the trees.'          , default=None)
    parser.add_option('-t', '--tag'       ,    dest='tag'            , help='Sample tag.'                                                , default='TTJets')
    parser.add_option('-f', '--force'     ,    dest='force'          , help='Use the force flag to overwrite the hadded samples.' , action='store_true'        , default=False)
    parser.add_option('-b', '--batch'     ,    dest='batch'          , help='Use this flag to run root in batch mode.'            , action='store_true'        , default=False)


    (opt, args) = parser.parse_args()

    if opt.inputdir is None:
        parser.error('No input directory defined!')

    inputdir = opt.inputdir+'/sub_scripts/'
    outputdir = inputdir.replace('/sub_scripts/','/merged/')
    sampletag = opt.tag
    force = opt.force
    batch = opt.batch

    if batch:
        ## run ROOT in batch mode                                                                                                                                                                                 
        sys.argv.append( '-b' )
        ROOT.gROOT.SetBatch()

    samples = getSamples(inputdir)
    pprint(samples)

    files = {}
    for sample in samples:
        if sampletag not in sample:
            continue
        files[sample] = [f for f in getFiles(inputdir) if sample in f]
    pprint(files)

    ## create output directory and merge all files
    inpath = inputdir.replace('sub_scripts','/merged')
    makeDirectory(inpath)
    mergeFiles(inputdir, outputdir, files, force)

    ## create output directory
    outpath = inputdir.replace('sub_scripts','/summed')
    makeDirectory(outpath)



    
    ## now, extract the summed histograms
    all_results = {}
    all_final_results = {}
    all_results_mu = {}
    all_final_results_mu = {}
    all_results_e = {}
    all_final_results_e = {}
    for sample in samples:
        if sampletag not in sample:
            continue
        path = outputdir+'/'+sample+'.root'

        ## sum also the the histograms for each channel individually


        tag = 'templ'
        templ_summed = getSummedShapes(path, tag,'')
        templ_final = getFinalShape(templ_summed,tag,'')
        templ_summed_mu = getSummedShapes(path, tag,'mu')
        templ_final_mu = getFinalShape(templ_summed_mu,tag,'mu')
        templ_summed_e = getSummedShapes(path, tag,'e')
        templ_final_e = getFinalShape(templ_summed_e,tag,'e')

        tag = 'orig'
        orig_summed = getSummedShapes(path, tag,'')
        orig_final = getFinalShape(orig_summed,tag,'')
        orig_summed_mu = getSummedShapes(path, tag,'mu')
        orig_final_mu = getFinalShape(orig_summed_mu,tag,'mu')
        orig_summed_e = getSummedShapes(path, tag,'e')
        orig_final_e = getFinalShape(orig_summed_e,tag,'e')


        ## make control plots and get results
        print sample
        results = {}
        results_mu = {}
        results_e = {}
        outfile = ROOT.TFile().Open(outpath+'/'+sample+'.root','RECREATE')
        for i in xrange(len(templ_summed)):
            c,results = getResults(templ_summed[i],orig_summed[i])
            c.Write()
            c_mu,results_mu = getResults(templ_summed_mu[i],orig_summed_mu[i])
            c_mu.Write()
            c_e,results_e = getResults(templ_summed_e[i],orig_summed_e[i])
            c_e.Write()
        cfinal,final_results=getResults(templ_final, orig_final)
        cfinal.Write()
        cfinal_mu,final_results_mu=getResults(templ_final_mu, orig_final_mu)
        cfinal_mu.Write()
        cfinal_e,final_results_e=getResults(templ_final_e, orig_final_e)
        cfinal_e.Write()

        all_results[sample] = results
        all_final_results[sample] = final_results
        all_results_mu[sample] = results_mu
        all_final_results_mu[sample] = final_results_mu
        all_results_e[sample] = results_e
        all_final_results_e[sample] = final_results_e
        

        ## write to output file
        templ_final.Write()
        for hist in templ_summed:
            hist.Write()
        orig_final.Write()
        for hist in orig_summed:
            hist.Write()
        ## muons
        templ_final_mu.Write()
        for hist in templ_summed_mu:
            hist.Write()
        orig_final_mu.Write()
        for hist in orig_summed_mu:
            hist.Write()
        ## electrons
        templ_final_e.Write()
        for hist in templ_summed_e:
            hist.Write()
        orig_final_e.Write()
        for hist in orig_summed_e:
            hist.Write()
        ## close the file
        outfile.Close()


    print all_final_results
    print all_final_results_mu
    makeSummaryPlot(all_final_results,samplelist,'incl')
    makeSummaryPlot(all_final_results_mu,samplelist,'mu')
    makeSummaryPlot(all_final_results_e,samplelist,'e')


    print 'done'



if __name__ == '__main__':
    main()



    ##


    ##


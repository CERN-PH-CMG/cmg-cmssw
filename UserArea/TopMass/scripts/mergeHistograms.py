#!/usr/bin/env python

## This is a script to merge and normalize the histograms obtaine form
## the final event selection
##
## It takes a directory conatining all root files with the histograms as an input and optionally
## an output file name. If no output file name is specified, then the default is test.root
##
## Example of a command:
## ./scripts/mergeHistograms.py -i histos/ -o bla.root

import sys
import ROOT
import os
import re
import optparse
from ROOT import gSystem, gDirectory

## load the topinfo.py
sys.path.append('../python')
sys.path.append('python')
import topinfo



## a dictionary translating the file names into the names of the histograms
## histograms with smae names will be added up into one histogram
sampleName = {
    'TTJets_MassiveBinDECAY':'ttbar_1725',
    'TTJets_mass166_5':'ttbar_1665',
    'TTJets_mass161_5':'ttbar_1615',
    'TTJets_mass169_5':'ttbar_1695',
    'TTJets_mass175_5':'ttbar_1755',
    'TTJets_mass181_5':'ttbar_1815',
    'TTJets_mass184_5':'ttbar_1845',
    'TTJets_mass178_5':'ttbar_1785',
    'TTJets_mass163_5':'ttbar_1635',
    'TTJets_matchingup'  :'ttbarsystmepsup_1725',
    'TTJets_matchingdown':'ttbarsystmepsdown_1725',
    'TTJets_scaledown'   :'ttbarsystq2down_1725',
    'TTJets_scaleup'     :'ttbarsystq2up_1725',
    'WJetsToLNu'        :'wjets_0',
    'DYJetsToLL_M-50'   :'zjets_0',
    'T_t-channel'       :'singletop_0',
    'T_s-channel'       :'singletop_0',
    'T_tW-channel-DR'   :'singletop_0',
    'Tbar_t-channel'    :'singletop_0',
    'Tbar_tW-channel-DR':'singletop_0',
    'Tbar_s-channel'    :'singletop_0',
    #    'SingleMuRun2012C-24Aug2012-v1AODV5_BPAT_CMG_V5_10_0'           :'data_0',
    'ElectronHadRun2012A-13Jul2012-v1AODV5_BPAT_CMG_V5_10_0'        :'data_0',
    'ElectronHadRun2012A-recover-06Aug2012-v1AODV5_BPAT_CMG_V5_10_0':'data_0',
    'MuHadRun2012A-13Jul2012-v1AODV5_BPAT_CMG_V5_10_0'              :'data_0',
    'MuHadRun2012A-recover-06Aug2012-v1AODV5_BPAT_CMG_V5_10_0'      :'data_0',
    'SingleElectronRun2012B-13Jul2012-v1AODV5_BPAT_CMG_V5_10_0'     :'data_0',
    'SingleElectronRun2012C-24Aug2012-v1AODV5_BPAT_CMG_V5_10_0'     :'data_0',
    'SingleMuRun2012B-13Jul2012-v1AODV5_BPAT_CMG_V5_10_0'           :'data_0',
    'SingleMuRun2012C-24Aug2012-v1AODV5_BPAT_CMG_V5_10_0'           :'data_0',
    'QCD_Pt_20_30_BCtoE'       :'qcd_0',
    'QCD_Pt_20_MuEnrichedPt_15':'qcd_0',
    'QCD_Pt_30_80_BCtoE'       :'qcd_0',
    'QCD_Pt_80_170_BCtoE'      :'qcd_0',
    'QCD_Pt_20_30_EMEnriched'  :'qcd_0',
    'QCD_Pt_80_170_EMEnriched' :'qcd_0',
    'QCD_Pt_30_80_EMEnriched'  :'qcd_0',
    '':'',
    '':'',
    ## '':'',
    }


## define some useful functins to handle the *root files and histograms

def openTFile(path, option='r'):
    f =  ROOT.TFile.Open(path,option)
    if not f.__nonzero__() or not f.IsOpen():
        raise NameError('File '+path+' not open')
    return f

def GetKeyNames( file, dir = "" ):
        file.cd(dir)
        return [key.GetName() for key in gDirectory.GetListOfKeys()]

def getHist(file, hist):
    h = file.Get(hist)
    if not h.__nonzero__():
        raise NameError('Histogram '+hist+' doesn\'t exist in '+str(file))
    return h

## here goes the main function
if __name__ == '__main__':
    print '''
 _____ _                       ___  ___                         
/  ___| |                      |  \/  |                         
\ `--.| |__   __ _ _ __   ___  | .  . | ___ _ __ __ _  ___ _ __ 
 `--. \ '_ \ / _` | '_ \ / _ \ | |\/| |/ _ \ '__/ _` |/ _ \ '__|
/\__/ / | | | (_| | |_) |  __/ | |  | |  __/ | | (_| |  __/ |   
\____/|_| |_|\__,_| .__/ \___| \_|  |_/\___|_|  \__, |\___|_|   
                  | |                            __/ |          
                  |_|                           |___/
'''
    usage = 'usage: %prog [options]'
    parser = optparse.OptionParser(usage)
    parser.add_option('-i', '--inputdir' ,    dest='inputdir'      , help='Name of the local input directory.'    , default=None)
    parser.add_option('-o', '--outfile' ,     dest='outfile'       , help='Outfile.'    , default='test.root')
    
    (opt, args) = parser.parse_args()


    if opt.inputdir is None:
        parser.error('No input directory defined!')

    ## run ROOT in batch mode
    sys.argv.append( '-b' )
    ROOT.gROOT.SetBatch()

    inputdir = opt.inputdir
    outfile  = opt.outfile
    ## get all *.root files in the directory which should get merged
    files = set([f for f in os.listdir(inputdir) if f.endswith('.root')])
    ## define a dict to store the final histograms
    hists = {}
    ## loop over all files
    for f in files:
        path = inputdir+'/'+f
        i_file = openTFile(path)

        ## get all keys in an individual file
        keys = GetKeyNames(i_file, 'ana')
        #print keys
        ## and loop over all histograms
        for key in keys:
            ## omit the counting histo...
            if 'countAll_h' in key:
                continue
            ## construct the histogram name
            tag = f.split('Histos')[0]
            #print tag
            sample = sampleName[tag]
            name = key.replace('_h','_'+sample).replace('Lxy','lxy')
            print name
            ## get the histogram
            hist = getHist(i_file,'ana/'+key).Clone(name)

            ## set some labels
            ## FIXME: this is just a dummy label at the moment
            hist.GetXaxis().SetTitle('SecVtx L_{xy} [cm]')
            hist.GetYaxis().SetTitle('entries')

            ## apply the correct normalization using the topinfo.py file
            ## FIXME: this can be done only once for each sample...
            if 'data' in name:
                weight = 1.
            else:
                for key in topinfo.samples.keys():
                    if tag in key:
                        nevents = topinfo.samples[key][0]
                        xsec = topinfo.samples[key][1]
                        weight = topinfo.lumi * xsec / nevents
            print weight
            hist.Scale(weight)
            
            ## push the histogram into the dict
            ## if a histogram with the same name already exists, the sum them up
            if not name in hists.keys():
                hists[name] = hist
            else:
                hist.Add(hists[name])
                hists[name] = hist
                #print hists

    ## open the output file, loop over all final histograms
    ## create drectories according to the channels and write the histograms to file
    o_file = ROOT.TFile.Open(outfile,'recreate')
    o_file.mkdir('e')
    o_file.mkdir('mu')
    print hists
    for h in hists:
        print h
        channel = h.split('_')[0]
        if channel == 'e':
            o_file.cd(channel)
        elif channel == 'mu':
            o_file.cd(channel)
        else:
            continue
        hists[h].Write()
    o_file.Close()
    ## done...

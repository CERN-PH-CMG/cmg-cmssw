#!/bin/env python

from CMGTools.Production.hadd import haddChunks
#import ROOT 
from ROOT import TTree, TFile, AddressOf, gROOT
import numpy
from datasetInfo import Dataset, DatasetDict



dd = DatasetDict()




class EventKey:
  def __init__(self, r, l, e):
    self.run=r;
    self.lumi=l;
    self.evt=e;






def postProcessBaby( dir, dataset, fast ) :

   print "-> Post-processing dataset: " + dataset

   oldFileName = dir + str("/") + dataset + str("/treeProducerSusyFullHad/treeProducerSusyFullHad_tree.root")
   oldfile = TFile(oldFileName)
   oldtree = oldfile.Get("treeProducerSusyFullHad")

   newFileName = dir + str("/") + dataset + str(".root")
   newfile = TFile(newFileName, "recreate")
   newtree = oldtree.CloneTree(0)
   newtree.SetName("mt2")


   oldtree.SetBranchStatus("*", 0) # disable all branches (faster)
   if not fast:
     oldtree.SetBranchStatus("run", 1) # enable only useful ones
     oldtree.SetBranchStatus("lumi", 1) # enable only useful ones
     oldtree.SetBranchStatus("evt", 1) # enable only useful ones



   scale1fb = numpy.zeros(1, dtype=float)
   xsec     = numpy.zeros(1, dtype=float)
   kfactor  = numpy.zeros(1, dtype=float)
   filter   = numpy.zeros(1, dtype=float)
   nEvts    = numpy.zeros(1, dtype=int)
   id       = numpy.zeros(1, dtype=int)

   newtree.Branch("evt_scale1fb", scale1fb, "evt_scale1fb/D");
   newtree.Branch("evt_xsec",     xsec,     "evt_xsec/D");
   newtree.Branch("evt_kfactor",  kfactor,  "evt_kfactor/D");
   newtree.Branch("evt_filter",   filter,   "evt_filter/D");
   newtree.Branch("evt_nEvts",    nEvts,    "evt_nEvts/I");
   newtree.Branch("evt_id",       id,       "evt_id/I");


   events = oldtree.GetEntries()

   process = dataset.split("_PU")[0]
   d = dd[process]


   scale1fb[0] = d.scale1fb(events)
   xsec    [0] = d.xsection
   kfactor [0] = d.kfactor 
   filter  [0] = d.filter  
   nEvts   [0] = events
   id      [0] = d.id      




   evlist = set()

   for i in oldtree :

     if not fast:
       ek = EventKey(i.run, i.lumi, i.evt)

       if ek not in evlist :
         evlist.add(ek)
         newtree.Fill()
     else:
       newtree.Fill()

   newfile.Write()
   newfile.Close()

   print "-> Saved post-processed babytree in : " + str(newfile.GetName())



def postProcess( dir, fast ) :

  for file in sorted(os.listdir(dir)):
      filepath = '/'.join( [dir, file] )
      if os.path.isdir(filepath):
          dataset = file
          if "_Chunk" in dataset: continue
          postProcessBaby( dir, dataset, fast )
    



if __name__ == '__main__':

    import os
    import sys
    import commands
    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog <dir>
    Find chunks in dir, and run recursive hadd to group all chunks.
    For example: 
    DYJets_Chunk0/, DYJets_Chunk1/ ... -> hadd -> DYJets/
    WJets_Chunk0/, WJets_Chunk1/ ... -> hadd -> WJets/
    """
    parser.add_option("-r","--remove", dest="remove",
                      default=False,action="store_true",
                      help="remove existing destination directories.")
    parser.add_option("-c","--clean", dest="clean",
                      default=False,action="store_true",
                      help="move chunks to Chunks/ after processing.")
    parser.add_option("-f","--fast", dest="fast",
                      default=False,action="store_true",
                      help="don't check for duplicates")

    (options,args) = parser.parse_args()

    if len(args)!=1:
        print 'provide exactly one directory in argument.'
        sys.exit(1)

    dir = args[0]


    checkOutput = commands.getoutput("chunkOutCheck.py " + str(dir) + "/*")
    print checkOutput

    checkOutput_lines = checkOutput.split("\n")
    badFiles = set()
    for aline in checkOutput_lines:
      if aline.startswith("['") :
        thisBadFile = aline.split("['")[1].split("']")[0].split(dir)[1]
        while thisBadFile.startswith("/"):
          thisBadFile = thisBadFile.split("/")[1]
        badFiles.add(thisBadFile)


    haddChunks(dir, options.remove, options.clean, badFiles)

    postProcess( dir, options.fast )


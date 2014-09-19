#!/bin/env python

from CMGTools.Production.hadd import haddChunks
#import ROOT 
from ROOT import TTree, TFile
import numpy




class EventKey:
  def __init__(self, r, l, e):
    self.run=r;
    self.lumi=l;
    self.evt=e;


def removeDuplicatesBaby( dir, dataset ) :

   print "-> Removing duplicates from dataset: " + dataset

   oldFileName = dir + str("/") + dataset + str("/treeProducerSusyFullHad/treeProducerSusyFullHad_tree.root")
   oldfile = TFile(oldFileName)
   oldtree = oldfile.Get("treeProducerSusyFullHad")

   newFileName = dir + str("/") + dataset + str(".root")
   newfile = TFile(newFileName, "recreate")
   newtree = oldtree.CloneTree(0)
   newtree.SetName("mt2")



   evlist = set()

   for i in oldtree :

     ek = EventKey(i.run, i.lumi, i.evt)

     if ek not in evlist :
       evlist.add(ek)
       newtree.Fill()

   newfile.Write()
   newfile.Close()




def removeDuplicates( dir ) :

  for file in sorted(os.listdir(dir)):
      filepath = '/'.join( [dir, file] )
      if os.path.isdir(filepath):
          dataset = file
          if "_Chunk" in dataset: continue
          removeDuplicatesBaby( dir, dataset )
    



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

    if len(badFiles) > 0 :
      print "-> Found the following incomplete Chunks: "
      print badFiles

    haddChunks(dir, options.remove, options.clean, badFiles)

    removeDuplicates( dir )




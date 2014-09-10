#!/bin/env python

from CMGTools.Production.hadd import haddChunks
#import ROOT 
from ROOT import TTree, TFile, AddressOf, gROOT
import numpy



class DatasetInfo:
  def __init__(self):
    self.input;
    self.output;
    self.filter;
    self.kfactor;
    self.xsec;
    self.id;


class EventKey:
  def __init__(self, r, l, e):
    self.run=r;
    self.lumi=l;
    self.evt=e;


def postProcessBaby( dir, dataset ) :

   oldFileName = dir + str("/") + dataset + str("/treeProducerSusyFullHad/treeProducerSusyFullHad_tree.root")
   newFileName = dir + str("/") + dataset + str("/") + dataset + str(".root")

   oldfile = TFile(oldFileName)
   oldtree = oldfile.Get("treeProducerSusyFullHad")

   #newtree = ROOT.TTree("mt2", "post processed baby tree for mt2 analysis")
   newfile = TFile("prova.root", "recreate")
   newtree = oldtree.CloneTree(0)
   newtree.SetName("mt2")

   evt_scale1fb = numpy.zeros(1, dtype=float)
   newtree.Branch("evt_scale1fb", evt_scale1fb, "evt_scale1fb/D");
   #newtree.Branch("evt_scale1fb", AddressOf(evt_scale1fb, 'evt_scale1fb/F'), "evt_scale1fb/F");
   #newtree.Branch("evt_xsec", AddressOf(xsec, "evt_xsec/F"), "evt_xsec/F");
   #newtree.Branch("evt_kfactor", AddressOf(kfactor, "evt_kfactor/F"), "evt_kfactor/F");
   #newtree.Branch("evt_filter", AddressOf(filter, "evt_filter/F"), "evt_filter/F");
   #newtree.Branch("evt_nEvts", AddressOf(events, "evt_nEvts/I"), "evt_nEvts/I");
   #newtree.Branch("evt_id", AddressOf(id, "evt_id/I"), "evt_id/I");


   evlist = set()

   for i in oldtree :

     ek = EventKey(i.run, i.lumi, i.evt)

     evt_scale1fb[0] = -13.

     if ek not in evlist :
       evlist.add(ek)
       newtree.Fill()

   newfile.Write()
   newfile.Close()

#   return
#
##//Get input tree
##TFile *oldfile = new TFile(inputFile.c_str());
##TTree *oldtree = (TTree*)oldfile->Get(treeName.c_str());
##Long64_t nentries = oldtree->GetEntries();
##cout << "In input tree, nentries = " << nentries << endl;
##int run,lumi,evt;
##oldtree->SetBranchAddress("run",&run);
##oldtree->SetBranchAddress("lumi",&lumi);
##oldtree->SetBranchAddress("evt",&evt);
##//Create a new file + a clone of old tree in new file
##TFile *newfile = new TFile(outputFile.c_str(),"recreate");
##TTree *newtree = oldtree->CloneTree(0);
##//Create set where we store list of event keys
##std::set<EventKey> previousEvents;
##for (Long64_t i=0;i<nentries; i++) {
##//for (Long64_t i=0;i<1000; i++) {
##oldtree->GetEntry(i);
##EventKey newEvent(run,lumi,evt);
##bool isDuplicate = !previousEvents.insert(newEvent).second;
##if(i%100000==0) {
##time_t t = time(0); // get time now
##tm * now = localtime( & t );
##cout << "Processing event: " << i << " at time "
##<< now->tm_hour << ":"
##<< now->tm_min << ":"
##<< now->tm_sec
##<< endl;
##}
##if(!isDuplicate) {
##if(fillNewTree) newtree->Fill();
##}else{
##cout << "Found duplicate! run,lumi,evt: "
##<< run << " , " << lumi << " , " << evt <<endl;
##}
##}
##//newtree->Print();
##newtree->AutoSave();
##delete oldfile;
##delete newfile;



def postProcessBabies( dir ) :

  for file in sorted(os.listdir(dir)):
      filepath = '/'.join( [dir, file] )
      if os.path.isdir(filepath):
          dataset = file
          if "_Chunk" in dataset: continue
          postProcessBaby( dir, dataset )
    



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
    badFiles = []
    for aline in checkOutput_lines:
      if aline.startswith("['") :
        thisBadFile = aline.split("['")[1].split("']")[0].split(dir)[1]
        while thisBadFile.startswith("/"):
          thisBadFile = thisBadFile.split("/")[1]
        badFiles.append(thisBadFile)


    haddChunks(dir, options.remove, options.clean, badFiles)

    postProcessBabies( dir )




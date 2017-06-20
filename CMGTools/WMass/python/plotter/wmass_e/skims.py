#!/usr/bin/env python
# e.g.: python wmass_e/skims.py wmass_e/mca-53X-wenu.txt wmass_e/wenu.txt /data1/emanuele/wmass/TREES_1LEP_53X_V2 /data1/emanuele/wmass/TREES_1LEP_53X_V2_WSKIM_V3 -f wmass_e/varsSkim_53X.txt
#       python wmass_e/skims.py wmass_e/mca-53X-zee.txt wmass_e/zee.txt /data1/emanuele/wmass/TREES_1LEP_53X_V2 /data1/emanuele/wmass/TREES_1LEP_53X_V2_ZEESKIM_V3 -f wmass_e/varsSkim_53X.txt
#       python wmass_e/skims.py wmass_e/mca-53X-wenu.txt wmass_e/qcd_e.txt /data1/emanuele/wmass/TREES_1LEP_53X_V2 /data1/emanuele/wmass/TREES_1LEP_53X_V2_QCDSKIM_V3 -f wmass_e/varsSkim_53X.txt

import os, subprocess

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] mc.txt cuts.txt treeDir outputDirSkims ")
    parser.add_option("-f", "--varfile",  dest="varfile", type="string", default=None, action="store",  help="File with the list of Branches to drop, as per TTree::SetBranchStatus")
    parser.add_option("--fo", "--friend-only",  dest="friendOnly", action="store_true", default=False,  help="Do not redo skim of the main trees, only of the friends")
    (options, args) = parser.parse_args() 

    mcargs = args[:2]
    treeDir = args[2]
    outputDirSkims = args[3]

    treeFDir = treeDir+"/friends"
    outputDirFSkims = outputDirSkims+"/friends"

    if not options.friendOnly:
       if not os.path.exists(outputDirSkims):
           os.makedirs(outputDirSkims)
           os.makedirs(outputDirFSkims)
       else:
           print "The skim output dir ",outputDirSkims," exists. Will remove it and substitute with new one. \nDo you agree?[y/N]\n"
           if raw_input()!='y':
               print 'Aborting'
               exit()
           os.system("rm -rf "+outputDirSkims)
           os.makedirs(outputDirSkims)
           os.makedirs(outputDirFSkims)
    else: print "Make only the friend trees in dir ",outputDirFSkims

    OPTS = ' -P '+treeDir+' --s2v -j 4 -F mjvars/t "'+treeFDir+'/evVarFriend_{cname}.root" '

    varsToKeep = []
    if options.varfile!=None:
        with open(options.varfile) as f:
            varsToKeep = f.read().splitlines()
        OPTS += "--drop '*' --keep "+" --keep ".join(varsToKeep)
    

    cmdSkim = "python skimTrees.py "+" ".join(mcargs)+" " + outputDirSkims + OPTS
    cmdFSkimEv = " python skimFTrees.py "+outputDirSkims+" "+treeFDir+" "+outputDirFSkims
    cmdFSkimSf = " python skimFTrees.py "+outputDirSkims+" "+treeFDir+" "+outputDirFSkims+' -f sfFriend -t "sf/t" '

    if not options.friendOnly:
        print "Now skimming the main trees, keeping the following vars:\n",varsToKeep
        print "This step may take time...\n"
        os.system(cmdSkim)
    print "Now skimming the event variables friend trees:\n"
    os.system(cmdFSkimEv)
    print "Now skimming the scale factors friend trees:\n"
    os.system(cmdFSkimSf)

    print "VERY DONE\n"



import urllib, urlparse, string, time, os, shutil

really_do_it=True

local_base_dir = "/afs/cern.ch/work/p/perrozzi/private/git/CMSSW_4_4_5/src/CMGTools/WMass/cfg"
# local_base_dir = "/tmp/perrozzi/"
# local_extension_dir = ["VVJets2/WZJetsTo2L2Q","VVJets2/ZZJetsTo2L2Q","VVJets2/ZZJetsTo4L"]
# local_extension_dir = ["DATA/data"] # MAINDIR/SUBDIR local
# local_extension_dir = ["VGamma/ZGToMuMuG","VGamma/ZGToTauTauG"]
# local_extension_dir = ["DATA/data_Run2011A_Nov8ReReco_v1","DATA/data_Run2011B_Nov19ReReco_v1"]
local_extension_dir = ["DYMM/DYJets"]

eos_base_dir = "/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2013_11_05/"
# eos_extension1_dir = ["VVJets","VVJets","VVJets"]
# eos_extension2_dir = ["WZJetsTo2L2Q","ZZJetsTo2L2Q","ZZJetsTo4L"]
eos_extension1_dir = [""] # MAINDIR IN EOS
eos_extension2_dir = ["DYJetsMM"] # SUBDIR IN EOS
# eos_extension1_dir = ["DATA","DATA"]
# eos_extension2_dir = ["RunA","RunB"]

# W_or_Z = ["W","Z"]
W_or_Z = ["Z"]

for j in range ( 0 , len(W_or_Z) ):
  for i in range ( 0 , len(local_extension_dir) ):
      
    # if not "batch_WJets" in local_extension_dir[i]: continue;
    # if not "batch_DATA" in local_extension_dir[i]: continue;

    W_or_Z_file_dir = W_or_Z[j]+"TreeProducer/"+W_or_Z[j]+"TreeProducer_tree.root"
    # command_string = '/afs/cern.ch/project/eos/installation/0.2.31/bin/eos.select cp '+local_base_dir+'/'+local_extension_dir[i]+'/'+W_or_Z_file_dir+' '+eos_base_dir+'/'+eos_extension_dir[i]+'/'
    command_string = 'cmsStage '+local_base_dir+'/'+local_extension_dir[i]+'/'+W_or_Z_file_dir+' '+eos_base_dir+'/'+eos_extension1_dir[i]+'/'+eos_extension2_dir[i]+'/'
    print command_string
    if(really_do_it):
      os.system("/afs/cern.ch/project/eos/installation/0.2.31/bin/eos.select mkdir "+eos_base_dir); 
      os.system("/afs/cern.ch/project/eos/installation/0.2.31/bin/eos.select mkdir "+eos_base_dir+'/'+eos_extension1_dir[i]+'/'); 
      os.system("/afs/cern.ch/project/eos/installation/0.2.31/bin/eos.select mkdir "+eos_base_dir+'/'+eos_extension1_dir[i]+'/'+eos_extension2_dir[i]+'/'); 
      os.system(command_string)

import os 
import sys
import shutil

from ROOT import gROOT

oldpwd = os.getcwd()

plotterdir = '/'.join([ os.environ['CMSSW_BASE'], 'src/CMGTools/H2TauTau/python/proto/plotter'] )
inclusive_macro = '/'.join([plotterdir,'plot_H2TauTauDataMC_TauMu_Inclusive.py'])
vbf_macro = '/'.join([plotterdir,'plot_H2TauTauDataMC_TauMu_VBF.py'])

cfg = 'tauMu_2011_cfg.py'
dir = 'Prod_Oct30'

cfg = '/'.join([oldpwd, cfg])
dir = '/'.join([oldpwd, dir])

dirs = [dir,
        '_'.join([dir,'Up']),
        '_'.join([dir,'Down'])
        ]

# dirs = [dir]

inclusive_cuts = {
    'Inclusive':'Xcat_IncX && mt<20',
    '0jet_low':'Xcat_IncX && Xcat_J0X && l1_pt<40 && mt<20',
    '0jet_high':'Xcat_IncX && Xcat_J0X && l1_pt>40 && mt<20',
    '1jet_low':'Xcat_IncX && Xcat_J1X && l1_pt<40 && mt<20',
    '1jet_high':'Xcat_IncX && Xcat_J1X && l1_pt>40 && mt<20',    
    }

vbf_cuts = {
    'vbf':'mt<20'
    }


def dataAndBackground(dir, cuts, macro):
    print os.getcwd()
    for cutname, cut in cuts.iteritems():
        os.mkdir(cutname)
        op = os.getcwd()
        os.chdir(cutname)
        cmd = "nohup python {macro} {dir} {cfg} -H svfitMass -E -C '{cut}' -b &".format(
            macro=macro,
            dir=dir,
            cfg=cfg,
            cut=cut
            )
        print cmd
        os.system(cmd)
        os.chdir(op)

remove_all = True

def chdir(dir):
    global oldpwd
    global remove_all
    oldpwd = os.getcwd()
    os.chdir(dir)
    dcdir = 'Datacards'
    force = False
    if os.path.isdir(dcdir):
        while 1:
            if not remove_all:
                answer = raw_input('/'.join([dir, dcdir]) + ' exists. Do you want to remove it? [y/n/a]')
                if answer=='y':
                    os.system('rm -r '+dcdir)
                    break
                elif answer=='a':
                    os.system('rm -r '+dcdir)
                    remove_all = True 
                if answer=='n':
                    sys.exit(1)
            else:
                os.system('rm -r '+dcdir)
                break
    os.mkdir(dcdir)
    os.chdir(dcdir)
    shutil.copyfile( cfg, '/'.join(['.', os.path.basename(cfg)]))

for dir in dirs:
    chdir(dir)
    dataAndBackground(dir, inclusive_cuts, inclusive_macro)
    # dataAndBackground(dir, vbf_cuts, vbf_macro)    
    os.chdir(oldpwd)

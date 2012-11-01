import os 
import sys
import shutil

from ROOT import gROOT

oldpwd = os.getcwd()

plotterdir = '/'.join([ os.environ['CMSSW_BASE'], 'src/CMGTools/H2TauTau/python/proto/plotter'] )
inclusive_macro = '/'.join([plotterdir,'plot_H2TauTauDataMC_TauMu_Inclusive.py'])
vbf_macro = '/'.join([plotterdir,'plot_H2TauTauDataMC_TauMu_VBF.py'])
mc_macro = '/'.join([plotterdir,'H2TauTauMC.py'])

cfg = 'tauMu_2011_cfg.py'
dir = 'Prod_Oct30'

cfg = '/'.join([oldpwd, cfg])
dir = '/'.join([oldpwd, dir])

dirs = [dir]

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

vbf_mc_cuts = {
    'vbf':'Xcat_IncX && Xcat_VBFX && mt<20 && diTau_charge==0'
    }

remove_all = False


def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc: # Python >2.5
        if exc.errno == errno.EEXIST:
            pass
        else: raise


def mkdir(dir):
    global remove_all
    adir = '/'.join([os.getcwd(), dir])
    if os.path.isdir(dir):
        while 1:
            if not remove_all:
                answer = raw_input(adir + ' exists. Do you want to remove it? [y/n/a]')
                if answer=='y':
                    os.system('rm -r '+dir)
                    break
                elif answer=='a':
                    os.system('rm -r '+dir)
                    remove_all = True 
                if answer=='n':
                    sys.exit(1)
            else:
                os.system('rm -r '+dir)
                break
    os.mkdir(dir)
    

def dataAndBackground(dir, cuts, macro):
    print os.getcwd()
    for cutname, cut in cuts.iteritems():
        mkdir(cutname)
        op = os.getcwd()
        os.chdir(cutname)
        # in batch mode, and don't plot the signal
        cmd = "nohup python {macro} {dir} {cfg} -g Dummy -H svfitMass -E -C '{cut}' -b &".format(
            macro=macro,
            dir=dir,
            cfg=cfg,
            cut=cut
            )
        print cmd
        os.system(cmd)
        os.chdir(op)



def mcTemplates(dir, cuts, refilter):
    print os.getcwd()
    macro = mc_macro
    for cutname, cut in cuts.iteritems():
        mkdir(cutname)
        op = os.getcwd()
        os.chdir(cutname)
        # in batch mode, and don't plot the signal
        cmd = "nohup python {macro} {dir} {cfg} -f {refilter} -H svfitMass -C '{cut}' -b &".format(
            macro=macro,
            dir=dir,
            cfg=cfg,
            cut=cut,
            refilter=refilter
            )
        print cmd
        os.system(cmd)
        os.chdir(op)
    

def chdir(dir, subdir):
    # oldpwd = os.getcwd()
    os.chdir(dir)
    dcdir = subdir
    if not os.path.isdir(dcdir):
        mkdir_p(dcdir)
    os.chdir(dcdir)
    shutil.copyfile( cfg, '/'.join(['.', os.path.basename(cfg)]))



for dir in dirs:
    
##     chdir(dir, 'Datacards/DataAndBackground')
##     #    dataAndBackground(dir, inclusive_cuts, inclusive_macro)
##     dataAndBackground(dir, vbf_cuts, vbf_macro)    
##     os.chdir(oldpwd)
    
    chdir(dir, 'Datacards/MC')
    mcTemplates(dir, inclusive_cuts, 'Higgs.*')
    mcTemplates(dir, vbf_mc_cuts, 'Higgs.*')
    os.chdir(oldpwd)

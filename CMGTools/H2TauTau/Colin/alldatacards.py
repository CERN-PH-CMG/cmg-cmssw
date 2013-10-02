import os 
import sys
import shutil

from ROOT import gROOT


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



def mcTemplates(dir, cuts, refilter, channel, prefix):
    print os.getcwd()
    macro = mc_macro
    for cutname, cut in cuts.iteritems():
        mkdir(cutname)
        op = os.getcwd()
        os.chdir(cutname)
        # in batch mode, and don't plot the signal
        print refilter
        cmd = "nohup python {macro} {dir} {cfg} -E -f '{refilter}' -c {channel} -p {prefix} -H svfitMass -C '{cut}' -b &".format(
            macro=macro,
            dir=dir,
            cfg=cfg,
            cut=cut,
            refilter=refilter,
            channel=channel,
            prefix=prefix
            )
        print cmd
        os.system(cmd)
        os.chdir(op)
    

def chdir(dir, subdir):
    # oldpwd = os.getcwd()
    # import pdb; pdb.set_trace()
    os.chdir(dir)
    dcdir = subdir
    if not os.path.isdir(dcdir):
        mkdir_p(dcdir)
    os.chdir(dcdir)
    shutil.copyfile( cfg, '/'.join(['.', os.path.basename(cfg)]))


if __name__ == '__main__':


    import os
    import sys
    from optparse import OptionParser

    parser = OptionParser()
    parser.usage = """
    %prog <flat_tree_dir> <channel>
    """    
    parser.add_option("-f", "--filter", 
                      dest="filter", 
                      help="Regexp filters to select components for the MC-only templates, separated by semicolons, e.g. Higgs;ZTT",
                      default='Higgs.*;Ztt.*')
    parser.add_option("-d", "--disabledata", 
                      dest="disabledata",
                      action='store_true',
                      help="Disable datacard production for the data and background estimation",
                      default=False)
    parser.add_option("-m", "--disablemc", 
                      dest="disablemc",
                      action='store_false',
                      help="Disable datacard production for MC",
                      default=False)

    
    (options,args) = parser.parse_args()
    
    if len(args)!=2:
        parser.print_usage()
        print 'provide exactly 2 arguments.'
        sys.exit(1) 

    # dir = 'Prod_MC_AndrewTrigger_Nov2'
    dir = args[0]
    channel = args[1]

    oldpwd = os.getcwd()

    plotterdir = '/'.join([ os.environ['CMSSW_BASE'], 'src/CMGTools/H2TauTau/python/proto/plotter'] )
    mc_macro = '/'.join([plotterdir,'plot_H2TauTauMC.py'])

    inclusive_macro = None
    vbf_macro = None
    inclusive_cuts = None
    vbf_cuts = {
        'vbf':'mt<20'
        }
    vbf_mc_cuts = {
        'vbf':'Xcat_IncX && Xcat_VBFX && mt<20 && diTau_charge==0'
        }
    
    if channel=='TauEle':
        inclusive_macro = '/'.join([plotterdir,'plot_H2TauTauDataMC_TauEle_Inclusive_Colin.py'])
        vbf_macro = '/'.join([plotterdir,'plot_H2TauTauDataMC_TauEle_VBF_Colin.py'])
        cfg = 'tauEle_2011_cfg.py'
        inclusive_cuts = {
            'inclusive':'Xcat_IncX && mt<20',
            '0jet_low':'Xcat_IncX && Xcat_J0X && l1_pt<40 && mt<20',
            '0jet_high':'Xcat_IncX && Xcat_J0X && l1_pt>40 && mt<20',
            'boost_low':'Xcat_IncX && Xcat_J1X && l1_pt<40 && mt<20 && met>30',
            'boost_high':'Xcat_IncX && Xcat_J1X && l1_pt>40 && mt<20 && met>30',    
            }
    elif channel=='TauMu':
        inclusive_macro = '/'.join([plotterdir,'plot_H2TauTauDataMC_TauMu_Inclusive.py'])
        vbf_macro = '/'.join([plotterdir,'plot_H2TauTauDataMC_TauMu_VBF.py'])
        cfg = 'tauMu_2011_cfg.py'
        inclusive_cuts = {
            'inclusive':'Xcat_IncX && mt<20',
            '0jet_low':'Xcat_IncX && Xcat_J0X && l1_pt<40 && mt<20',
            '0jet_high':'Xcat_IncX && Xcat_J0X && l1_pt>40 && mt<20',
            'boost_low':'Xcat_IncX && Xcat_J1X && l1_pt<40 && mt<20',
            'boost_high':'Xcat_IncX && Xcat_J1X && l1_pt>40 && mt<20',    
            }
    else:
        print 'channel not recognized:', channel
        
    cfg = '/'.join([oldpwd, cfg])
    dir = '/'.join([oldpwd, dir])

    dirs = [dir]

    remove_all = False

    for dir in dirs:
        print dir
        
        if not options.disabledata:
            chdir(dir, 'Datacards/DataAndBackground')
            dataAndBackground(dir, inclusive_cuts, inclusive_macro)
            dataAndBackground(dir, vbf_cuts, vbf_macro)    
            os.chdir(oldpwd)

        if not options.disablemc:
            chdir(dir, 'Datacards/MC')
            mcTemplates(dir, inclusive_cuts, options.filter, channel, prefix='MC')
            mcTemplates(dir, vbf_mc_cuts, options.filter, channel, prefix='MC')
            os.chdir(oldpwd)

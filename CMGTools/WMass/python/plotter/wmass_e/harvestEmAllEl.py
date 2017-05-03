import CombineHarvester.CombineTools.ch as ch
import CombineHarvester.CombinePdfs.morphing as morphing
import ROOT
import glob, datetime, os

def harvestEm(subdir, mwrange, charge='both'):
    cmb = ch.CombineHarvester()
    
    # Read all the cards.
    # CH stores metadata about each object (Observation, Process, Systematic),
    # this is extracted from the card names with some regex
    # old configuration for card in glob.glob(subdir+'/dc_mt_mass*.txt'):
    for card in glob.glob(subdir+'/wenu_mass*.txt'): #.format(ch='' if charge == 'both' else 'plus*' if charge == 'plus' else 'minus*')):
        print "card is ",card
        cmb.QuickParseDatacard(card, """wenu_mass(?<MASS>\d+)_$CHANNEL.txt""")
    
    # Need a unqiue bin name for each plus/minus,pt and eta combination
    # We extracted this part of the datacard name into the channel variable above,
    # so can just copy it and override the bin "1" that was in all the cards
    cmb.ForEachObj(lambda obj: obj.set_bin(obj.channel()))
    
    # The process name also needs to be common between bins
    cmb.ForEachProc(lambda obj: obj.set_process('signal'))
    cmb.ForEachSyst(lambda obj: obj.set_process('signal'))
    
    # We'll have three copies of the observation, one for each mass point.
    # Filter all but one copy.
    cmb.FilterObs(lambda obj: obj.mass() != '-1')
    
    # Create workspace to hold the morphing pdfs and the mass
    w = ROOT.RooWorkspace('morph', 'morph')
    mass = w.factory('mw[{mwrange}]'.format(mwrange=mwrange))
    
    # BuildRooMorphing will dump a load of debug plots here
    debug = ROOT.TFile(subdir+'/debug.root', 'RECREATE')
    
    # Run for each bin,process combination
    for b in cmb.bin_set():
        for p in cmb.cp().bin([b]).process_set():
            morphing.BuildRooMorphing(w, cmb, b, p, mass, verbose=True, file=debug)
    
    # Just to be safe
    mass.setConstant(True)
    
    # Now the workspace is copied into the CH instance and the pdfs attached to the processes
    # (this relies on us knowing that BuildRooMorphing will name the pdfs in a particular way)
    cmb.AddWorkspace(w, True)
    cmb.cp().process(['signal']).ExtractPdfs(cmb, 'morph', '$BIN_$PROCESS_morph', '')
    
    # Adjust the rateParams a bit - we currently have three for each bin (one for each mass),
    # but we only want one. Easiest to drop the existing ones completely and create new ones
    #cmb.syst_type(['rateParam'], False)
    #cmb.cp().process(['signal']).AddSyst(cmb, 'norm_$BIN', 'rateParam', ch.SystMap()(1.00))
    
    # Have to set the range by hand
    #for sys in cmb.cp().syst_type(['rateParam']).syst_name_set():
    #    cmb.GetParameter(sys).set_range(0.5, 1.5)
    
    # Print the contents of the model
    cmb.PrintAll()
    
    # Write out the cards, one per bin
    writer = ch.CardWriter('$TAG/$BIN.txt', '$TAG/shapes.root')
    writer.SetVerbosity(1)
    writer.WriteCards(subdir+'/w_cards_morphed_{charge}'.format(charge=charge), cmb)

date = datetime.date.today().isoformat()
date+='_charges'

#card_dir = 'inputForCombine/unmorph/2016-10-20/'
#card_dir = 'inputForCombine/unmorph/2016-10-24/'
#card_dir = 'inputForCombine/unmorph/2016-10-25/'
#card_dir = 'inputForCombine/unmorph/2016-10-27/'
#card_dir = 'inputForCombine/unmorph/2016-10-28/'
#card_dir = 'inputForCombine/unmorph/2016-11-01/'
#card_dir = 'inputForCombine/unmorph/2016-11-03/'
#buggy card_dir = 'inputForCombine/unmorph/2016-11-04/'
#card_dir = 'inputForCombine/unmorph/2016-11-07/'
#card_dir = 'inputForCombine/unmorph/2016-11-25/'
card_dir = 'cards/test1/'
subdirs = [x[0] for x in os.walk(card_dir)]

mwrange = '19,20,21'
npoints = 3
central = 20

runHarvest = True
runBatch   = False
justHadd   = False

charges = ['neg', 'pos']

for charge in charges:

    for isub, subdir in enumerate(subdirs):
        if subdir == subdirs[0]: continue
        if 'w_cards_morphed' in subdir: continue
        name = subdir.split('/')[-1]
        if not 'eta_' in name: continue
    
        if justHadd:
            print 'hadding {name}'.format(name=name)
            print '-------------------------------------------------------'
            os.system('hadd higgsCombine{date}_{name}_noPDFUncertainty.POINTSFULL.MultiDimFit.mH120.root higgsCombine{date}_{name}_noPDFUncertainty.POINTS.*.MultiDimFit.mH120.root'.format(date=date, name=name) )
            os.system('hadd higgsCombine{date}_{name}.POINTSFULL.MultiDimFit.mH120.root higgsCombine{date}_{name}.POINTS.*.MultiDimFit.mH120.root'.format(date=date, name=name) )
            continue
    
        print '--------------------------------------------------------------------'
        print '- running for {mode} -----------------------------------------------'.format(mode=name)
        print '- in subdirectory {subdir} -----------------------------------------'.format(subdir=subdir)
        print '--------------------------------------------------------------------'
        #if name == 'full_3d': continue
    
        if runHarvest: 
            ## run the combine harvester which combines all the datacards etc.
            harvestEm(subdir, mwrange, charge)
            ## running combineCards to make the combined plus+minus datacard

            dcs = '{subdir}/w_cards_morphed_{charge}/*.txt '.format(subdir=subdir,charge=charge)
            target_dc = '{subdir}/wenu_cards_morphed_{charge}/morphed_datacard.txt'.format(subdir=subdir,charge=charge)
            if os.path.isfile(target_dc):
                print 'removing existing combined datacard first!'
                os.system('rm {dc}'.format(dc=target_dc) )

            print 'running combineCards.py'
            combineCardsCmd = 'combineCards.py {dcs} >& {target_dc}'.format(subdir=subdir, dcs=dcs, target_dc=target_dc)
            print combineCardsCmd
            ## run combineCards and make the workspace
            os.system(combineCardsCmd )
            print 'running text2workspace'
            t2wCmd = 'text2workspace.py --X-allow-no-background {target_dc} '.format(subdir=subdir, target_dc=target_dc)
            print t2wCmd
            os.system(t2wCmd)
            target_ws = target_dc.replace('txt','root')
    
        ## constructing the command
        combine_base  = 'combine -t -1 -M MultiDimFit --setPhysicsModelParameters mw={central},r=1 --setPhysicsModelParameterRanges mw={mwrange} '.format(central=central,mwrange=mwrange)
        combine_base += ' --redefineSignalPOIs=mw --algo grid --points {npoints} {target_ws} '.format(npoints=npoints, subdir=subdir, target_ws=target_ws)
    
        saveNuisances = ''
        saveNuisances += ' --saveSpecifiedNuis {vs} '.format(vs=','.join('v'+str(i) for i in range(1,27)))
    
        run_combine_pdfUnc = combine_base + ' -n {date}_{name} {sn} '.format(date=date,name=name,sn=saveNuisances) 
        run_combine_noPdf  = combine_base + ' -n {date}_{name}_noPDFUncertainty --freezeNuisanceGroups pdfUncertainties '.format(date=date,name=name)
    
        if runBatch:
            run_combine_pdfUnc += ' --job-mode lxbatch --split-points 10 --sub-opts="-q 8nh" --task-name {name}                  '.format(name=name)
            run_combine_noPdf  += ' --job-mode lxbatch --split-points 10 --sub-opts="-q 8nh" --task-name {name}_noPDFUncertainty '.format(name=name)
            run_combine_pdfUnc  = 'combineTool.py ' + ' '.join(run_combine_pdfUnc.split()[1:])
            run_combine_noPdf   = 'combineTool.py ' + ' '.join(run_combine_noPdf .split()[1:])
    
    
        ## running combine once with the systematics and once without
        print '-- running combine command ------------------------------'
        print '---     with uncertainties: -----------------------------'
        print run_combine_pdfUnc
    
        os.system(run_combine_pdfUnc)
    
        # print '---     without uncertainties: --------------------------'
        # print run_combine_noPdf
        # os.system(run_combine_noPdf )
    
        impactBase = 'combineTool.py -M Impacts -n {date}_{name} -d {target_ws} -m {mass} '.format(subdir=subdir,mass=name[-1],date=date,name=name, target_ws=target_ws)
        impactBase += ' --setPhysicsModelParameters mw=0,r=1  --redefineSignalPOIs=mw --setPhysicsModelParameterRanges mw={mwrange} -t -1 '.format(mwrange=mwrange)
        impactInitial = impactBase+'  --robustFit 1 --doInitialFit '
        impactFits    = impactBase+'  --robustFit 1 --doFits '
        impactJSON    = impactBase+'  -o impacts_{name}_{charge}.json '.format(name=name, charge=charge)
        impactPlot    = 'plotImpacts.py -i impacts_{name}_{charge}.json -o impacts_{name}_{charge} --transparent'.format(name=name,charge=charge)
    
        os.system(impactInitial)
        os.system(impactFits   )
        os.system(impactJSON   )
        os.system(impactPlot   )

## combineTool.py -M Impacts -d morphed_datacard_plusminus.root -m 999 --setPhysicsModelParameters mw=101,r=1  --redefineSignalPOIs=mw  --setPhysicsModelParameterRanges mw=91,111 -t -1 --robustFit 1 --doInitialFit
## combineTool.py -M Impacts -d morphed_datacard_plusminus.root -m 999 --setPhysicsModelParameters mw=101,r=1  --redefineSignalPOIs=mw  --setPhysicsModelParameterRanges mw=91,111 -t -1 --robustFit 1 --doFits
## combineTool.py -M Impacts -d morphed_datacard_plusminus.root -m 999 --setPhysicsModelParameters mw=101,r=1  --redefineSignalPOIs=mw  --setPhysicsModelParameterRanges mw=91,111 -t -1 -o impacts.json
## plotImpacts.py -i impacts.json -o impacts



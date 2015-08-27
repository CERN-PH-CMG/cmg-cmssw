##Basic usage instructions

All the work and flag-setting is done in the `launch_analysis.py` file
(run with `./launch_analysis.py` in the `analysis` directory)


####Part 0: set environment variables

Inside the `CMSSW_5_3_22` area, run

    cmsenv  
    source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.24/x86_64-slc6-gcc47-opt/root/bin/thisroot.sh
Needed just once.


####Part I: building templates and statistical error


1. `Zanalysis` builds chunked rootfiles  
Controlled by the `runZanalysis` flag.  
Which samples are analyzed is controlled by the `resubmit_sample` variable
(accepts comma separated string values, all in a single string)  
The `WMassNSteps` variable (default: `5`) controls the number of mass values to produce:  
`N = 2 * WMassNSteps + 1`  
To send the jobs in batch set the parallelization variables block to:

        useBatch = 1;
        batchQueue = "1nh";
In this case you'll probably need to run the analysis multiple times to resubmit the jobs that have failed,
simply rerun `launch_analysis.py` with the same settings


2. Merge_MC takes all the chunks and merges into complete histograms.  
It also assembles datalike distributions with signal+background, if they have been computed before.
It's controlled by the `mergeSigEWKbkg` flag


3. Building datacards to feed the fitter  
Controlled with the `runPrepareDataCardsFast` flag


4. Performing fits  
Controlled with the `runClosureTestLikeLihoodRatio` flag
It can be run as a batch job (using the same block above) or not.  
It the latter case it also does the next step


5. Merge fit results  
Merges the single results of the fit into the "parabolas" plots.  
If needed can be controlled with the `mergeResults` flag  
The rootfile can be found at `JobsOutput/long_name/DataCards/likelihood_results_Wlike.root`


####Part II: systematics

We currently put the systematic corrections only on the datalike distribution,
instead of on all the templates.  
The resulting number is close enough, and speeds up the computations considerably

The `WMassNSteps` is automatically set to `0`, as you only need one mass value (the "true" one)


To build a datalike distribution with a given systematic you need to change the variable that controls that systematic:

Lepton calibration (non-closure)   -> `MuonCorrGlobalScaleNsigma = 0; # vary global muon scale    (0=no)`  
Lepton calibration (stat. uncert.) -> `MuonCorrKalmanNvarsNsigma = 0; # vary each muon fit eigenv (0=no)`  
Recoil calibration (stat. uncert.) -> `RecoilCorrVarDiagoParU1orU2fromDATAorMC = 0;
    # SYST VARIATIONS: 0=NONE, 1= U1 DATA p1, 2= U1 DATA p2, 3= U2 DATA, 4= U1 MC p1, 5= U1 MC p1, 6= U1 MC p1`  
QED uncertainties (theor. uncert.) -> `syst_ewk_Alcaraz = "-1"; # -1=none, 0=fixed POWHEG QCD+EWK NLO`


Done that, rerun the 1. Zanalysis and 2. Merge_MC with the new settings

Preparing the datacards is different from part I, as you need to fit the templates against this datalike.

To do so do step 3. setting the `DataCards_templateFromFolder` variable to the name of the folder with the **templates**
you previously computed (the full pathname).
Leaving it empty makes it use the current working set.

Then run step 4. & 5. and enjoy the new distribution.


####Part III: recoil non-closure

Currently the recoil non-closure is evaluated comparing Powheg morphed to Madgraph against Madgraph without corrections (to avoid a series of issues comparing to DATA would have).

To build the uncorrected Madgraph distributions set the `useRecoilCorr` variable to `0`
(remember to set  `WMassNSteps` at `0` to save computations).  
To build the Powheg morphed to Madgraph distributions set the `correctToMadgraph` to `1`.  
If you need the fit stat. error band you'll also need to build the varied distributions, as explained in Part II, with the  `RecoilCorrVarDiagoParU1orU2fromDATAorMC` flag.

After the merge you should have 8 rootfiles, copy them somewhere and rename them:

* Madgraph uncorrected -> `madnocorr.root`
* Powhed to Madgraph central -> `0.root`
* First block of variated Pow2Mad -> `1.root`
* ...
* Last block of variated Pow2Mad -> `6.root`

Now they are ready to be passed under the `closure_recoil_plots{,2d}.C` macros, in `utils`.

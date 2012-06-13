import FWCore.ParameterSet.Config as cms

layout = cms.PSet(
    ## dataset
    #dataset = cms.string(" 2012, #sqrt{s} = 8 TeV, H #rightarrow #tau #tau, L = 1.6 fb^{-1}"),
    dataset = cms.string(" 2011, #sqrt{s} = 7 TeV, H #rightarrow #tau #tau, L = 4.9 fb^{-1}"),
    #dataset = cms.string(" 2011/2012, #sqrt{s} = 7-8 TeV, H #rightarrow #tau #tau, L = 6.5 fb^{-1}"),
    ## x-axis title
    xaxis = cms.string("m_{H} [GeV]"),
    ## x-axis title
    yaxis = cms.string("95% CL limit on #sigma/#sigma_{SM}"),
    ## plot expected only
    expectedOnly = cms.bool(True),
    ## is this mssm?
    mssm = cms.bool(False),
    ## print to png
    png  = cms.bool(True),
    ## print to pdf
    pdf  = cms.bool(False),
    ## print to txt
    txt  = cms.bool(True),
    ## print to root
    root = cms.bool(True),
    ## min for plotting
    min = cms.double(0.),
    ## max for plotting
    max = cms.double(6), ##12
    ## min for plotting
    log = cms.int32(0),
    ## define verbosity level
    verbosity   = cms.uint32(0),
    ## define output label
    outputLabel = cms.string("sm"), 
    ## define masspoints for limit plot
    masspoints = cms.vdouble(
    110.
   #,111.
   #,112.
   #,113.
   #,114.
    ,115.
   #,116.
   #,117.
   #,118.
   #,119.
   ,120.
   #,121.
   #,122.
   #,123.
   #,124.
   ,125.
   #,126.
   #,127.
   #,128.
   #,129.
   ,130.
   #,131.
   #,132.
   #,133.
   #,134.
   ,135.
   #,136.
   #,137.
   #,138.
   #,139.
   ,140.
   #,141.
   #,142.
   #,143.
   #,144.
   ,145.
  ),
)

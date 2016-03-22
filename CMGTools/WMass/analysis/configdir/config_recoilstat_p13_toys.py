### Configuration variables for launch_analysis

outfolder_prefix = "PREFIX"

### Muon trigger efficiency
useEffSF = 2  # 0=no, 1=MuonPOG, 2=Heiner all, 3=Heiner no tight, 4=Heiner no iso, 5=Heiner no tight subleading mu, 6=Heiner no hlt
              # 13=Heiner tight 1%, 14=Heiner iso 1%, 15=Heiner tight subleading mu 1%, 16=Heiner hlt 1%
efficiency_toys = 0  # 0=No, >1=Yes

### MUON
MuonCorrGlobalScaleNsigma = 0  # vary global muon scale    (0=no)
MuonCorrKalmanNvarsNsigma = 0  # vary each muon fit eigenv (0=no)

WlikeCharge = 1  # Charge of the Wlike (+1,-1)

### RECOIL
useRecoilCorr = 2  # 0=none, 1=yes, 2=PDFw3gaus, 3=RooKeys
RecoilCorrVarDiagoParU1orU2fromDATAorMC = 13  # SYST VARIATIONS: 0=NONE, RAPBIN 1 (1= U1 DATA p1, 2= U1 DATA p2, 3= U2 DATA, 4= U1 MC p1, 5= U1 MC p2, 6= U2 MC) RAPBIN 2 (7= U1 DATA p1, 8= U1 DATA p2, 9= U2 DATA, 10= U1 MC p1, 11= U1 MC p2, 12= U2 MC) -- 13= TOYS
RecoilCorrVarDiagoParSigmas = 1  # Number of sigmas for recoil syst

### Working setup
useBatch = 0
batchQueue = "1nh"

WMassNSteps = 5    # 60 -- N of mass steps above and below the central
etaMaxMuons = 0.9  # 0.6, 0.8, 1.2, 1.6, 2.1

### ANALYSIS
runZanalysis = 1
controlplots = 0

mergeSigEWKbkg = 0

## PERFORM W or Z MASS FIT
runPrepareDataCardsFast = 0  # TEMPLATES ARE IN THE TEMPLATE FOLDER, PSEUDO-DATA IN THE CURRENT FOLDER
DataCards_templateFromFolder=""  # evaluate systematics wrt folder (or leave it empty) -- full template folder

## NEW FIT
runClosureTestLikeLihoodRatio = 0  # 1: also executes merge if not using batch jobs
mergeResults = 0
blind_offset_string = ""  # "" -> 0

# DMPD
DM analysis framework of the CMS PD group

## 1. CMSSW Release

CMSSW_7_4_7

## 2. Setup of the CMG-Heppy framework

Carefully follow **all** the instructions of the [CMG Twiki Page](https://twiki.cern.ch/twiki/bin/viewauth/CMS/CMGToolsReleasesExperimental#Git_MiniAOD_release_for_Summ_AN1)
 
## 3. Manual changes to Heppy (for Syncronization)

### TauAnalyzer.py
[PhysicsTools/Heppy/python/analyzers/objects/TauAnalyzer.py]

#### L89 REPLACE 
```python
tau.tauID(self.cfg_ana.inclusive_tauID)
```
WITH 
```python
tau.tauID(self.cfg_ana.inclusive_tauID) < self.cfg_ana.inclusive_tauIDnHits
```

#### L108 REPLACE 
```python
tau.tauID(self.cfg_ana.loose_tauID)
```
WITH 
```python
tau.tauID(self.cfg_ana.loose_tauID) < self.cfg_ana.loose_tauIDnHits
```

#### L145-170 ADD
```python
inclusive_tauIDnHits = 0,
loose_tauIDnHits = 0,
```
### PhotonAnalyzer.py
[PhysicsTools/Heppy/python/analyzers/objects/PhotonAnalyzer.py]

#### L76-82 REPLACE WITH
```python
if   abs(gamma.eta()) < 1.0:   gamma.EffectiveArea03 = [ 0.0234, 0.0053, 0.0896 ]
elif abs(gamma.eta()) < 1.479: gamma.EffectiveArea03 = [ 0.0189, 0.0103, 0.0762 ]
elif abs(gamma.eta()) < 2.0:   gamma.EffectiveArea03 = [ 0.0171, 0.0057, 0.0383 ]
elif abs(gamma.eta()) < 2.2:   gamma.EffectiveArea03 = [ 0.0129, 0.0070, 0.0534 ]
elif abs(gamma.eta()) < 2.3:   gamma.EffectiveArea03 = [ 0.0110, 0.0152, 0.0846 ]
elif abs(gamma.eta()) < 2.4:   gamma.EffectiveArea03 = [ 0.0074, 0.0232, 0.1032 ]
else:                          gamma.EffectiveArea03 = [ 0.0035, 0.1709, 0.1598 ]
```

### JetAnalyzer.py
[PhysicsTools/Heppy/python/analyzers/objects/JetAnalyzer.py]

#### L123-124 MODIFY
```python
if self.cfg_ana.do_mc_match:
  self.matchJets(event, allJets)
```
WITH
```python
self.matchJets(event, allJets)
```
#### L236-237 MODIFY
```python
if self.cfg_ana.do_mc_match:
  self.jetFlavour(event)
```
WITH
```python
self.jetFlavour(event)
```

#### L260-264 MODIFY
```python
if self.cfg_ana.do_mc_match:
  setattr(event,"bqObjects"              +self.cfg_ana.collectionPostFix, self.bqObjects              )
  setattr(event,"cqObjects"              +self.cfg_ana.collectionPostFix, self.cqObjects              )
  setattr(event,"partons"                +self.cfg_ana.collectionPostFix, self.partons                )
  setattr(event,"heaviestQCDFlavour"     +self.cfg_ana.collectionPostFix, self.heaviestQCDFlavour     )
```
WITH
```python
setattr(event,"bqObjects"              +self.cfg_ana.collectionPostFix, self.bqObjects              )
setattr(event,"cqObjects"              +self.cfg_ana.collectionPostFix, self.cqObjects              )
setattr(event,"partons"                +self.cfg_ana.collectionPostFix, self.partons                )
setattr(event,"heaviestQCDFlavour"     +self.cfg_ana.collectionPostFix, self.heaviestQCDFlavour     )
```

### Photon.py
[PhysicsTools/Heppy/python/physicsobjects/Photon.py]

#### FIND AND REPLACE
```python
conversionVeto": [True,True]
```
WITH
```python
conversionVeto": [False,False]
```

## 4. Manual changes to Heppy (to run on LSF@LNL)

### heppy_batch.py
[PhysicsTools/HeppyCore/scripts/heppy_batch.py]

#### L17 MODIFY
```python
#BSUB -q local
```
WITH
```python
#BSUB -q local-cms-short
```

### batchmanager.py
[PhysicsTools/HeppyCore/python/utils/batchmanager.py]

#### L12 COMMENT
```python
import eostools as castortools
```

#### L250-264 REPLACE
```python
hostName = os.environ['HOSTNAME']

onLxplus = hostName.startswith('lxplus')
onPSI    = hostName.startswith('t3ui')
onNAF =  hostName.startswith('naf')

batchCmd = batch.split()[0]

if batchCmd == 'bsub':
    if not onLxplus:
        err = 'Cannot run %s on %s' % (batchCmd, hostName)
        raise ValueError( err )
    else:
        print 'running on LSF : %s from %s' % (batchCmd, hostName)
        return 'LXPLUS'
```
WITH
```python
hostName = os.environ['HOSTNAME']

onLxplus = hostName.startswith('lxplus')
onPSI    = hostName.startswith('t3ui'  )
onPADOVA = ( hostName.startswith('t2-ui') and re.match('.*pd.infn.*',hostName) ) or ( hostName.startswith('t2-cld') and re.match('.*lnl.infn.*',hostName) )

batchCmd = batch.split()[0]

if batchCmd == 'bsub':
    if not (onLxplus or onPADOVA) :
        err = 'Cannot run %s on %s' % (batchCmd, hostName)
        raise ValueError( err )
    elif onPADOVA:
        print 'running on LSF padova: %s from %s' % (batchCmd, hostName)
        return 'PADOVA'
    else:
        print 'running on LSF lxplus: %s from %s' % (batchCmd, hostName)
        return 'LXPLUS'
```

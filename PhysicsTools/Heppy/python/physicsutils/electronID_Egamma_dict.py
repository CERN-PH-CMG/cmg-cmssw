# Used to translate miniAOD names to FWLite

wps_dict = {
  "cutBasedElectronID-Fall17-94X-V2-veto"    : ("Fall1794XV2", "veto"),
  "cutBasedElectronID-Fall17-94X-V2-loose"   : ("Fall1794XV2", "loose"),
  "cutBasedElectronID-Fall17-94X-V2-medium"  : ("Fall1794XV2", "medium"),
  "cutBasedElectronID-Fall17-94X-V2-tight"   : ("Fall1794XV2", "tight"),
  "cutBasedElectronID-Fall17-94X-V1-veto"    : ("Fall1794XV1", "veto"),
  "cutBasedElectronID-Fall17-94X-V1-loose"   : ("Fall1794XV1", "loose"),
  "cutBasedElectronID-Fall17-94X-V1-medium"  : ("Fall1794XV1", "medium"),
  "cutBasedElectronID-Fall17-94X-V1-tight"   : ("Fall1794XV1", "tight"),
  "mvaEleID-Fall17-noIso-V2-Loose"           : ("Fall17NoIsoV2", "wpLoose"), # kept alternate name for compatibility with H->tau tau code
  "mvaEleID-Fall17-noIso-V2-wpLoose"         : ("Fall17NoIsoV2", "wpLoose"), # official E/gamma name is wpLoose
  "mvaEleID-Fall17-noIso-V2-wp90"            : ("Fall17NoIsoV2", "wp90"),
  "mvaEleID-Fall17-noIso-V2-wp80"            : ("Fall17NoIsoV2", "wp80"),
  "mvaEleID-Fall17-iso-V2-Loose"             : ("Fall17IsoV2", "wpLoose"),  # kept alternate name for compatibility with H->tau tau code
  "mvaEleID-Fall17-iso-V2-wpLoose"           : ("Fall17IsoV2", "wpLoose"),  # official E/gamma name is wpLoose
  "mvaEleID-Fall17-iso-V2-wp90"              : ("Fall17IsoV2", "wp90"),
  "mvaEleID-Fall17-iso-V2-wp80"              : ("Fall17IsoV2", "wp80"),
  "mvaEleID-Fall17-noIso-V1-wpLoose"         : ("Fall17NoIsoV1", "wpLoose"),
  "mvaEleID-Fall17-noIso-V1-wp90"            : ("Fall17NoIsoV1", "wp90"),
  "mvaEleID-Fall17-noIso-V1-wp80"            : ("Fall17NoIsoV1", "wp80"),
  "mvaEleID-Fall17-iso-V1-wpLoose"           : ("Fall17IsoV1", "wpLoose"),
  "mvaEleID-Fall17-iso-V1-wp90"              : ("Fall17IsoV1", "wp90"),
  "mvaEleID-Fall17-iso-V1-wp80"              : ("Fall17IsoV1", "wp80"),
  "mvaEleID-Spring16-HZZ-V1-wpLoose"         : ("Spring16HZZV1", "wpLoose"),
  "mvaEleID-Spring16-GeneralPurpose-V1-wp90" : ("Spring16GPV1" , "wp90"),
  "mvaEleID-Spring16-GeneralPurpose-V1-wp80" : ("Spring16GPV1" , "wp80"),
  "mvaEleID-Spring15-25ns-nonTrig-V1-wpLoose": ("Spring15NonTrig25nsV1", "wpLoose"),
  "mvaEleID-Spring15-25ns-nonTrig-V1-wp90"   : ("Spring15NonTrig25nsV1", "wp90"),
  "mvaEleID-Spring15-25ns-nonTrig-V1-wp80"   : ("Spring15NonTrig25nsV1", "wp80"),
}

methods_dict = {
  "mvaEleID-Fall17-noIso-V1"           : "Fall17NoIsoV1",
  "mvaEleID-Fall17-iso-V1"             : "Fall17IsoV1",
  "mvaEleID-Fall17-noIso-V2"           : "Fall17NoIsoV2",
  "mvaEleID-Fall17-iso-V2"             : "Fall17IsoV2",
  "mvaEleID-Spring16-HZZ-V1"           : "Spring16HZZV1",
  "mvaEleID-Spring16-GeneralPurpose-V1": "Spring16GPV1",
  "mvaEleID-Spring15-25ns-nonTrig-V1"  : "Spring15NonTrig25nsV1",
}

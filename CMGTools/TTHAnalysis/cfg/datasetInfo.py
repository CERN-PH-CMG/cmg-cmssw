#!/bin/env python



class Dataset:
  def __init__(self, name, id, xsect, kfact=1., filt=1.):
    self.name=name;
    self.id=id;
    self.xsection=xsect;
    self.kfactor=kfact;
    self.filter=filt;

  def scale1fb(self, events):
    if self.id>100: # mc
      return self.xsection*self.kfactor*1000.*self.filter/events;
    else: # data
      return 1.



class DatasetDict(dict):

    def __keytransform__(self, key):
        return key

    def __getitem__(self, key):
        return dict.__getitem__(self, self.__keytransform__(key))

    def __setitem__(self, key, value):
        return dict.__setitem__(self, self.__keytransform__(key), value)

    def addDataset( self, d ):
        dict.__setitem__(self, self.__keytransform__(d.id),   d)
        dict.__setitem__(self, self.__keytransform__(d.name), d)

    def __init__(self, *args, **kwargs):
        self.update(*args, **kwargs)
        #                         name                          id,      xsection [pb],    kfactor,    filter
        self.addDataset( Dataset("HT_Run2015A"               ,    1,                1.,         1.,        1.) )
        self.addDataset( Dataset("Muon_Run2015A"             ,    3,                1.,         1.,        1.) )

        self.addDataset( Dataset("QCD_Pt-5to10"              ,  102,  80710000000.0000,         1.,        1.) )
        self.addDataset( Dataset("QCD_Pt-10to15"             ,  103,   7528000000.0000,         1.,        1.) )
        self.addDataset( Dataset("QCD_Pt-15to30"             ,  104,   2237000000.0000,         1.,        1.) )
        self.addDataset( Dataset("QCD_Pt-30to50"             ,  105,    161500000.0000,         1.,        1.) )
        self.addDataset( Dataset("QCD_Pt-50to80"             ,  106,     22110000.0000,         1.,        1.) )
        self.addDataset( Dataset("QCD_Pt-80to120"            ,  107,      3000114.3000,         1.,        1.) )
        self.addDataset( Dataset("QCD_Pt-120to170"           ,  108,       493200.0000,         1.,        1.) )
        self.addDataset( Dataset("QCD_Pt-170to300"           ,  109,       120300.0000,         1.,        1.) )
        self.addDataset( Dataset("QCD_Pt-300to470"           ,  110,         7475.0000,         1.,        1.) )
        self.addDataset( Dataset("QCD_Pt-470to600"           ,  111,          587.1000,         1.,        1.) )
        self.addDataset( Dataset("QCD_Pt-600to800"           ,  112,          167.0000,         1.,        1.) )
        self.addDataset( Dataset("QCD_Pt-800to1000"          ,  113,           28.2500,         1.,        1.) )
        self.addDataset( Dataset("QCD_Pt-1000to1400"         ,  114,            8.1950,         1.,        1.) )
        self.addDataset( Dataset("QCD_Pt-1400to1800"         ,  115,            0.7346,         1.,        1.) )
        self.addDataset( Dataset("QCD_Pt-1800"               ,  116,            0.1091,         1.,        1.) )

        self.addDataset( Dataset("GJets_HT-100to200"         ,  201,         1534.0000,         1.,        1.) )
        self.addDataset( Dataset("GJets_HT-200to400"         ,  202,          489.9000,         1.,        1.) )
        self.addDataset( Dataset("GJets_HT-400to600"         ,  203,           62.0500,         1.,        1.) )

        self.addDataset( Dataset("TTJets_MSDecaysCKM_central",  300,          424.5000,         1.,        1.) )

        self.addDataset( Dataset("WJetsToLNu_HT100to200"     ,  501,         1817.0000,         1.,        1.) )
        self.addDataset( Dataset("WJetsToLNu_HT200to400"     ,  502,          471.6000,         1.,        1.) )
        self.addDataset( Dataset("WJetsToLNu_HT400to600"     ,  503,           55.6100,         1.,        1.) )
        self.addDataset( Dataset("WJetsToLNu_HT600toInf"     ,  504,           18.8100,         1.,        1.) )

        self.addDataset( Dataset("DYJetsToLL_M-50_HT200to400",  600,           52.2400,         1.,        1.) )
        self.addDataset( Dataset("DYJetsToLL_M-50_HT400to600",  601,            6.5460,         1.,        1.) )
        self.addDataset( Dataset("DYJetsToLL_M-50_HT600toInf",  602,            2.1790,         1.,        1.) )
        self.addDataset( Dataset("ZJetsToNuNu_HT200to400"    ,  605,          100.8000,         1.,        1.) )


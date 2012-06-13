import ROOT as rt
import os, pickle, sys

def mergeNorms(files):
    
    bins = {}

    for f in files:
        name = f.replace('.root','.pkl')
        if not os.path.exists(name): continue
        norm = pickle.load(file(name))
        for key in norm.keys():
            if bins.has_key(key):
                bins[key] = bins[key] + norm[key]
            else:
                bins[key] = norm[key]
    return bins

def loadNorms():

    pkl = "/afs/cern.ch/user/w/wreece/work/LimitSetting/RazorMultiJet2011/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z-Summer11-PU_START42_V11_FastSim-v1-wreece_030412-ByPoint.pkl"
    return pickle.load(file(pkl))

def getBox(nB, nEle, nMu, nTau, diLep = False):
    #order is mu, ele, tau, had
    #boxMap = {'MuEle':0,'MuMu':1,'EleEle':2,'Mu':3,'Ele':4,'Had':5, 'BJet':6, 'Tau':7,'TauTau':8}
    BOX_NUM = 5
    if nEle > 0 and nMu > 0 and diLep:
        BOX_NUM = 0
    elif nMu > 1 and diLep:
        BOX_NUM = 1
    elif nEle > 1 and diLep:
        BOX_NUM = 2
    elif nTau > 1 and diLep:
        BOX_NUM = 8
    elif nMu > 0:
        BOX_NUM = 3
    elif nEle > 0:
        BOX_NUM = 4
    elif nTau > 0:
        BOX_NUM = 7
    elif nB > 0:
        BOX_NUM = 6
    return BOX_NUM

def getBoxGenLevel(genInfo):
    #const int result = (nTop*10000) + (nBot*1000) + (nEle*100) + (nMuon*10) + nTau;
    gen = str(genInfo)
    nB = int(gen[-4])
    nEle = int(gen[-3])
    nMu = int(gen[-2])
    nTau = int(gen[-1])
    return getBox(nB,nEle,nMu,nTau, diLep = True)

def getBTagScaleBinned(pt, ptmin, ptmax, payload):

    for i in xrange(len(ptmin)):
        if (pt > ptmin[i]) and (pt <= ptmax[i]):
            return payload[i]
    return payload[-1]

def getBTagScale(pt):
    """Return the data/MC scale factor from muon+jet events with the TCHEM tagger"""
    #see https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFb-mujet_payload.txt
    if pt > 670: pt = 670
    if pt < 30: pt = 30
    return 0.932251*((1.+(0.00335634*pt))/(1.+(0.00305994*pt)))

def getMistagScale(pt):
    #for the TCHEM tagger
    #https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/MistagFuncs.C
    if pt > 670: pt = 670
    if pt < 20: pt = 20
    return (-0.00256163+(0.000332759*pt))+(-2.39887e-07*(pt*pt))

def getBTagScaleLight(pt, version = 0):
    #https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFlightFuncs.C
    if pt > 670: pt = 670
    if pt < 20: pt = 20
    if version == 0:
        return (1.06268*((1+(0.00390509*pt))+(-5.85405e-05*(pt*pt))))+(7.87135e-07*(pt*(pt*(pt/(1+(0.01259*pt))))))
    elif version < 0:
        return (0.967092*((1+(0.00201431*pt))+(-1.49359e-05*(pt*pt))))+(6.94324e-08*(pt*(pt*(pt/(1+(0.00459787*pt))))))
    else:
        return (1.22691*((1+(0.00211682*pt))+(-2.07959e-05*(pt*pt))))+(1.72938e-07*(pt*(pt*(pt/(1+(0.00658853*pt))))))

def getBTagScaleWithFlavour(pt, flav):

    #b and c jets get the same scale
    if abs(flav) > 3:
        return getBTagScale(pt)
    else:
        return getBTagScaleLight(pt)

def getBTagScaleError(pt):
    
    ptmin = [30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500]
    ptmax = [40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670]
    
    SFb_error = [
        0.0311456,
        0.0303825,
        0.0209488,
        0.0216987,
        0.0227149,
        0.0260294,
        0.0205766,
        0.0227065,
        0.0260481,
        0.0278001,
        0.0295361,
        0.0306555,
        0.0367805,
        0.0527368]
    return getBTagScaleBinned(pt, ptmin, ptmax, SFb_error)

def getBTagScaleErrorWithFlavour(pt, flav):
    #see https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG
    if abs(flav) == 5:
        return getBTagScaleError(pt)
    elif abs(flav) == 4:
        return 2.*getBTagScaleError(pt)
    else:
        sf = getBTagScaleLight(pt)
        return max( abs(sf - getBTagScaleLight(pt, version = -1) ), abs(sf - getBTagScaleLight(pt, version = +1) ) ) 

def getBTagScaleFastSimTCHEM(pt):

    #from https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/FastSim_correction_payload_b.txt

    ptmin = [30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500]
    ptmax = [40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670]

    CFb = [0.984903,0.984349,0.989823,0.994377,0.994823,0.998853,1.00011,1.00192,0.999844,0.994276,0.962486,0.960869,0.921400,0.884047]
    return getBTagScaleBinned(pt, ptmin, ptmax, CFb)

def getBTagScaleFastSimTCHEMCharm(pt):

    #from https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/FastSim_correction_payload_c.txt

    ptmin = [30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500]
    ptmax = [40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670]

    CFc = [0.985571,0.991018,0.995646,1.01291,1.00856,1.01693,1.01937,1.02938,1.00252,0.996369,1.03542,1.06410,0.996128,1.08340]
    return getBTagScaleBinned(pt, ptmin, ptmax, CFc)

def getBTagScaleFastSimTCHEMLight(pt):

    #from https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/FastSim_correction_payload_udsg.txt

    ptmin = [30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500]
    ptmax = [40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670]

    CFb = [1.14868,1.21129,1.17566,1.21275,1.21488,1.25174,1.25167,1.26069,1.37430,1.30289,1.33446,1.47801,1.40297,1.42618]
    return getBTagScaleBinned(pt, ptmin, ptmax, CFb)

def getBTagScaleFastSimTCHEMWithFlavour(pt, flav):

    if abs(flav) == 5:
        return getBTagScaleFastSimTCHEM(pt)
    elif abs(flav) == 4:
        return getBTagScaleFastSimTCHEMCharm(pt)
    else:
        return getBTagScaleFastSimTCHEMLight(pt)

def getBTagScaleFastSimTCHEMError(pt, model):

    #from https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/FastSim_correction_payload_b.txt

    ptmin = [30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500]
    ptmax = [40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670]

    CFb_error = [0.00613653,0.00199196,0.00215753,0.00121753,0.00169263,0.00156044,0.00223008,0.00145730,0.00229011,0.00697229,0.00849029,0.00920664,0.0257637,0.0201347]
    CFb_T1_syst = [0.00763714,0.00398421,-0.00751030,-0.00886828,-0.00707424,-0.00922754,-0.0120088,-0.0153528,-0.0238886,-0.0491736,-0.0562929,-0.0832841,-0.0879707,-0.0910950]
    CFb_T1bbbb_syst = [0.00892949,0.0103067,0.00767028,0.00564462,0.00644326,0.00403184,0.00446469,0.00229353,-0.00525987,-0.0176956,-0.0227776,-0.0391628,-0.0318639,-0.0188904]
    CFb_T1tttt_syst = [0.00716144,0.00659549,0.00515735,0.00384551,0.00435913,0.00394750,0.00587666,0.00569624,0.000432074,-0.0108556,-0.0150331,-0.0310161,-0.0266697,-0.0185043]
    CFb_T2_syst = [0.00679091,0.000142276,-0.0100957,-0.0187663,-0.0204510,-0.0269493,-0.0234305,-0.0303673,-0.0451287,-0.0695636,-0.0687965,-0.0950772,-0.0997596,-0.0986330]
    CFb_T2bb_syst = [0.0133252,0.0155243,0.0154232,0.0160086,0.0163068,0.0155054,0.0171985,0.0156823,0.00952578,-0.00272945,-0.00640747,-0.0198778,-0.00637200,0.0188875]
    CFb_T2bw_syst = [0.0126070,0.0116517,0.00808352,0.00572801,0.00496042,0.00384587,0.00546443,0.00614893,0.00132501,-0.0100787,-0.0133782,-0.0281932,-0.0190974,0.000622183]
    CFb_T2tt_syst = [0.000917375,0.00268757,0.00236177,0.000609517,0.000788450,-0.00103161,-0.000584781,-0.00135735,-0.00428265,-0.00812903,-0.00133541,-0.00498018,0.00866503,0.0191085]
    CFb_T3w_syst = [0.0160910,0.0132154,0.0124112,0.0103626,0.0120900,0.00918794,0.00987530,0.00894481,0.00138414,-0.0112605,-0.0154934,-0.0302078,-0.0229885,-0.00569662]

    sms = {'T2tt':CFb_T2tt_syst,'T1tttt':CFb_T1tttt_syst,'T1':CFb_T1_syst,'T2':CFb_T2_syst}

    er = getBTagScaleBinned(pt, ptmin, ptmax, CFb_error)
    ercl = getBTagScaleBinned(pt, ptmin, ptmax, sms[model[0:2]])
    erm = getBTagScaleBinned(pt, ptmin, ptmax, sms[model])
    
    return (0.5*(ercl+erm)) - er

def getBTagScaleFastSimTCHEMErrorCharm(pt, model):

    #from https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/FastSim_correction_payload_c.txt

    ptmin = [30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500]
    ptmax = [40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670]

    CFb_error = [0.0125665,0.0131793,0.00586663,0.0150140,0.00890338,0.00884408,0.0114173,0.00904615,0.0251103,0.0292547,0.0321335,0.0816165,0.116164,0.0961097]
    CFb_T1_syst = [-0.0144778,-0.0391577,-0.0386187,-0.0452738,-0.0450074,-0.0517720,-0.0571389,-0.0664429,-0.0638823,-0.0828049,-0.125426,-0.161338,-0.113687,-0.0840099]
    CFb_T1bbbb_syst = [-0.0375821,-0.0540032,-0.0510494,-0.0789025,-0.0773368,-0.0939592,-0.105237,-0.125091,-0.142553,-0.162939,-0.210844,-0.246647,-0.201256,-0.194069]
    CFb_T1tttt_syst = [-0.0217229,-0.0164408,-0.0110269,-0.0151919,-0.00349712,-0.00317687,0.00251853,0.00360411,0.0111872,0.00584400,-0.0163592,-0.0201520,0.0286310,-0.0161453]
    CFb_T2_syst = [-0.0184297,-0.0173887,-0.0200683,-0.0328717,-0.00987270,-0.00741643,-0.00282955,-0.00638413,0.00469184,0.00309139,-0.0270509,-0.0498854,0.0284578,0.114177]
    CFb_T2bb_syst = [-0.0398276,-0.0240264,-0.0452479,-0.0594234,-0.0670149,-0.0735905,-0.0903004,-0.112308,-0.119699,-0.130972,-0.191086,-0.208647,-0.187053,-0.173428]
    CFb_T2bw_syst = [-0.0271228,-0.0247075,-0.0204258,-0.0245152,-0.0180615,-0.0165494,-0.0111949,-0.00788534,0.000936866,0.00752139,0.0190793,0.0552080,0.133976,0.197805]
    CFb_T2tt_syst = [-0.0266692,-0.0236049,-0.0242122,-0.0268060,-0.0187514,-0.0205835,-0.0172605,-0.0161971,-0.000747621,0.0100936,0.0124856,0.0363658,0.101250,0.0423019]
    CFb_T3w_syst = [-0.0183119,-0.0135405,-0.00764287,-0.00913349,-0.000201166,-5.40614e-05,0.00611967,0.00879967,0.0205840,0.0229479,-0.00353134,-0.0174986,0.0612400,0.138599]

    sms = {'T2tt':CFb_T2tt_syst,'T1tttt':CFb_T1tttt_syst,'T1':CFb_T1_syst,'T2':CFb_T2_syst}

    er = getBTagScaleBinned(pt, ptmin, ptmax, CFb_error)
    ercl = getBTagScaleBinned(pt, ptmin, ptmax, sms[model[0:2]])
    erm = getBTagScaleBinned(pt, ptmin, ptmax, sms[model])
    
    return (0.5*(ercl+erm)) - er

def getBTagScaleFastSimTCHEMErrorLight(pt, eta, model):

    #from https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/FastSim_correction_payload_udsg.txt

    ptmin = [30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500]
    ptmax = [40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670]

    if abs(eta) < 1.2:
        #these are the central values
        CFb_error = [0.0836960,0.0620631,0.0660143,0.0468159,0.0562934,0.0504945,0.0722798,0.0543950,0.0692661,0.0432473,0.0486005,0.0684991,0.101527,0.151238]
        CFb_T1_syst = [0.338603,0.437256,0.440432,0.459399,0.474636,0.524075,0.537927,0.520092,0.574010,0.552342,0.672783,0.792442,0.816496,1.13301]
        CFb_T1bbbb_syst = [0.0160306,0.0757968,0.0432971,0.0451966,0.0424747,0.0442542,0.0311419,-0.00498879,0.0221044,-0.0175752,0.0367338,0.120792,0.133364,0.295575]
        CFb_T1tttt_syst = [-0.0529133,-0.000480771,-0.00739771,0.00801998,0.0179709,0.0318846,0.0339304,0.0240896,0.0677412,0.0296138,0.0894420,0.190033,0.177628,0.214164]
        CFb_T2_syst = [0.344096,0.418596,0.423815,0.426871,0.458367,0.518538,0.551038,0.538732,0.578300,0.584436,0.664414,0.796732,0.814349,1.10579]
        CFb_T2bb_syst = [0.254086,0.319446,0.303863,0.323081,0.334110,0.346253,0.358051,0.310793,0.330196,0.264228,0.323744,0.382188,0.356799,0.535754]
        CFb_T2bw_syst = [0.127753,0.187009,0.159161,0.182510,0.183581,0.223679,0.226874,0.220785,0.262627,0.218247,0.280463,0.390897,0.427447,0.652699]
        CFb_T2tt_syst = [0.0602869,0.114597,0.101755,0.122819,0.140907,0.168496,0.189532,0.190157,0.248626,0.226991,0.268064,0.347572,0.327394,0.281085]
        CFb_T3w_syst = [0.218849,0.269390,0.278818,0.308740,0.334840,0.371084,0.396040,0.396950,0.457565,0.449399,0.544739,0.648471,0.682185,0.956866]
    else:
        #these are the forward values
        CFb_error = [0.217006,0.195035,0.198745,0.165793,0.157533,0.101572,0.175720,0.153702,0.167288,0.184342,0.184656,0.375708,0.493768,2.09962]
        CFb_T1_syst = [0.886678,1.01666,1.08284,1.26998,1.37682,1.52859,1.86833,2.28342,3.30515,4.62201,6.28638,8.39926,10.5474,14.4199]
        CFb_T1bbbb_syst = [0.604844,0.669382,0.628342,0.674114,0.682413,0.728405,0.846242,0.909704,1.17220,1.38281,1.70274,2.31952,2.49635,4.25097]
        CFb_T1tttt_syst = [0.732481,0.913868,0.978251,1.15067,1.26656,1.41190,1.71831,2.04051,2.63840,3.07196,3.66822,4.56219,4.67986,6.41515]
        CFb_T2_syst = [0.691207,0.735393,0.810300,0.953306,1.00186,1.07890,1.29266,1.49806,2.11583,2.74504,3.59195,5.24847,7.12510,11.4361]
        CFb_T2bb_syst = [0.684631,0.710458,0.704026,0.821475,0.860852,0.873799,1.03380,1.12001,1.37115,1.67132,2.05963,2.70426,2.66984,5.09678]
        CFb_T2bw_syst = [0.783436,0.897535,0.954873,1.07773,1.18204,1.29029,1.54843,1.71916,2.16292,2.59613,2.91203,3.81979,4.27104,6.03475]
        CFb_T2tt_syst = [0.754214,0.892473,0.953744,1.06942,1.16650,1.26228,1.52509,1.73076,2.19651,2.49553,2.84679,3.70724,3.94893,5.42563]
        CFb_T3w_syst = [0.920113,1.06111,1.14472,1.29299,1.44659,1.57705,1.90011,2.30868,3.14430,4.17406,5.39204,7.10060,8.38560,12.4521]

    sms = {'T2tt':CFb_T2tt_syst,'T1tttt':CFb_T1tttt_syst,'T1':CFb_T1_syst,'T2':CFb_T2_syst}

    er = getBTagScaleBinned(pt, ptmin, ptmax, CFb_error)
    ercl = getBTagScaleBinned(pt, ptmin, ptmax, sms[model[0:2]])
    erm = getBTagScaleBinned(pt, ptmin, ptmax, sms[model])
    
    return (0.5*(ercl+erm)) - er

def getBTagScaleFastSimTCHEMErrorWithFlavour(pt, eta, model, flav):

    if abs(flav) == 5:
        return getBTagScaleFastSimTCHEMError(pt, model)
    elif abs(flav) == 4:
        return getBTagScaleFastSimTCHEMErrorCharm(pt, model)
    else:
        return getBTagScaleFastSimTCHEMErrorLight(pt, eta, model)


def quad(a, b):
    return rt.TMath.Sqrt(a**2 + b**2)

def decoratePoint(point):
    return '%i_%i' % point

if __name__ == '__main__':

    model = 'T2tt'

    rt.gROOT.ProcessLine("""
struct Extras{\
    Int_t PROC;\
    Double_t LEP_W;\
    Double_t W;\
    Double_t W_EFF;\
    Double_t LEP_W_SYS;\
    Double_t LEP_W_OLD;\
    Int_t BOX_NUM;\
    Int_t BOX_NUM_GEN;\
};""")
    from ROOT import Extras, std
    extras = Extras()
    extras.PROC = 0
    extras.LEP_W = 1.0
    extras.W = 1.0
    extras.W_EFF_SYS = 0.0

    import glob
    #fileList = glob.glob('SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z-Summer11-PU_START42_V11_FastSim-v1-wreece_030412-*-AllSMS.root')
    fileList = glob.glob('SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z-Summer11-PU_START42_V11_FastSim-v1-wreece_030412-JetFL_*.root')
    #fileList = glob.glob('SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z-Summer11-PU_START42_V11_FastSim-v1-wreece_030412_*.root')
    norms = mergeNorms(fileList)
    #norms = loadNorms()
    pickle.dump(norms,file('foo.pkl','wb'))
    print norms

    oldTree = rt.TChain('RMRTree')
    for f in fileList:
        oldTree.Add(f)

    outputFile = rt.TFile.Open('foo.root','recreate')
    def cloneTree(point):
        newTree = oldTree.CloneTree(0)
        newTree.SetName('%s_%s' % (oldTree.GetName(),decoratePoint(point)) )
        newTree.Branch('PROC',rt.AddressOf(extras,'PROC'),'PROC/I')
        newTree.Branch('LEP_W',rt.AddressOf(extras,'LEP_W'),'LEP_W/D')
        newTree.Branch('LEP_W_SYS',rt.AddressOf(extras,'LEP_W_SYS'),'LEP_W_SYS/D')
        newTree.Branch('LEP_W_OLD',rt.AddressOf(extras,'LEP_W_OLD'),'LEP_W_OLD/D')
        newTree.Branch('W_EFF',rt.AddressOf(extras,'W_EFF'),'W_EFF/D')
        newTree.Branch('W',rt.AddressOf(extras,'W'),'W/D')
        newTree.Branch('BOX_NUM',rt.AddressOf(extras,'BOX_NUM'),'BOX_NUM/I')
        newTree.Branch('BOX_NUM_GEN',rt.AddressOf(extras,'BOX_NUM_GEN'),'BOX_NUM_GEN/I')
        return newTree                
        
    #make one tree per point
    trees = {}
    for point in norms.iterkeys():
        trees[point] = cloneTree(point)

    #fill the trees by point
    #for i in xrange(10000):
    for i in xrange(oldTree.GetEntries()):
        oldTree.GetEntry(i)
        
        point = (oldTree.mStop,oldTree.mLSP)
        total = 1.*norms[point]
        extras.W_EFF = 1.0/total

        #extras.BOX_NUM = getBox(1,oldTree.nElectronTight,oldTree.nMuonTight,oldTree.nTauTight)
        extras.BOX_NUM_GEN = getBoxGenLevel(oldTree.genInfo)

        #choose between Had and BJet boxes
        #if True:
        if oldTree.nBJet > 0:
            extras.BOX_NUM = 6
            #take a data/MC scale factor
            extras.LEP_W = getBTagScaleWithFlavour(oldTree.maxTCHE_PT,oldTree.maxTCHE_FL)*getBTagScaleFastSimTCHEMWithFlavour(oldTree.maxTCHE_PT,oldTree.maxTCHE_FL)
            #just take the old approach where everything was a b-quark
            extras.LEP_W_OLD = getBTagScale(oldTree.maxTCHE_PT)*getBTagScaleFastSimTCHEM(oldTree.maxTCHE_PT)
            extras.LEP_W_SYS = quad(getBTagScaleErrorWithFlavour(oldTree.maxTCHE_PT,oldTree.maxTCHE_FL), getBTagScaleFastSimTCHEMErrorWithFlavour(oldTree.maxTCHE_PT, oldTree.maxTCHE_ETA, model, oldTree.maxTCHE_FL))

            #print 'first',oldTree.maxTCHE, oldTree.maxTCHE_PT, oldTree.maxTCHE_FL, extras.LEP_W, extras.LEP_W_SYS, extras.LEP_W_OLD
            #if abs(oldTree.maxTCHE_FL) < 4:
            #    print 'value',getBTagScaleWithFlavour(oldTree.maxTCHE_PT,oldTree.maxTCHE_FL),getBTagScaleFastSimTCHEMWithFlavour(oldTree.maxTCHE_PT,oldTree.maxTCHE_FL)
            #    print 'error',getBTagScaleErrorWithFlavour(oldTree.maxTCHE_PT,oldTree.maxTCHE_FL), getBTagScaleFastSimTCHEMErrorWithFlavour(oldTree.maxTCHE_PT, model, oldTree.maxTCHE_FL)

            #extras.LEP_W = 1.0
        elif oldTree.nBJetLoose == 0:
            #Had Box
            extras.BOX_NUM = 5
            #ignore the mis-tag here
            extras.LEP_W = 1.0
            extras.LEP_W_OLD = 1.0
        else:
            #this is neither our control channel or the signal box
            continue

        #the number of generator level taus
        nGenTau = int(str(oldTree.genInfo)[-1])
        if nGenTau > 0:
            #the data/MC scale factor for the tau ID
            extras.LEP_W *= (1/0.979) #i.e. tau events are more likely to pass the selection in data than in MC as the veto is worse
            
        trees[point].Fill()

    #save all of the trees in one file
    for tree in trees.itervalues():
        tree.Write()
    outputFile.Close()

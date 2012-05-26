#Here we pick if we want the baseline or the CMG analyzer


##USE THIS TOI CROSS CHECK THE TWIKI(OLD +DEPRACATED
#from FourLeptonAnalyzerBaseline import FourLeptonAnalyzerBaseline as FourLeptonAnalyzer


#THIS IS ONLY TO STUDY THE FSR IN MC ONLY!!!
#from FourLeptonAnalyzerFSRStudy import FourLeptonAnalyzerFSRStudy as FourLeptonAnalyzer



#THIS IS TUNED TO PROVIDE NTUPLES for fake rate measurement application and
#full analysis. Make your CMG developements using that.
#It uses the FSR Algorithm
from FourLeptonAnalyzerCMG import FourLeptonAnalyzerCMG as FourLeptonAnalyzer





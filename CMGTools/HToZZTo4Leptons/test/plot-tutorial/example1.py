from CMGTools.HToZZTo4Leptons.macros.plotters.TreePlotter import *



#make a tree plotter
mc1 = TreePlotter("MC1.root","FourLeptonTreeProducer/tree")
#take the weight from the pck file
mc1.setupFromFile("MC1.pck")

#add scale factor with systematics . Here it assumes there is an event weight in the tree
#the next line adds the eventWeight and assigns to it an error of 0.0
#dont worry for errors yet , this is to build datacards

mc1.addCorrectionFactor('eventWeight','eventWeight',0.0,'lnN')

#now set the fill and the line properties

mc1.setFillProperties(0,ROOT.kWhite)
mc1.setLineProperties(1,ROOT.kOrange+10,3)

#now lets make the other 1
mc2 = TreePlotter("MC2.root","FourLeptonTreeProducer/tree")
mc2.setupFromFile("MC2.pck")
mc2.addCorrectionFactor('eventWeight','eventWeight',0.0,'lnN')
mc2.setFillProperties(0,ROOT.kRed)

#cool now lets load the data
data = TreePlotter("DATA.root","FourLeptonTreeProducer/tree")


#OK now use them all in a stack
zzStack = StackPlotter()
zzStack.addPlotter(mc1,"MC1","MC1","background")
zzStack.addPlotter(mc2,"MC2","MC2","signal")
zzStack.addPlotter(data,"data_obs","Data","data")



#DONE!
#Now do
#>ipython 
#>from example1 import *
#>zzStack.drawStack("mass","mass>100","12000",20,100,160,"m_{4l}","GeV")
#and it should be done



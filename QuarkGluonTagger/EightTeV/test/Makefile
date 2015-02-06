CC = g++
CFLAGS = -Wall -c -g

ROOFIT_INCLUDE := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep INCLUDE= | sed 's|INCLUDE=||')
ROOFIT_LIBDIR := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep LIBDIR= | sed 's|LIBDIR=||')

INCLUDES = -I. -I$(ROOTSYS)/include  -I$(ROOFIT_INCLUDE)/ -I$(CMSSW_BASE)/src -I$(CMSSW_BASE)/src/CommonTools -I$(CMSSW_BASE)/src/UserCode/pandolf/ -I$(CMSSW_BASE)/src/QuarkGluonTagger/EightTeV/interface

ROOTSYS  ?= ERROR_RootSysIsNotDefined

ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags --libs` 

EXTRALIBS  :=  -L$(ROOTSYS)/lib -L$(ROOFIT_LIBDIR)/ -lHtml -lMathCore -lGenVector -lMinuit -lEG -lRooFitCore -lRooFit `root-config --libs` -lTMVA


checkSyst: checkSyst.cpp QGSyst.o DrawBase.o fitTools.o 
	$(CC) -Wall $(INCLUDES) -o checkSyst checkSyst.cpp QGSyst.o DrawBase.o fitTools.o  $(ROOTFLAG) $(EXTRALIBS)

computeSystSmearings: computeSystSmearings.cpp QGSyst.o DrawBase.o fitTools.o 
	$(CC) -Wall $(INCLUDES) -o computeSystSmearings computeSystSmearings.cpp QGSyst.o DrawBase.o fitTools.o  $(ROOTFLAG) $(EXTRALIBS)

drawQGLMCPlots: DrawBase.o fitTools.o Bins.o QGLikelihoodCalculator.o QGMLPCalculator.o parameters.o drawQGLMCPlots.cpp
	$(CC) -Wall $(INCLUDES) -o drawQGLMCPlots drawQGLMCPlots.cpp DrawBase.o fitTools.o Bins.o QGLikelihoodCalculator.o QGMLPCalculator.o parameters.o $(ROOTFLAG) $(EXTRALIBS) -L/afs/cern.ch/work/a/amarini/QGApproval/CMSSW_5_3_9_patch1/lib/slc5_amd64_gcc462 -lFWCoreParameterSet

drawQGLDataMCPlots: DrawBase.o fitTools.o drawQGLDataMCPlots.cpp
	$(CC) -Wall $(INCLUDES) -o drawQGLDataMCPlots drawQGLDataMCPlots.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS) 

drawFractions: DrawBase.o fitTools.o drawFractions.cpp
	$(CC) -Wall $(INCLUDES) -o drawFractions drawFractions.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS) 

makeSunilTreeFlat: DrawBase.o fitTools.o Bins.o PUWeight.o QGLikelihoodCalculator.o QGMLPCalculator.o parameters.o makeSunilTreeFlat.cpp
	$(CC) -Wall $(INCLUDES) -o makeSunilTreeFlat makeSunilTreeFlat.cpp DrawBase.o fitTools.o PUWeight.o Bins.o QGLikelihoodCalculator.o QGMLPCalculator.o parameters.o $(ROOTFLAG) $(EXTRALIBS) -L/afs/cern.ch/work/a/amarini/QGApproval/CMSSW_5_3_9_patch1/lib/slc5_amd64_gcc462 -lFWCoreParameterSet

makeAndreaTreeFlat: DrawBase.o fitTools.o Bins.o PUWeight.o QGLikelihoodCalculator.o QGMLPCalculator.o parameters.o makeAndreaTreeFlat.cpp
	$(CC) -Wall $(INCLUDES) -o makeAndreaTreeFlat makeAndreaTreeFlat.cpp DrawBase.o fitTools.o PUWeight.o Bins.o QGLikelihoodCalculator.o QGMLPCalculator.o parameters.o $(ROOTFLAG) $(EXTRALIBS) -L/afs/cern.ch/work/a/amarini/QGApproval/CMSSW_5_3_9_patch1/lib/slc5_amd64_gcc462 -lFWCoreParameterSet

stupidTree: DrawBase.o fitTools.o Bins.o PUWeight.o QGLikelihoodCalculator.o QGMLPCalculator.o parameters.o stupidTree.cpp
	$(CC) -Wall $(INCLUDES) -o stupidTree stupidTree.cpp DrawBase.o fitTools.o PUWeight.o Bins.o QGLikelihoodCalculator.o QGMLPCalculator.o parameters.o $(ROOTFLAG) $(EXTRALIBS) -L/afs/cern.ch/work/a/amarini/QGApproval/CMSSW_5_3_9_patch1/lib/slc5_amd64_gcc462 -lFWCoreParameterSet

create_ptAveWeights: create_ptAveWeights.cpp
	$(CC) -Wall $(INCLUDES) -o create_ptAveWeights create_ptAveWeights.cpp $(ROOTFLAG) $(EXTRALIBS) 

create_rhoWeights: create_rhoWeights.cpp
	$(CC) -Wall $(INCLUDES) -o create_rhoWeights create_rhoWeights.cpp $(ROOTFLAG) $(EXTRALIBS) 

create_ptRhoWeights: create_ptRhoWeights.cpp
	$(CC) -Wall $(INCLUDES) -o create_ptRhoWeights create_ptRhoWeights.cpp $(ROOTFLAG) $(EXTRALIBS) 



QGLikelihoodCalculator.o: $(CMSSW_BASE)/src/QuarkGluonTagger/EightTeV/src/QGLikelihoodCalculator.cc
	$(CC) $(CFLAGS) $(INCLUDES) $(CMSSW_BASE)/src/QuarkGluonTagger/EightTeV/src/QGLikelihoodCalculator.cc $(ROOTFLAG)

QGMLPCalculator.o: $(CMSSW_BASE)/src/QuarkGluonTagger/EightTeV/src/QGMLPCalculator.cc
	$(CC) $(CFLAGS) $(INCLUDES) $(CMSSW_BASE)/src/QuarkGluonTagger/EightTeV/src/QGMLPCalculator.cc $(ROOTFLAG)

parameters.o: $(CMSSW_BASE)/src/QuarkGluonTagger/EightTeV/src/parameters.cc
	$(CC) $(CFLAGS) $(INCLUDES) $(CMSSW_BASE)/src/QuarkGluonTagger/EightTeV/src/parameters.cc $(ROOTFLAG)

Bins.o: $(CMSSW_BASE)/src/QuarkGluonTagger/EightTeV/src/Bins.cc
	$(CC) $(CFLAGS) $(INCLUDES) $(CMSSW_BASE)/src/QuarkGluonTagger/EightTeV/src/Bins.cc $(ROOTFLAG)

QGSyst.o: $(CMSSW_BASE)/src/QuarkGluonTagger/EightTeV/src/QGSyst.cc
	$(CC) $(CFLAGS) $(INCLUDES) $(CMSSW_BASE)/src/QuarkGluonTagger/EightTeV/src/QGSyst.cc $(ROOTFLAG)

DrawBase.o: $(CMSSW_BASE)/src/CommonTools/DrawBase.C fitTools.o
	$(CC) $(CFLAGS) $(INCLUDES) fitTools.o $(CMSSW_BASE)/src/CommonTools/DrawBase.C $(ROOTFLAG) $(EXTRALIBS) 

fitTools.o: $(CMSSW_BASE)/src/CommonTools/fitTools.C
	$(CC) $(CFLAGS) $(INCLUDES) $(CMSSW_BASE)/src/CommonTools/fitTools.C $(ROOTFLAG) $(EXTRALIBS)

PUWeight.o: $(CMSSW_BASE)/src/CommonTools/PUWeight.C 
	$(CC) $(CFLAGS) $(INCLUDES) $(CMSSW_BASE)/src/CommonTools/PUWeight.C $(ROOTFLAG) $(EXTRALIBS) 



clean:
	rm *.o

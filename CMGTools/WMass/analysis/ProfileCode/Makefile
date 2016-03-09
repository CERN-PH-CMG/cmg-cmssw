CXX=g++
CPPFLAGS=-g -O2 -Wall -Wextra
ROOTINCLUDE=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --cflags --glibs)
ROOFITINCLUDE=-I $(ROOFITSYS)/include
ROOFITLIBS=-L $(ROOFITSYS)/lib -lRooFit -lRooStats -lRooFit -lRooFitCore
MORELIBS=-lFoam -lMathMore
INCLUDE_DIR=../includes/
INCLUDE=-I $(INCLUDE_DIR)
LIBS=-lm
OBJS=runZanalysis.o Zanalysis.o common_stuff.o RecoilCorrector.o KalmanCalibratorParam.o PdfDiagonalizer.o
#CXX_FILES=Zanalysis.C common_stuff.C RecoilCorrector.cc KalmanCalibratorParam.cc KalmanCalibrator.cc PdfDiagonalizer.cc runZanalysis.C

build: Zanalysis

Zanalysis: $(OBJS)
	$(CXX) $(CPPFLAGS) $(ROOTLIBS) $(LIBS) $(ROOFITLIBS) $(OBJS) -o runZanalysis

runZanalysis.o: runZanalysis.C Zanalysis.h
	$(CXX) -c runZanalysis.C -o runZanalysis.o $(CPPFLAGS) $(ROOTINCLUDE)
	
Zanalysis.o: Zanalysis.C Zanalysis.h HTransformToHelicityFrame.c HTransformToCS.c $(INCLUDE_DIR)/common.hpp
	$(CXX) -c Zanalysis.C -o Zanalysis.o $(CPPFLAGS) $(ROOTINCLUDE) $(ROOFITINCLUDE) $(INCLUDE)
	
common_stuff.o: common_stuff.C common_stuff.h
	$(CXX) -c common_stuff.C -o common_stuff.o $(CPPFLAGS) $(ROOTINCLUDE)
	
RecoilCorrector.o: RecoilCorrector.cc RecoilCorrector.h
	$(CXX) -c RecoilCorrector.cc -o RecoilCorrector.o $(CPPFLAGS) $(ROOTINCLUDE) $(ROOFITINCLUDE)

KalmanCalibratorParam.o: KalmanCalibratorParam.cc KalmanCalibratorParam.h
	$(CXX) -c KalmanCalibratorParam.cc -o KalmanCalibratorParam.o $(CPPFLAGS) $(ROOTINCLUDE)

PdfDiagonalizer.o: PdfDiagonalizer.cc PdfDiagonalizer.h
	$(CXX) -c PdfDiagonalizer.cc -o PdfDiagonalizer.o $(CPPFLAGS) $(ROOTINCLUDE) $(ROOFITINCLUDE)

clean:
	rm -f runZanalysis
	rm -f $(OBJS)

rebuild: clean build

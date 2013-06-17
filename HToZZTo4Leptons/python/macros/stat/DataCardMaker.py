import ROOT
from CMGTools.HToZZTo4Leptons.macros.stat.PDFFactory import *

class DataCardMaker(object):
    def __init__(self,finalstate,period = '7TeV',category='inclusive',analysis = 'hzz4l'):
        """
        Initialize DataCard maker . You give a channel and a subchannel
        I use the final state for channel and the period (11,12) in subchannel
        """
        self.shapes=[]
        self.data = None
        self.channel=finalstate
        self.period=period
        self.category=category
        self.analysis=analysis
        self.signalCounter=0
        self.bkgCounter=1
        self.parametricSysts=[]
        self.inflatedShapeErrors=[]
        self.extrapolationFactor=1.0
        self.suffix=finalstate+'_'+period+'_'+self.category

    def extrapolateToHigherLumi(self,factor):
        print 'Setting extrapolation factor to',factor
        print 'This works only for parametric shapes'
        
        self.extrapolationFactor = factor

    def processExpr(self,shape,expr,w,pdfFactory,mini,maxi,isTop = True):
        if isTop:
            name=shape['name']
        else:
            name = shape['name']+expr['pdfName']+self.channel+'_'+self.period+'_'+self.category
        
        if expr['type']=='leaf':

#            if expr['pdf'] == 'Pt':
#                if len(expr['syst'])==0:
#                    pdfname=name
#                else:
#                    pdfname=name+'nominal'
#                    
#                pdfFactory.makeTsallis(pdfname,self.observables[expr['var'][0]])
#                pdfFactory.loadPtPreset(shape['name']+'_'+self.period)
#                for p in pdfFactory.params:
#                    w.var(p).setConstant(1)
#                    print 'NAME:',p+'_'+pdfname,'VALUE:',w.var(p).getVal()
#                    w.var(p).SetName(p+'_'+pdfname)
#                pdfList=ROOT.RooArgList()
#                coeffList=ROOT.RooArgList()
#                pdfList.add(w.pdf(pdfname))
#                
#                for syst in expr['syst']:    
#                    pdfFactory.makeTsallis(name+syst+'_up',self.observables[expr['var'][0]])
#                    if pdfFactory.loadPtPreset(shape['name']+'_'+self.period+'_'+syst+'_Up'):
#                        for p in pdfFactory.params:
#                            w.var(p).setConstant(1)
#                            print 'NAME:',p+'_'+pdfname,'VALUE:',w.var(p).getVal()
#                            w.var(p).SetName(p+'_'+name+syst+'up')
#                        pdfFactory.makeTsallis(name+syst+'_dwn',self.observables[expr['var'][0]])
#                        pdfFactory.loadPtPreset(shape['name']+'_'+self.period+'_'+syst+'_Dwn')
#                        for p in pdfFactory.params:
#                            w.var(p).setConstant(1)
#                            print 'NAME:',p+'_'+pdfname,'VALUE:',w.var(p).getVal()
#                            w.var(p).SetName(p+'_'+name+syst+'dwn')
#
#                        pdfList.add(w.pdf(name+syst+'_up'))
#                        pdfList.add(w.pdf(name+syst+'_dwn'))
#                        w.factory(syst+'[-3,3]')
#                        self.setParamError(syst,0.0,1.0)
#                        coeffList.add(w.var(syst))
#                if len(expr['syst'])>0:    
#                    print 'PDFS and COefficiencts'
#                    pdfList.Print()
#                    coeffList.Print()
#                    pdf = ROOT.VerticalInterpPdf(name,name,pdfList,coeffList)
#                    getattr(w,'importClassCode')(ROOT.VerticalInterpPdf.Class(),1)
#                    getattr(w,'import')(pdf)


#            elif expr['pdf'] == 'PtAndMELA':
#                if len(expr['syst'])==0:
#                    pdfname=name
#                else:
#                    pdfname=name+'nominal'

                #create the new Pt/M which is Pt/(M*alpha)
#                formula = ''
#                if shape['name'] == 'ggH':
#                    formula='1.16763-0.987732*@0+1.61603*@0*@0-0.806264*@0*@0*@0'
#                elif shape['name'] in ['qqH','ZH','WH','ttH']:
#                    formula='1.1615-0.943562*@0+1.50617*@0*@0-0.731642*@0*@0*@0'
#                elif shape['name'] in ['QQZZ','GGZZ']:
#                    formula='0.90454+0.842558*@0-2.54485*@0*@0+2.24854*@0*@0*@0'
#                elif shape['name'] =='FAKES':
#                    formula='1.15298-0.880692*@0+0.896615*@0*@0-0.0458547*@0*@0*@0'
#                else:
#                    print "FUCKED UP"
#
#                w.factory('expr::alpha_'+shape['name']+"('@1/( "+formula+")',"+self.observables[expr['var'][1]]+","+self.observables[expr['var'][0]]+")")
                                    
                    
#                pdfFactory.makeTsallis(pdfname,'alpha_'+shape['name'],True)
#                pdfFactory.loadPtPreset(shape['name']+'_'+self.period)
#                for p in pdfFactory.params:
#                    w.var(p).setConstant(1)
#                    print 'NAME:',p+'_'+pdfname,'VALUE:',w.var(p).getVal()
#                    w.var(p).SetName(p+'_'+pdfname)
#                pdfList=ROOT.RooArgList()
#                coeffList=ROOT.RooArgList()
#                pdfList.add(w.pdf(pdfname))
                
#                for syst in expr['syst']:    
#                    pdfFactory.makeTsallis(name+syst+'_up',self.observables[expr['var'][0]])
#                    if pdfFactory.loadPtPreset(shape['name']+'_'+self.period+'_'+syst+'_Up'):
#                        for p in pdfFactory.params:
#                            w.var(p).setConstant(1)
#                            print 'NAME:',p+'_'+pdfname,'VALUE:',w.var(p).getVal()
#                            w.var(p).SetName(p+'_'+name+syst+'up')
#                        pdfFactory.makeTsallis(name+syst+'_dwn',self.observables[expr['var'][0]])
#                        pdfFactory.loadPtPreset(shape['name']+'_'+self.period+'_'+syst+'_Dwn')
#                        for p in pdfFactory.params:
#                            w.var(p).setConstant(1)
#                            print 'NAME:',p+'_'+pdfname,'VALUE:',w.var(p).getVal()
#                            w.var(p).SetName(p+'_'+name+syst+'dwn')#
#
#                        pdfList.add(w.pdf(name+syst+'_up'))
#                        pdfList.add(w.pdf(name+syst+'_dwn'))
#                        w.factory(syst+'[-3,3]')
#                        self.setParamError(syst,0.0,1.0)
#                        coeffList.add(w.var(syst))
#                if len(expr['syst'])>0:    
#                    print 'PDFS and COefficiencts'
#                    pdfList.Print()
#                    coeffList.Print()
#                    pdf = ROOT.VerticalInterpPdf(name,name,pdfList,coeffList)
#                    getattr(w,'importClassCode')(ROOT.VerticalInterpPdf.Class(),1)
#                    getattr(w,'import')(pdf)
#
#
#

            if expr['pdf'] == 'Mass':
                shape['plotter'].make1DPdf(w,name,mini[expr['var'][0]],maxi[expr['var'][0]])

            elif expr['pdf'] == 'Template':
                vars=[]
                for v in expr['var']:
                    vars.append(self.observables[v])
                pdfFactory.makeTemplateFromFile(name,vars,expr['pdfName'],self.channel+'_'+shape['name']+'_'+self.period,expr['syst'])
        elif expr['type']=='branch':
            name1 = shape['name']+expr['first']['pdfName']+self.channel+'_'+self.period+'_'+self.category
            name2 = shape['name']+expr['second']['pdfName']+self.channel+'_'+self.period+'_'+self.category
            self.processExpr(shape,expr['first'],w,pdfFactory,mini,maxi,False)
            self.processExpr(shape,expr['second'],w,pdfFactory,mini,maxi,False)
            if 'third' in expr:
                self.processExpr(shape,expr['third'],w,pdfFactory,mini,maxi,False)
            if 'fourth' in expr:
                self.processExpr(shape,expr['fourth'],w,pdfFactory,mini,maxi,False)
            if expr['oper']=='PROD':
                pdfFactory.makeProduct(name,name1,name2)
            if expr['oper']=='FPROD':
                pdfFactory.makeFastProduct(name,name1,name2)
            if expr['oper']=='COND':
                names=[name1,name2]
                conds=[self.observables[expr['second']['var'][1]]]
                if 'third' in expr:
                    names.append(shape['name']+expr['third']['pdfName']+self.channel+'_'+self.period+'_'+self.category)
                    conds.append(self.observables[expr['third']['var'][1]])
                if 'fourth' in expr:
                    names.append(shape['name']+expr['fourth']['pdfName']+self.channel+'_'+self.period+'_'+self.category)
                    conds.append(self.observables[expr['fourth']['var'][1]])
                pdfFactory.makeMultiConditional(name,names,conds)
            if expr['oper']=='FCOND':
                names=[name1,name2]
                conds=[self.observables[expr['second']['var'][1]],self.observables[expr['second']['var'][2]]]
                pdfFactory.makeFastCond(name,names,conds)


    def unfoldHisto(self,h,name):
        totalBins=h.GetNbinsX()*h.GetNbinsY()
        h2=ROOT.TH1D(name,'',totalBins,0,totalBins)
        N=1
        for i in range(1,h.GetNbinsX()+1):
            for j in range(1,h.GetNbinsY()+1):
                h2.SetBinContent(N,h.GetBinContent(i,j))
                h2.SetBinError(N,h.GetBinError(i,j))
                N=N+1
        return h2 

    def setShapeError(self,name,shape,error):
        self.inflatedShapeErrors.append({'name':name,'shape':shape,'error':error})

    def setParamError(self,name,mean,error,mini=None,maxi=None):
        found=False
        for s in self.parametricSysts:
            if s['name'] == name:
                found=True
                s['mean']=mean
                s['error']=error
                s['min']=mini
                s['max']=maxi
                break
        if not found:
            self.parametricSysts.append({'name':name,'mean':mean,'error':error,'min':mini,'max':maxi})

    def addShape(self,name,plotter,origin='background',model='binned',floatingParameters = ''):
        """
        Register a shape . Give name,corresponding plotter , fit model and a set of floating parameters
        (for parametric shapes) separated by comma. The floating parameters are left floating in the fit
        """
        out=dict()
        out['plotter']=plotter
        out['channel']=self.channel
        out['name'] = name
        out['type']=origin
        out['model']=model
        if origin == 'signal':
            out['ID'] = self.signalCounter
            self.signalCounter = self.signalCounter-1
        else:
            out['ID'] = self.bkgCounter
            self.bkgCounter = self.bkgCounter+1

        out['systs'] =plotter.corrFactors     
        out['floating'] = floatingParameters
        self.shapes.append(out)


    def addData(self,plotter,model='binned'):
        out=dict()
        out['plotter']=plotter
        out['name'] = 'data_obs'
        out['model']=model
        self.data=out


#    def treatParameters(self,shape,w,params,suffix):
#        
#        for p in params:
#            if w.var(p).
#            if p['name'] not in floatingParams:
#                p['var'].setConstant(ROOT.kTRUE)
#            else:
#                #search if the systematic is overriden in plotter
#                isOverriden=False
#                name=p['name']
#                if suffix !='':
#                    name =name+'__'+suffix##
#
#                for syst in self.parametricSysts:
#                    print name,p['name'],syst['name']
#                    if syst['name'] == name:
#                        isOverriden=True
#                        break
#                # if not overriden add it 
#                if not isOverriden:    
#                    self.setParamError(name,p['var'].getVal(),p['var'].getError())


    def writeCard(self,var,cuts,lumi,bins,mini,maxi):
        #Open ROOT file
        filename=self.analysis+self.suffix
        FR = ROOT.TFile(filename+".root","RECREATE")
        FR.cd()
        #create workspace for parametric stuff and PDF factory 
        w = ROOT.RooWorkspace('w')
        pdfFactory = PDFFactory(w)
        
        #Open datacard
        f = open(filename+'.txt','w',)
        f.write('imax 1\n')
        f.write('jmax {n}\n'.format(n=len(self.shapes)-1))
        f.write('kmax *\n')
        f.write('-------------------------\n')
        #Create the shape mappings
        for shape in self.shapes+[self.data]:
            if shape['model'] in ['binned','binned2D']:
                f.write('shapes {name} {channel} {file}.root {name} {name}_$SYSTEMATIC\n'.format(name=shape['name'],channel=self.suffix,file=filename))
            else:    
                f.write('shapes {name} {channel} {file}.root w:{name}\n'.format(name=shape['name'],channel=self.suffix,file=filename))
        f.write('-------------------------\n')
        f.write('bin '+self.suffix+'\n')
        
        #Create the dataset AND THE VARIABLES
        dataPlotter = self.data['plotter']
        if self.data['model'] == 'binned':
            h = dataPlotter.drawTH1(var,cuts,lumi,bins[0],mini[0],maxi[0])
            h.SetName(self.data['name'])
            h.Write()
            self.observables=[]
            dataRate = h.Integral()
        elif self.data['model'] == 'binned2D':
            h = dataPlotter.drawTH2(var,cuts,lumi,bins[0],mini[0],maxi[0],bins[1],mini[1],maxi[1])
            h.SetName(self.data['name']+'2D')
            preRate=h.Integral()
            h2=self.unfoldHisto(h,self.data['name'])
            h2.Write()
            h.Write()

            self.observables=[]
            dataRate = h2.Integral()

        elif self.data['model'] == 'roobinned':
            if len(bins)==1:
                dataset,observables=dataPlotter.makeDataHist(var,cuts,bins[0],mini[0],maxi[0],1000,-1e-9,1e+9,1000,-1e-9,1e+9,'data_obs')
            elif len(bins)==2:    
                dataset,observables=dataPlotter.makeDataHist(var,cuts,bins[0],mini[0],maxi[0],bins[1],mini[1],maxi[1],1000,-1e-9,1e+9,'data_obs')
            else:
                dataset,observables=dataPlotter.makeDataHist(var,cuts,bins[0],mini[0],maxi[0],bins[1],mini[1],maxi[1],bins[2],mini[2],maxi[2],'data_obs')
                

            self.observables=[]


            for obs in observables:
                self.observables.append(obs.GetName())
            dataRate = dataset.sumEntries()
            getattr(w,'import')(dataset)
        else:
            dataset,observables=dataPlotter.makeDataSet(var,cuts,mini,maxi,self.data['name'])
            self.observables=[]
            for obs,nbins in zip(observables,bins):
#                dataset.changeObservableName(obs.GetName(),obs.GetName()+'_'+self.suffix)
#                self.observables.append(obs.GetName()+'_'+self.suffix)
                self.observables.append(obs.GetName())
                obs.setBins(nbins)
            dataRate = dataset.sumEntries()
            getattr(w,'import')(dataset)
            
        f.write('observation %d\n' % (int(dataRate)))
        f.write('-------------------------\n')
        FR.cd()        
        

        #Now write all the shapes to file and save thgeir rates
        print 'OBSERVABLES',self.observables
        for shape in self.shapes:

            #first write the rate



            ############## 
            # PLAIN TH1  #
            ##############
#            if type(shape['model']) is str and  shape['model'] == 'binned':
#                h = shape['plotter'].drawTH1(var,cuts,lumi,bins[0],mini[0],maxi[0])
#                h.SetName(shape['name'])
#                shape['rate']=h.Integral()
#                FR.cd()
#                h.Write()

            ############## 
            # PLAIN TH2  #
            ##############
 #           if type(shape['model']) is str and shape['model'] == 'binned2D':
 #               h = shape['plotter'].drawTH2(var,cuts,lumi,bins[0],mini[0],maxi[0],bins[1],mini[1],maxi[1])
 ##               h.SetName(shape['name']+'2D')
 #               shape['rate']=h.Integral()
 #               h.Smooth()
 #               h.Scale(shape['rate']/h.Integral())
 #               h2=self.unfoldHisto(h,shape['name'])
 #               FR.cd()
 #               h2.Write()
 #               h.Write()


            #############
            #RooHistPDF #
            #############
#            elif type(shape['model']) is str and shape['model'] == 'roobinned':
#                tmpData,tmpObs=shape['plotter'].makeDataHist(var,cuts,binsx,minx,maxx,binsy,miny,maxy,'tmpData')
#                shape['rate']=tmpData.sumEntries()

#                if len(self.observables)==1:
#                    h = shape['plotter'].drawTH1(var,cuts,lumi,binsx,minx,maxx)
#                else:
##                    h = shape['plotter'].drawTH2(var,cuts,lumi,binsx,minx,maxx,binsy,miny,maxy)
#                print self.observables    
#                pdfFactory.makeTemplateFromHisto(w,shape['name'],self.observables,h)    
            #############
            #RooHistPDF##
            #FROM FILE##
            #############
#            elif type(shape['model']) is str and shape['model'].find('roobinned')>-1:
#                parameters=shape['model'].split('|')
#                type = parameters[1]
#                if len(parameters)>2:
#                    scale=parameters[2]
#                else:
#                    scale='dummy'

#                subname = self.channel+'_'+self.period+'_'+self.category
                    
#                pdfFactory.makeTemplateFromFile(shape['name'],self.observables,type,self.channel+'_'+shape['name']+'_'+self.period,scale)

#                if len(self.observables)==1:
#                    h = shape['plotter'].drawTH1(var,cuts,lumi,bins[0],mini[0],maxi[0])
#                else:
#                    h = shape['plotter'].drawTH2(var,cuts,lumi,bins[0],mini[0],maxi[0],bins[1],mini[1],maxi[1])
#                print self.observables    
#                shape['rate']=h.Integral()



            ##########
            #Param1D #
            ##########
#            elif type(shape['model']) is str and shape['model'] =='param1D':
#                shape['plotter'].make1DPdf(w,shape['name'],minx,maxx)
#                if shape['type'] =='signal':
#                    shape['rate']=1
#                else:    
#                    h = shape['plotter'].drawTH1(var,cuts,lumi,bins[0],mini[0],maxi[0])
#                    shape['rate']=h.Integral()
            ##########
            #Param2D #
            ##########
#            elif type(shape['model']) is str and shape['model'].find('param2D_')>-1:
#                parameters=shape['model'].split('param2D_')[1].split('|')
#                type = parameters[0]
#                if len(parameters)>1:
#                    scale=parameters[1]
#                else:
#                    scale='dummy'

#                subname = self.channel+'_'+self.period+'_'+self.category
                    
#                shape['plotter'].make1DPdf(w,shape['name']+'_'+subname+'Mass',mini[0],maxi[0])
#                pdfFactory.makeTemplateFromFile(shape['name']+"_"+subname+type,self.observables,type,self.channel+'_'+shape['name'],scale)
#                pdfFactory.makeConditional(shape['name'],shape['name']+'_'+subname+'Mass',shape['name']+'_'+subname+type,self.observables[1])
#                if shape['type'] =='signal':
#                    shape['rate']=1
#                    
#                else:    
#                    h = shape['plotter'].drawTH1(var.split(':')[1],cuts,lumi,bins[0],mini[0],maxi[0])
#                    shape['rate']=h.Integral()

            ########
            #Custom#
            ########


            self.processExpr(shape,shape['model'],w,pdfFactory,mini,maxi,True)

            if shape['type'] =='signal':
                shape['rate']=1
            else:    
                h = shape['plotter'].drawTH1(var.split(':')[1],cuts,lumi,bins[0],mini[0],maxi[0])
                shape['rate']=h.Integral()

#            else:
#                print 'UNKNOWN SHAPE MODEL: NOT MAKING PDF'





                
        FR.cd()        



        #RESET the binning
        for obs,nbins in zip(self.observables,bins):
            w.var(obs).setBins(nbins)

        w.Write()        


        FR.Close()

        ####DATACARD WRITING

            
        #print header columns
        f.write('bin\t') 
        for shape in self.shapes:
            f.write(self.suffix+'\t')
        f.write('\n')
        
        #Sort the shapes by ID 
        shapes = sorted(self.shapes,key=lambda x: x['ID'])
        #print names
        f.write('process\t')
        for shape in shapes:
            f.write(shape['name']+'\t')
        f.write('\n')

        #Print ID
        f.write('process\t')
        for shape in shapes:
            f.write(str(shape['ID'])+'\t')
        f.write('\n')

        #print rates
        f.write('rate\t')
        for shape in shapes:
            f.write(str(shape['rate']*self.extrapolationFactor)+'\t')
        f.write('\n')
        

        #Ok now the systematics. First lets make a list of 
        #all of them(actually their names and the model)
        systematics=[]
        for shape in shapes:
            for syst in shape['systs']:
                #zero suppress
                if syst['error'] != 0.0 or len(syst['error'])>0:
                    systematics.append(syst)

        #now we have a list of systematics but there are duplicates
        #Lets  remove duplicates
        systs =[]
        for sys in systematics:
            found=False
            for sysPrint in systs:
                if sysPrint['name']==sys['name']:
                    found=True

            if not found:
                systs.append(sys)


        #OK now fill the systematics tables
        for syst in systs:
            writeLine=True
            if syst['model']=='lnN' or syst['model']=='lnN2':
                f.write(syst['name']+" lnN\t")
            elif syst['model']=='gmN':
                f.write(syst['name']+" "+syst['model']+" "+ str(int(syst['count']))+ '\t')

            for shape in shapes:
              #check if this shape has this systematic
                found=False
                toWrite=''
                for shapeSyst in shape['systs']:
                    if shapeSyst['name']==syst['name']:
                        found=True
                        if shapeSyst['model'] == 'lnN':
                            toWrite = str(1+shapeSyst['error'])+'\t'
                        elif shapeSyst['model'] == 'lnN2':
                            toWrite = str(shapeSyst['error'][0]) +'/'+str(shapeSyst['error'][1])+'\t'
                        elif shapeSyst['model'] == 'gmN':
                            toWrite=str(shapeSyst['error'])+'\t'
                        break
                #check if we wanted to inflate this error
                for infl in self.inflatedShapeErrors:
                    if infl['name']==syst['name'] and \
                            infl['shape'] ==shape['name']:
                        toWrite = str(1+infl['error'])+'\t'
                if  not found:
                    f.write('- \t')
                else:
                    f.write(toWrite)
            f.write('\n')
       
        #Finally if we have parametric PDFs add the constraints of thewir parameters
        #remove duplicates

        for syst in self.parametricSysts:
            if syst['error']>0.0 :
                if (syst['min'] is None) or (syst['max'] is None):   
                    toWrite='{name} param {mean} {rms} \n'.format(name=syst['name'],mean=syst['mean'], rms=syst['error'])
                else:
                    toWrite='{name} param {mean} {rms} [{mini},{maxi}]\n'.format(name=syst['name'],mean=syst['mean'], rms=syst['error'], mini=syst['min'], maxi=syst['max'])
    
                f.write(toWrite)
                


            
        f.close()
    





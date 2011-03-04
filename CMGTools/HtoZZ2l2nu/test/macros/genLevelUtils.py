#
# iterates of a collection of gen candidates and builds the decay tree
#
def getDecayTreeFrom(hgenCandHandle,selPdgId=25,debug=False) :

    mothers=[]
    daughters=[]
    finalstates=[]
    for hgencand in hgenCandHandle.product() :
        pdgid=hgencand.pdgId()
        if(pdgid!=selPdgId) : continue
        
        #now iterate over the daughters
        idaughters=[]
        ifinalstates=[]
        for idau in xrange(0,hgencand.numberOfDaughters()) :
            hgendaucand=hgencand.daughter(idau)
            daupdgid=hgendaucand.pdgId()
            if(daupdgid==pdgid): continue
            idaughters.append(hgendaucand)

            #check final states
            idaughterfinalstates=[]
            for jdau in xrange(0,hgendaucand.numberOfDaughters()) :
                hgengrandaucand=hgendaucand.daughter(jdau)
                gdpdgid=hgengrandaucand.pdgId()
                if(gdpdgid==daupdgid): continue
                idaughterfinalstates.append(hgengrandaucand)
            ifinalstates.append(idaughterfinalstates)

        finalstates.append(ifinalstates)
        daughters.append(idaughters)
        mothers.append(hgencand)

    if(debug==True) : printDecayTree(mothers,daughters,finalstates)
    
    return mothers,daughters,finalstates

# 
# debugger
#
def printDecayTree(mothers,daughters,finalstates) :
    
    for im in xrange(0,len(mothers)) :
        outstr = str(mothers[im].pdgId())+' -> '
        for id in xrange(0,len(daughters[im])) :
            outstr += str(daughters[im][id].pdgId()) + ' ( '
            for ifs in xrange(0,len(finalstates[im][id])) :
                outstr +=  str(finalstates[im][id][ifs].pdgId()) + ' '
            outstr += ') '
        print outstr
        

#
#
#
def getDecayCounters(mothers,daughters,finalstates) :

    nmuons=0
    nelecs=0
    ntaus=0
    nneutrinos=0
    njets=0
    for im in xrange(0,len(mothers)) :
        for id in xrange(0,len(daughters[im])) :
            for ifs in xrange(0,len(finalstates[im][id])) :
                pdgid=abs(finalstates[im][id][ifs].pdgId())
                if(pdgid==11) : nelecs+=1
                if(pdgid==13) : nmuons+=1
                if(pdgid==15) : ntaus+=1
                if(pdgid==12 or pdgid==14 or pdgid==16) : nneutrinos+=1
                if(pdgid<6): njets+=1
    return nmuons,nelecs,ntaus,nneutrinos,njets
        

def convertSignal(filein,fileout):
    f = open(filein)
    ff=open(fileout,'w')

    ff.write("from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator\n")
    ff.write("kreator = ComponentCreator()\n")  
    ff.write('signalSamples=[] \n')
    for line in f:
        prefix = line.split('/')[1]
        mass = prefix.split('M-')[1].split('_')[0]
        name=prefix.split('M-')[0]+mass 
        ff.write('{name}=kreator.makeMCComponent("{name}", "{sample}", "CMS", ".*root",1.0)\n'.format(name=name,sample=line.split('\n')[0]))
        ff.write('signalSamples.append({name})\n'.format(name=name))
    f.close()
    ff.close()

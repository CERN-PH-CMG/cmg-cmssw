import copy

def sampleShift( locals, inputList, ext ):
    '''Generate copies of components in inputList.
    Returns a list containing the copies, and add the copies
    to the locals namespace of the context calling this function.
    set local to locals() when calling the function.'''
    newList = []
    for sam in inputList:
        newName = '_'.join( [sam.name, ext] )
        newSam = copy.deepcopy(sam)
        newSam.name = newName
        locals[newName] = newSam
        newList.append( newSam )
    return newList

    
def selectShift(components, shift='Up'):    
    shiftNames = []
    mods = []
    newComps = []
    if shift == 'Nom':
        for c in components:
            if not c.name.endswith('Up') and not c.name.endswith('Down'):
                newComps.append(c)
        return newComps
    for c in components:
        if c.name.endswith(shift):
            shiftNames.append(c.name)
            mods.append(c.name.replace('_Up','').replace('_Down',''))
    for c in components:
        base = c.name.replace('_Up','').replace('_Down','')
        if c.name in shiftNames:
            newComps.append(c)
        elif base in mods:
            continue
        else:
            newComps.append(c)
    return newComps

        

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
    

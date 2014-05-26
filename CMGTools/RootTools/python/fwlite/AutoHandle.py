#ROOTTOOLS
from DataFormats.FWLite import Events, Handle
        

class AutoHandle( Handle, object ):
    '''Handle + label.'''

    handles = {}
    
    def __init__(self, label, type, mayFail=False, fallbackLabel=None):
        '''Note: label can be a tuple : (module_label, collection_label, process)'''
        self.label = label
        self.fallbackLabel = fallbackLabel
        self.type = type
        self.mayFail = mayFail
        Handle.__init__(self, self.type)

    def Load(self, event):
        '''Load self from a given event.

        Call this function, and then just call self.product() to get the collection'''
        try:
            event.getByLabel( self.label, self)
        except ValueError:
            errstr = '''
            Cannot find collection with:
            type = {type}
            label = {label}
            '''.format(type = self.type, label = self.label)
            if not self.mayFail and self.fallbackLabel == None:
                raise ValueError(errstr)
            if self.fallbackLabel != None:
                try:
                    event.getByLabel( self.fallbackLabel, self)
                except ValueError:
                    errstr = '''
                    Cannot find collection with:
                    type = {type}
                    label = {label} or {lab2}
                    '''.format(type = self.type, label = self.label, lab2 = self.fallbackLabel)
                    if not self.mayFail:
                        raise ValueError(errstr)



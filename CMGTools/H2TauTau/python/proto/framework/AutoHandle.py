#ROOTTOOLS
from DataFormats.FWLite import Events, Handle
        

class AutoHandle( Handle, object ):
    '''Handle + label.'''

    handles = {}
    
    def __init__(self, label, type):
        '''Note: label can be a tuple : (module_label, collection_label, process)'''
        self.label = label
        self.type = type
        Handle.__init__(self, self.type)

    def Load(self, event):
        '''Load self from a given event.

        Call this function, and then just call self.product() to get the collection'''
        event.getByLabel( self.label, self)

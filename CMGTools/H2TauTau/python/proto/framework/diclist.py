class diclist( list ):

    def __init__(self, *args, **kwargs):
        super( diclist, self).__init__(*args, **kwargs)
        self.dico = {}

    def add( self, key, value ):
        if key in self.dico:
            raise ValueError("key '{key}' already exists".format(key=key) )
        self.dico[key] = len(self)
        self.append(value)

    def __getitem__(self, index):
        try:
            index = int( index )
            return super(diclist, self).__getitem__(index)
        except ValueError:
            return super(diclist, self).__getitem__( self.dico[index] )
            
    def __setitem__(self, index, value):
        try:
            index = int( index )
            return super(diclist, self).__setitem__(index, value)
        except ValueError:
            return super(diclist, self).__setitem__( self.dico[index], value )
            
if __name__ == '__main__':
    dl = diclist()
    dl.add('b', 1)
    dl.add('a', 2)
    dl.add('c', 3)

    print dl
    print dl['b'], dl['a']
    
    dl.add('c', 2)
    

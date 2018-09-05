import pprint
import copy
import collections
import fnmatch

from ROOT import TChain

class Event(object):
    '''Event class.

    The Looper passes an Event object to each of its Analyzers,
    which in turn can:
    - read some information
    - add more information
    - modify existing information.

    A printout can be obtained by doing e.g.:
    
      event = Event() 
      print event 

    The printout can be controlled by the following class attributes:
      print_nstrip : number of items in sequence to be printed before stripping the following items
      print_patterns : list of patterns. By default, this list is set to ['*'] so that all attributes are
                    printed

    Example:
      event = Event()
      Event.print_nstrip = 5  # print only the 5 first items of sequences
      Event.print_patterns = ['*particles*', 'jet*'] # only print the attributes that 
                                                     # contain "particles" in their name or
                                                     # have a name starting by "jet" 
       
    Object attributes:
      iEv: event processing index, starting at 0
      eventWeight: a weight, set to 1 at the beginning of the processing
      input: input, as determined by the looper
      analyzers: list of analyzers that processed this event, with their result, in the form:
          [(analyzer_name, result?), ...]
    #TODO: provide a clear interface for access control (put, get, del products) - we should keep track of the name and id of the analyzer.
    '''

    print_nstrip = 10
    print_patterns = ['*']

    def __init__(self, iEv, input_data=None, setup=None, eventWeight=1):
        self.iEv = iEv
        self.input = input_data
        self.setup = setup
        self.eventWeight = eventWeight
        self.analyzers = []

    def _get_print_attrs(self, subname=""):
        '''returns a dict of printable information of an event
        arguments
        * subname is used when called recursively and is the name of the parent object'''
        selected_attrs = copy.copy(self.__dict__) #initial selection of what we can print
        selected_attrs.pop('setup') #get rid of some bits
        selected_attrs.pop('input')
        
        # Colin: defining stripped_attrs
        stripped_attrs = dict()
        
        #first of all check for matches with print patterns
        for name, value in selected_attrs.iteritems():
            if any([fnmatch.fnmatch(name, pattern) for pattern in self.__class__.print_patterns]):
                stripped_attrs[name] = value
        for name, value in stripped_attrs.iteritems():
            if hasattr(value, '__len__') and \
               hasattr(value.__len__, '__call__') and \
               len(value)>self.__class__.print_nstrip+1:
                # taking the first 10 elements and converting to a python list 
                # note that value could be a wrapped C++ vector
                if isinstance(value, collections.Mapping):
                    entries = [entry for entry in value.iteritems()]
                    entries = entries[:self.__class__.print_nstrip]
                    entries
                    stripped_attrs[name] = dict(entries)
                else:
                    stripped_attrs[name] = [ val for val in value[:self.__class__.print_nstrip] ]
                    stripped_attrs[name].append('...')
                    stripped_attrs[name].append(value[-1])
        return stripped_attrs

    def _print_elements(self, name, value):
        '''returns a dict ready for printing (limited to print_nstrip elements)
        Note this function allows for lists or for dicts of dicts
        contents of lists are not handled recursively
        arguments 
        * name = name of attribute
        * value = its value
        '''
        newdata=dict()
        if hasattr(value, '__len__') and isinstance(value, collections.Mapping): #dict:      
            subdict = dict()
            for newname, entry in value.iteritems(): #allow recursion in case this dict contains a dict
                subdict.update(self._print_elements(newname, entry)) 
            if len(value) > self.__class__.print_nstrip+1: #use only part of the dict
                entries = [entry for  entry in subdict.iteritems()]
                entries = entries[:self.__class__.print_nstrip]
                entries.append(("...", "...")) # no guarantees where abouts this is printed
                newdata[name] = dict(entries) 
            else: #not too big so using whole dict is OK
                newdata[name] = subdict 
        elif hasattr(value, '__len__') and len(value)>self.__class__.print_nstrip+1: #list 
            newdata[name] = [val for val in value[:self.__class__.print_nstrip]]
            newdata[name].append('...')
            newdata[name].append(value[-1])   
        else:
            newdata[name] = value
        return newdata    

    def __str__(self):
        #prints an event showing at most print_nstrip elements of lists and dicts
        # if an event contains an event (such as a papasevent)
        # it will print the papasevent in the same way
        header = '{type}: {iEv}'.format(type=self.__class__.__name__, iEv=self.iEv)
        print_attrs = self._get_print_attrs()
        contents = pprint.pformat(print_attrs, indent=4)
        return '\n'.join([header, contents])

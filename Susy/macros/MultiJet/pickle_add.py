import pickle, os

def mergeNorms(files):
    
    bins = {}

    for f in files:
        name = f
        if not os.path.exists(name): continue
        norm = pickle.load(file(name))
        for key in norm.keys():
            if bins.has_key(key):
                bins[key] = bins[key] + norm[key]
            else:
                bins[key] = norm[key]
    return bins

if __name__ == '__main__':

    from optparse import OptionParser
    parser = OptionParser()

    parser.add_option("-o", "--output", dest="output",
                  help="The output file name", default='out.pkl')

    (options,args) = parser.parse_args()

    bins = mergeNorms(args)
    pickle.dump(bins,file(options.output,'wb'))
    

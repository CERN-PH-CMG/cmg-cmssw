import sys
import os

cat = sys.argv[1]

def display(fnam):
    os.system('display {fnam} &'.format(fnam=fnam))

fnam1 = 'HCP-121101/postfit/sm/eleTau_{cat}_unscaled_7TeV_.png'.format(cat=cat)
fnam2 = 'Prod_Nov1/Datacards/DataAndBackground/{cat}/svfitMass.png'.format(cat=cat)

display(fnam1)
display(fnam2)


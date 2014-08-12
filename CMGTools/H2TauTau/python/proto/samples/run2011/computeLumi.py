import os

def computeLumi(comp, test=True):
    lumiCalc = 'lumiCalc2.py'
    for trigpat in comp.triggers:
        trigbase = trigpat.replace('*','X')
        trigre = trigpat.replace('*','.*')
        cmd = 'nohup {lumiCalc} -i {json} recorded --hltpath {trigpat} | grep -E "{trigre}" > {name}_{trigbase}.txt &'.format(
            name = comp.name,
            lumiCalc = lumiCalc,
            json = comp.json,
            trigpat = trigpat,
            trigbase = trigbase,
            trigre = trigre
            )
        print cmd
        if not test:
            os.system(cmd)
        

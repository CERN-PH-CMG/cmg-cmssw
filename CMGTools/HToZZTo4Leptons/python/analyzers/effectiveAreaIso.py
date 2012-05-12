
def effectiveAreaIso(object,rho):
    EffectiveArea = 0.0
    if abs(object.eta()) >= 0.0 and abs(object.eta()) < 1.0 :   EffectiveArea = 0.340;
    if abs(object.eta()) >= 1.0 and abs(object.eta()) < 1.479 : EffectiveArea = 0.310;
    if abs(object.eta()) >= 1.479 and abs(object.eta()) < 2.0 : EffectiveArea = 0.315;
    if abs(object.eta()) >= 2.0 and abs(object.eta()) < 2.2 :   EffectiveArea = 0.415;
    if abs(object.eta()) >= 2.2 and abs(object.eta()) < 2.3 :   EffectiveArea = 0.658;
    if abs(object.eta()) >= 2.3 :                                EffectiveArea = 0.405;

    return object.chargedHadronIso()+max(0.,object.neutralHadronIso()+object.photonIso()-rho*EffectiveArea)

def addEffectiveAreaIso(list,rho):
    for object in list:
        object.eaIso = effectiveAreaIso(object,rho)
        

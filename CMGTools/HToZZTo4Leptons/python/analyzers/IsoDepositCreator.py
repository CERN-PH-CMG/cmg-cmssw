from CMGTools.RootTools.utils.DeltaR import deltaR

       ####Associate charged Hadrons to vertices
class IsoDepositCreator(object):
           def __init__(self,vertices,hadrons):
               self.vertices=vertices
               self.hadrons=hadrons


           def getIsolation(self,object):
               deposits=[]
               for vertex in self.vertices:
                   deposits.append(0.0)

               for hadron in self.hadrons:
                   if deltaR(object.eta(),object.phi(),hadron.eta(),hadron.phi())<0.4:
                       i=self.associatedVertex(hadron)
                       deposits[i]=deposits[i]+hadron.pt()


               return deposits
           


           def associatedVertex(self,hadron):
               minVertex=0
               minDZ=10000
               
               for i,vertex in enumerate(self.vertices):
                   dz=abs(hadron.trackRef().dz(vertex.position()))
                   if dz<minDZ:
                     minDZ=dz
                     minVertex=i
                       
               return minVertex    



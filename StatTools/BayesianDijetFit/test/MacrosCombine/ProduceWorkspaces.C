#include "R2JJFitter.cc"

void ProduceWorkspaces(){
  bool isWW = true;
 
  for (int i = 10; i < 11; i++){

    int mass = 1000+i*100;
    cout << "mass = " << mass << endl; 
    //    runfits(mass, true);
    runfits(mass, false);

  }


}

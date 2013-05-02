#include "R2JJFitter.cc"

void ProduceWorkspaces(){
 
  for (int i = 0; i < 21; i++){

    int mass = 1000+i*100;
    cout << "mass = " << mass << endl; 
    runfits(mass, true);
    runfits(mass, false);

  }

}

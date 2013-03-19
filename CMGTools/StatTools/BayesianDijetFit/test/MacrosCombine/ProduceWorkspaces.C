#include "R2JJFitter.cc"

void ProduceWorkspaces(){
 
  for (int i = 0; i < 14; i++){

    int mass = 1000+i*100;
    cout << "mass = " << mass << endl; 
    runfits(mass, true);
    runfits(mass, false);

  }

}

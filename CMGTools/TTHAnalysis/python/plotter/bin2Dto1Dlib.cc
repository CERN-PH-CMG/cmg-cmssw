#include <assert.h>

void bin2Dto1D_copyarray(float *s, float *t, int n){
  for (int i=0; i<n; i++) *(t+i)=*(s+i);
}

float bin2Dto1D(float x1, float x2, int binning_code)
{

  int n1 = 0;
  int n2 = 0;
  const int maxbins = 10;
  float b1[maxbins];
  float b2[maxbins];

  if (binning_code==0){
    const int n1_ = 6;
    float c1[n1_]={10,15,25,35,50,70};
    const int n2_ = 4;
    float c2[n2_]={0,1,2,2.5};
    bin2Dto1D_copyarray(c1,b1,n1_);
    bin2Dto1D_copyarray(c2,b2,n2_);
    n1=n1_;
    n2=n2_;
  }
  if (binning_code==1){
    const int n1_ = 6;
    float c1[n1_]={10,25,35,70};
    const int n2_ = 4;
    float c2[n2_]={0,1,2,2.5};
    bin2Dto1D_copyarray(c1,b1,n1_);
    bin2Dto1D_copyarray(c2,b2,n2_);
    n1=n1_;
    n2=n2_;
  }

  assert (n1!=0 && n2!=0);

  int a1=0; int a2=0;
  while (a1<n1-2 && x1>=b1[a1+1]) a1++;
  while (a2<n2-2 && x2>=b2[a2+1]) a2++;
  return a2*(n1-1)+a1+1;

}

void bin2Dto1Dlib(){}

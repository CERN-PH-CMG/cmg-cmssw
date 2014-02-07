      PARAMETER (maxpro= 10)
      REAL*8 cross, sthr, reduc
      REAL*8 chnnz, chnni, chnns
      COMMON / cropro / cross(maxpro), sthr(maxpro), reduc(maxpro)
      CHARACTER*18 chapro(maxpro)
      DATA chapro /
     .             'e+e- --> h Z    ',
     .             'e+e- --> H Z    ',
     .             'e+e- --> h A    ',
     .             'e+e- --> H A    ',
     .             'W+W- --> h      ',
     .             'W+W- --> H      ',
     .             'Z Z  --> h      ',
     .             'Z Z  --> H      ',
     .             'e+e- --> H+H-   ',
     .             'e+e- --> h gamma'
     .                                /
      COMMON / prosim / iproyn(maxpro),nevpro(maxpro),interf(maxpro)
      COMMON / wwzzin / chnnz(maxpro),chnni(maxpro),chnns(maxpro)

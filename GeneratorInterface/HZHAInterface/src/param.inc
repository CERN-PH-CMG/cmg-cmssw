      REAL*8 amhig, amh, gmh, ama, amz, amw, gmz,
     .       amtau, amb, amc, amt, ame, ammu, amu,
     .       amd, ams, amhp, gmw, amst, amsb,
     .       amsq, amneut, amchar, amarun
      REAL*8 tauh 
      REAL*8 xlamda5
      REAL*8 wwmax, w1, w2
      REAL*8 alfa, beta, topmix, botmix,
     .       aa, bb, fieldn, umat, vmat,
     .       ssmat, qqmat
      REAL*8 sa, ca, sb, cb, ta, tb, sab2, cab2,
     .       s2a, c2a, s2b, c2b, sb2, cb2, cab, sab,
     .       corhbb, corgbb
      REAL*8 susM, susMU, susAt, susAb, susSMQ, susSMU,
     .       susSMD, susSML, susSME, susM1, susM2, susM3,
     .       susM0, susM12, susMSNU, susGLU, suspar
      REAL*8 ecs, crs, wsup, wtot, wtot2 
C
      PARAMETER ( nchan=16, nhig=3 )
      COMMON / hmasss / amhig(nhig), amh, gmh, ama, amz, amw, gmz,
     .                  amtau, amb, amc, amt, ame, ammu, amu,
     .                  amd, ams, amhp, gmw, amst(2), amsb(2),
     .                  amsq, amneut(4),amchar(2), amarun
      COMMON / lifeti / tauh(nhig)
      COMMON / conqcd / xlamda5
      COMMON / wwzzch / wwmax(2,nhig), jtyp(2,nhig), w1, w2
      COMMON / mixing / alfa, beta, topmix, botmix,
     .                  aa(nhig,4,4),bb(nhig,2,2),
     .                  fieldn(4,4), umat(2,2), vmat(2,2),
     .                  ssmat(4,4),qqmat(4,4)
      COMMON / coupls / sa, ca, sb, cb, ta, tb, sab2, cab2,
     .                  s2a, c2a, s2b, c2b, sb2, cb2, cab, sab,
     .                  corhbb, corgbb
      COMMON / susyms / susM, susMU, susAt, susAb, susSMQ, susSMU,
     .                  susSMD, susSML, susSME, susM1, susM2, susM3,
     .                  susM0, susM12, susMSNU, susGLU
      COMMON / flags  / idbg
      DIMENSION suspar(12)
      EQUIVALENCE(susM,suspar(1))
C
      PARAMETER(nstep=20)
      COMMON / crocro / ecs(nstep),crs(nstep),wsup(nstep)
      COMMON / poidsm / wtot(4),wtot2(4),ntry(4),nacc(4)
C

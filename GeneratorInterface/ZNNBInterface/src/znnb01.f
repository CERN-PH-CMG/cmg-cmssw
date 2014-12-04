      SUBROUTINE znnbin(ecm)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'parznn.h'
C      REAL*8 y, pt, distrpty
      include 'crosec.h'
C
      crs(nstep) = znnbsi(ecm)
      WRITE(6,*) 'Born cross section = ', crs(nstep),' pb'
      ecs(nstep) = ecm
C
      emin = 93.
      emax = ecm
      DO istep = 1, nstep-1
        ecs(istep) = emin + FLOAT(istep-1)/FLOAT(nstep-1)*(emax-emin)
        crs(istep) = znnbsi(ecs(istep))
      ENDDO
C
      rs = ecm/1000.
      s =rs**2
C
      wmax = 0.
      ymax=LOG(rs/z)
      ny = 100
      npt = 100
      ptx = 0.
      yx = 0.
      DO iy = 1, ny
        y = ymax*FLOAT(iy)/FLOAT(ny)
        ptmax = SQRT( ( (s+z2)/(2*rs*COSH(y)) )**2-z2 )
        DO ipt = 1, npt
          pt = ptmax*FLOAT(ipt)/FLOAT(npt)
          wpt = 2. * distrpty(pt,y) * ymax * ptmax
          IF ( wpt .GT. wmax ) THEN 
            wmax = wpt
            ptx = pt
            yx = y
          ENDIF
        ENDDO
      ENDDO
      wmax = wmax*1.5
C
      wtot = 0.
      wtot2 = 0.
      ntry = 0
      nacc = 0
C
      RETURN
      END

      DOUBLE PRECISION FUNCTION ZNNBSI(ecm)
********************************************************************
C Computes the total cross-section for the process
C e+e- --> Z nu nubar (neglecting Z-exchange diagrams and keeping
C all the W-exchange ones). Output in the file FOR080.DAT
********************************************************************
C   by S.Ambrosanio and B.Mele (1991)
********************************************************************
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'parznn.h'
      EXTERNAL DISTRPT,DISTRY
C
      RS = ecm/1000.
      S=RS**2
C
C       limits on ABS(rapidity) and transverse momentum of the Z boson:
C
      YMXABS=DLOG(RS/Z)
      PTMXABS=(S-Z2)/2./RS
C
C       Here one computes the total cross-section integrating
C       dsigma/dpt dy with two different orders of the integration
C       variables (as a check of the integral)
C
      SIGTOTPT=DGAUSS(DISTRPT,0D0,PTMXABS,1.D-6)
      SIGTOTY =DGAUSS(DISTRY,0D0,YMXABS,1.D-6)
      DIFF=(SIGTOTPT-SIGTOTY)/SIGTOTY
      znnbsi = (sigtotpt+sigtoty)/2.
      RETURN
      END

***************************************************************
C       This is the trans.mom. distribution
      DOUBLE PRECISION FUNCTION DISTRPT (PT)
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      include 'parznn.h'
      EXTERNAL FSUB1
      DIMENSION XX(2)
C
C       YMAX is the maximum of the absolute value of the rapidity
C
      X=(S+Z2)/(2.*RS*SQRT(PT**2+Z2))
      YMAX=LOG(X+SQRT(X**2-1.))
      YMIN=0.
C
      XX(2) = PT
      DISTRPT = 2.* DGMLT1(FSUB1,YMIN,YMAX,10,6,XX)
C
      RETURN
      END

*******************************************************************
C       This is the rapidity distribution
      DOUBLE PRECISION FUNCTION DISTRY(Y)

      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      include 'parznn.h'
      EXTERNAL FSUB2
      DIMENSION XX(2)
C
C       PTMAX is the max. value of the trans.mom.
C
      PTMAX=SQRT(((S+Z2)/(2*RS*COSH(Y)))**2-Z2)
      PTMIN = 0.
C
      XX(2) = Y
      DISTRY = 2.* DGMLT1(FSUB2,PTMIN,PTMAX,10,6,XX)
C
      RETURN
      END

**************************************************************
C  The following function is used for checking the integral

      DOUBLE PRECISION FUNCTION DISTRYPT (Y,PT)
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      EXTERNAL DISTRPTY
C
      DISTRYPT = DISTRPTY (PT,Y)
C
      RETURN
      END


**************************************************************
**************************************************************
C  The following subroutine gives the double distribution
C  in the transverse momemtum and rapidity of Z boson, well
C  normalized in units of pb/TeV.
C  It's expressed as a function of the three invariants: S,CP,CM

      DOUBLE PRECISION FUNCTION DISTRPTY (PT,Y)
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      include 'parznn.h'
C
      AMT=SQRT(Z2+PT**2)
      EZ=AMT*COSH(Y)
      PZ=SQRT(EZ**2-Z2)
      CTHZ=AMT*SINH(Y)/PZ

C       ----------------------------------------
      CP=RS*(EZ+PZ*CTHZ)
      CM=RS*(EZ-PZ*CTHZ)
C       this are the two main Lorentz-invariants
C       expressed in the C.o.M. frame
C       ---------------------------------------

      C2=CP*CM-S*Z2
      C0=S*(S+Z2-CP-CM)

      AP=S*(CP-Z2)
      AM=S*(CM-Z2)

      BP=2.*S*(C2+CP*(CP-Z2)-Z2*(S-CM))
      BM=2.*S*(C2+CM*(CM-Z2)-Z2*(S-CP))

      RPCM=CM*(S-CM)-C2
      RMCP=CP*(S-CP)-C2

      ALWS=C2+W2*(2.*S+2.*W2-CP-CM)
      ALWP=RMCP+W2*(CP+CM)
      ALWM=RPCM+W2*(CP+CM)

      ALPWP=AP+W2*(CP+CM)
      ALPWM=AM+W2*(CP+CM)

      BEWS=W2**2*(S+W2-CP)*(S+W2-CM)
      BEWP=S*W2*Z2*(S+W2-CP)
      BEWM=S*W2*Z2*(S+W2-CM)

      OGP=LOG(1.+(S-CP)/W2)
      OGM=LOG(1.+(S-CM)/W2)
      OGS=LOG(2.*EZ*(EZ+PZ)/Z2-1.)

      OGWP=LOG(ALWP*(ALWP+SQRT(ALWP**2-4.*BEWP))/2./BEWP-1.)
      OGWM=LOG(ALWM*(ALWM+SQRT(ALWM**2-4.*BEWM))/2./BEWM-1.)
      OGWS=LOG(ALWS*(ALWS+SQRT(ALWS**2-4.*BEWS))/2./BEWS-1.)

      OGPWP=LOG(ALPWP*(ALPWP+SQRT(ALPWP**2-4.*BEWM))/2./BEWM-1.)
      OGPWM=LOG(ALPWM*(ALPWM+SQRT(ALPWM**2-4.*BEWP))/2./BEWP-1.)

C       =======================================
C       Analytic expressions for the integrals
C       on neutrinos variables
C       =======================================

      AI1P=-OGP/(S-CP)
      AI1M=-OGM/(S-CM)
      AI1S=OGS/(2.*RS*PZ)

      AI2P=S*Z2/BEWP
      AI2M=S*Z2/BEWM

      AI3P=-OGPWP/SQRT(ALPWP**2-4.*BEWM)
      AI3M=-OGPWM/SQRT(ALPWM**2-4.*BEWP)
      AI3S=OGWS/SQRT(ALWS**2-4.*BEWS)

      AI4P=-OGWP/SQRT(ALWP**2-4.*BEWP)
      AI4M=-OGWM/SQRT(ALWM**2-4.*BEWM)

      AI5P=((C0+W2*(S-CP))/(S+W2-CP)-(C2+W2*(S-CP))/W2
     *    -((S-CM)*(C2+W2*(S-CM))+W2*(C2-C0))*AI3S)/(ALWS**2-4.*BEWS)
      AI5M=((C0+W2*(S-CM))/(S+W2-CM)-(C2+W2*(S-CM))/W2
     *    -((S-CP)*(C2+W2*(S-CP))+W2*(C2-C0))*AI3S)/(ALWS**2-4.*BEWS)

      AI6S=((S-CP)**2/(W2*(S+W2-CP))+(S-CM)**2/(W2*(S+W2-CM))
     *    -12.*C0*C2/(ALWS**2-4.*BEWS)
     *    +(2.*(C2-C0)+6.*C0*C2*ALWS/(ALWS**2-4.*BEWS))*AI3S)
     *    /(ALWS**2-4.*BEWS)
      AI6P=(4.*PZ**2/Z2+(S-CM)**2/(W2*(S+W2-CM))
     *    -12.*C0*C2/(ALPWP**2-4.*BEWM)
     *    -(2.*(AP-RPCM)+6.*C0*C2*ALPWP/(ALPWP**2-4.*BEWM))*AI3P)
     *    /(ALPWP**2-4.*BEWM)
      AI6M=(4.*PZ**2/Z2+(S-CP)**2/(W2*(S+W2-CP))
     *    -12.*C0*C2/(ALPWM**2-4.*BEWP)
     *    -(2.*(AM-RMCP)+6.*C0*C2*ALPWM/(ALPWM**2-4.*BEWP))*AI3M)
     *    /(ALPWM**2-4.*BEWP)

      AI7P=(AP/W2-RPCM/(S+W2-CM)-(BP/2.+4.*S*PZ**2*W2)*AI3P)
     *    /(ALPWP**2-4.*BEWM)
      AI7M=(AM/W2-RMCP/(S+W2-CP)-(BM/2.+4.*S*PZ**2*W2)*AI3M)
     *    /(ALPWM**2-4.*BEWP)

      AI8P=(2.*W2+2.*(S+W2-CM)-2.*EZ*ALPWP/(Z2*RS)
     *    +(W2*(AP-RPCM)+(S-CM)*AP)*AI3P)/(ALPWP**2-4.*BEWM)
      AI8M=(2.*W2+2.*(S+W2-CP)-2.*EZ*ALPWM/(Z2*RS)
     *    +(W2*(AM-RMCP)+(S-CP)*AM)*AI3M)/(ALPWM**2-4.*BEWP)

      AI9P=(C2*OGP-(C2-C0)*(1.-W2*OGP/(S-CP)))/(S-CP)**2
      AI9M=(C2*OGM-(C2-C0)*(1.-W2*OGM/(S-CM)))/(S-CM)**2

      AI10P=-(C2*(S-CP)/W2+(C2-C0)*(1.-(1.+W2/(S-CP))*OGP))
     *    /((S-CP)**2*(S+W2-CP))
      AI10M=-(C2*(S-CM)/W2+(C2-C0)*(1.-(1.+W2/(S-CM))*OGM))
     *    /((S-CM)**2*(S+W2-CM))

      AI11P=(AP-RPCM-BP*AI1S/2.)/4./S/PZ**2
      AI11M=(AM-RMCP-BM*AI1S/2.)/4./S/PZ**2
C     =======================================================

      SD=1./S
      CPD=1./CP
      CMD=1./CM
      CSD=1./(CP+CM)
      CSZD=1./(2.*Z2-CP-CM)
      CPZD=1./(Z2-CP)
      CMZD=1./(Z2-CM)

C       ======================================================
C        Here we define ELMS that is the square of the matrix
C        element (averaged
C        and summed over the spins) integrated over the
C        neutrino variables. It is symmetric in the CP-CM
C        exchange (cos(thetaZ) --> -cos(thetaZ))
C        and is proportional to the invariant distribution
C        Ez*(dsigma/d3pz)
C       ======================================================
C       Output of the SCOONSCHIP program

      XTOTAL=0.
      TLPART= -2.*SD
      XTOTAL=XTOTAL+TLPART
      HDPART=(XW-1./2)
      TLPART= 4.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=(XW-1./2)*(1.-XW)
      TLPART= -8.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=(1.-XW)
      TLPART= 4.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=(1.-XW)**2
      TLPART= -4.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=-8.*CPZD*(XW-1./2)
      XTOTAL=XTOTAL+HDPART
      HDPART=8.*CPZD*(XW-1./2)*(1.-XW)
      XTOTAL=XTOTAL+HDPART
      HDPART=4.*CPZD*(XW-1./2)**2
      XTOTAL=XTOTAL+HDPART
      HDPART=CPZD**2*(XW-1./2)**2
      TLPART= -4.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=-8.*CMZD*(XW-1./2)
      XTOTAL=XTOTAL+HDPART
      HDPART=8.*CMZD*(XW-1./2)*(1.-XW)
      XTOTAL=XTOTAL+HDPART
      HDPART=4.*CMZD*(XW-1./2)**2
      XTOTAL=XTOTAL+HDPART
      HDPART=CMZD**2*(XW-1./2)**2
      TLPART= -4.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=CSZD*(XW-1./2)**2
      TLPART= 4.*CP*ZD2 +4.*CM*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=CSZD*CPZD*(XW-1./2)**2
      TLPART= -4.*CM -4.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=CSZD*CMZD*(XW-1./2)**2
      TLPART= -4.*CP -4.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=CSD
      TLPART= -1.*CP*ZD2 -1.*CM*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P
      TLPART= 1./2.*S*ZD2 -1./2.*CM*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P*(XW-1./2)
      TLPART= -2. +2.*S*ZD2 -4.*CP*ZD2 +2.*CM*ZD2 +4.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P*(XW-1./2)*(1.-XW)
      TLPART= 4. -4.*S*ZD2 +8.*CP*ZD2 -4.*CM*ZD2 -8.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P*(XW-1./2)**2
      TLPART= 2.*S*ZD2 -2.*CM*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P*(1.-XW)
      TLPART= 2. +2.*S*ZD2 -4.*CP*ZD2 +2.*CM*ZD2 +4.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P*(1.-XW)**2
      TLPART= -4. -2.*S*ZD2 +4.*CP*ZD2 -2.*CM*ZD2 -4.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P*CPZD*(XW-1./2)
      TLPART= -4.*S -4.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P*CPZD*(XW-1./2)*(1.-XW)
      TLPART= 4.*S +8.*W2 +4.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P*CMZD*(XW-1./2)
      TLPART= -6.*S +4.*CP -8.*W2 -2.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P*CMZD*(XW-1./2)*(1.-XW)
      TLPART= 16.*S -12.*CP +16.*W2 +4.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P*CMZD*(XW-1./2)**2
      TLPART= 4.*S -8.*CP +8.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P*CMZD**2*(XW-1./2)**2
      TLPART= -8.*S*Z2 +8.*CP*Z2 -8.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P*CSZD*(XW-1./2)**2
      TLPART= 16.*S +4.*S*CP*ZD2 +4.*S*CM*ZD2 -4.*CP -4.*CP*CM*ZD2
     1  +4.*CP*W2*ZD2 -4.*CP**2*ZD2 -8.*CM +4.*CM*W2*ZD2 +4.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P*CSZD*CPZD*(XW-1./2)**2
      TLPART= -12.*S*CM +4.*S*W2 -4.*S*Z2 +8.*S**2 -4.*CM*W2
     1  +8.*CM*Z2 -8.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P*CSZD*CMZD*(XW-1./2)**2
      TLPART= -12.*S*CP +4.*S*W2 -4.*S*Z2 +8.*S**2 -8.*CP*W2
     1  +4.*CP*Z2 +4.*CP**2 -4.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1P*CSD
      TLPART= -1.*S*CP*ZD2 -1.*S*CM*ZD2 +1.*CP*CM*ZD2 -1.*CP*W2*ZD2
     1  +1.*CP**2*ZD2 -1.*CM*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M
      TLPART= 1./2.*S*ZD2 -1./2.*CP*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M*(XW-1./2)
      TLPART= -2. +2.*S*ZD2 +2.*CP*ZD2 -4.*CM*ZD2 +4.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M*(XW-1./2)*(1.-XW)
      TLPART= 4. -4.*S*ZD2 -4.*CP*ZD2 +8.*CM*ZD2 -8.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M*(XW-1./2)**2
      TLPART= 2.*S*ZD2 -2.*CP*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M*(1.-XW)
      TLPART= 2. +2.*S*ZD2 +2.*CP*ZD2 -4.*CM*ZD2 +4.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M*(1.-XW)**2
      TLPART= -4. -2.*S*ZD2 -2.*CP*ZD2 +4.*CM*ZD2 -4.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M*CPZD*(XW-1./2)
      TLPART= -6.*S +4.*CM -8.*W2 -2.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M*CPZD*(XW-1./2)*(1.-XW)
      TLPART= 16.*S -12.*CM +16.*W2 +4.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M*CPZD*(XW-1./2)**2
      TLPART= 4.*S -8.*CM +8.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M*CPZD**2*(XW-1./2)**2
      TLPART= -8.*S*Z2 +8.*CM*Z2 -8.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M*CMZD*(XW-1./2)
      TLPART= -4.*S -4.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M*CMZD*(XW-1./2)*(1.-XW)
      TLPART= 4.*S +8.*W2 +4.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M*CSZD*(XW-1./2)**2
      TLPART= 16.*S +4.*S*CP*ZD2 +4.*S*CM*ZD2 -8.*CP -4.*CP*CM*ZD2
     1  +4.*CP*W2*ZD2 -4.*CM +4.*CM*W2*ZD2 -4.*CM**2*ZD2 +4.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M*CSZD*CPZD*(XW-1./2)**2
      TLPART= -12.*S*CM +4.*S*W2 -4.*S*Z2 +8.*S**2 -8.*CM*W2
     1  +4.*CM*Z2 +4.*CM**2 -4.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M*CSZD*CMZD*(XW-1./2)**2
      TLPART= -12.*S*CP +4.*S*W2 -4.*S*Z2 +8.*S**2 -4.*CP*W2
     1  +8.*CP*Z2 -8.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1M*CSD
      TLPART= -1.*S*CP*ZD2 -1.*S*CM*ZD2 +1.*CP*CM*ZD2 -1.*CP*W2*ZD2
     1  -1.*CM*W2*ZD2 +1.*CM**2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=8.*AI1S*(XW-1./2)
      XTOTAL=XTOTAL+HDPART
      HDPART=-8.*AI1S*(1.-XW)
      XTOTAL=XTOTAL+HDPART
      HDPART=AI1S*CPZD*(XW-1./2)
      TLPART= 20.*S -8.*CM +16.*W2 +4.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1S*CMZD*(XW-1./2)
      TLPART= 20.*S -8.*CP +16.*W2 +4.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI1S*CSD
      TLPART= 4.*S -4.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2P
      TLPART= -1./2.*S*CP*ZD2 -1./2.*S*CM*ZD2 +1./2.*S*W2*ZD2
     1  +1./2.*S**2*ZD2 -1.*CP +1./2.*CP*CM*ZD2 -1./2.*CM*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2P*(XW-1./2)
      TLPART= 2.*S*CP*ZD2 +2.*S*CM*ZD2 -2.*S*W2*ZD2 -2.*S**2*ZD2
     1  +2.*CP -2.*CP*CM*ZD2 +2.*CM*W2*ZD2 -2.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2P*(XW-1./2)*(1.-XW)
      TLPART= -4.*S*CP*ZD2 -4.*S*CM*ZD2 +4.*S*W2*ZD2 +4.*S**2*ZD2
     1  -4.*CP +4.*CP*CM*ZD2 -4.*CM*W2*ZD2 +8.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2P*(XW-1./2)**2
      TLPART= -2.*S*CP*ZD2 -2.*S*CM*ZD2 +2.*S*W2*ZD2 +2.*S**2*ZD2
     1  +2.*CP*CM*ZD2 -2.*CM*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2P*(1.-XW)
      TLPART= 2.*S*CP*ZD2 +2.*S*CM*ZD2 -2.*S*W2*ZD2 -2.*S**2*ZD2
     1  +4.*CP -2.*CP*CM*ZD2 +2.*CM*W2*ZD2 +2.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2P*(1.-XW)**2
      TLPART= -2.*S*CP*ZD2 -2.*S*CM*ZD2 +2.*S*W2*ZD2 +2.*S**2*ZD2
     1  -4.*CP +2.*CP*CM*ZD2 -2.*CM*W2*ZD2 -4.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2P*CMZD*(XW-1./2)
      TLPART= 2.*S*CP -6.*S*W2 +4.*CP*W2 -2.*CP**2 -2.*W2*Z2
     1  -4.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2P*CMZD*(XW-1./2)*(1.-XW)
      TLPART= -4.*S*CP +12.*S*W2 +4.*S*Z2 -8.*CP*W2 -4.*CP*Z2
     1  +4.*CP**2 +4.*W2*Z2 +8.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2P*CMZD*(XW-1./2)**2
      TLPART= -4.*S*CP +4.*S*W2 -8.*CP*W2 +4.*CP**2 +4.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2P*CMZD**2*(XW-1./2)**2
      TLPART= 8.*S*CP*Z2 -8.*S*W2*Z2 -4.*S**2*Z2 +8.*CP*W2*Z2
     1  -4.*CP**2*Z2 -4.*W2**2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2M
      TLPART= -1./2.*S*CP*ZD2 -1./2.*S*CM*ZD2 +1./2.*S*W2*ZD2
     1  +1./2.*S**2*ZD2 +1./2.*CP*CM*ZD2 -1./2.*CP*W2*ZD2 -1.*CM
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2M*(XW-1./2)
      TLPART= 2.*S*CP*ZD2 +2.*S*CM*ZD2 -2.*S*W2*ZD2 -2.*S**2*ZD2
     1  -2.*CP*CM*ZD2 +2.*CP*W2*ZD2 +2.*CM -2.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2M*(XW-1./2)*(1.-XW)
      TLPART= -4.*S*CP*ZD2 -4.*S*CM*ZD2 +4.*S*W2*ZD2 +4.*S**2*ZD2
     1  +4.*CP*CM*ZD2 -4.*CP*W2*ZD2 -4.*CM +8.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2M*(XW-1./2)**2
      TLPART= -2.*S*CP*ZD2 -2.*S*CM*ZD2 +2.*S*W2*ZD2 +2.*S**2*ZD2
     1  +2.*CP*CM*ZD2 -2.*CP*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2M*(1.-XW)
      TLPART= 2.*S*CP*ZD2 +2.*S*CM*ZD2 -2.*S*W2*ZD2 -2.*S**2*ZD2
     1  -2.*CP*CM*ZD2 +2.*CP*W2*ZD2 +4.*CM +2.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2M*(1.-XW)**2
      TLPART= -2.*S*CP*ZD2 -2.*S*CM*ZD2 +2.*S*W2*ZD2 +2.*S**2*ZD2
     1  +2.*CP*CM*ZD2 -2.*CP*W2*ZD2 -4.*CM -4.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2M*CPZD*(XW-1./2)
      TLPART= 2.*S*CM -6.*S*W2 +4.*CM*W2 -2.*CM**2 -2.*W2*Z2
     1  -4.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2M*CPZD*(XW-1./2)*(1.-XW)
      TLPART= -4.*S*CM +12.*S*W2 +4.*S*Z2 -8.*CM*W2 -4.*CM*Z2
     1  +4.*CM**2 +4.*W2*Z2 +8.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2M*CPZD*(XW-1./2)**2
      TLPART= -4.*S*CM +4.*S*W2 -8.*CM*W2 +4.*CM**2 +4.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI2M*CPZD**2*(XW-1./2)**2
      TLPART= 8.*S*CM*Z2 -8.*S*W2*Z2 -4.*S**2*Z2 +8.*CM*W2*Z2
     1  -4.*CM**2*Z2 -4.*W2**2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3P
      TLPART= 1.*CM +1.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3P*(XW-1./2)
      TLPART= 8.*S*CPD*W2 +4.*S**2*CPD -2.*CM +4.*CPD*W2**2 -2.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3P*(1.-XW)
      TLPART= 6.*S*CM*CPD -4.*S*CM*CPD**2*W2 +2.*S*CM*CPD**2*Z2
     1  -16.*S*CPD*W2 -6.*S*CPD*Z2 +8.*S*CPD**2*W2*Z2
     1  +8.*S*CPD**2*W2**2 -2.*S*CPD**2*Z2**2 -4.*S**2*CPD
     1  +4.*S**2*CPD**2*W2 -2.*CM +10.*CM*CPD*W2 +2.*CM*CPD*Z2
     1  -2.*CM*CPD**2*W2*Z2 -4.*CM*CPD**2*W2**2 -2.*CM**2*CPD
     1  +2.*CM**2*CPD**2*W2 -14.*CPD*W2*Z2 -12.*CPD*W2**2
     1  +8.*CPD**2*W2**2*Z2 +4.*CPD**2*W2**3 -2.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3P*CPZD*(XW-1./2)
      TLPART= -4.*S*CM +16.*S*W2 +4.*S**2 -8.*CM*W2 +2.*CM**2
     1  +8.*W2*Z2 +12.*W2**2 +2.*Z2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3P*CMZD*(XW-1./2)
      TLPART= 12.*S*CPD*W2**2 +12.*S**2*CPD*W2 +4.*S**3*CPD
     1  +4.*CPD*W2**3
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3P*CSD
      TLPART= -1.*S*CM -2.*S*CM*CPD*W2 -1.*S*CM*CPD*Z2
     1  +2.*S*CPD*W2**2 +1.*S*CPD*Z2**2 -1.*S*Z2 -2.*S**2*CM*CPD
     1  +4.*S**2*CPD*W2 +2.*S**2*CPD*Z2 +2.*S**3*CPD +1.*CM*CPD*W2*Z2
     1  -1.*CM*W2 -1.*CM**2*CPD*W2 -2.*CPD*W2**2*Z2 -1.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3M
      TLPART= 1.*CP +1.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3M*(XW-1./2)
      TLPART= 8.*S*CMD*W2 +4.*S**2*CMD -2.*CP +4.*CMD*W2**2 -2.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3M*(1.-XW)
      TLPART= 6.*S*CP*CMD -4.*S*CP*CMD**2*W2 +2.*S*CP*CMD**2*Z2
     1  -16.*S*CMD*W2 -6.*S*CMD*Z2 +8.*S*CMD**2*W2*Z2
     1  +8.*S*CMD**2*W2**2 -2.*S*CMD**2*Z2**2 -4.*S**2*CMD
     1  +4.*S**2*CMD**2*W2 -2.*CP +10.*CP*CMD*W2 +2.*CP*CMD*Z2
     1  -2.*CP*CMD**2*W2*Z2 -4.*CP*CMD**2*W2**2 -2.*CP**2*CMD
     1  +2.*CP**2*CMD**2*W2 -14.*CMD*W2*Z2 -12.*CMD*W2**2
     1  +8.*CMD**2*W2**2*Z2 +4.*CMD**2*W2**3 -2.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3M*CPZD*(XW-1./2)
      TLPART= 12.*S*CMD*W2**2 +12.*S**2*CMD*W2 +4.*S**3*CMD
     1  +4.*CMD*W2**3
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3M*CMZD*(XW-1./2)
      TLPART= -4.*S*CP +16.*S*W2 +4.*S**2 -8.*CP*W2 +2.*CP**2
     1  +8.*W2*Z2 +12.*W2**2 +2.*Z2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3M*CSD
      TLPART= -1.*S*CP -2.*S*CP*CMD*W2 -1.*S*CP*CMD*Z2
     1  +2.*S*CMD*W2**2 +1.*S*CMD*Z2**2 -1.*S*Z2 -2.*S**2*CP*CMD
     1  +4.*S**2*CMD*W2 +2.*S**2*CMD*Z2 +2.*S**3*CMD +1.*CP*CMD*W2*Z2
     1  -1.*CP*W2 -1.*CP**2*CMD*W2 -2.*CMD*W2**2*Z2 -1.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3S*(XW-1./2)
      TLPART= -8.*S -4.*S*CP*ZD2 -4.*S*CM*ZD2 +8.*S*CPD*W2
     1  +8.*S*CMD*W2 +8.*S*W2*ZD2 +4.*S**2*CPD +4.*S**2*CMD
     1  +4.*S**2*ZD2 +4.*CP*CM*ZD2 -4.*CP*W2*ZD2 -4.*CM*W2*ZD2
     1  +4.*CPD*W2**2 +4.*CMD*W2**2 -8.*W2 +4.*W2**2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3S*(XW-1./2)*(1.-XW)
      TLPART= 8.*S*CP*ZD2 +8.*S*CM*ZD2 -16.*S*W2*ZD2 -8.*S**2*ZD2
     1  +8.*CP -8.*CP*CM*ZD2 +8.*CP*W2*ZD2 +8.*CM +8.*CM*W2*ZD2
     1  -8.*W2**2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3S*(1.-XW)
      TLPART= 16.*S +6.*S*CP*CMD -4.*S*CP*CMD**2*W2
     1  +2.*S*CP*CMD**2*Z2 -4.*S*CP*ZD2 +6.*S*CM*CPD -4.*S*CM*CPD**2*W2
     1  +2.*S*CM*CPD**2*Z2 -4.*S*CM*ZD2 -16.*S*CPD*W2 -6.*S*CPD*Z2
     1  +8.*S*CPD**2*W2*Z2 +8.*S*CPD**2*W2**2 -2.*S*CPD**2*Z2**2
     1  -16.*S*CMD*W2 -6.*S*CMD*Z2 +8.*S*CMD**2*W2*Z2
     1  +8.*S*CMD**2*W2**2 -2.*S*CMD**2*Z2**2 +8.*S*W2*ZD2 -4.*S**2*CPD
     1  +4.*S**2*CPD**2*W2 -4.*S**2*CMD +4.*S**2*CMD**2*W2 +4.*S**2*ZD2
     1  -10.*CP +4.*CP*CM*ZD2 +10.*CP*CMD*W2 +2.*CP*CMD*Z2
     1  -2.*CP*CMD**2*W2*Z2 -4.*CP*CMD**2*W2**2 -4.*CP*W2*ZD2
     1  -2.*CP**2*CMD +2.*CP**2*CMD**2*W2
      TLPART=TLPART -10.*CM +10.*CM*CPD*W2 +2.*CM*CPD*Z2
     1  -2.*CM*CPD**2*W2*Z2 -4.*CM*CPD**2*W2**2 -4.*CM*W2*ZD2
     1  -2.*CM**2*CPD +2.*CM**2*CPD**2*W2 -14.*CPD*W2*Z2 -12.*CPD*W2**2
     1  +8.*CPD**2*W2**2*Z2 +4.*CPD**2*W2**3 -14.*CMD*W2*Z2
     1  -12.*CMD*W2**2 +8.*CMD**2*W2**2*Z2 +4.*CMD**2*W2**3 +20.*W2
     1  +4.*W2**2*ZD2 +12.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3S*(1.-XW)**2
      TLPART= -16.*S +4.*S*CP*ZD2 +4.*S*CM*ZD2 -8.*S*W2*ZD2
     1  -4.*S**2*ZD2 +12.*CP -4.*CP*CM*ZD2 +4.*CP*W2*ZD2 +12.*CM
     1  +4.*CM*W2*ZD2 -16.*W2 -4.*W2**2*ZD2 -14.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3S*CPZD*(XW-1./2)
      TLPART= 4.*S*CM +12.*S*CMD*W2**2 -16.*S*W2 -8.*S**2
     1  +12.*S**2*CMD*W2 +4.*S**3*CMD +4.*CM*W2 +4.*CMD*W2**3
     1  -8.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3S*CPZD*(XW-1./2)*(1.-XW)
      TLPART= -4.*S*CM +24.*S*W2 +4.*S*Z2 +8.*S**2 -12.*CM*W2
     1  -8.*CM*Z2 +12.*W2*Z2 +16.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3S*CMZD*(XW-1./2)
      TLPART= 4.*S*CP +12.*S*CPD*W2**2 -16.*S*W2 -8.*S**2
     1  +12.*S**2*CPD*W2 +4.*S**3*CPD +4.*CP*W2 +4.*CPD*W2**3
     1  -8.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3S*CMZD*(XW-1./2)*(1.-XW)
      TLPART= -4.*S*CP +24.*S*W2 +4.*S*Z2 +8.*S**2 -12.*CP*W2
     1  -8.*CP*Z2 +12.*W2*Z2 +16.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3S*CSZD*(XW-1./2)**2
      TLPART= -16.*S*CP -8.*S*CP*CM*ZD2 +8.*S*CP*W2*ZD2
     1  -4.*S*CP**2*ZD2 -16.*S*CM +8.*S*CM*W2*ZD2 -4.*S*CM**2*ZD2
     1  +32.*S*W2 +8.*S*Z2 +24.*S**2 +4.*S**2*CP*ZD2 +4.*S**2*CM*ZD2
     1  -8.*CP*CM*W2*ZD2 +4.*CP*CM**2*ZD2 -12.*CP*W2 +4.*CP*W2**2*ZD2
     1  +4.*CP**2 +4.*CP**2*CM*ZD2 -4.*CP**2*W2*ZD2 -12.*CM*W2
     1  +4.*CM*W2**2*ZD2 +4.*CM**2 -4.*CM**2*W2*ZD2 +8.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3S*CSZD*CPZD*(XW-1./2)**2
      TLPART= -24.*S*CM*W2 +4.*S*CM*Z2 +8.*S*CM**2 -8.*S*W2*Z2
     1  +8.*S*W2**2 -16.*S**2*CM +16.*S**2*W2 +8.*S**3 +12.*CM*W2*Z2
     1  -8.*CM*W2**2 +4.*CM**2*W2 -4.*CM**2*Z2 -8.*W2**2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3S*CSZD*CMZD*(XW-1./2)**2
      TLPART= -24.*S*CP*W2 +4.*S*CP*Z2 +8.*S*CP**2 -8.*S*W2*Z2
     1  +8.*S*W2**2 -16.*S**2*CP +16.*S**2*W2 +8.*S**3 +12.*CP*W2*Z2
     1  -8.*CP*W2**2 +4.*CP**2*W2 -4.*CP**2*Z2 -8.*W2**2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI3S*CSD
      TLPART= 2.*S*CP*CM*ZD2 -2.*S*CP*CMD*W2 -1.*S*CP*CMD*Z2
     1  -2.*S*CP*W2*ZD2 +1.*S*CP**2*ZD2 -2.*S*CM*CPD*W2 -1.*S*CM*CPD*Z2
     1  -2.*S*CM*W2*ZD2 +1.*S*CM**2*ZD2 +2.*S*CPD*W2**2 +1.*S*CPD*Z2**2
     1  +2.*S*CMD*W2**2 +1.*S*CMD*Z2**2 -4.*S*W2 -2.*S*Z2 -4.*S**2
     1  -2.*S**2*CP*CMD -1.*S**2*CP*ZD2 -2.*S**2*CM*CPD -1.*S**2*CM*ZD2
     1  +4.*S**2*CPD*W2 +2.*S**2*CPD*Z2 +4.*S**2*CMD*W2 +2.*S**2*CMD*Z2
     1  +2.*S**3*CPD +2.*S**3*CMD +2.*CP*CM +2.*CP*CM*W2*ZD2
     1  -1.*CP*CM**2*ZD2 +1.*CP*CMD*W2*Z2 -1.*CP*W2 -1.*CP*W2**2*ZD2
     1  -1.*CP*Z2 +1.*CP**2 -1.*CP**2*CM*ZD2
      TLPART=TLPART -1.*CP**2*CMD*W2 +1.*CP**2*W2*ZD2
     1  +1.*CM*CPD*W2*Z2 -1.*CM*W2 -1.*CM*W2**2*ZD2 -1.*CM*Z2 +1.*CM**2
     1  -1.*CM**2*CPD*W2 +1.*CM**2*W2*ZD2 -2.*CPD*W2**2*Z2
     1  -2.*CMD*W2**2*Z2 +2.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI4P*(XW-1./2)
      TLPART= 8.*S -8.*S*CPD*W2 -4.*S**2*CPD -4.*CP -4.*CPD*W2**2
     1  +8.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI4P*(1.-XW)
      TLPART= -8.*S -6.*S*CM*CPD +4.*S*CM*CPD**2*W2
     1  -2.*S*CM*CPD**2*Z2 +16.*S*CPD*W2 +6.*S*CPD*Z2
     1  -8.*S*CPD**2*W2*Z2 -8.*S*CPD**2*W2**2 +2.*S*CPD**2*Z2**2
     1  +4.*S**2*CPD -4.*S**2*CPD**2*W2 +4.*CP +6.*CM -10.*CM*CPD*W2
     1  -2.*CM*CPD*Z2 +2.*CM*CPD**2*W2*Z2 +4.*CM*CPD**2*W2**2
     1  +2.*CM**2*CPD -2.*CM**2*CPD**2*W2 +14.*CPD*W2*Z2 +12.*CPD*W2**2
     1  -8.*CPD**2*W2**2*Z2 -4.*CPD**2*W2**3 -12.*W2 -6.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI4P*CMZD*(XW-1./2)
      TLPART= -12.*S*CP -12.*S*CPD*W2**2 +24.*S*W2 +12.*S**2
     1  -12.*S**2*CPD*W2 -4.*S**3*CPD -12.*CP*W2 +4.*CP**2
     1  -4.*CPD*W2**3 +12.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI4P*CSD
      TLPART= -2.*S*CP -1.*S*CM +2.*S*CM*CPD*W2 +1.*S*CM*CPD*Z2
     1  -2.*S*CPD*W2**2 -1.*S*CPD*Z2**2 +4.*S*W2 +1.*S*Z2 +4.*S**2
     1  +2.*S**2*CM*CPD -4.*S**2*CPD*W2 -2.*S**2*CPD*Z2 -2.*S**3*CPD
     1  -1.*CP*CM +1.*CP*Z2 -1.*CM*CPD*W2*Z2 +1.*CM*W2 +1.*CM*Z2
     1  -1.*CM**2 +1.*CM**2*CPD*W2 +2.*CPD*W2**2*Z2 -3.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI4M*(XW-1./2)
      TLPART= 8.*S -8.*S*CMD*W2 -4.*S**2*CMD -4.*CM -4.*CMD*W2**2
     1  +8.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI4M*(1.-XW)
      TLPART= -8.*S -6.*S*CP*CMD +4.*S*CP*CMD**2*W2
     1  -2.*S*CP*CMD**2*Z2 +16.*S*CMD*W2 +6.*S*CMD*Z2
     1  -8.*S*CMD**2*W2*Z2 -8.*S*CMD**2*W2**2 +2.*S*CMD**2*Z2**2
     1  +4.*S**2*CMD -4.*S**2*CMD**2*W2 +6.*CP -10.*CP*CMD*W2
     1  -2.*CP*CMD*Z2 +2.*CP*CMD**2*W2*Z2 +4.*CP*CMD**2*W2**2
     1  +2.*CP**2*CMD -2.*CP**2*CMD**2*W2 +4.*CM +14.*CMD*W2*Z2
     1  +12.*CMD*W2**2 -8.*CMD**2*W2**2*Z2 -4.*CMD**2*W2**3 -12.*W2
     1  -6.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI4M*CPZD*(XW-1./2)
      TLPART= -12.*S*CM -12.*S*CMD*W2**2 +24.*S*W2 +12.*S**2
     1  -12.*S**2*CMD*W2 -4.*S**3*CMD -12.*CM*W2 +4.*CM**2
     1  -4.*CMD*W2**3 +12.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI4M*CSD
      TLPART= -1.*S*CP +2.*S*CP*CMD*W2 +1.*S*CP*CMD*Z2 -2.*S*CM
     1  -2.*S*CMD*W2**2 -1.*S*CMD*Z2**2 +4.*S*W2 +1.*S*Z2 +4.*S**2
     1  +2.*S**2*CP*CMD -4.*S**2*CMD*W2 -2.*S**2*CMD*Z2 -2.*S**3*CMD
     1  -1.*CP*CM -1.*CP*CMD*W2*Z2 +1.*CP*W2 +1.*CP*Z2 -1.*CP**2
     1  +1.*CP**2*CMD*W2 +1.*CM*Z2 +2.*CMD*W2**2*Z2 -3.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI5P*(XW-1./2)*(1.-XW)
      TLPART= 4.*S*CP +4.*S*CM -4.*S*Z2 -4.*S**2 +4.*CP*W2 -4.*CP**2
     1  +4.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI5P*(1.-XW)
      TLPART= -2.*S*CP +4.*S*CP*CMD*W2 -2.*S*CP*CMD*Z2 -2.*S*CM
     1  -8.*S*CMD*W2*Z2 -8.*S*CMD*W2**2 +2.*S*CMD*Z2**2 +8.*S*W2
     1  +2.*S*Z2 +2.*S**2 -4.*S**2*CMD*W2 +2.*CP*CMD*W2*Z2
     1  +4.*CP*CMD*W2**2 -2.*CP*W2 -2.*CP*Z2 +2.*CP**2 -2.*CP**2*CMD*W2
     1  -4.*CM*W2 -8.*CMD*W2**2*Z2 -4.*CMD*W2**3 +6.*W2*Z2 +6.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI5P*(1.-XW)**2
      TLPART= 4.*S*CP +4.*S*CM -16.*S*W2 -6.*S*Z2 -4.*S**2 +4.*CP*W2
     1  +6.*CP*Z2 -4.*CP**2 +12.*CM*W2 -14.*W2*Z2 -12.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI5P*CMZD*(XW-1./2)*(1.-XW)
      TLPART= -8.*S*CP*W2 -4.*S*CP*Z2 +8.*S*W2*Z2 +16.*S*W2**2
     1  +8.*S**2*W2 -12.*CP*W2*Z2 -8.*CP*W2**2 +4.*CP**2*W2
     1  +4.*CP**2*Z2 +8.*W2**2*Z2 +8.*W2**3
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI5M*(XW-1./2)*(1.-XW)
      TLPART= 4.*S*CP +4.*S*CM -4.*S*Z2 -4.*S**2 +4.*CM*W2 -4.*CM**2
     1  +4.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI5M*(1.-XW)
      TLPART= -2.*S*CP -2.*S*CM +4.*S*CM*CPD*W2 -2.*S*CM*CPD*Z2
     1  -8.*S*CPD*W2*Z2 -8.*S*CPD*W2**2 +2.*S*CPD*Z2**2 +8.*S*W2
     1  +2.*S*Z2 +2.*S**2 -4.*S**2*CPD*W2 -4.*CP*W2 +2.*CM*CPD*W2*Z2
     1  +4.*CM*CPD*W2**2 -2.*CM*W2 -2.*CM*Z2 +2.*CM**2 -2.*CM**2*CPD*W2
     1  -8.*CPD*W2**2*Z2 -4.*CPD*W2**3 +6.*W2*Z2 +6.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI5M*(1.-XW)**2
      TLPART= 4.*S*CP +4.*S*CM -16.*S*W2 -6.*S*Z2 -4.*S**2 +12.*CP*W2
     1  +4.*CM*W2 +6.*CM*Z2 -4.*CM**2 -14.*W2*Z2 -12.*W2**2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI5M*CPZD*(XW-1./2)*(1.-XW)
      TLPART= -8.*S*CM*W2 -4.*S*CM*Z2 +8.*S*W2*Z2 +16.*S*W2**2
     1  +8.*S**2*W2 -12.*CM*W2*Z2 -8.*CM*W2**2 +4.*CM**2*W2
     1  +4.*CM**2*Z2 +8.*W2**2*Z2 +8.*W2**3
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI6S*(1.-XW)**2
      TLPART= 8.*S*CP*W2 -2.*S*CP*Z2 +8.*S*CM*W2 -2.*S*CM*Z2
     1  -12.*S*W2*Z2 -16.*S*W2**2 +4.*S*Z2**2 -8.*S**2*W2 +2.*S**2*Z2
     1  -2.*CP*CM*Z2 +6.*CP*W2*Z2 +8.*CP*W2**2 -4.*CP**2*W2
     1  +6.*CM*W2*Z2 +8.*CM*W2**2 -4.*CM**2*W2 -14.*W2**2*Z2 -8.*W2**3
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI6P
      TLPART= -2.*S*W2*Z2 -1.*S**2*Z2 -1.*W2**2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI6M
      TLPART= -2.*S*W2*Z2 -1.*S**2*Z2 -1.*W2**2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI7P
      TLPART= 1.*S*CM +1.*S*Z2 +1.*CM*W2 +1.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI7P*(XW-1./2)
      TLPART= -2.*S*CM -2.*S*Z2 -2.*CM*W2 -2.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI7P*(1.-XW)
      TLPART= -2.*S*CM +4.*S*CM*CPD*W2 -2.*S*CM*CPD*Z2
     1  -8.*S*CPD*W2*Z2 -8.*S*CPD*W2**2 +2.*S*CPD*Z2**2 -2.*S*Z2
     1  -4.*S**2*CPD*W2 +2.*CM*CPD*W2*Z2 +4.*CM*CPD*W2**2 -2.*CM*W2
     1  -2.*CM**2*CPD*W2 -8.*CPD*W2**2*Z2 -4.*CPD*W2**3 -2.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI7P*CPZD*(XW-1./2)
      TLPART= -4.*S*CM*W2 +4.*S*CM*Z2 +8.*S*W2**2 +4.*S**2*W2
     1  -4.*S**2*Z2 -4.*CM*W2**2 +2.*CM**2*W2 +2.*W2*Z2**2 +4.*W2**2*Z2
     1  +4.*W2**3
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI7M
      TLPART= 1.*S*CP +1.*S*Z2 +1.*CP*W2 +1.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI7M*(XW-1./2)
      TLPART= -2.*S*CP -2.*S*Z2 -2.*CP*W2 -2.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI7M*(1.-XW)
      TLPART= -2.*S*CP +4.*S*CP*CMD*W2 -2.*S*CP*CMD*Z2
     1  -8.*S*CMD*W2*Z2 -8.*S*CMD*W2**2 +2.*S*CMD*Z2**2 -2.*S*Z2
     1  -4.*S**2*CMD*W2 +2.*CP*CMD*W2*Z2 +4.*CP*CMD*W2**2 -2.*CP*W2
     1  -2.*CP**2*CMD*W2 -8.*CMD*W2**2*Z2 -4.*CMD*W2**3 -2.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI7M*CMZD*(XW-1./2)
      TLPART= -4.*S*CP*W2 +4.*S*CP*Z2 +8.*S*W2**2 +4.*S**2*W2
     1  -4.*S**2*Z2 -4.*CP*W2**2 +2.*CP**2*W2 +2.*W2*Z2**2 +4.*W2**2*Z2
     1  +4.*W2**3
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI8P
      TLPART= -2.*S*Z2 -2.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI8M
      TLPART= -2.*S*Z2 -2.*W2*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI9P
      TLPART= 1./2.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI9P*(XW-1./2)
      TLPART= -2.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI9P*(XW-1./2)*(1.-XW)
      TLPART= 4.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI9P*(XW-1./2)**2
      TLPART= 2.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI9P*(1.-XW)
      TLPART= -2.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI9P*(1.-XW)**2
      TLPART= 2.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=-4.*AI9P*CMZD*(XW-1./2)**2
      XTOTAL=XTOTAL+HDPART
      HDPART=4.*AI9P*CSZD*(XW-1./2)**2
      XTOTAL=XTOTAL+HDPART
      HDPART=AI9P*CSZD*CPZD*(XW-1./2)**2
      TLPART= 4.*S -4.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI9P*CSZD*CMZD*(XW-1./2)**2
      TLPART= 4.*S -4.*CP
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI9M
      TLPART= 1./2.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI9M*(XW-1./2)
      TLPART= -2.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI9M*(XW-1./2)*(1.-XW)
      TLPART= 4.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI9M*(XW-1./2)**2
      TLPART= 2.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI9M*(1.-XW)
      TLPART= -2.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI9M*(1.-XW)**2
      TLPART= 2.*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=-4.*AI9M*CPZD*(XW-1./2)**2
      XTOTAL=XTOTAL+HDPART
      HDPART=4.*AI9M*CSZD*(XW-1./2)**2
      XTOTAL=XTOTAL+HDPART
      HDPART=AI9M*CSZD*CPZD*(XW-1./2)**2
      TLPART= 4.*S -4.*CM
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI9M*CSZD*CMZD*(XW-1./2)**2
      TLPART= 4.*S -4.*Z2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10P
      TLPART= 1./2.*S*ZD2 -1./2.*CP*ZD2 +1./2.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10P*(XW-1./2)
      TLPART= -2.*S*ZD2 +2.*CP*ZD2 -2.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10P*(XW-1./2)*(1.-XW)
      TLPART= 4.*S*ZD2 -4.*CP*ZD2 +4.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10P*(XW-1./2)**2
      TLPART= 2.*S*ZD2 -2.*CP*ZD2 +2.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10P*(1.-XW)
      TLPART= -2.*S*ZD2 +2.*CP*ZD2 -2.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10P*(1.-XW)**2
      TLPART= 2.*S*ZD2 -2.*CP*ZD2 +2.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10P*CMZD*(XW-1./2)
      TLPART= 2.*S -2.*CP
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10P*CMZD*(XW-1./2)*(1.-XW)
      TLPART= -4.*S +4.*CP
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10P*CMZD*(XW-1./2)**2
      TLPART= -4.*S +4.*CP -4.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10M
      TLPART= 1./2.*S*ZD2 -1./2.*CM*ZD2 +1./2.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10M*(XW-1./2)
      TLPART= -2.*S*ZD2 +2.*CM*ZD2 -2.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10M*(XW-1./2)*(1.-XW)
      TLPART= 4.*S*ZD2 -4.*CM*ZD2 +4.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10M*(XW-1./2)**2
      TLPART= 2.*S*ZD2 -2.*CM*ZD2 +2.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10M*(1.-XW)
      TLPART= -2.*S*ZD2 +2.*CM*ZD2 -2.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10M*(1.-XW)**2
      TLPART= 2.*S*ZD2 -2.*CM*ZD2 +2.*W2*ZD2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10M*CPZD*(XW-1./2)
      TLPART= 2.*S -2.*CM
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10M*CPZD*(XW-1./2)*(1.-XW)
      TLPART= -4.*S +4.*CM
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=AI10M*CPZD*(XW-1./2)**2
      TLPART= -4.*S +4.*CM -4.*W2
      XTOTAL=XTOTAL+HDPART*TLPART
      HDPART=4.*AI11P*CPZD*(XW-1./2)
      XTOTAL=XTOTAL+HDPART
      HDPART=4.*AI11P*CMZD*(XW-1./2)
      XTOTAL=XTOTAL+HDPART
      HDPART=4.*AI11M*CPZD*(XW-1./2)
      XTOTAL=XTOTAL+HDPART
      HDPART=4.*AI11M*CMZD*(XW-1./2)
      XTOTAL=XTOTAL+HDPART


C       ***************************
      ELMS=32.*PI*COST2*XTOTAL
C       ***************************

C       The following is the invariant distribution
C       Ez*(dsigma/d3pz) in TeV^-2
C       ***************************
      DISTRINV=ELMS/(4.*PI)**5/S
C       ***************************

C        The following is dsigma/dpt dy in (pb/TeV).
C        The factor 2pi is due to the integral on
C        the Z azimut
C       *******************************************
      DISTRPTY=FCONV*DISTRINV*2.*PI*PT
C       *******************************************

      RETURN
      END

      SUBROUTINE FSUB1(M,U1,F1,XX)
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      DIMENSION U1(M), F1(M), XX(2)
      DO 1 L = 1, M
      XX(1) = U1(L)
    1 F1(L) = DISTRYPT(XX(1),XX(2))
      RETURN
      END
CPJ

      SUBROUTINE FSUB2(M,U1,F1,XX)
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      DIMENSION U1(M), F1(M), XX(2)
      DO 1 L = 1, M
      XX(1) = U1(L)
    1 F1(L) = DISTRPTY(XX(1),XX(2))
      RETURN
      END
CPJ

      SUBROUTINE znnbge(ecms,qg1,qg2,qz,qn,qb)
C-----------------------------------------------------------------------
C
C   Compute Higgs production cross sections with initial state
C   radiation and beamstrahlung effects.
C   Event generation
C
C   Patrick Janot -- 27 Aug 1991
C----------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION qg1(4),qg2(4),qz(4),qn(4),qb(4)
      DIMENSION rg2(4),rz(4),rn(4),rb(4)
      LOGICAL first
      DATA first/.TRUE./
      include 'lundcom.h'
      include 'miscl.h'
      EXTERNAL sigma
C
      s = ecms**2
      e = ecms/2.
      CALL vzero(qg1(1),4*2)
      CALL vzero(qg2(1),4*2)
      CALL vzero(qz(1),4*2)
      CALL vzero(qn(1),4*2)
      CALL vzero(qb(1),4*2)
      CALL vzero(rz(1),4*2)
      CALL vzero(rn(1),4*2)
      CALL vzero(rb(1),4*2)
C
C  First compute the total cross section with Brems- or beams-strahlung
C  (if requested)
C
      IF ( first ) THEN
C
        sthr = (pmas(23,1)+2.)**2
        CALL remt1(e,sigma,sthr,1,cross,1D0)
        first = .FALSE.
      ENDIF
C
C  Event generation
C
 1    CALL remt2(qg1,rg2,1)
      s1 = s * (1.-qg1(4)/e)
      e1 = SQRT(s1)/2.
      s2 = s1 * (1.-rg2(4)/e1)
      IF ( s2 .LE. sthr ) THEN
        WRITE(6,*) ' *** Warning *** Not enough energy for production'
     .       ,s2,' < ',sthr,' !'
        GOTO 1
      ENDIF
      CALL dsigma(s2,qz,qn,qb)
C
      zm = sqrt(qz(4)**2-qz(1)**2-qz(2)**2-qz(3)**2)

      CALL remt3(qz, rz, 2)
      CALL remt3(qn, rn, 2)
      CALL remt3(qb, rb, 2)
      CALL remt3(rz, qz, 1)
      CALL remt3(rn, qn, 1)
      CALL remt3(rb, qb, 1)
      CALL remt3(rg2,qg2,1)

      zm = sqrt(qz(4)**2-qz(1)**2-qz(2)**2-qz(3)**2)

C
C  End of event generation
      RETURN
      END

      DOUBLE PRECISION FUNCTION sigma(s)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'crosec.h'
      ecm = SQRT(s)
      DO istep = 1, nstep
        IF ( ecm .LE. ecs(istep) ) GOTO 2
      ENDDO
    2 a = (crs(istep)-crs(istep-1))/(ecs(istep)-ecs(istep-1))
      b = crs(istep) - a*ecs(istep)
      sigma = a * ecm + b
      RETURN
      END

      SUBROUTINE dsigma(s1,qz,qn,qb)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'parznn.h'
C      REAL*8 y, pt, distrpty
      DIMENSION qz(4), qn(4), qb(4)
      include 'crosec.h'
C
      S=S1/1E6
      RS = SQRT(S)
C
      YMAX=LOG(RS/Z)
  100 Y = YMAX*RNDM2(Y)
C
      PTMAX=SQRT(((S+Z2)/(2*RS*COSH(Y)))**2-Z2)
      PT = PTMAX*RNDM2(PT)
C
      weight = 2.*distrpty(pt,y) * ptmax * ymax
      wnorm = weight*crs(nstep)/sigma(s1)
C      CALL hfill(10000,wnorm,0.,1.)
      IF ( wnorm .GT. WMAX ) THEN
        wmax = wnorm
        WRITE(6,*) '+++ Warning +++ WMAX = ',wmax,
     .             ' (ecm,pt,y = ',SQRT(s1),pt,y,')'
      ENDIF
C
      PHI = 2.*PI*RNDM2(PHI)
C
      IF ( RNDM2(y) .GT. .5 ) y = -y
C
      ntry = ntry + 1
      wtot = wtot + weight
      wtot2 = wtot2 + weight**2
      IF ( weight/wmax .LT. RNDM2(weight) ) GOTO 100
      nacc = nacc + 1
C
      ET  = 1000. * SQRT(PT**2+Z2)
      QZ(1) = PT*COS(PHI)*1000.
      QZ(2) = PT*SIN(PHI)*1000.
      QZ(3) = ET * ( EXP(Y) - 1. ) / ( 2. * EXP(Y/2.) )
      QZ(4) = SQRT(ET**2+QZ(3)**2)
C
      RETURN
      END

      SUBROUTINE hhlu1(ipa,kf,px,py,pz,pe)
C------------------------------------------------------------------
C  Add one entry to the LUND event record
C
C  Patrick Janot -- 26 Aug 1991
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'lundcom.h'
C
      pm=pymass(kf)
      DO 100 J=1,5
      k7lu(ipa,j)=0
      p7lu(ipa,j)=0.
  100 v7lu(ipa,j)=0.
C...Store parton/particle in K and P vectors.
      k7lu(ipa,1)=1
      k7lu(ipa,2)=kf
      p7lu(ipa,5)=pm
      p7lu(ipa,4)=SQRT(px**2+py**2+pz**2+pm**2)
      p7lu(ipa,1)=px
      p7lu(ipa,2)=py
      p7lu(ipa,3)=pz
C
      RETURN
      END

      SUBROUTINE REMT1(EBEAM,CROSS,STHR,INDEX,sig1,xrad)
C-----------------------------------------------------------------------
C! The famous Kleiss initial state radiator, modified for two photons.
C
C It calculates some quantities, and performs the
C numerical integration over the photon spectrum.
C
C EBEAM=Beam energy (in gev)
C CROSS=Nonradiative cross section, to be defined
C       with one variable: cross(s),
C       where s is the invariant mass of the e+e- pair.
C STHR =The kinematical threshold, i.e. the lowest allowed
C       value of s for which the nonradiative process can
C       take place ( in gev**2 )
C XRAD =The fudge factor that increases the beta to simulate
C       the beamstrahlung
C
c 851015 A. Schwarz. have to call REMT1 always when mass
c        of Z0-decay products changes, since threshold changes
c        as well; dimension of X(1000) changed to X(1000,10)
C 921109 P. Janot. Change the effective distribution in 1/x
C        integrated between a fictive xmin and 1. to the expo-
C        nentiated distribution in 1/x**(1-beta) integrated
C        between 0. and 1. as it should be.
C 921110 P. Janot. Generation of 2 ISR photons.
C 921111 P. Janot. Implementation of effective beamstrahlung option
C-----------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON / flags  / idbg
      DIMENSION X(1000,10),F(1000),A(1000),Y(1000),Z(1000),XNEW(1000)
      DIMENSION xn(10),wmax(10)
      DIMENSION QK(4),QK1(4),QK2(4),QIN(4),QOUT(4),QKA(4),QKB(4)
C      REAL*4 xrad
C      REAL*4 rndm
      EXTERNAL cross
      DATA INIT/0/
C
      SAVE
C
C DEFINITION OF BREMSSTRAHLUNG SPECTRUM
      DATA ame/.511D-3/
C
C Definition of Bremsstrahlung spectrum
C
      xkmax(sb)  = 1D0-sthr/sb
      xps(sb)    = 2D0*ame**2/sb
      xpt(sb)    = (2D0+xps(sb))/xps(sb)
      beta(sb)   = 2D0*alf/pi*(1D0-2D0*xps(sb))*(xpl-1D0)*xrad
      spectr(xk) = bet * dj1
     .                 * (1D0+3D0*bet/4D0+alf/pi*(pi**2/3D0-1D0/2D0)
     .                 - xk**(1D0-bet) + xk**(2D0-bet)/2D0
     .                 - bet**2/24D0*(2D0*pi**2-37D0/4D0+xpl/3D0)
     .                 + bet/8D0*strange(xk,bet) )
     .                 * cross(s*(1D0-xk))
C        Store EBEAM into local variable EBEA for later use
      EBEA = EBEAM
C
C Initialize a few quantities and constants
C
      s   = 4.*ebeam**2
      sb  = s
      pi  = 4.*DATAN(1.d0)
      tpi = 2.*pi
C     alf = ulalem(SNGL(s))
      alf = 1./137.035989
      xpl  = LOG(xpt(s))
      x1   = 0.
      xn(index) = xkmax(s)
      wmax(index) = 0.
      bet  = beta(s)
      dj1 = xn(index)**bet/bet
      sig1 = 0.
C
C Parameters of numerical integration step
      N    = 500
      ITER = 6
C
C Initialize by choosing equidistant u values ( du = Cx**(beta-1)dx)
C with an increased sampling around mz.
C
      IF ( idbg .GE. 0 ) THEN
        WRITE(6,*) ' '
        WRITE(6,*) '----------------------------------------'
        WRITE(6,*) '    Compute radiative corrections      '
        WRITE(6,*) '        for process # ',index
        WRITE(6,*) '----------------------------------------'
      ENDIF
      IT=0
      M=N-1
      DU=1./FLOAT(M)
      X(1,INDEX)=0.
      DO 101 I=2,N
  101 X(I,INDEX)=X(I-1,INDEX)+DU
C
C Starting point for iterations
C
  100 CONTINUE
C
C Calculate function values
C
      DO 102 I=1,N
  102 F(I)=SPECTR(xn(index)*X(I,INDEX)**(1D0/bet))
C
C CALCULATE BIN AREAS
      DO 103 I=1,M
  103 A(I)=(X(I+1,INDEX)-X(I,INDEX))*(F(I+1)+F(I))/2.
C
C CALCULATE CUMULATIVE SPECTRUM Y VALUES
      Y(1)=0.D0
      DO 104 I=2,N
  104 Y(I)=Y(I-1)+A(I-1)
C
C PUT EQUIDISTANT POINTS ON Y SCALE
      DZ=Y(N)/FLOAT(M)
      Z(1)=0.D0
      DO 105 I=2,N
  105 Z(I)=Z(I-1)+DZ
C
C DETERMINE SPACING OF Z POINTS IN BETWEEN Y POINTS
C FROM THIS, DETERMINE NEW X VALUES AND FINALLY REPLACE OLD VALUES
      XNEW(1)=X(1,INDEX)
      XNEW(N)=X(N,INDEX)
      K=1
      DO 108 I=2,M
  106 IF( Y(K+1) .GT. Z(I) ) GOTO 107
      K=K+1
      GOTO 106
  107 R= ( Z(I) - Y(K) ) / ( Y(K+1) - Y(K) )
  108 XNEW(I) = X(K,INDEX) + ( X(K+1,INDEX)-X(K,INDEX) )*R
      DO 109 I=1,N
  109 X(I,INDEX)=XNEW(I)
C
C CHECK ON END OF ITERATIONS AND RETURN
      IT=IT+1
      IF ( it .LE. 3 ) THEN
        SIG1 = 0.
        NT = 1
      ELSE
        NT = IT-2
      ENDIF
      SIG1 = SIG1+Y(M)
C     PRINT 3,IT,SIG1/FLOAT(NT)
C    3 FORMAT(' Iteration # ',i3,'  Integral =',e15.6)
      IF(IT.LT.ITER) GOTO 100
C
C PRESENT RESULTS IN FORM OF CORRECTION
      SIG0 = CROSS(S)
      SIG1 = SIG1/FLOAT(NT)
      DELT = (SIG1/SIG0-1.)*100.
C     IF(INIT.GT.1) RETURN
C     INIT = 2
      IF ( idbg .GE. 0 ) PRINT 4,SIG0,SIG1,DELT
    4 FORMAT(/' Results of the initialization step :',/,
     .        ' Nonradiative cross section :',e15.6,/,
     .        '    Radiative cross section :',e15.6,/,
     .        '    Radiative correction    :',f10.3,' %',/)
C     WRITE(6,500) (SQRT(s*(1.-xn*x(i,index)**(1./bet))),i=1,500)
C 500 FORMAT(10(F10.5,1X))
      RETURN
      ENTRY REMT2(QK1,QK2,IDEC)
C-----------------------------------------------------------------------
C THIS PART GENERATES A BREMSSTRAHLUNG PHOTON
C AND CALCULATES WHICH BEAM AXIS TO CHOOSE FOR
C THE GENERATION OF THE 'NONRADIATIVE' CROSS SECTION.
C THE PHOTON ENERGY SPECTRUM MUST HAVE BEEN EXAMINED
C BY CALLING ENTRY 'REMT1' BEFORE THE FIRST CALL TO
C THIS ENTRY.
C-----------------------------------------------------------------------
C
C INITIALIZE FLAG FOR REMT3
C     INDX = MINDEX(IDEC)
      INDX = IDEC
      IR=0
C
C GENERATE total PHOTON ENERGY FROM CUMULATIVE SPECTRUM BINS
C  200 R=M*RNDM2(1D0)
      R=M*RNDM2(1D0)
      I=INT(R)
      S=R-I
      UK = X(I+1,INDX) + S*( X(I+2,INDX)-X(I+1,INDX) )
      XK = xn(INDX)*uk**(1./bet)
      IF ( xk .LE. 1D-17 ) xk = 1D-17
C
C Generate the energy of the photons
C
      IF ( xk .LE. 1D-6 ) THEN
        yx = xk**2/4.
      ELSE
        yx = (1.-SQRT(1.-xk))**2
      ENDIF
      yk = (RNDM2(0D0)*yx**bet)**(1./bet)
      IF ( yk/xk .GT. 1D-6 .OR. yk/xk**2 .GT. 1D-6 ) THEN
        xka  = .5*( (xk+yk)-SQRT((xk+yk)**2-4.*yk) )
      ELSE
        xka = yk/xk
      ENDIF
C 1D-15 rather than 1D-20 for these poor UNIX computers
      IF ( xka .LE. 1D-15 ) xka = 1D-15
      xkb  = xk+yk-yk/xk
      IF ( xkb .LE. 1D-15 ) xkb = 1D-15
C
      IF ( RNDM2(0.2D0) .GT. 0.5D0 ) THEN
        xk1 = xka
        xk2 = xkb
      ELSE
        xk2 = xka
        xk1 = xkb
      ENDIF
C
C GENERATE AZIMUTHAL SCATTERING ANGLE OF THE two PHOTONs
      FG1 = TPI*RNDM2(1.0D0)
      FG2 = TPI*RNDM2(1.5D0)
C
C GENERATE COSINE OF POLAR SCATTERING ANGLE OF THE two PHOTONs
      sb1 = sb
  201 V1 = XPS(sb1) * ( XPT(sb1)**RNDM2(2.0D0) - 1. )
      W1 = XPS(sb1) + V1*(1.-.5*V1)
      W1 = RNDM2(3.0D0)/(1.-(XK1*XK1*W1+2.*XPS(Sb1)*(1.-XK1)/W1)
     .                 /(1.+(1.-XK1)**2))
      IF(W1.GT.1.) GOTO 201
      W1 = -1. + 2.*W1
      CG1=SIGN(1.-V1,W1)
      sb2 = sb1*(1.-xk1)
  202 V2 = XPS(sb2) * ( XPT(sb2)**RNDM2(2.5D0) - 1. )
      W2 = XPS(sb2) + V2*(1.-.5*V2)
      W2 = RNDM2(3.5D0)/(1.-(XK2*XK2*W2+2.*XPS(Sb2)*(1.-XK2)/W2)
     .                 /(1.+(1.-XK2)**2))
      IF(W2.GT.1.) GOTO 202
      W2 = -1. + 2.*W2
      CG2=SIGN(1.-V2,W2)
C
C CHOOSE WHICH OF THE TWO Z AXES SHOULD BE CONSIDERED

      CH1=-1.
      CH2=-1.
      IF(ABS(W1).LT.(1./(1.+(1.-2./(1.+XK1*CG1/(2.-XK1)))**2)))
     .CH1=+1.
      IF(ABS(W2).LT.(1./(1.+(1.-2./(1.+XK2*CG2/(2.-XK2)))**2)))
     .CH2=+1.
C
C CONSTRUCT PHOTON FOUR-MOMENTA
      SG1=SQRT(V1*(2.-V1))
      QK1(4)=XK1*SQRT(sb1)/2.
      QK1(1)=QK1(4)*SG1*COS(FG1)
      QK1(2)=QK1(4)*SG1*SIN(FG1)
      QK1(3)=QK1(4)*CG1
C
      SG2=SQRT(V2*(2.-V2))
      Qk2(4)=XK2*SQRT(sb2)/2.
      Qk2(1)=Qk2(4)*SG2*COS(FG2)
      Qk2(2)=Qk2(4)*SG2*SIN(FG2)
      Qk2(3)=Qk2(4)*CG2
C - Correction for UNIX !!!
      CALL ucopy(qk1(1),qka(1),4*2)
      CALL ucopy(qk2(1),qkb(1),4*2)
C
      RETURN
C
      ENTRY REMT3(QIN,QOUT,IPHOT)
C-----------------------------------------------------------------------
C THIS PART PERFORMS THE ROTATIONS AND BOOSTS OF THE I.S.R.
C FORMALISM AFTER THE USER'S BLACK BOX HAS RUN AN EVENT.
C THE INPUT VECTOR (FROM USERS BLACK BOX) IS QIN;
C THE RESULTING VECTOR IN THE LAB FRAME IS QOUT.
C-----------------------------------------------------------------------
C
C INITIALIZATION PART: ONCE FOR EVERY GENERATED PHOTON MOMENTUM
C     IF(IR.NE.0) GOTO 301
C     IR=1
      IF ( iphot .EQ. 1 ) THEN
        CALL ucopy(qka(1),qk(1),4*2)
        ebea = SQRT(sb1)/2.
        ch = ch1
      ELSE
        CALL ucopy(qkb(1),qk(1),4*2)
        ebea = SQRT(sb2)/2.
        ch = ch2
      ENDIF
C
C CALCULATE ROTATTION PARAMETERS FOR BEAM DIRECTION IN C.M.S.
      YK=QK(4)**2-QK(1)**2-QK(2)**2-QK(3)**2
      XKP = SQRT( QK(1)**2 + QK(2)**2 )
      XKM = 2.* SQRT( EBEA*(EBEA-QK(4)) + YK/4. )
      XKD = 2.*EBEA - QK(4) + XKM
      XKA = ( CH + QK(3)/XKD )/XKM
      XKB = SQRT( (1.+XKA*QK(3))**2 + (XKA*XKP)**2 )
      S1  = XKA*XKP/XKB
      C1  = (1.+XKA*QK(3))/XKB
      S2  = QK(1)/XKP
      C2  = QK(2)/XKP
      Y1=C1**2+S1**2-1.
      Y2=C2**2+S2**2-1.
C
C ROTATE INPUT VECTOR QIN(I) TO CORRESPOND WITH CHOZEN Z-AXIS
C  301 QQ =  C1*QIN(2) + S1*QIN(3)
      QQ =  C1*QIN(2) + S1*QIN(3)
      QZ = -S1*QIN(2) + C1*QIN(3)
      QX =  C2*QIN(1) + S2*QQ
      QY = -S2*QIN(1) + C2*QQ
C
C BOOST ROTATED VECTOR TO LAB FRAME VECTOR QOUT
      QOUT(4)=((XKD-XKM)*QIN(4)-QK(1)*QX-QK(2)*QY-QK(3)*QZ)/XKM
      QQ     =(QIN(4)+QOUT(4))/XKD
      QOUT(1)= QX - QK(1)*QQ
      QOUT(2)= QY - QK(2)*QQ
      QOUT(3)= QZ - QK(3)*QQ
C
      RETURN
      END

      DOUBLE PRECISION FUNCTION strange(x,bet)
C      DOUBLE PRECISION FUNCTION strange(x,bet)
C-------------------------------------------------------------
C! Function used for the ISR computation to alpha_QED**2
C
C-------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
      IF ( x .GT. 0D0 ) THEN
        strange = x**(1D0-bet)
     .          * (4D0*(2D0-x)*LOG(1D0/x)
     .          - (1D0+3D0*(1D0-x)**2)/x*LOG(1D0-x)
     .          -  6D0+x )
      ELSE
        strange = 0D0
      ENDIF
      RETURN
      END

      DOUBLE PRECISION FUNCTION rndm2(dummy)
C-----------------------------------------------------------------
C! Generate a random number in double precision 
C
C Patrick Janot -- 7-Oct-1998
C Modifications: 
C   o Patrick Janot -- 20-Nov-1999
C     Comes back to DBLE(rndm) instead of the concatenation 
C     of two RNDM's, since the latter gives different results 
C     on different machines (!)
C-----------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      EXTERNAL RNDMCMSSW
      REAL*8 RNDMCMSSW, dummy2
C
C      rndm2 = DBLE(rndmCMSSW(dummy2))
      rndm2 = rndmCMSSW(dummy2)
C
      RETURN 
      END

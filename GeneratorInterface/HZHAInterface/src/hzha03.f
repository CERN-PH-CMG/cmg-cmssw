      SUBROUTINE ADDGLU(QIN1,QIN2,QOUT1,QOUT2,QOUT3,IOPT)
C-----------------------------------------------------------
C
C!EVENT GENERATOR AFFIX TO GO FROM TWO TO THREE JETS IN THE
C!DECAY OF A HEAVY COLOUR-SINGLET VECTOR BOSON (Z,W,...)
C THE MOMENTA 'QIN' ARE INPUT, THE MOMENTA 'QOUT' ARE OUTPUT
C
CRK-----------------
C IOPT=0 : THE CORRECT ALGORITHM
C IOPT=1 : CHOOSE X'S WITH EQUAL PROBABILITY
C IOPT=2 : CHOOSE THE LARGEST OF THE X'S
C IOPT=3 : CHOOSE ALWAYS X1
C-----------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
C      REAL*4 RNDM
      EXTERNAL RNDM2
      DIMENSION QIN1(4),QIN2(4),QOUT1(4),QOUT2(4),QOUT3(4)
      DIMENSION Q(4),R1(4),R2(4),E1(3),E2(3)
      DATA PI/3.1415926536D0/
      DATA XLAM/0.2D0/
C
C CONSTRUCT THE OVERALL MOMENTUM FROM THE QIN, AND ITS MASS
      DO 101 K=1,4
  101 Q(K)=QIN1(K)+QIN2(K)
      W=DSQRT(Q(4)**2-Q(3)**2-Q(2)**2-Q(1)**2)
C
C CONSTRUCT QIN1,2 IN Q'S C.M. FRAME, CALL'EM R1,2
      R1(4)=.5*W
      R2(4)=W-R1(4)
      RR=(QIN1(4)+R1(4))/(Q(4)+W)
      DO 102 K=1,3
      R1(K)=QIN1(K)-RR*Q(K)
  102 R2(K)=-R1(K)
C
C COMPUTE THE VALUE OF ALFA(S), ASSUMING THREE
C QUARK GENERATRIONS AND LAMBDA(QCD)=0.2 GEV
C     ALFAS=6.*PI/27./DLOG(W/XLAM)
      ALFAS=1.D0/137.036D0
C
C COMPUTE GAMMA, ASSUMING THE SMALL-GAMMA APPROXIMATION IS VALID
C (CHECKED OUT AS HELPED BY ROEL'S ZX SPECTRUM)
      GG=2.5-PI**2/3.-3.*(ALFAS+PI)/(2.*ALFAS)
      G=DEXP(.75*(-1.-DSQRT(1.-8.*GG/9.)))
C      PRINT *,'GAMMA =',G
C      IF(1.EQ.1) STOP
C
C GENERATE THE VALUES FOR X1 AND X2 BY MAPPING AND W.R.P.
  301 X1=1.-(1.-G)/DEXP(RNDM2(1D0)*DLOG((1.-G)/G))
      X2=1.-(1.-G)/DEXP(RNDM2(2D0)*DLOG((1.-G)/G))
      IF((X1+X2).GT.1.D0) GOTO 302
      X1=1.-X1
      X2=1.-X2
  302 CONTINUE
      WEIGHT=.5*X1*X2*(X1*X1+X2*X2)/(X1*X2+(1.-X1)*(1.-X2))
      WMAX=(1.-G)**4/((1.-G)**2+G**2)
      IF(WMAX.LT.WEIGHT) PRINT 303,G,X1,X2,WMAX,WEIGHT
  303 FORMAT(' WEIGHT ANOMALY:',5D15.6)
      IF(RNDM2(3D0).GT.(WEIGHT/WMAX)) GOTO 301
C
C CHOOSE BETWEEN THE TWO POSSIBLE ORIENTATIONS
C OPTIONS 1,2 AND 3 ARE WRONG: OPTION 0 GIVES THE CORRECT RESULT
CRK-------------
      IF(IOPT.NE.0) GOTO 351
      XX=X1**2/(X1**2+X2**2)
C      CALL HISTO1(4,10,0.D0,1.D0,XX,1.D0)
      IF(RNDM2(4D0).GT.XX) GOTO 501
      GOTO 401
CRK-------------
  351 IF(IOPT.NE.1) GOTO 352
      IF(RNDM2(4D0).GT.0.5D0) GOTO 501
      GOTO 401
CRK-------------
  352 IF(IOPT.NE.2) GOTO 353
      IF(X2.GT.X1) GOTO 501
      GOTO 401
CRK-------------
  353 IF(IOPT.NE.3) GOTO 354
      GOTO 401
CRK-------------
  354 PRINT 355,IOPT
  355 FORMAT(' WRONG OPTION: IOPT=',I10)
      STOP
CRK-------------
C
C CASE A: Q1 RETAINS ITS ORIGINAL DIRECTION
C COMPUTE THE ANGULAR PARAMETERS OF THE Q1 DIRECTION...
  401 DO 402 K=1,3
  402 E1(K)=R1(K)/R1(4)
      C=E1(3)
      S=DSQRT(1.-C**2)
      CF=E1(2)/S
      SF=E1(1)/S
C ...THE ANGLES OF Q2 W.R.T Q1...
      C12=1.-2./X1-2./X2+2./X1/X2
      S12=DSQRT(1.-C12**2)
      PSI=2.*PI*RNDM2(5D0)
      CP=DCOS(PSI)
      SP=DSIN(PSI)
C ...AND COMPUTE THE DIRECTION OF Q2
      E2(1)=SF*(C*S12*SP+S*C12)-CF*S12*CP
      E2(2)=CF*(C*S12*SP+S*C12)+SF*S12*CP
      E2(3)=-S*S12*SP+C*C12
      GOTO 601
C
C CASE B: Q2 RETAINS ITS ORIGINAL DIRECTION
C COMPUTE THE ANGULAR PARAMETERS OF THE Q2 DIRECTION...
  501 DO 502 K=1,3
  502 E2(K)=R2(K)/R2(4)
      C=E2(3)
      S=DSQRT(1.-C**2)
      CF=E2(2)/S
      SF=E2(1)/S
C ...THE ANGLES OF Q1 W.R.T Q2...
      C12=1.-2./X1-2./X2+2./X1/X2
      S12=DSQRT(1.-C12**2)
      PSI=2.*PI*RNDM2(5D0)
      CP=DCOS(PSI)
      SP=DSIN(PSI)
C ...AND COMPUTE THE DIRECTION OF Q1
      E1(1)=SF*(C*S12*SP+S*C12)-CF*S12*CP
      E1(2)=CF*(C*S12*SP+S*C12)+SF*S12*CP
      E1(3)=-S*S12*SP+C*C12
C
C RETURN FROM BOTH CASES: CONSTRUCT Q1 AND Q2 IN THE C.M. FRAME...
  601 R1(4)=X1*R1(4)
      R2(4)=X2*R2(4)
      DO 602 K=1,3
      R1(K)=R1(4)*E1(K)
  602 R2(K)=R2(4)*E2(K)
C ...BOOST'EM TO THE LAB FRAME...
      QOUT1(4)=(Q(4)*R1(4)+Q(3)*R1(3)+Q(2)*R1(2)+Q(1)*R1(1))/W
      QOUT2(4)=(Q(4)*R2(4)+Q(3)*R2(3)+Q(2)*R2(2)+Q(1)*R2(1))/W
      QQ1=(QOUT1(4)+R1(4))/(Q(4)+W)
      QQ2=(QOUT2(4)+R2(4))/(Q(4)+W)
      DO 603 K=1,3
      QOUT1(K)=R1(K)+Q(K)*QQ1
  603 QOUT2(K)=R2(K)+Q(K)*QQ2
C ...AND CONSTRUCT THE GLUON MOMENTUM BY MOMENTUM CONSERVATION
      DO 605 K=1,4
  605 QOUT3(K)=Q(K)-QOUT1(K)-QOUT2(K)
      RETURN
      END

      SUBROUTINE anomal(ipro,s)
C---------------------------------------------------------------------
C! Computes the parameters related to the anomalous contribution to 
C  e+e- -> H f fbar 
C
C Patrick Janot -- 29 Oct 1999
C---------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'consts.inc'
      include 'datapi.inc'
      include 'miscl.inc'
      LOGICAL first
      DATA first /.TRUE./
      COMMON / mytry / afla,ateb
C
      IF (ipro .NE. 1 .OR. ism .NE. 1 ) THEN 
        AAA = 1D0
        BBB = 1D0
        GOTO 999
      ENDIF
C
      ghzg1 = cw*sw*(fw-fb) * s 
      ghzg2 = cw*sw*(sw2*fbb-cw2*fww) * 4D0 * s
      ghzz1 = (cw2*fw+sw2*fb) * s / 16D0              
      ghzz2 = -(sw2*sw2*fbb+cw2*cw2*fww ) * s / 2D0
      ghzg  = ghzg2 + ghzg1/2D0
      v   = -5D-1 + 2D0*sw2
      a   = -5D-1
C
C Watch out, the next line is a (good) approximation. I have
C approximated the Z mass by a constant, which is excellent
C an approximation at LEP2, and slightly worse at LEP1...
C
      ss     = SQRT(s)
      xmz    = MIN(amz,ss-amh)
      xmz2   = xmz*xmz
      gmz2   = gmz*gmz
      ez     = (s+xmz2-amh*amh)/(2D0*ss)
C
      Azz   = 1D0
     &      + 4D0 * ghzz2 * ez/ss
     &      -       ghzz1 * ( 1D0 + 2D0*ez/ss + xmz2/s )
      Azz2  = Azz*Azz
C
      Bzz   = 1D0
     &      -       ghzz1 * ( 1D0 + xmz2/s )
      Bzz2  = Bzz*Bzz
C
      Agg   =       ghzg  * ez/ss
     &      +       ghzg1 * xmz2/s
      Agg2  = Agg*Agg
C
      Bgg2  =       ghzg1**2 
     &      -       ghzg **2 * xmz2/s 
C
      Azg2  = Agg*Azz
C
      Bzg2  = -     ghzg  * (-4D0*ghzz2+2D0*ghzz1) * xmz2/s
     &      -       ghzg1 * (xmz2/s-ghzz1*(1D0+xmz2/s))
C
      AAA   = Azz2
     &      + sw2*cw2/(v**2+a**2)
     &      * ((1D0-xmz2/s)**2+xmz2/s*gmz2/s)
     &      * Agg2
     &      + sw*cw*v/(v**2+a**2)
     &      * (1D0-xmz2/s)
     &      * Azg2
C
      BBB   = Bzz2
     &      + sw2*cw2/(v**2+a**2)
     &      * ((1D0-xmz2/s)**2+xmz2/s*gmz2/s)
     &      * Bgg2
     &      + sw*cw*v/(v**2+a**2)
     &      * (1D0-xmz2/s)
     &      * Bzg2
C
 999  RETURN 
      END

      DOUBLE PRECISION FUNCTION brelec(xx)
C-------------------------------------------------------------------
C! Compute the decay branching ratio of W* --> e nue
C  when produced in the chi' --> chi+ W* decay.
C
C  Input:     -- xx,  the mass difference chi'-chi+
C
C  Output:    -- brelec, the decay branching ratio
C
C  V. Bertin, for the CHA001 generator
C
C  Modif:  Patrick Janot (31 Aug 1995)
C          Adapt the routine for the HZHAxx generator
C-------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION seuil(4),brtot(5)
      DATA seuil/.106D0,.140D0,1.8D0,2.0D0/
      DATA brtot/1D0,2D0,5D0,6D0,9D0/
C
      br = brtot(1)
      DO is = 1, 4
       IF ( xx .GE. seuil(is) ) br = brtot(is+1)
      ENDDO
      brelec = 1D0 / br
C
      RETURN
      END
      DOUBLE PRECISION FUNCTION brnunu(xh,xl,icp)
C-------------------------------------------------------------------
C! Compute the decay branching ratio of Z* --> nu nubar
C  when produced in the chi' --> chi Z* decay.
C
C  Input:     -- xh,  the first neutralino mass
C             -- xl,  the second neutralino mass
C             -- icp, the relative mass sign
C
C  Output:    -- brnunu, the decay branching ratio
C
C  V. Bertin, for the CHA001 generator
C
C  Modif:  Patrick Janot (31 Aug 1995)
C          Adapt the routine for the HZHAxx generator
C-------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      REAL*8 xh,xl
      DIMENSION dlim(19),bnup(20),bnum(20)
      DATA dlim/.001,.212,.270,.988,1.5,2.,3.,4.,5.,7.,8.,10.,
     .          12.,15.,20.,25.,30.,60.,90./
      DATA bnup/1.,.856,.774,.600,.497,.356,.323,.303,.286,.271,.256,
     .          .256,.248,.234,.222,.213,.209,.2075,.205,.204/
      DATA bnum/1.,.857,.729,.465,.389,.289,.275,.278,.248,.234,.232,
     .          .232,.232,.217,.209,.203,.203,.203,.203,.204/
C
      dx = xh-xl
      IF ( icp .GE. 0 ) brneu = bnup(1)
      IF ( icp .LT. 0 ) brneu = bnum(1)
      DO idm = 1,18
       IF ( dx .LE. dlim(3) ) THEN
        IF ( dx .GT. dlim(idm) .AND. dx .LE. dlim(idm+1) ) THEN
         IF ( icp .GE. 0 ) brneu = bnup(idm+1)
         IF ( icp .LT. 0 ) brneu = bnum(idm+1)
        ENDIF
       ELSE IF ( dx .LT. dlim(19) ) THEN
        IF ( dx .GT. dlim(idm) .AND. dx .LE. dlim(idm+1) ) THEN
         IF ( icp .GE. 0 ) THEN
          brneu = (bnup(idm+1) - bnup(idm)) * (dx - dlim(idm))
     .           / (dlim(idm+1) - dlim(idm)) + bnup(idm)
         ELSE
          brneu = (bnum(idm+1) - bnum(idm)) * (dx - dlim(idm))
     .           / (dlim(idm+1) - dlim(idm)) + bnum(idm)
         ENDIF
        ENDIF
       ELSE
        IF ( icp .GE. 0 ) brneu = bnup(20)
        IF ( icp .LT. 0 ) brneu = bnum(20)
       ENDIF
      ENDDO
C
      brnunu = brneu
C
      RETURN
      END
      DOUBLE PRECISION FUNCTION brwipj(a,b,c,d)
C ------------------------------------------------------------------
C! Perfom a 1-dim convolution Sigma(hZ) * Breit Wigner
C
C  Inputs:        a   is ecm**2
C                 b   is the Higgs mass
C                 c,  is the Higgs width
C
C  Output:        brwipj, the hZ cross section with width effects
C
C  Patrick Janot -- 01 Sep 1995
C -------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
C      REAL*4 a,b,c
      COMMON /BHV/ s,am1,w1
      EXTERNAL fsub
      DIMENSION x(1)
      include 'datapi.inc'
      s      = a
      am1    = b
      w1     = c
      zma    = d
      xlo    = -DATAN2(am1,w1)
      xhi1   = xlo
      IF ( s .GT. zma**2 )
     .    xhi1   =  DATAN2((SQRT(s)-zma)**2-am1**2,am1*w1)
      xhi2   =  DATAN2(s-am1**2,am1*w1)
C
      brwipj = 0D0
C
      IF ( xhi1 .GT. xlo )
     .    brwipj = brwipj + DGMLT1(fsub,xlo,xhi1,1,6,x)
      IF ( xhi2 .GT. xhi1 )
     .    brwipj = brwipj + DGMLT1(fsub,xhi1,xhi2,1,6,x)
C
      brwipj = brwipj / (piby2+DATAN2(am1,w1))
C
      RETURN
      END
      DOUBLE PRECISION FUNCTION brwisi(a,b,c,d,e)
C -------------------------------------------------------------------
C! Perfom a 2-dim convolution Sigma(hA)*BreitWigner(h)*BreitWigner(A)
C
C  Inputs          a   is ecm**2
C                  b,c are h,A on-shell masses
C                  d,e are h,A widths
C
C  Output:         brwisi, the hA cross section with width effects
C
C  Patrick Janot -- 01 Sep 1995
C -------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
C      REAL*4 a,b,c,d,e
      COMMON /BWC/ s,am1,am2,w1,w2,v1,v2
      EXTERNAL fsub2
      DIMENSION x(2)
      include 'datapi.inc'
      s      = a
      am1    = b
      am2    = c
      w1     = d
      w2     = e
      x2lo   = -DATAN2(am2,w2)
      x2hi   =  DATAN2(s-am2**2,am2*w2)
      brwisi = DGMLT2(fsub2,x2lo,x2hi,1,6,x)
     .       / (piby2+DATAN2(am1,w1))
     .       / (piby2+DATAN2(am2,w2))
C
      RETURN
      END
      DOUBLE PRECISION FUNCTION brwwzz(a,b,c,d,e)
C ------------------------------------------------------------------
C! Compute the decay width of a Higgs boson into W+W- and Z Z
C  with a 2-dim convolution phase space * Breit Wigner
C
C  Inputs :   a   is mh (Higgs mass)
C             b   is mv (Boson mass)
C             c   is gv (Boson width)
C
C Patrick Janot. 19 Sep 1995.
C
C Modifications:
C   o Patrick Janot -- 24 Nov 1999
C     Split the integration in two parts for an adequate accuracy
C -------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON /BWC/ s,am1,am2,w1,w2,v1,v2
      EXTERNAL fsubwz2
      DIMENSION x(2)
      include 'datapi.inc'
      s      = a**2
      am1    = b
      am2    = b
      w1     = c
      w2     = c
      v1     = d
      v2     = e
      x2lo   = -DATAN2(am2,w2)
      xint   =  DATAN2((a-am2)**2-am2**2,am2*w2)
      x2hi   =  DATAN2(s-am2**2,am2*w2)
C
      IF ( ABS(a-b)/b .GE. 5D-6 ) THEN 
        brwwzz = ( DGMLT2(fsubwz2,x2lo,xint,20,8,x)
     &         +   DGMLT2(fsubwz2,xint,x2hi,20,8,x) )
      ELSE
        brwwzz =   DGMLT2(fsubwz2,x2lo,x2hi,20,8,x)
      ENDIF
C
      brwwzz = brwwzz
     .       / (piby2+DATAN2(am1,w1))
     .       / (piby2+DATAN2(am2,w2))
C
      RETURN
      END
      SUBROUTINE bwgene(xmin,xmax,x,g,w,dj)
C-----------------------------------------------------------------
C! Generate w with a Breit-Wigner probability, centered on x
C  and of width g. Variable dj is the corresponding Jacobian.
C
C  Patrick Janot -- 15 Oct. 1992
C-----------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
C      REAL*4 xmin,xmax,x,g,w,dj,RNDM
C      EXTERNAL RNDM
C
      argmin = (xmin**2-x**2)/(x*g)
      argmax = (xmax**2-x**2)/(x*g)
      w2 = x**2 + x * g *  DTAN (
     .                   ( DATAN(argmax) - DATAN(argmin) )
     .                   * RNDM2(w2)
     .                   + DATAN(argmin) )
C                                                                       
      IF ( w2 .LT. 0 ) THEN 
        w = (xmin+xmax)/2.
      ELSE
        w  = DSQRT(w2)
      ENDIF
C
      dj = DATAN(argmax)-DATAN(argmin)
      dj = dj / (x*g)
C
      RETURN
      END
      SUBROUTINE chaneucp
C------------------------------------------------------------------
C!  Compute neutralino and chargino couplings to the Higgses
C
C  Input:    /PARAM/ MSSM parameters
C
C  Output:   /PARAM/ aa1,aa2,aa3, the couplings to the neutral
C                                 Higgs bosons
C
C  P. Janot -- 4 December 1994
C------------------------------------------------------------------
      IMPLICIT REAL*8 (A-H,O-Z)
      include 'param.inc'
C
C Couplings F_ijh of the neutral Higgs boson "h" to a neutralino
C pair chi_i,chi_j ("h" = generic name for H,h,A)
C
      DO 1 i=1,4
        DO 2 j=1,4
          aa(1,i,j) = ( qqmat(i,j)*ca-ssmat(i,j)*sa)/2.
          aa(2,i,j) = ( qqmat(i,j)*sa+ssmat(i,j)*ca)/2.
          aa(3,i,j) = (-qqmat(i,j)*sb+ssmat(i,j)*cb)/2.
    2   CONTINUE
    1 CONTINUE
C
C Couplings F_ijh of the neutral Higgs boson "h" to a chargino
C pair chi+_i,chi-_j ("h" = generic name for H,h,A)
C
      DO 3 i=1,2
        DO 4 j=1,2
          bb(1,i,j) = ( ca*vmat(i,1)*umat(j,2)
     .                 +sa*vmat(i,2)*umat(j,1))/SQRT(2.)
          bb(2,i,j) = (-sa*vmat(i,1)*umat(j,2)
     .                 +ca*vmat(i,2)*umat(j,1))/SQRT(2.)
          bb(3,i,j) = (-sb*vmat(i,1)*umat(j,2)
     .                 -cb*vmat(i,2)*umat(j,1))/SQRT(2.)
    4   CONTINUE
    3 CONTINUE
C
      RETURN
      END
      SUBROUTINE chargi
C------------------------------------------------------------------
C!  Compute the chargino masses from the MSSM parameters
C
C  Input:    /PARAM/ MSSM parameters
C
C  Output:   /PARAM/ amchar(2) the chargino masses
C
C
C  P. Janot -- 4 December 1994
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'datapi.inc'
      include 'consts.inc'
      include 'miscl.inc'
      DIMENSION u(2,2),v(2,2)
C
C The chargino mass matrix
C
      a11 = susM2
      a12 = SQRT(2.)*amw*sb
      a21 = SQRT(2.)*amw*cb
      a22 = susMU
C
C The angle of the two diagonalization matrices
C
      r1   = 2.*(a22*a12+a11*a21)
      t1   = a22**2-a11**2+a21**2-a12**2
      phi1 = DATAN2(r1,t1)/2.
      c1   = COS(phi1)
      s1   = SIN(phi1)
C
      r2   = 2.*(a22*a21+a11*a12)
      t2   = a22**2-a11**2+a12**2-a21**2
      phi2 = DATAN2(r2,t2)/2.
      c2   = COS(phi2)
      s2   = SIN(phi2)
C
C The eigenvalues (=the chargino masses)
C
      amchar(1) =  a11*c1*c2+a22*s1*s2-a12*c1*s2-a21*s1*c2
      amchar(2) =  a11*s1*s2+a22*c1*c2+a12*s1*c2+a21*c1*s2
C
C Check for mass positivity and mass ordering
C
      eps1 = SIGN(1D0,amchar(1))
      eps2 =-SIGN(1D0,amchar(2))
C
C The diagonalization matrices U and V
C
      IF ( ABS(amchar(2)) .LE. ABS(amchar(1)) ) THEN
        u(1,1) = -s1
        u(1,2) = -c1
        u(2,1) =  c1
        u(2,2) = -s1
        v(1,1) =  s2*eps2
        v(1,2) =  c2*eps2
        v(2,1) =  c2*eps1
        v(2,2) = -s2*eps1
      ELSE
        u(1,1) = -c1
        u(1,2) =  s1
        u(2,1) = -s1
        u(2,2) = -c1
        v(1,1) = -c2*eps1
        v(1,2) =  s2*eps1
        v(2,1) =  s2*eps2
        v(2,2) =  c2*eps2
      ENDIF
      CALL ucopy(u(1,1),umat(1,1),2*2*2)
      CALL ucopy(v(1,1),vmat(1,1),2*2*2)
C
C The mass after ordering
C
      amchar(1) = u(1,1)*a11*v(1,1)
     .          + u(1,1)*a12*v(1,2)
     .          + u(1,2)*a21*v(1,1)
     .          + u(1,2)*a22*v(1,2)
      amchar(2) = u(2,1)*a11*v(2,1)
     .          + u(2,1)*a12*v(2,2)
     .          + u(2,2)*a21*v(2,1)
     .          + u(2,2)*a22*v(2,2)
C
      RETURN
      END
      DOUBLE PRECISION FUNCTION chicha(ichip,ichar,isens)
C-------------------------------------------------------------------
C! Compute the decay width of chi --> chi+ W* or chi+ --> chi W*
C  where chi' is any of the four neutralinos.
C
C  Input:     -- ichip, the neutralino index
C             -- ichar, the chargino index
C             -- isens, = 1 for chi  --> chi+ W*
C                       = 2 for chi+ --> chi  W*
C
C  Output:    -- chicha, the decay width in GeV
C
C  V. Bertin, for the CHA001 generator
C
C  Modif:  Patrick Janot (31 Aug 1995)
C          Adapt the routine for the HZHAxx generator
C-------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'consts.inc'
      include 'datapi.inc'
      REAL*8 a,g,xx,xip,clr2,cltr, DGMLT1, xlo,xhi,x(1)
      COMMON / chachi / a,g,xx,xip,clr2,cltr
      EXTERNAL dgmlt1, fsubcha
C
C     PRINT *,'ichii :',ichip
      chicha = 0.
      IF ( isens .EQ. 1 ) THEN
        xip = ABS(amneut(ichip))
        dif = (xip - amchar(ichar))
        xx = amchar(ichar) / amneut(ichip)
        IF ( dif .LT. .0005 ) GOTO 999
      ELSEIF ( isens .EQ. 2 ) THEN
        xip = ABS(amchar(ichar))
        dif = (xip - ABS(amneut(ichip)))
        xx = amneut(ichip) / amchar(ichar)
        IF ( dif .LT. .0005 ) GOTO 999
      ELSE
        GOTO 999
      ENDIF
C
      copl = fieldn(2,ichip)*vmat(ichar,1)
     .     - fieldn(4,ichip)*vmat(ichar,2)/SQRT(2.)
      copr = fieldn(2,ichip)*umat(ichar,1)
     .     + fieldn(3,ichip)*umat(ichar,2)/SQRT(2.)
      clr2 = copl**2 + copr**2
      cltr = copl * copr
C     PRINT *,'copl copr clr2 cltr :',copl,copr,clr2,cltr
C
      a = amw
      g = gmw
      xlo    = -DATAN2(a/xip,g/xip)
      xhi    =  DATAN2((1D0-DABS(xx))**2-(a/xip)**2,a*g/xip**2)
      partot = DGMLT1(fsubcha,xlo,xhi,1,6,x)
     .       / ( a * g / xip**2 )
C
      brel = brelec(dif)
C
      chicha = alpha(0)**2 * xip * partot / brel
     .       / (48.*pi*sw2**2)
C     PRINT *,'gamcha partot brel :',gamcha,partot,brel
      RETURN
C
 999  CONTINUE
      RETURN
      END
      SUBROUTINE chidec
C------------------------------------------------------------------
C! Compute the decay width and branching ratios of the neutralinos
C  into Z* chi, W*-/+ chi+/- and chi gamma, and of the charginos
C  yet into W* chi only ( Z* chi+ to come)
C
C  Patrick Janot -- 31 August 1995
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'hhdeck.inc'
C
      CALL vzero(brneut(1,1),4*nchneut*2)
      CALL vzero(brchar(1,1),2*nchchar*2)
      CALL vzero(widneut(1),4*2)
      CALL vzero(widchar(1),2*2)
C
      DO ineut = 1, 4
C
C chi' --> chi gamma
C
        DO jneut = 1, ineut-1
          brneut(jneut,ineut) = chipho(ineut,jneut)
        ENDDO
C
C chi' --> chi Z*
C
        DO jneut = 1, ineut-1
          brneut(3+jneut,ineut) = chizst(ineut,jneut)
        ENDDO
C
C chi' --> chi+/- W*-/+
C chi+ --> chi'   W*-/+
C
        DO jchar = 1, 2
          IF ( ABS(amneut(ineut)) .GT. ABS(amchar(jchar)) ) THEN
            brneut(6+jchar,ineut) = chicha(ineut,jchar,1)
          ELSE
            brchar(ineut,jchar) = chicha(ineut,jchar,2)
          ENDIF
        ENDDO
C
      ENDDO
C
C chi+(2) --> chi+(1) Z*-/+  ! Not implemented yet
C
CCCC  brchar(5,2) = chazst(1.)
C
C Total width for the neutralinos
C
      DO ineut = 1, 4
        widneut(ineut) = 0.
        DO ic = 1 , nchneut
          widneut(ineut) = widneut(ineut) + brneut(ic,ineut)
        ENDDO
C
C And branching ratios
C
        IF ( widneut(ineut) .NE. 0. ) THEN
          DO ic = 1 , nchneut
            brneut(ic,ineut) = brneut(ic,ineut)/widneut(ineut)
          ENDDO
        ENDIF
C
      ENDDO
C
C Total width for the charginos
C
      DO ichar = 1, 2
        widchar(ichar) = 0.
        DO ic = 1 , nchchar
          widchar(ichar) = widchar(ichar) + brchar(ic,ichar)
        ENDDO
C
C And branching ratios
C
        IF ( widchar(ichar) .NE. 0. ) THEN
          DO ic = 1 , nchchar
            brchar(ic,ichar) = brchar(ic,ichar)/widchar(ichar)
          ENDDO
        ENDIF
C
      ENDDO
C
C Here we go !
C
      channeut(7,1) = 'chi10 --> chi1+/- W  '
      channeut(8,1) = 'chi10 --> chi2+/- W  '
      channeut(1,2) = 'chi20 --> chi10 gamma'
      channeut(4,2) = 'chi20 --> chi10   Z  '
      channeut(7,2) = 'chi20 --> chi1+/- W  '
      channeut(8,2) = 'chi20 --> chi2+/- W  '
      channeut(1,3) = 'chi30 --> chi10 gamma'
      channeut(2,3) = 'chi30 --> chi20 gamma'
      channeut(4,3) = 'chi30 --> chi10   Z  '
      channeut(5,3) = 'chi30 --> chi20   Z  '
      channeut(7,3) = 'chi30 --> chi1+/- W  '
      channeut(8,3) = 'chi30 --> chi2+/- W  '
      channeut(1,4) = 'chi40 --> chi10 gamma'
      channeut(2,4) = 'chi40 --> chi20 gamma'
      channeut(3,4) = 'chi40 --> chi30 gamma'
      channeut(4,4) = 'chi40 --> chi10   Z  '
      channeut(5,4) = 'chi40 --> chi20   Z  '
      channeut(6,4) = 'chi40 --> chi30   Z  '
      channeut(7,4) = 'chi40 --> chi1+/- W  '
      channeut(8,4) = 'chi40 --> chi2+/- W  '
C
      chanchar(1,1) = 'chi1+ --> chi10   W  '
      chanchar(2,1) = 'chi1+ --> chi20   W  '
      chanchar(3,1) = 'chi1+ --> chi30   W  '
      chanchar(4,1) = 'chi1+ --> chi40   W  '
      chanchar(1,2) = 'chi2+ --> chi10   W  '
      chanchar(2,2) = 'chi2+ --> chi20   W  '
      chanchar(3,2) = 'chi2+ --> chi30   W  '
      chanchar(4,2) = 'chi2+ --> chi40   W  '
      chanchar(5,2) = 'chi2+ --> chi1+   Z  '
C
      RETURN
      END
      SUBROUTINE chideca(p1,ichi1,ichi,ichi2,jchi,ifn)
C------------------------------------------------------------------
C!  Derive the quadrimomenta of the neutral/charg-ino decay
C   particles
C
C   Input:   -- p1(4),  the quadri-momentum  of the initial
C                       charg/neutral-ino
C            -- ichi1,  its index
C            -- ichi,   its electric charge (absolute value)
C            -- ichi2,  the index of the final charg/neutral-ino
C            -- jchi,   its electric charge
C            -- ifn,    the final state boson index
C                        = 0 for a photon
C                        = 1 for a Z
C                        = 2 for a W
C
C   Output:  -- pvect4,  the quadri-momenta of the final state
C                        particles, stored in /VECT4/
C
C  P. Janot --  31 Aug 1995
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION p1(4),p2(4),p3(4)
      REAL*8 betax,betay,betaz
      include 'param.inc'
      include 'hhdeck.inc'
      include 'datapi.inc'
C
      CALL vzero(pvect4(1,1),10*2)
C
C Masses of the two charg/neutral-inos
C
      IF ( ichi .EQ. 0 ) THEN
        chi1mas = ABS(amneut(ichi1))
      ELSE
        chi1mas = ABS(amchar(ichi1))
      ENDIF
C
      IF ( jchi .EQ. 0 ) THEN
        chi2mas = ABS(amneut(ichi2))
      ELSE
        chi2mas = ABS(amchar(ichi2))
      ENDIF
      IF ( idbg .GE. 10 ) THEN
        WRITE(6,*) ' +++ CHIDECA +++ '
        WRITE(6,*) 'am1/am2 : ',chi1mas,chi2mas
      ENDIF
C
C Generate the mass of the final boson according to a Breit-Wigner
C
    1 CONTINUE
      IF ( ifn .EQ. 0 ) THEN
        zstarm = 0.
      ELSE
        zstarmx = chi1mas-chi2mas
        zstarmn = 0.
        IF ( ifn .EQ. 1 ) THEN
          amm = amz
          gmm = gmz
        ELSE
          amm = amw
          gmm = gmw
        ENDIF
        CALL bwgene(zstarmn,zstarmx,amm,gmm,zstarm,djdum)
        IF ( zstarm .GE. zstarmx .OR. zstarm .LE. zstarmn ) GOTO 1
      ENDIF
      IF ( idbg .GE. 10 ) THEN
        WRITE(6,*) ' +++ CHIDECA +++ '
        WRITE(6,*) 'zstarm : ',zstarm
      ENDIF
C
C Compute the phase space factor
C
      phspace = SQRT( (chi1mas**2-(chi2mas+zstarm)**2)
     .               *(chi1mas**2-(chi2mas-zstarm)**2) )
     .        / (2. * chi1mas )
      phspmax = (chi1mas**2 - chi2mas**2)
     .        / (2. * chi1mas )
C
C Select according phase space
C
      IF ( phspace .LT. phspmax * RNDM2(zstarm) ) GOTO 1
C
C The boson quadri-momentum (assuming a uniform decay)
C
      phistr = 2.*pi*RNDM2(zstarm)
      costar = RNDM2(zstarm)
      sintar = SQRT(1.-costar**2)
      p3(1) = phspace * sintar * COS(phistr)
      p3(2) = phspace * sintar * SIN(phistr)
      p3(3) = phspace * costar
      p3(4) = SQRT(phspace**2+zstarm**2)
C
C The charg/neutral-ino quadri-momentum
C
      p2(1) = -p3(1)
      p2(2) = -p3(2)
      p2(3) = -p3(3)
      p2(4) = SQRT(phspace**2+chi2mas**2)
      IF ( idbg .GE. 10 ) THEN
        WRITE(6,*) ' +++ CHIDECA +++ Avant boost '
        WRITE(6,*) ' p2    : ',p2
        WRITE(6,*) ' p3    : ',p3
        WRITE(6,*) ' p2+p3 : ',(p2(i)+p3(i),i=1,4)
      ENDIF
C
C Boost in the LAB frame
C
      betax = -p1(1)/p1(4)
      betay = -p1(2)/p1(4)
      betaz = -p1(3)/p1(4)
      CALL lorenz(betax,betay,betaz,p2)
      CALL lorenz(betax,betay,betaz,p3)
      CALL ucopy(p2(1),pvect4(1,1),4*2)
      CALL ucopy(p3(1),pvect4(1,2),4*2)
      pvect4(5,1) = chi2mas
      pvect4(5,2) = zstarm
C
      IF ( idbg .GE. 10 ) THEN
        WRITE(6,*) ' +++ CHIDECA +++ Apres boost'
        WRITE(6,*) ' p2    : ',p2
        WRITE(6,*) ' p3    : ',p3
        WRITE(6,*) ' p2+p3 : ',(p2(i)+p3(i),i=1,4)
        WRITE(6,*) ' p1    : ',p1
      ENDIF
  999 RETURN
      END
      DOUBLE PRECISION FUNCTION chipho(ichip, ichi)
C-------------------------------------------------------------------
C! Compute the decay width of chi' --> chi gamma
C  where chi' and chi are any two of the four neutralinos.
C
C  Input:     -- ichip, the first neutralino index
C             -- ichi,  the second neutralino index
C
C  Output:    -- chipho, the decay width in GeV
C
C  V. Bertin, for the CHA001 generator
C
C  Modif:  Patrick Janot (31 Aug 1995)
C          Adapt the routine for the HZHAxx generator
C          In particular, the sfermion and Higgs masses have
C          been computed with the MSSM relations instead of
C          being entered with DATA statements
C-------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'consts.inc'
      include 'datapi.inc'
      REAL*8 fi,fj,fk,fi2
      EXTERNAL fi,fj,fk,fi2
      DIMENSION amsf(8),amf(8),ch(8),color(8)
      DIMENSION fielnp(4),fielnm(4)
      DATA color/3.,3.,3.,3.,3.,1.,1.,1./
C
      chigam = 0.
c     PRINT *,'ichii ichio :',ichip,ichi
C
C-- Masses & CP & charges
C
      xxip = amneut(ichip)
      xip  = ABS(xxip)
      xxi  = amneut(ichi)
      xi   = ABS(xxi)
      amhg  = amw
      DO i = 1, 6
        amsf(i) = susSMQ
      ENDDO
      amsf(7) = amsb(1)
      amsf(8) = amsb(2)
      amf(1) = amd
      amf(2) = amu
      amf(3) = ams
      amf(4) = amc
      amf(5) = amb
      amf(6) = ame
      amf(7) = ammu
      amf(8) = amtau
      ch(1)  =-1./3.
      ch(3)  =-1./3.
      ch(5)  =-1./3.
      ch(2)  = 2./3.
      ch(4)  = 2./3.
      ch(6)  =-1.
      ch(7)  =-1.
      ch(8)  =-1.
C
      IF ( xi .GE. xip ) GOTO 999
C
C-- Relevant fields combinaisons
C
      tw = sw/cw
      fielnp(ichip) = fieldn(2,ichip) + tw * fieldn(1,ichip)
      fielnm(ichip) = fieldn(2,ichip) - tw * fieldn(1,ichip)
      fielnp(ichi ) = fieldn(2,ichi)  + tw * fieldn(1,ichi)
      fielnm(ichi ) = fieldn(2,ichi)  - tw * fieldn(1,ichi)
C
C-- Initialisation
C
      copw = 0.
      copf = 0.
      copg = 0.
      coph = 0.
      chigam = 0.
C
      DO 1 icharg = 1,2
C      IF ( xip .GE. amw + amchar(icharg) ) GOTO 901
C--- W-Chargino loop
       aw = fieldn(2,ichi)*fieldn(2,ichip)*
     .      (vmat(icharg,1)**2 - umat(icharg,1)**2)
     .    + .5 * (fieldn(4,ichi)*fieldn(4,ichip)*vmat(icharg,2)**2
     .         -  fieldn(3,ichi)*fieldn(3,ichip)*umat(icharg,2)**2)
     .    - (vmat(icharg,1) * vmat(icharg,2) *
     .       (fieldn(2,ichi) * fieldn(4,ichip) +
     .        fieldn(4,ichi) * fieldn(2,ichip))
     .     + umat(icharg,1) * umat(icharg,2) *
     .       (fieldn(2,ichi) * fieldn(3,ichip) +
     .        fieldn(3,ichi) * fieldn(2,ichip))) / SQRT(2.)
       bw = .5 * umat(icharg,2) * vmat(icharg,2) *
     .            (fieldn(3,ichi) * fieldn(4,ichip) -
     .             fieldn(4,ichi) * fieldn(3,ichip))
     .     + (umat(icharg,1) * vmat(icharg,2) *
     .        (fieldn(2,ichi) * fieldn(4,ichip) -
     .         fieldn(4,ichi) * fieldn(2,ichip))
     .      + umat(icharg,2) * vmat(icharg,1) *
     .        (fieldn(2,ichi) * fieldn(3,ichip) -
     .         fieldn(3,ichi) * fieldn(2,ichip))) / SQRT(2.)
       copw = copw
     .      + aw * (xxip
     .           * (FI2(amw/amz,amchar(icharg)/amz,xip/amz,xi/amz)
     .           -  FJ( amw/amz,amchar(icharg)/amz,xip/amz,xi/amz)
     .           -  FK( amw/amz,amchar(icharg)/amz,xip/amz,xi/amz))
     .           +  xxi
     .           * (FJ( amw/amz,amchar(icharg)/amz,xip/amz,xi/amz)
     .           -  FK( amw/amz,amchar(icharg)/amz,xip/amz,xi/amz)))
     .           +  2. * bw * amchar(icharg)
     .           *  FJ( amw/amz,amchar(icharg)/amz,xip/amz,xi/amz)
C     PRINT *,'W loop : ich aw bw copw :',icharg,aw,bw,copw
C
C--- H-Chargino loop
C      IF ( xip .GE. amhp + amchar(icharg) ) GOTO 902
       ah = s2b
     .     * (umat(icharg,1) * vmat(icharg,1)
     .      * (fieldn(4,ichi) * fieldn(3,ichip)
     .       - fieldn(3,ichi) * fieldn(4,ichip))
     .     - (fielnp(ichip)  *
     .        (fieldn(3,ichi) * umat(icharg,1) * vmat(icharg,2)
     .       + fieldn(4,ichi) * umat(icharg,2) * vmat(icharg,1))
     .      - fielnp(ichi) *
     .        (fieldn(3,ichip) * umat(icharg,1) * vmat(icharg,2)
     .       + fieldn(4,ichip) * umat(icharg,2) * vmat(icharg,1)))
     .      / SQRT(2.))
       bh = 2.* cb2 * (fieldn(4,ichi) * vmat(icharg,1)
     .               + fielnp(ichi) * vmat(icharg,2) / SQRT(2.))
     .              * (fieldn(4,ichip) * vmat(icharg,1)
     .               + fielnp(ichip) * vmat(icharg,2) / SQRT(2.))
     .    - 2.* sb2 * (fieldn(3,ichi) * umat(icharg,1)
     .               - fielnp(ichi) * umat(icharg,2) / SQRT(2.))
     .              * (fieldn(3,ichip) * umat(icharg,1)
     .               - fielnp(ichip) * umat(icharg,2) / SQRT(2.))
       coph = coph +
     .        bh * ( xxip
     .           * (FI2(amhp/amz,amchar(icharg)/amz,xip/amz,xi/amz)
     .           -  FK( amhp/amz,amchar(icharg)/amz,xip/amz,xi/amz))
     .           -   xxi
     .           *  FK( amhp/amz,amchar(icharg)/amz,xip/amz,xi/amz))
     .           + amchar(icharg) * ah
     .           *  FI( amhp/amz,amchar(icharg)/amz,xip/amz,xi/amz)
C     PRINT *,'H loop : ich ah bh coph :',icharg,ah,bh,coph
C
C--- G-Chargino loop
       ag = -s2b
     .     * (umat(icharg,1) * vmat(icharg,1)
     .      * (fieldn(4,ichi) * fieldn(3,ichip)
     .       - fieldn(3,ichi) * fieldn(4,ichip))
     .     - (fielnp(ichip)  *
     .        (fieldn(3,ichi) * umat(icharg,1) * vmat(icharg,2)
     .       + fieldn(4,ichi) * umat(icharg,2) * vmat(icharg,1))
     .      - fielnp(ichi) *
     .        (fieldn(3,ichip) * umat(icharg,1) * vmat(icharg,2)
     .       + fieldn(4,ichip) * umat(icharg,2) * vmat(icharg,1)))
     .      / SQRT(2.))
       bg = 2.* sb2 * (fieldn(4,ichi) * vmat(icharg,1)
     .               + fielnp(ichi) * vmat(icharg,2) / SQRT(2.))
     .              * (fieldn(4,ichip) * vmat(icharg,1)
     .               + fielnp(ichip) * vmat(icharg,2) / SQRT(2.))
     .    - 2.* cb2 * (fieldn(3,ichi) * umat(icharg,1)
     .               - fielnp(ichi) * umat(icharg,2) / SQRT(2.))
     .              * (fieldn(3,ichip) * umat(icharg,1)
     .               - fielnp(ichip) * umat(icharg,2) / SQRT(2.))
       copg = copg +
     .        bg * ( xxip
     .           * (FI2(amhg/amz,amchar(icharg)/amz,xip/amz,xi/amz)
     .           -  FK( amhg/amz,amchar(icharg)/amz,xip/amz,xi/amz))
     .           -   xxi
     .           *  FK( amhg/amz,amchar(icharg)/amz,xip/amz,xi/amz))
     .           +  amchar(icharg) * ag
     .           *  FI( amhg/amz,amchar(icharg)/amz,xip/amz,xi/amz)
C     PRINT *,'G loop : ich ag bg copg :',icharg,ag,bg,copg
 1    CONTINUE
C
C--- Sfermion-Fermion loop
      DO 2 isfer=1,8
C      IF ( xip .GE. amsf(isfer) + amf(isfer) ) GOTO 903
       IF ( ch(isfer) .GE. 0. ) THEN
C--- Up fermions except top
        af = -2. * amf(isfer) / (amw * sb)
     .     * (fieldn(4,ichi) * (.5 * fielnm(ichip)
     .     +  ch(isfer) * tw * fieldn(1,ichip))
     .     - fieldn(4,ichip) * (.5 *fielnm(ichi)
     .     + ch(isfer) * tw * fieldn(1,ichi))
     .     - ch(isfer) * tw
     .     * (fieldn(4,ichip) * fieldn(1,ichi)
     .     -  fieldn(1,ichip) * fieldn(4,ichi)))
        bf = fielnm(ichip) * fielnm(ichi)
     .     + 2.* ch(isfer) * tw
     .     * (fieldn(1,ichi ) * fielnm(ichip)
     .     +  fieldn(1,ichip) * fielnm(ichi))
       ELSE
C--- Down fermions
        af = -2. * amf(isfer) / (amw * cb)
     .     * (fieldn(3,ichi) * (-.5 * fielnm(ichip)
     .     + ch(isfer) * tw * fieldn(1,ichip))
     .     - fieldn(3,ichip) * (-.5 *fielnm(ichi)
     .     + ch(isfer) * tw * fieldn(1,ichi))
     .     - ch(isfer) * tw
     .     * (fieldn(3,ichip) * fieldn(1,ichi)
     .     -  fieldn(1,ichip) * fieldn(3,ichi)))
        bf = fielnm(ichip) * fielnm(ichi)
     .     - 2.* ch(isfer) * tw
     .     * (fieldn(1,ichi) * fielnm(ichip)
     .     + fieldn(1,ichip) * fielnm(ichi))
       ENDIF
       copf = copf
     .      + ch(isfer) * color(isfer)
     .      * (bf*(xxip
     .           *(FI2(amsf(isfer)/amz,amf(isfer)/amz,xip/amz,xi/amz)
     .           - FK( amsf(isfer)/amz,amf(isfer)/amz,xip/amz,xi/amz))
     .           - xxi
     .           * FK( amsf(isfer)/amz,amf(isfer)/amz,xip/amz,xi/amz))
     .        +af* amf(isfer)
     .           * FI( amsf(isfer)/amz,amf(isfer)/amz,xip/amz,xi/amz))
C     PRINT *,'F loop : isf af bf copf :',isfer,af,bf,copf
 2    CONTINUE
C
      ctt = COS(topmix)
      stt = SIN(topmix)
C
C-- 1st scalar top
C
      at1= (ctt * (fielnm(ichi) + 4./3. * tw * fieldn(1,ichi))
     .    + stt * fieldn(4,ichi) * amt / (amw * sb))
     .   * (ctt * fieldn(4,ichip) * amt / (amw * sb)
     .    - 4./3. * stt * fieldn(1,ichip) * tw)
     .   - (ctt * fieldn(4,ichi) * amt / (amw * sb)
     .    - 4./3. * stt * fieldn(1,ichi) * tw)
     .   * (ctt * (fielnm(ichip) + 4./3. * tw * fieldn(1,ichip))
     .    + stt * fieldn(4,ichip) * amt / (amw * sb))
      bt1= (ctt * (fielnm(ichi) + 4./3. * tw * fieldn(1,ichi))
     .    + stt * fieldn(4,ichi) * amt / (amw * sb))
     .   * (ctt * (fielnm(ichip) + 4./3. * tw * fieldn(1,ichip))
     .    + stt * fieldn(4,ichip) * amt / (amw * sb))
     .   - (ctt * fieldn(4,ichi) * amt / (amw * sb)
     .    - 4./3. * stt * fieldn(1,ichi) * tw)
     .   * (ctt * fieldn(4,ichip) * amt / (amw * sb)
     .    - 4./3. * stt * fieldn(1,ichip) * tw)
      copf = copf
     .    + 2./3. * 3. *
     .      (bt1 * (xxip
     .           * (FI2(amst(1)/amz,amt/amz,xip/amz,xi/amz)
     .           -  FK( amst(1)/amz,amt/amz,xip/amz,xi/amz))
     .           - xxi
     .           *  FK( amst(1)/amz,amt/amz,xip/amz,xi/amz))
     .      +at1 * amt
     .           *  FI( amst(1)/amz,amt/amz,xip/amz,xi/amz))
c     PRINT *,'T1 loop : at1 bt1 copf :',at1,bt1,copf
C--- 2nd scalar top
      at2= (stt * (fielnm(ichi) + 4./3. * tw * fieldn(1,ichi))
     .    - ctt * fieldn(4,ichi) * amt / (amw * sb))
     .   * (stt * fieldn(4,ichip) * amt / (amw * sb)
     .    + 4./3. * ctt * fieldn(1,ichip) * tw)
     .   - (stt * fieldn(4,ichi) * amt / (amw * sb)
     .    + 4./3. * ctt * fieldn(1,ichi) * tw)
     .   * (stt * (fielnm(ichip) + 4./3. * tw * fieldn(1,ichip))
     .    - ctt * fieldn(4,ichip) * amt / (amw * sb))
      bt2= (stt * (fielnm(ichi) + 4./3. * tw * fieldn(1,ichi))
     .    - ctt * fieldn(4,ichi) * amt / (amw * sb))
     .   * (stt * (fielnm(ichip) + 4./3. * tw * fieldn(1,ichip))
     .    - ctt * fieldn(4,ichip) * amt / (amw * sb))
     .   - (stt * fieldn(4,ichi) * amt / (amw * sb)
     .    + 4./3. * ctt * fieldn(1,ichi) * tw)
     .   * (stt * fieldn(4,ichip) * amt / (amw * sb)
     .    + 4./3. * ctt * fieldn(1,ichip) * tw)
      copf = copf
     .     + 2./3. * 3. *
     .      (bt2 * (xxip
     .           * (FI2(amst(2)/amz,amt/amz,xip/amz,xi/amz)
     .           -  FK( amst(2)/amz,amt/amz,xip/amz,xi/amz))
     .           - xxi
     .           *  FK( amst(2)/amz,amt/amz,xip/amz,xi/amz))
     .     + at2 * amt
     .           *  FI( amst(2)/amz,amt/amz,xip/amz,xi/amz))
C     PRINT *,'T2 loop : at2 bt2 copf :',at2,bt2,copf
C
C-- Total coupling constant
C
      copt = copw - (coph + copg +copf)/ 4.
C
C-- Width chip --> chi gamma
C
      chigam = alpha(0)**3 * (copt/amz)**2
     .       * ((xip/amz)**2 - (xi/amz)**2)**3
     .       / (xip/amz)**3
     .       / (8. * pi**2 * sw2**2)
     .       * amz
C     PRINT *,'copt chigam :',copt,chigam
C
      chipho = chigam
C
C     RETURN
C
C900  CONTINUE
C     WRITE(6,*) 'Impossible decay ',ichip,' into ',ichi
C     chipho = 0.
C     RETURN
C
C901  CONTINUE
C     WRITE(6,*) 'Posssible direct decay into W-Chargino'
C     chipho = 0.
C     RETURN
C
C902  CONTINUE
C     WRITE(6,*) 'Posssible direct decay into H-Chargino'
C     chipho = 0.
C     RETURN
C
C903  CONTINUE
C     WRITE(6,*) 'Posssible direct decay into Fermion-Sfermion'
C     chipho = 0.
C     RETURN
C
  999 RETURN
      END
      DOUBLE PRECISION FUNCTION chizst(ichip,ichi)
C-------------------------------------------------------------------
C! Compute the decay width of chi' --> chi Z*
C  where chi' and chi are any two of the four neutralinos.
C
C  Input:     -- ichip, the first neutralino index
C             -- ichi,  the second neutralino index
C
C  Output:    -- chizst, the decay width in GeV
C
C  V. Bertin, for the CHA001 generator
C
C  Modif:  Patrick Janot (31 Aug 1995)
C          Adapt the routine for the HZHAxx generator
C-------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'consts.inc'
      include 'datapi.inc'
      REAL*8 a,g,xx,xi,xip,clr2,cltr, DGMLT1, xlo,xhi,x(1)
      COMMON / chachi / a,g,xx,xip,clr2,cltr
      EXTERNAL dgmlt1, fsubzst
C
C     PRINT *,'ichii ichio :',ichip,ichi
      chizst = 0.
      xip = ABS(amneut(ichip))
      xi  = ABS(amneut(ichi))
      xx = amneut(ichi) / amneut(ichip)
      isgn = 1
      IF ( xx .LT. 0D0 ) isgn = -1
      IF ( xip .LE. xi ) GOTO 999
C
      a = amz
      g = gmz
      xlo    = -DATAN2(a/xip,g/xip)
      xhi    =  DATAN2((1D0-DABS(xx))**2-(a/xip)**2,a*g/xip**2)
      partot = DGMLT1(fsubzst,xlo,xhi,1,6,x)
     .       / ( a * g / xip**2 )
C
      atrix = - fieldn(3,ichip)*fieldn(3,ichi)
     +        + fieldn(4,ichip)*fieldn(4,ichi)
C
      bneutr = brnunu(xip,xi,isgn) / 3.
C
      gamchp = alpha(0)**2 * xip * atrix**2 * partot
     .       / ( 192. * pi * bneutr )
     .       / ( sw2 * cw2 )**2
C     PRINT *,'partot atrix bneutr :',partot,atrix,bneutr
C
      chizst = gamchp
C     PRINT *,'gamchp :',gamchp
C
 999  CONTINUE
      RETURN
      END
      SUBROUTINE COMBRA
C------------------------------------------------------------------
C!  Compute branching ratios
C
C  Input:    /PARAM/
C
C  Output:   /HHDECK/
C
C   P. Janot -- 24 August 1991
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'hhdeck.inc'
      include 'lundcom.inc'
      include 'consts.inc'
      include 'datapi.inc'
      include 'miscl.inc'
      include 'hccomm.inc'
      COMPLEX*16 f12, f1, f0, gzi1, gzi2, branch1, branch2, branch3
      COMPLEX*16 gagasm, gagall, ggsm, ggall
      REAL*8 phspgz, brwwzz
      EXTERNAL f12, f1, f0, gzi1, gzi2, phspgz, brwwzz
      COMMON /gagagg/ gagasm(nhig),gagall(nhig),ggsm(nhig),ggall(nhig)
      CHARACTER*1 star, higgs(nhig)
      DATA higgs/'H','h','A'/
      DATA hbar/6.583173D-25/
C
C
      ada(am1,am2,am3) = (am1**2-am2**2-am3**2)**2-4.*am2**2*am3**2
C
      CALL vzero(branch(1,1),nhig*nchan*2)
      CALL vzero(width(1),nhig*2)
      CALL vzero(parwth(1),nhig*2)
      CALL vzero(wwmax(1,1),2*nhig*2)
      CALL vzero(jtyp(1,1),2*nhig)
C
      couplh = 1./(32.*pi*amw**2)
      brtau = couplh*amtau**2
      bre   = couplh*ame**2
      brmu  = couplh*ammu**2
      brb   = 3.*couplh*amb**2
      brc   = 3.*couplh*amc**2
      brs   = 3.*couplh*ams**2
      brt   = 3.*couplh*amt**2
      brw   = 1./(64.*pi*amw**2)
      brz   = 1./(128.*pi*amz**2*cw2)
      brga  = 1./(1024.*pi**3*amw**2)
      brgg  = 1./(512.*pi**3*amw**2)
      brgz  = 1./(512.*pi**3*amw**2)
C
C Compute partial widths
C
      DO 1 jhig = 1, nhig
C
        IF ( ism .EQ. 1 .AND. jhig .NE. 2 ) GOTO 1
C
C Define couplings
C
        IF ( jhig .EQ. 1 ) THEN
          rup  = sa/sb
          su1  = sa/sb
          su2  =-ca/sb
          rdw  = ca/cb
          sd1  = ca/cb
          sd2  =-sa/cb
          rw   = COS(beta-alfa)
          rhp  = rw - c2b*cab/(2.*cw2)
          epsh = 1.
          rsf  = cab
          kpw  = 3
          eta  = 1.
          bbcor = corgbb
        ELSEIF ( jhig .EQ. 2 ) THEN
          IF ( ism .LE. 0 ) THEN ! Generic two-doublet model (020498)
            rup  = ca/sb
            su1  = ca/sb
            su2  = sa/sb
            rdw  =-sa/cb
            sd1  =-sa/cb
            sd2  =-ca/cb
            rw   = SIN(beta-alfa)
            rhp  = rw + c2b*sab/(2.*cw2)
            epsh = 1.
            rsf  = -sab
            kpw  = 3
            eta  = 1.
            bbcor = corhbb
          ELSE
            rup  = 1.
            su1  = 1.
            su2  = 1.
            rdw  = 1.
            sd1  = 1.
            sd2  = 1.
            rw   = 1.
            rhp  = 0.
            epsh = 0.
            rsf  = 0.
            kpw  = 3
            eta  = 0.
            bbcor = 1.
          ENDIF
        ELSEIF ( jhig .EQ. 3 ) THEN
          rup  = 1./tb
          su1  = 0.
          su2  = 0.
          rdw  = tb
          sd1  = 0.
          sd2  = 0.
          rw   = 0.
          rhp  = 0.
          epsh = 0.
          rsf  = 0.
          kpw  = 1
          eta  = -1.
          bbcor = 1.
        ENDIF
C
C Running quark masses
C
        runamu = amu/100.
        runamd = amd/100.
        runams = runmas(ams,jhig,rads)
        runamc = runmas(amc,jhig,radc)
        runamb = runmas(amb,jhig,radb)
        runamt = runmas(amt,jhig,radt)
        rggamu = 1.
        rggamd = 1.
        rggams = 1.
        rggamc = 1.
        rggamb = 1.
        rggamt = 1.
C
C h,H,A --> gamma gamma
C
        branch1 =
C Leptons
     .         rdw * f12(jhig,ame)
     .       + rdw * f12(jhig,ammu)
     .       + rdw * f12(jhig,amtau)
C Down type quarks
     .       + rdw * f12(jhig,amd*SQRT(rggamd)) / 3.
     .       + rdw * f12(jhig,ams*SQRT(rggams)) / 3.
     .       + rdw * f12(jhig,amb*SQRT(rggamb)) / 3. * bbcor
C Up type quarks
     .       + rup * f12(jhig,amu*SQRT(rggamu)) * 4./3.
     .       + rup * f12(jhig,amc*SQRT(rggamc)) * 4./3.
     .       + rup * f12(jhig,amt*SQRT(rggamt)) * 4./3.
C W boson
     .       + rw * f1(jhig,amw)
C
        gagasm(jhig) = branch1
C       WRITE(6,*) jhig,branch1
        IF ( ism .LE. 0 ) branch1 = branch1 ! Generic two-doublet model (020498)
C Charged Higgs bosons
     .       + rhp * f0(jhig,amhp) * amw**2/amhp**2
C       WRITE(6,*) jhig,branch1
        IF ( ism .EQ. 0 ) branch1 = branch1
C S-leptons (Left)
     .     + ( epsh*(ame  **2/amz**2*rdw + (sw2-1./2.)*rsf)
     .       + epsh*(ammu **2/amz**2*rdw + (sw2-1./2.)*rsf)
     .       + epsh*(amtau**2/amz**2*rdw + (sw2-1./2.)*rsf) )
     .     * f0(jhig,susSML) * (amz/susSML)**2
C S-leptons (Right)
     .     + ( epsh*(ame  **2/amz**2*rdw -  sw2*rsf)
     .       + epsh*(ammu **2/amz**2*rdw -  sw2*rsf)
     .       + epsh*(amtau**2/amz**2*rdw -  sw2*rsf) )
     .     * f0(jhig,susSME) * (amz/susSME)**2
C S-Down type quarks (Left)
     .     + ( epsh*(amd**2/amz**2*rdw + (sw2/3.-1./2.)*rsf)
     .       + epsh*(ams**2/amz**2*rdw + (sw2/3.-1./2.)*rsf) )
     .     * f0(jhig,susSMQ) * (amz/susSMQ)**2 / 3.
C S-Down type quarks (Right)
     .     + ( epsh*(amd**2/amz**2*rdw -  sw2/3.*rsf)
     .       + epsh*(ams**2/amz**2*rdw -  sw2/3.*rsf) )
C    .     * f0(jhig,susSMD) * (amz/susSMD)**2 / 3.
     .     * f0(jhig,susSMQ) * (amz/susSMQ)**2 / 3.
C S-Up type quarks (Left)
     .     + ( epsh*(amu**2/amz**2*rup - (sw2*2./3.-1./2.)*rsf)
     .       + epsh*(amc**2/amz**2*rup - (sw2*2./3.-1./2.)*rsf) )
     .     * f0(jhig,susSMQ) * (amz/susSMQ)**2 * 4./3.
C S-Up type quarks (Right)
C       WRITE(6,*) jhig,branch1
        IF ( ism .EQ. 0 ) branch1 = branch1
     .     + ( epsh*(amu**2/amz**2*rup +  sw2*2./3.*rsf)
     .       + epsh*(amc**2/amz**2*rup +  sw2*2./3.*rsf) )
C    .     * f0(jhig,susSMU) * (amz/susSMU)**2 * 4./3.
     .     * f0(jhig,susSMQ) * (amz/susSMQ)**2 * 4./3.
C       WRITE(6,*) jhig,branch1
        IF ( ism .EQ. 0 ) branch1 = branch1
C S-bottom quarks
     .     + epsh
     .     * ( (amb**2/amz**2*rdw+(sw2/3.-1./2.)*rsf)
     .       * (SIN(botmix))**2
     .       + (amb**2/amz**2*rdw- sw2/3.       *rsf)
     .       * (COS(botmix))**2
     .       - (amb/amz*(sd1*susAb/amz+sd2*susMu/amz))
     .       * SIN(botmix)*COS(botmix) )
     .     * f0(jhig,amsb(2)) * (amz/amsb(2))**2 / 3.
C
     .     + epsh
     .     * ( (amb**2/amz**2*rdw+(sw2/3.-1./2.)*rsf)
     .       * (COS(botmix))**2
     .       + (amb**2/amz**2*rdw- sw2/3.       *rsf)
     .       * (SIN(botmix))**2
     .       + (amb/amz*(sd1*susAb/amz+sd2*susMu/amz))
     .       * SIN(botmix)*COS(botmix) )
     .     * f0(jhig,amsb(1)) * (amz/amsb(1))**2 / 3.
C       WRITE(6,*) jhig,branch1
        IF ( ism .EQ. 0 ) branch1 = branch1
C S-top quarks
     .     + epsh
     .     * ( (amt**2/amz**2*rup-(sw2*2./3.-1./2.)*rsf)
     .       * (SIN(topmix))**2
     .       + (amt**2/amz**2*rup+ sw2*2./3.       *rsf)
     .       * (COS(topmix))**2
     .       - (amt/amz*(su1*susAt/amz+su2*susMu/amz))
     .       * SIN(topmix)*COS(topmix) )
     .     * f0(jhig,amst(2)) * (amz/amst(2))**2 * 4./3.
C
     .     + epsh
     .     * ( (amt**2/amz**2*rup-(sw2*2./3.-1./2.)*rsf)
     .       * (COS(topmix))**2
     .       + (amt**2/amz**2*rup+ sw2*2./3.       *rsf)
     .       * (SIN(topmix))**2
     .       + (amt/amz*(su1*susAt/amz+su2*susMu/amz))
     .       * SIN(topmix)*COS(topmix) )
     .     * f0(jhig,amst(1)) * (amz/amst(1))**2 * 4./3.
C       WRITE(6,*) jhig,branch1
        IF ( ism .EQ. 0 ) branch1 = branch1
C Charginos
     .       + 2.*bb(jhig,1,1) * f12(jhig,amchar(1))* amw/amchar(1)
     .       + 2.*bb(jhig,2,2) * f12(jhig,amchar(2))* amw/amchar(2)
C That's almost about it: don't forget the anomalous couplings!
        IF ( jhig .EQ. 2 .AND. ism .EQ. 1 )
     &       branch1 = branch1
     &               - 8D0*pi*amw**2/alpha(jhig)
     &               * sw2
     &               * (fbb+fww)
             
C That''s it
C       WRITE(6,*) jhig,branch1
        gagall(jhig) = branch1
        branch(1,jhig) = brga * alpha(jhig)**2 * gweak2(jhig)
     .                        * amhig(jhig)**3
     .                        * CDABS(branch1)**2
C
C h,H,A --> gluon gluon
C
        xmh = amhig(jhig)
        IF ( xmh .GT. 1. ) THEN
          fnh = 3D0
          IF ( amc .LE. xmh/2. ) fnh = fnh + 1D0
          IF ( amb .LE. xmh/2. ) fnh = fnh + 1D0
          IF ( amt .LE. xmh/2. ) fnh = fnh + 1D0
          alphah = alphas(jhig)
          gw2    = gweak2(jhig)
C
          branch2 =
C Down type quarks
     .         rdw * f12(jhig,amd*SQRT(rggamd))
     .       + rdw * f12(jhig,ams*SQRT(rggams))
     .       + rdw * f12(jhig,amb*SQRT(rggamb)) * bbcor
C Up type quarks
     .       + rup * f12(jhig,amu*SQRT(rggamu))
     .       + rup * f12(jhig,amc*SQRT(rggamc))
     .       + rup * f12(jhig,amt*SQRT(rggamt))
C
C         WRITE(6,*) jhig,branch2
          ggsm (jhig) = branch2
          IF ( ism .EQ. 0 ) branch2 = branch2
C S-Down type quarks (Left)
     .     + ( epsh*(amd**2/amz**2*rdw + (sw2/3.-1./2.)*rsf)
     .       + epsh*(ams**2/amz**2*rdw + (sw2/3.-1./2.)*rsf) )
     .     * f0(jhig,susSMQ) * (amz/susSMQ)**2
C S-Down type quarks (Right)
     .     + ( epsh*(amd**2/amz**2*rdw -  sw2/3.*rsf)
     .       + epsh*(ams**2/amz**2*rdw -  sw2/3.*rsf) )
C    .     * f0(jhig,susSMD) * (amz/susSMD)**2
     .     * f0(jhig,susSMQ) * (amz/susSMQ)**2
C         WRITE(6,*) jhig,branch2
          IF ( ism .EQ. 0 ) branch2 = branch2
C S-Up type quarks (Left)
     .     + ( epsh*(amu**2/amz**2*rup - (sw2*2./3.-1./2.)*rsf)
     .       + epsh*(amc**2/amz**2*rup - (sw2*2./3.-1./2.)*rsf) )
     .     * f0(jhig,susSMQ) * (amz/susSMQ)**2
C S-Up type quarks (Right)
     .     + ( epsh*(amu**2/amz**2*rup +  sw2*2./3.*rsf)
     .       + epsh*(amc**2/amz**2*rup +  sw2*2./3.*rsf) )
C    .     * f0(jhig,susSMU) * (amz/susSMU)**2
     .     * f0(jhig,susSMQ) * (amz/susSMQ)**2
C S-bottom quarks
C         WRITE(6,*) jhig,branch2
          IF ( ism .EQ. 0 ) branch2 = branch2
     .     + epsh
     .     * ( (amb**2/amz**2*rdw+(sw2/3.-1./2.)*rsf)
     .       * (SIN(botmix))**2
     .       + (amb**2/amz**2*rdw- sw2/3.       *rsf)
     .       * (COS(botmix))**2
     .       - (amb/amz*(sd1*susAb/amz+sd2*susMu/amz))
     .       * SIN(botmix)*COS(botmix) )
     .     * f0(jhig,amsb(2)) * (amz/amsb(2))**2
C
     .     + epsh
     .     * ( (amb**2/amz**2*rdw+(sw2/3.-1./2.)*rsf)
     .       * (COS(botmix))**2
     .       + (amb**2/amz**2*rdw- sw2/3.       *rsf)
     .       * (SIN(botmix))**2
     .       + (amb/amz*(sd1*susAb/amz+sd2*susMu/amz))
     .       * SIN(botmix)*COS(botmix) )
     .     * f0(jhig,amsb(1)) * (amz/amsb(1))**2
C S-top quarks
C         WRITE(6,*) jhig,branch2
          IF ( ism .EQ. 0 ) branch2 = branch2
     .     + epsh
     .     * ( (amt**2/amz**2*rup-(sw2*2./3.-1./2.)*rsf)
     .       * (SIN(topmix))**2
     .       + (amt**2/amz**2*rup+ sw2*2./3.       *rsf)
     .       * (COS(topmix))**2
     .       - (amt/amz*(su1*susAt/amz+su2*susMu/amz))
     .       * SIN(topmix)*COS(topmix) )
     .     * f0(jhig,amst(2)) * (amz/amst(2))**2
C
     .     + epsh
     .     * ( (amt**2/amz**2*rup-(sw2*2./3.-1./2.)*rsf)
     .       * (COS(topmix))**2
     .       + (amt**2/amz**2*rup+ sw2*2./3.       *rsf)
     .       * (SIN(topmix))**2
     .       + (amt/amz*(su1*susAt/amz+su2*susMu/amz))
     .       * SIN(topmix)*COS(topmix) )
     .     * f0(jhig,amst(1)) * (amz/amst(1))**2
C That''s it
C
          ggall(jhig) = branch2
C         WRITE(6,*) jhig,branch2
C         WRITE(6,*) '-------------------'
C         WRITE(6,*) jhig,gagasm(jhig),' (gaga sm )'
C         WRITE(6,*) jhig,gagall(jhig),' (gaga all)'
C         WRITE(6,*) jhig,ggsm  (jhig) ,'(glgl sm )'
C         WRITE(6,*) jhig,ggall (jhig),' (glgl all)'
C         WRITE(6,*) '-------------------'
          branch(2,jhig) = brgg * alphah**2 * xmh**3 * gw2
     .                   * CDABS(branch2)**2
C But account also for large QCD corrections (Kniehl et al)
C     .                   * ( 1D0 + alphah/pi*(95./4.-7.*fnh/6.) )
C But account also for large QCD corrections (Steinhauser, 2007)
          tau = xmh**2/(2.*amt)**2
          xLt = LOG(4.*tau)
          h0NLO = 95./4. - 7.*fnh/6.
          h1NLO = 5803./540. - 14.*xLt/15. - 29.*fnh/60.
          h2NLO = 1029839./189000. - 1543.*xLt/1575. - 89533.*fnh/378000.
          dNLO = h0NLO + tau * h1NLO + tau**2 * h2NLO
          h0NNLO = 156.808 + 5.708*xLt
          h1NNLO = 109.365 + 4.645*xLt
          h2NNLO = 74.434 + 3.297*xLt
          dNNLO = h0NNLO + tau * h1NNLO + tau**2 * h2NNLO
          dNNNLO = 467.684 + 122.441*xLt + 10.941*xLt**2
          branch(2,jhig) = branch(2,jhig) 
     &                   * (1D0 + alphah/pi * dNLO 
     &                          + (alphah/pi)**2 * dNNLO 
     &                          + (alphah/pi)**3 * dNNNLO )
        ELSE
          branch(2,jhig) = 0.
        ENDIF
C
C h,H,A --> gamma Z0
C
        xmh = amhig(jhig)
        IF ( xmh .GT. 20. ) THEN
          tw = sw/cw
          tauw = 4.*amw**2/amhig(jhig)**2
          branch3 =
C Leptons
     .   2.* (-1./2. + 2.*sw2) / (sw*cw)
     .     * (  rdw * (epsh*gzi1(jhig,ame  )-gzi2(jhig,ame  ))
     .        + rdw * (epsh*gzi1(jhig,ammu )-gzi2(jhig,ammu ))
     .        + rdw * (epsh*gzi1(jhig,amtau)-gzi2(jhig,amtau)) )
C Down type quarks
     . + 2.* (-1./2. + 2.*sw2/3.) / (sw*cw)
     .     * ( rdw * (epsh*gzi1(jhig,amd)-gzi2(jhig,amd))
     .       + rdw * (epsh*gzi1(jhig,ams)-gzi2(jhig,ams))
     .       + rdw * (epsh*gzi1(jhig,amb)-gzi2(jhig,amb)) * bbcor )
C Up type quarks
     . - 4.* (+1./2. - 4.*sw2/3.) / (sw*cw)
     .     * ( rup * (epsh*gzi1(jhig,amu)-gzi2(jhig,amu))
     .       + rup * (epsh*gzi1(jhig,amc)-gzi2(jhig,amc))
     .       + rup * (epsh*gzi1(jhig,amt)-gzi2(jhig,amt)) )
C W boson
     . - 1./tw * rw
     .    * ( 4.*(3.-tw**2)                  * gzi2(jhig,amw)
     .    + ((1.+2./tauw)*tw**2-(5.+2./tauw))* gzi1(jhig,amw))
C
C         WRITE(6,*) jhig,branch3
          IF ( ism .LE. 0 ) branch3 = branch3 ! Generic two-doublet model (020498)
C Charged Higgs bosons
     . + (1.-2.*sw2) / (sw*cw)
     .       * rhp * gzi1(jhig,amhp) * amw**2/amhp**2
C         WRITE(6,*) jhig,branch3
          IF ( ism .EQ. 0 ) branch3 = branch3
C S-leptons (Left)
     . + 2.* (-1./2. + sw2) / (sw*cw)
     .     * ( epsh*(ame  **2/amz**2*rdw + (sw2-1./2.)*rsf)
     .       + epsh*(ammu **2/amz**2*rdw + (sw2-1./2.)*rsf)
     .       + epsh*(amtau**2/amz**2*rdw + (sw2-1./2.)*rsf) )
     .     * gzi1(jhig,susSML) * (amz/susSML)**2
C S-leptons (Right)
     . + 2.*           sw2  / (sw*cw)
     .     * ( epsh*(ame  **2/amz**2*rdw -  sw2*rsf)
     .       + epsh*(ammu **2/amz**2*rdw -  sw2*rsf)
     .       + epsh*(amtau**2/amz**2*rdw -  sw2*rsf) )
     .     * gzi1(jhig,susSME) * (amz/susSME)**2
C S-Down type quarks (Left)
     . + 2.* (-1./2. + sw2/3.) / (sw*cw)
     .     * ( epsh*(amd**2/amz**2*rdw + (sw2/3.-1./2.)*rsf)
     .       + epsh*(ams**2/amz**2*rdw + (sw2/3.-1./2.)*rsf) )
     .     * gzi1(jhig,susSMQ) * (amz/susSMQ)**2
C S-Down type quarks (Right)
     . + 2.*           sw2/3.  / (sw*cw)
     .     * ( epsh*(amd**2/amz**2*rdw -  sw2/3.*rsf)
     .       + epsh*(ams**2/amz**2*rdw -  sw2/3.*rsf) )
C    .     * gzi1(jhig,susSMD) * (amz/susSMD)**2
     .     * gzi1(jhig,susSMQ) * (amz/susSMQ)**2
C S-Up type quarks (Left)
     . - 4.* (1./2. - 2.*sw2/3.) / (sw*cw)
     .     * ( epsh*(amu**2/amz**2*rup - (sw2*2./3.-1./2.)*rsf)
     .       + epsh*(amc**2/amz**2*rup - (sw2*2./3.-1./2.)*rsf) )
     .     * gzi1(jhig,susSMQ) * (amz/susSMQ)**2
C S-Up type quarks (Right)
C         WRITE(6,*) jhig,branch3
          IF ( ism .EQ. 0 ) branch3 = branch3
     . - 4.* (      - 2.*sw2/3.) / (sw*cw)
     .     + ( epsh*(amu**2/amz**2*rup +  sw2*2./3.*rsf)
     .       + epsh*(amc**2/amz**2*rup +  sw2*2./3.*rsf) )
C    .     * gzi1(jhig,susSMU) * (amz/susSMU)**2
     .     * gzi1(jhig,susSMQ) * (amz/susSMQ)**2
C         WRITE(6,*) jhig,branch1
          IF ( ism .EQ. 0 ) branch3 = branch3
C S-bottom quarks
     .     + epsh
     .     * ( 2.* (-1./2. +    sw2/3.) / (sw*cw)
     .       + (amb**2/amz**2*rdw+(sw2/3.-1./2.)*rsf)
     .       * (SIN(botmix))**2
     .     +   2.*              sw2/3.  / (sw*cw)
     .       * (amb**2/amz**2*rdw- sw2/3.       *rsf)
     .       * (COS(botmix))**2
     .     -   1.* (-1./2. + 2.*sw2/3.) / (sw*cw)
     .       * (amb/amz*(sd1*susAb/amz+sd2*susMu/amz))
     .       * SIN(botmix)*COS(botmix) )
     .     * gzi1(jhig,amsb(2)) * (amz/amsb(2))**2
C
     .     + epsh
     .     * ( 2.* (-1./2. +    sw2/3.) / (sw*cw)
     .       * (amb**2/amz**2*rdw+(sw2/3.-1./2.)*rsf)
     .       * (COS(botmix))**2
     .     +   2.*              sw2/3.  / (sw*cw)
     .       * (amb**2/amz**2*rdw- sw2/3.       *rsf)
     .       * (SIN(botmix))**2
     .     +   1.* (-1./2. + 2.*sw2/3.) / (sw*cw)
     .       * (amb/amz*(sd1*susAb/amz+sd2*susMu/amz))
     .       * SIN(botmix)*COS(botmix) )
     .     * gzi1(jhig,amsb(1)) * (amz/amsb(1))**2
C         WRITE(6,*) jhig,branch1
          IF ( ism .EQ. 0 ) branch3 = branch3
C S-top quarks
     .     + epsh
     .     * ( - 4.* (1./2. - 2.*sw2/3.) / (sw*cw)
     .       * (amt**2/amz**2*rup-(sw2*2./3.-1./2.)*rsf)
     .       * (SIN(topmix))**2
     .     -     4.* (      - 2.*sw2/3.) / (sw*cw)
     .       * (amt**2/amz**2*rup+ sw2*2./3.       *rsf)
     .       * (COS(topmix))**2
     .     +     2.* (1./2. - 4.*sw2/3.) / (sw*cw)
     .       * (amt/amz*(su1*susAt/amz+su2*susMu/amz))
     .       * SIN(topmix)*COS(topmix) )
     .     * gzi1(jhig,amst(2)) * (amz/amst(2))**2
C
     .     + epsh
     .     * ( - 4.* (1./2. - 2.*sw2/3.) / (sw*cw)
     .       * (amt**2/amz**2*rup-(sw2*2./3.-1./2.)*rsf)
     .       * (COS(topmix))**2
     .     -     4.* (      - 2.*sw2/3.) / (sw*cw)
     .       * (amt**2/amz**2*rup+ sw2*2./3.       *rsf)
     .       * (SIN(topmix))**2
     .     -     2.* (1./2. - 4.*sw2/3.) / (sw*cw)
     .       * (amt/amz*(su1*susAt/amz+su2*susMu/amz))
     .       * SIN(topmix)*COS(topmix) )
     .     * gzi1(jhig,amst(1)) * (amz/amst(1))**2
C         WRITE(6,*) jhig,branch3
          IF ( ism .EQ. 0 ) branch3 = branch3
C Charginos
     .     + 2.*bb(jhig,1,1) * 2. / (cw*sw)
     .       * ( - vmat(1,1)**2 - vmat(1,2)**2/2.
     .           - umat(1,1)**2 - umat(1,2)**2/2.
     .           + 2.*sw2 )
     .       * (epsh*gzi1(jhig,amchar(1))-gzi2(jhig,amchar(1)))
     .       *  amw/amchar(1)
     .     + 2.*bb(jhig,2,2) * 2. / (cw*sw)
     .       * ( - vmat(2,1)**2 - vmat(2,2)**2/2.
     .           - umat(2,1)**2 - umat(2,2)**2/2.
     .           + 2.*sw2 )
     .       * (epsh*gzi1(jhig,amchar(2))-gzi2(jhig,amchar(2)))
     .       *  amw/amchar(2)
C That's almost about it: don't forget the anomalous couplings!
          IF ( jhig .EQ. 2 .AND. ism .EQ. 1 )
     &       branch3 = branch3
     &               + 2D0*SQRT(2D0)*pi*amw**2/alpha(jhig)
     &               * sw/cw
     &               * ( (fw-fb) + 4D0*(sw2*fbb-cw2*fww) )
C That''s it
C         WRITE(6,*) jhig,branch3
          phsp =  phspgz(amhig(jhig)**2,amz,gmz)
          branch(11,jhig) = brgz * alpha(jhig)**2 * gweak2(jhig)
     .                           * amhig(jhig)**3
     .                           * CDABS(branch3)**2
     .                           * phspgz(amhig(jhig)**2,amz,gmz)
        ELSE
          branch(11,jhig) = 0.
        ENDIF
C
C h,H,A --> e+e-
C
        IF ( amhig(jhig) .GT. 2.*ame) branch(12,jhig) =
     .            gweak2(jhig)*bre*rdw**2*amhig(jhig)*
     .            SQRT(1D0-4D0*ame**2/amhig(jhig)**2)**kpw
     .            * weakcor(jhig,ame,1D0,rup/rdw,rw/rdw)
C
C h,H,A --> mu+mu-
C
        IF ( amhig(jhig) .GT. 2.*ammu) branch(13,jhig) =
     .            gweak2(jhig)*brmu*rdw**2*amhig(jhig)*
     .            SQRT(1D0-4D0*ammu**2/amhig(jhig)**2)**kpw
     .            * weakcor(jhig,ammu,1D0,rup/rdw,rw/rdw)
C
C h,H,A --> tau+tau-
C
        IF ( amhig(jhig) .GT. 2.*amtau) branch(3,jhig) =
     .            gweak2(jhig)*brtau*rdw**2*amhig(jhig)*
     .            SQRT(1D0-4D0*amtau**2/amhig(jhig)**2)**kpw
     .            * weakcor(jhig,amtau,1D0,rup/rdw,rw/rdw)
C
C h,H,A --> s sbar
C
        IF ( amhig(jhig) .GT. DMAX1(1D0,2D0*ams) )
     .    branch(14,jhig) = runams * rads *
     .            gweak2(jhig)*brs*rdw**2*amhig(jhig)*
     .            SQRT(1D0-4D0*ams**2*runams/amhig(jhig)**2)**kpw
     .            * weakcor(jhig,ams,1D0/3D0,rup/rdw,rw/rdw)
C
C h,H,A --> b bbar
C
        IF ( amhig(jhig) .GT. 2.*amb)
     .    branch(5,jhig) = runamb * radb *
     .            gweak2(jhig)*brb*(rdw*bbcor)**2*amhig(jhig)*
     .            SQRT(1D0-4D0*amb**2*runamb/amhig(jhig)**2)**kpw
     .            * weakcor(jhig,amb,1D0/3D0,rup/rdw,rw/rdw)
C
C h,H,A --> c cbar
C
        IF ( amhig(jhig) .GT. 2.*amc )
     .    branch(4,jhig) = runamc * radc *
     .            gweak2(jhig)*brc*rup**2*amhig(jhig)*
     .            SQRT(1D0-4D0*amc**2*runamc/amhig(jhig)**2)**kpw
     .            * weakcor(jhig,amc,2D0/3D0,rup/rup,rw/rup)
C
C  h,H,A --> t tbar
C
        IF ( amhig(jhig) .GT. 2.*amt)
     .    branch(6,jhig) = runamt * radt *
     .            gweak2(jhig)*brt*rup**2*amhig(jhig)*
     .            SQRT(1D0-4D0*amt**2/amhig(jhig)**2)**kpw
C
C  h,H,A --> W W, Z Z
C
        IF ( jhig .NE. 3 .AND. amhig(jhig) .GT. 20. ) THEN
          const = 3. * SQRT(2.) * G_F / (4.*pi*amhig(jhig)**3)
          IF ( jhig .EQ. 2 .AND. ism .EQ. 1 ) THEN 
            vg1 =  amw**2 * fw 
            vg2 = -amw**2 * fww
          ELSE
            vg1 = 0D0
            vg2 = 0D0
          ENDIF
          compww = const*brwwzz(amhig(jhig),amw,gmw,vg1,vg2)
          IF ( jhig .EQ. 2 .AND. ism .EQ. 1 ) THEN 
            vg1 =  amw**2 / cw2 * (cw2   *fw +sw2   *fb)  
            vg2 = -amw**2 / cw2 * (sw2**2*fbb+cw2**2*fww)
          ELSE
            vg1 = 0D0
            vg2 = 0D0
          ENDIF
          compzz = const*brwwzz(amhig(jhig),amz,gmz,vg1,vg2)/2.
          branch(7,jhig) = rw**2 * compww
          branch(8,jhig) = rw**2 * compzz
        ENDIF
C
        IF ( ism .EQ. 1 ) THEN
          CALL vzero(amneut(1),4*2)
          CALL vzero(amchar(1),2*2)
          CALL vzero(wneut(1,1,1),4*4*nhig*2)
          CALL vzero(wchar(1,1,1),2*2*nhig*2)
          wneut(1,1,jhig) = 1.
          branch(15,jhig) = branch(12,jhig)/1D6
          branch(16,jhig) = 0.
        ENDIF
C
C Now, only the MSSM is concerned.
C
        IF ( ism .EQ. 0 ) THEN
C
C h,H,A --> chi chi
C
          DO i = 1, 4
            DO j = i, 4
              IF ( i .EQ. j ) THEN
                dij = 1.
              ELSE
                dij = 0.
              ENDIF
              IF ( amhig(jhig) .GT.
     .              ABS(amneut(i))+ABS(amneut(j)) ) THEN
                wneut(i,j,jhig) =
     .              gweak2(jhig)/(8.*pi*amhig(jhig)**3*(1.+dij))
     .            * aa(jhig,i,j)**2
     .            * (amhig(jhig)**2-(amneut(i)+eta*amneut(j))**2)
     .            * SQRT((amneut(i)**2+amneut(j)**2-amhig(jhig)**2)**2
     .                -4.*amneut(i)**2*amneut(j)**2)
              ELSE
                wneut(i,j,jhig) = 0.
              ENDIF
              branch(15,jhig) = branch(15,jhig) + wneut(i,j,jhig)
            ENDDO
          ENDDO
C
          IF ( branch(15,jhig) .GT. 0. ) THEN
            DO i = 1, 4
              DO j = 1, 4
                wneut(i,j,jhig) = wneut(i,j,jhig) / branch(15,jhig)
              ENDDO
            ENDDO
          ENDIF
C
C h,H,A --> chi+chi-
C
          DO i = 1, 2
            DO j = 1, 2
              IF ( amhig(jhig) .GT.
     .              ABS(amchar(i))+ABS(amchar(j)) ) THEN
                wchar(i,j,jhig) =
     .              gweak2(jhig)/(16.*pi*amhig(jhig)**3)
     .            * ( (bb(jhig,i,j)**2+bb(jhig,j,i)**2)
     .               *(amhig(jhig)**2-amchar(i)**2-amchar(j)**2)
     .               -4.*bb(jhig,i,j)*bb(jhig,j,i)
     .               *eta*amchar(i)*amchar(j) )
     .            * SQRT((amchar(i)**2+amchar(j)**2-amhig(jhig)**2)**2
     .                -4.*amchar(i)**2*amchar(j)**2)
              ELSE
                wchar(i,j,jhig) = 0.
              ENDIF
              branch(16,jhig) = branch(16,jhig) + wchar(i,j,jhig)
            ENDDO
          ENDDO
C
          IF ( branch(16,jhig) .GT. 0. ) THEN
            DO i = 1, 2
              DO j = 1, 2
                wchar(i,j,jhig) = wchar(i,j,jhig) / branch(16,jhig)
              ENDDO
            ENDDO
          ENDIF
C
        ENDIF
C
    1 CONTINUE
C
      IF ( ism .EQ. 1 ) GOTO 2
C
C  H --> A A
C
      IF ( gmh .GT. 2.*ama ) branch(9,1) =
     .     gweak2(1)*amz**2*SQRT(1.-4.*ama**2/gmh**2)
     .              * (c2b*cab)**2
     .              / (128.*pi*gmh*cw2)
C
C  h --> A A
C
      IF ( amh .GT. 2.*ama ) branch(9,2) =
     .     gweak2(2)*amz**2*SQRT(1.-4.*ama**2/amh**2)
C    .              * (c2b*sab)**2
C Add radiative corrections (Zwirner et al, CERN-TH 6151/91)
     .              * (c2b*sab
     .              + 3.*gweak2(2)*cw2*ca*cb**2*amt**4
     .              / (8.*pi**2*sb**3*amw**4)
     .              * LOG(1.+amsq**2/amt**2) ) **2
C
     .              / (128.*pi*amh*cw2)
C
C  A --> Z h
C
      x = ada(ama,amz,amh)
      IF ( ama .GT. amh+amz .AND. x .GT. 0. )
     .  branch(9,3) = gweak2(3)*SQRT(x)*cab2
     .              / (64.*pi*ama**3*cw2)
     .              * (amz**2-2.*(ama**2+amh**2)
     .              + (ama**2-amh**2)**2/amz**2)
C
C  H --> h h
C
      IF ( gmh .GT. 2.*amh ) branch(10,1) =
     .     gweak2(1)*amz**2*SQRT(1.-4.*amh**2/gmh**2)
     .              * (c2a*cab-2.*s2a*sab)**2
     .              / (128.*pi*gmh*cw2)
C
    2 CONTINUE
      CALL vzero(tauh(1),nhig*2)
      DO 3 jhig = 1, nhig
        DO jchan = 1, nchan
          width(jhig) = width(jhig) + branch(jchan,jhig)
          IF ( ichan(jchan,jhig) .EQ. 1 )
     .    parwth(jhig) = parwth(jhig) + branch(jchan,jhig)
        ENDDO
C
        IF ( width(jhig) .EQ. 0. ) THEN
          IF ( idbg .GE. 0 ) WRITE(6,2001) higgs(jhig),amhig(jhig)
          GOTO 3
        ELSE
          tauh(jhig) = hbar/width(jhig)
          IF ( idbg .GE. 0 ) WRITE(6,1001)
     .    higgs(jhig),amhig(jhig),width(jhig),parwth(jhig)
        ENDIF
C
        btot = 0.
        DO jchan = 1, nchan
          star = ' '
          if (ichan(jchan,jhig) .eq. 1) star = '*'
          IF ( width(jhig) .GT. 0. )
     .    branch(jchan,jhig) = branch(jchan,jhig)/width(jhig)*100.
          btot = btot + branch(jchan,jhig)
          IF ( idbg .GE. 0 .AND. branch(jchan,jhig) .GT. 0.)
     .    WRITE(6,1002) channel(jchan,jhig),branch(jchan,jhig),star
          IF ( ichan(jchan,jhig) .EQ. 1 .AND.
     .         branch(jchan,jhig) .GT. 0. ) THEN
            branch(jchan,jhig) = branch(jchan,jhig)*width(jhig)/
     .                         parwth(jhig)/100.
          ELSE
            branch(jchan,jhig) = 0.
          ENDIF
        ENDDO
C
        IF ( branch(15,jhig) .GT. 0. ) THEN
          IF ( idbg .GE. 0 ) WRITE(6,1005)
          DO ineut = 1, 4
            DO jneut = ineut,4
              IF ( ineut .EQ. jneut ) THEN
                br = wneut(ineut,jneut,jhig)
              ELSE
                br = wneut(ineut,jneut,jhig)+wneut(jneut,ineut,jhig)
              ENDIF
              br = br*branch(15,jhig)*parwth(jhig)/width(jhig)*100.
              IF ( br .GT. 0. .AND. idbg .GE. 0 ) THEN
                IF ( jhig.EQ. 1 ) WRITE(6,1006) ineut,jneut,br
                IF ( jhig.EQ. 2 ) WRITE(6,1016) ineut,jneut,br
                IF ( jhig.EQ. 3 ) WRITE(6,1026) ineut,jneut,br
              ENDIF
            ENDDO
          ENDDO
        ENDIF
C
        IF ( branch(16,jhig) .GT. 0. ) THEN
          IF ( idbg .GE. 0 ) WRITE(6,1007)
          DO ichar = 1, 2
            DO jchar = ichar,2
              IF ( ichar .EQ. jchar ) THEN
                br = wchar(ichar,jchar,jhig)
              ELSE
                br = wchar(ichar,jchar,jhig)+wchar(jchar,ichar,jhig)
              ENDIF
              br = br*branch(16,jhig)*parwth(jhig)/width(jhig)*100.
              IF ( br .GT. 0. .AND. idbg .GE. 0 ) THEN
                IF ( jhig .EQ. 1 ) WRITE(6,1008) ichar,jchar,br
                IF ( jhig .EQ. 2 ) WRITE(6,1018) ichar,jchar,br
                IF ( jhig .EQ. 3 ) WRITE(6,1028) ichar,jchar,br
              ENDIF
            ENDDO
          ENDDO
        ENDIF
    3 CONTINUE
      IF ( idbg .GE. 0 ) WRITE(6,1004)
C
C Fill internal width table
C
      CALL vzero(xywid(1,1,1),2*nchan*nhig*2)
      DO jhig = 1 , nhig
        xywid(1,1,jhig) = 0.
        xywid(1,2,jhig) = 0.
        xywid(1,3,jhig) = 0.
        xywid(1,4,jhig) = 0.
        xywid(1,5,jhig) = 0.
        xywid(1,6,jhig) = 0.
        xywid(1,7,jhig) = pmas(24,2)
        xywid(1,8,jhig) = pmas(23,2)
        xywid(1,9,jhig) = width(3)
        xywid(1,10,jhig) = width(2)
        xywid(1,11,jhig) = 0.
        xywid(1,12,jhig) = 0.
        xywid(1,13,jhig) = 0.
        xywid(1,14,jhig) = 0.
        xywid(1,15,jhig) = 0.
        DO jchan = 1 , nchan
          xywid(2,jchan,jhig) = xywid(1,jchan,jhig)
        ENDDO
        xywid(2,11,jhig) = pmas(23,2)
      ENDDO
C
      xywid(1,9,3) = width(2)
      xywid(2,9,3) = pmas(23,2)
C
      pmas(25,2) = width(2)
      pmas(35,2) = width(1)
      pmas(36,2) = width(3)
C
      pmas(25,3) = AMIN1(pmas(25,1),10.*width(2))
      pmas(35,3) = AMIN1(pmas(35,1),10.*width(1))
      pmas(36,3) = AMIN1(pmas(36,1),10.*width(3))
C
      IF( ism.LE.0 ) THEN ! Generic two-doublet model (020498)
C - The Charged Higgs now ... First define channel names and check data
C - ... then calculate widths&BR and store them in common blocks ...
        mh(1)= gmh           ! H mass
        mh(2)= amh           ! h mass
        mh(3)= ama           ! A mass
        mh(4)= amhp          ! H+ mass
        tbt=   tb            ! tan beta= v2/v1
        taf=   ta            ! tan alpha, mixing ...
        CALL hcsetj( idbg )
      END IF
C
  999 RETURN
C-----------------------------------------------------------------------
 1000 FORMAT(/' With Lambda_QCD(5) = ',F8.5,' GeV,'/
     .        '      alpha_s(mZ)   = ',F8.5,' ... '/)
 1001 FORMAT(/50('-')//
     .       1x,'The following branching ratios have been computed :'/
     .       1X,'   ',A1,' mass                 : ',F9.3,' GeV/c**2'/
     .       1X,'    Total decay width     : ',F9.5,' GeV'/
     .       1X,'    Width to be generated : ',F9.5,' GeV'//)
 2001 FORMAT(/50('-')/
     .       1X,'   ',A1,' (mass : ',F8.3,' GeV/c**2)'/
     .       1x,'    won''t be decayed by HHDECAY !'//)
 1002 FORMAT(1X,'Channel ',A14,' --- BR = ',F10.5,' % (',A1,')')
 1003 FORMAT(1x,'The branching fraction into the lightest ',
     .          'neutralino pair is ',F10.5,'%'/)
 1004 FORMAT(/1X,'   (*) = channel requested'/)
 1005 FORMAT(/8x,' Detail of the neutralino BRs : ')
 1006 FORMAT(10x,' o H --> chi',I1,'0 chi',I1,'0 ',20('.'),1x,F9.5,'%')
 1016 FORMAT(10x,' o h --> chi',I1,'0 chi',I1,'0 ',20('.'),1x,F9.5,'%')
 1026 FORMAT(10x,' o A --> chi',I1,'0 chi',I1,'0 ',20('.'),1x,F9.5,'%')
 1007 FORMAT(/8x,' Detail of the chargino BRs : ')
 1008 FORMAT(10x,' o H --> chi',I1,'+ chi',I1,'- ',20('.'),1x,F9.5,'%')
 1018 FORMAT(10x,' o h --> chi',I1,'+ chi',I1,'- ',20('.'),1x,F9.5,'%')
 1028 FORMAT(10x,' o A --> chi',I1,'+ chi',I1,'- ',20('.'),1x,F9.5,'%')
      END
      DOUBLE PRECISION FUNCTION CROCOM(ipro, sbeam)
C----------------------------------------------------------------------
C! Cross sections for e+e- --> hZ, HZ, hA, HA, hnn, Hnn, hee, Hee
C
C  Input :        ipro,   process Id
C                 sbeam,  is ecm**2
C
C  Output:        crocom, the cross-section value
C
C  Patrick Janot -- 31 oct 1992
C----------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'lundcom.inc'
      include 'param.inc'
      include 'consts.inc'
      include 'datapi.inc'
      include 'miscl.inc'
      include 'zzdeck.inc'
      include 'hhdeck.inc'
      include 'hhprod.inc'
      REAL*8 kappa
      REAL*8 brwipj, brwisi, sighhc
      EXTERNAL brwipj, brwisi, sighhc
      LOGICAL first
      DATA first /.TRUE./
      ada(am1,am2,am3) = (am1**2-am2**2-am3**2)**2-4.*am2**2*am3**2
C
C  Calcul des sections efficaces de production (hZ,HZ,hA,HA,WWh,WWH)
C
      ebeam = SQRT(sbeam)/2.
C
      brwig = (sbeam-amz**2)**2 + amz**2*gmz**2
      nevt  = 250
      crocom = 0.
C
      IF ( ipro .EQ. 1 ) THEN
C
        IF ( iklei.EQ.0 .AND. amh+amz.GT.2.*ebeam-10.*gmz ) THEN
          DO 1 ievt = 1, nevt
            bmin = 20.
            bmax = 2.*ebeam
            CALL bwgene(bmin,bmax,amz,gmz,bmz,djdum)
            almbda = ada(2.*ebeam,amh,bmz)
            IF ( amh+bmz .GT. 2.*ebeam .OR. almbda .LE. 0. ) GOTO 1
            weight = sqrt(almbda)*(almbda+12.*sbeam*amz**2)
            sbj =
     .      pi * alpha2 * weight * (1.+(1.-4.*sw2)**2)
     .      / (192.*sbeam**2*sw2**2*cw2**2*brwig)
            crocom = crocom + sab2*sbj
    1     CONTINUE
          crocom = crocom/FLOAT(nevt)
        ELSEIF ( iklei .EQ. 0 ) THEN
          almbda = ada(2.*ebeam,amh,amz)
          weight = sqrt(almbda)*(almbda+12.*sbeam*amz**2)
          sbj =
     .    pi * alpha2 * weight * (1.+(1.-4.*sw2)**2)
     .    / (192.*sbeam**2*sw2**2*cw2**2*brwig)
          crocom = sab2*sbj 
        ELSE
          crocom = alpha2 * brwipj(sbeam,amh,width(2),amz)
          IF ( ism .LE. 0 ) THEN
            topcor = ca/sb / SIN(beta-alfa)
            crocom = crocom * sab2 * (1.-8./3.*deltar*topcor)
     .                             / (1.-8./3.*deltar       )
            IF ( crocom .LT. 0D0 ) crocom = 0D0
          ENDIF
C
        ENDIF
C
      ELSEIF ( ipro .EQ. 2 .AND. ism .LE. 0 ) THEN
C
        IF ( iklei.EQ.0 .AND. gmh+amz .GT. 2.*ebeam-10.*gmz ) THEN
          DO 2 ievt = 1, nevt
            bmin = 20.
            bmax = 2.*ebeam
            CALL bwgene(bmin,bmax,amz,gmz,bmz,djdum)
            almbda = ada(2.*ebeam,gmh,bmz)
            IF ( gmh+bmz .GT. 2.*ebeam .OR. almbda .LE. 0. ) GOTO 2
            weight = sqrt(almbda)*(almbda+12.*sbeam*amz**2)
            sbj =
     .      pi * alpha2 * weight * (1.+(1.-4.*sw2)**2)
     .      / (192.*sbeam**2*sw2**2*cw2**2*brwig)
            crocom = crocom + cab2*sbj
    2     CONTINUE
          crocom = crocom/FLOAT(nevt) 
        ELSEIF ( iklei .EQ. 0 ) THEN
          almbda = ada(2.*ebeam,gmh,amz)
          weight = sqrt(almbda)*(almbda+12.*sbeam*amz**2)
          sbj =
     .    pi * alpha2 * weight * (1.+(1.-4.*sw2)**2)
     .    / (192.*sbeam**2*sw2**2*cw2**2*brwig)
          crocom = cab2*sbj 
        ELSE
          crocom = alpha2 * brwipj(sbeam,gmh,width(1),amz)
          IF ( ism .LE. 0 ) THEN
            topcor = sa/sb / COS(beta-alfa)
            crocom = crocom * cab2 * (1.-8./3.*deltar*topcor)
     .                             / (1.-8./3.*deltar       )
            IF ( crocom .LT. 0D0 ) crocom = 0D0
          ENDIF
C
        ENDIF
C
      ELSEIF ( ipro .EQ. 3 .AND. ism .LE. 0 ) THEN
C
        kappa = brwisi(sbeam,amh,ama,width(2),width(3)) / 8.
        shA = pi*alpha2*kappa * (8.*sw2**2-4.*sw2+1.) * sbeam
     .      / (12.*cw2**2*sw2**2*brwig)
        crocom = cab2*shA
C
      ELSEIF ( ipro .EQ. 4 .AND. ism .LE. 0 ) THEN
C
        kappa = brwisi(sbeam,gmh,ama,width(1),width(3)) / 8.
        sHA = pi*alpha2*kappa * (8.*sw2**2-4.*sw2+1.) * sbeam
     .      / (12.*cw2**2*sw2**2*brwig)
        crocom = sab2*sHA
C
      ELSEIF ( ipro .GE. 5 .AND. ipro .LE. 8 ) THEN
C
C WW/ZZ fusion only
C
        IF ( interf(ipro) .EQ. 0 ) THEN 
C
          IF ( ( ipro .EQ. 5 .OR. ipro .EQ. 7 ) .OR.
     .            ism .NE. 1 ) THEN 
            chnns(ipro) = sigmawwh(sbeam,ipro)
          ELSE
            chnns(ipro) = 0D0
          ENDIF
          chnni(ipro) = 0D0
          chnnz(ipro) = 0D0
C
C With hZ and interference in addition, if requested:
C
        ELSE
C
          IF ( ( ipro .EQ. 5 .OR. ipro .EQ. 7 ) .OR.
     .            ism .NE. 1 ) THEN 
            chnns(ipro) = sigmawwh(sbeam,ipro) 
            chnni(ipro) = sigintf (sbeam,ipro)
          ELSE
            chnns(ipro) = 0D0
            chnni(ipro) = 0D0
          ENDIF
          IF ( ipro .EQ. 5 .OR. ipro .EQ. 7 ) THEN 
            chnnz(ipro) = brwipj(sbeam,amh,width(2),amz) * alpha2
          ELSEIF ( ism .NE. 1 ) THEN   
            chnnz(ipro) = brwipj(sbeam,gmh,width(1),amz) * alpha2
          ENDIF
        ENDIF
C
C MSSM corrections and couplings 
C
        IF ( ism .NE. 1 ) THEN 
          IF ( ipro .EQ. 5 .OR. ipro .EQ. 7 ) THEN 
            topcor = ca/sb / SIN(beta-alfa)
            chnnz(ipro) = sab2 * chnnz(ipro)
     &                  * (1.-8./3.*deltar*topcor)
     .                  / (1.-8./3.*deltar       )
            chnni(ipro) = sab2 * chnni(ipro)
            chnns(ipro) = sab2 * chnns(ipro)
          ELSE
            topcor = sa/sb / COS(beta-alfa)
            chnnz(ipro) = cab2 * chnnz(ipro)
     &                  * (1.-8./3.*deltar*topcor)
     &                  / (1.-8./3.*deltar       )
            chnni(ipro) = cab2 * chnni(ipro)
            chnns(ipro) = cab2 * chnns(ipro)
          ENDIF
          IF ( chnnz(ipro) .LT. 0D0 ) chnnz(ipro) = 0D0
        ENDIF
C
C Z -> nu_e nu_e(bar) or e+e- branching fraction
C
        IF ( interf(ipro) .EQ. 1 ) THEN 
          IF ( ipro .EQ. 5 .OR. ipro .EQ. 6 ) THEN 
            chnnz(ipro) = chnnz(ipro) * brai(2)
          ELSE
            chnnz(ipro) = chnnz(ipro) * brai(1)
          ENDIF
        ENDIF
C
        crocom = chnnz(ipro) + chnni(ipro) + chnns(ipro)
        IF ( crocom .LT. 0D0 ) crocom = 0D0
C
      ELSEIF ( ipro.EQ.9 .AND. ism .LE. 0 ) THEN
C
        crocom = sighhc(sbeam,0)*1000.
C
      ELSEIF ( ipro .EQ. 10 .AND. ism .EQ. 1 ) THEN 
C
        crocom = hgamma(sbeam)
C
      ELSE
      ENDIF
C
      crocom = crocom * reduc(ipro)
C
  999 RETURN
      END
      SUBROUTINE decchi(ch,ichi1,ichi,ich)
C----------------------------------------------------------------
C! Decay a chargino or a neutralino into two bodies
C  ( chi' --> chi gamma, chi Z(*), chi+/- W(*)-/+)
C  ( chi+ --> chi W+(*))
C  Cascades are allowed by consecutive calls to DECCHI
C
C Input:   o ch(1-4) = the quadri-momentum of the chi
C          o ichi1   = neutralino/chargino index
C          o ichi    = its charge
C          o ich     = the position in the LUJET common block
C
C Output:  o p2(1-4) = the LSP quadri-momentum
C          o p3(1-4) = the virtual boson quadri-momentum
C
C Patrick Janot -- 31 Aug 1994
C----------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION ch(4)
      include 'param.inc'
      include 'hhdeck.inc'
      include 'datapi.inc'
C
      IF ( ichi.EQ.0 .AND. (ichi1.LE.0.OR.ichi.GT.4) ) THEN
        WRITE(6,*) '+++ DECCHI +++ Wrong Neutralino #',ichi1
        STOP 99
      ENDIF
C
      IF ( ichi.EQ.1 .AND. (ichi1.LE.0.OR.ichi.GT.2) ) THEN
        WRITE(6,*) '+++ DECCHI +++ Wrong Chargino #',ichi1
        STOP 99
      ENDIF
C
      IF ( ichi .EQ. 0 .AND. widneut(ichi1) .EQ. 0.) THEN
        WRITE(6,*) '+++ DECCHI +++ No accesible channels for ',
     .             'neutralino # ',ichi1
        STOP 99
      ENDIF
C
      IF ( ichi .EQ. 1 .AND. widchar(ichi1) .EQ. 0.) THEN
        WRITE(6,*) '+++ DECCHI +++ No accesible channels for ',
     .             'chargino # ',ichi1
        STOP 99
      ENDIF
C
      IF ( ichi .EQ. 0 ) THEN
        nchn = nchneut
      ELSE
        nchn = nchchar
      ENDIF
C
C  Choice of the decay channel.
C
      rnch = RNDM2(dummy)
      rint = 0.D0
      DO jc = 1 , nchn
        IF ( ichi .EQ. 0 ) THEN
          brch = brneut(jc,ichi1)
        ELSE
          brch = brchar(jc,ichi1)
        ENDIF
        rint = rint + brch
        if ( rnch .lt. rint ) GOTO 30
      ENDDO
  30  CONTINUE
C
C Store the indices of the final state:
C       o ichi2 = the neutral/charg-ino index
C       o jchi  = 0 for a neutralino, 1 for a chargino
C       o ifn   = 0 for a photon, 1 for a Z and 2 for a W
C
      IF ( ichi .EQ. 0 ) THEN
        ichi2 = jc-(jc-1)/3*3
        jchi  = (jc-1)/6
        ifn   = (jc-1)/3
        IF ( RNDM2(DBLE(jchi)) .LT. 0.5D0 ) jchi = -jchi
      ELSE
        ichi2 = jc-(jc-1)/4*4
        jchi  = (jc-1)/4 * ichi
        ifn   = 2-jc/4
      ENDIF
C
      IF ( idbg .GE. 10 ) THEN
        WRITE(6,*) ' +++ DECCHI +++ '
        WRITE(6,*) 'ichi1,ichi : ',ichi1,ichi
        WRITE(6,*) 'ichi2,jchi : ',ichi2,jchi
        WRITE(6,*) 'ifn       : ',ifn
      ENDIF
C
C Build the relevant quadri-impulsions
C
      CALL chideca(ch,ichi1,ichi,ichi2,jchi,ifn)
C
C Fill the common LUJET accordingly
C
      CALL filujt(ich,ichi2,jchi,ifn)
C
  999 RETURN
      END
      SUBROUTINE dilokp(xreal,ximag,f,ff)
C------------------------------------------------------------------
C! Function used in the determination of chi' --> chi gamma
C
C V. Bertin for CHA001
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      REAL*8 xreal,ximag,f,b2,ff,pi
      COMPLEX*16 f1,f2,zwei
C
      pi = 3.1415926535897932
C--berechnet (f(1/z)+f(1/zquer))*.5 --->F...(z=(xreal,ximag))
      b2 = xreal*xreal+ximag*ximag
      xreal = xreal/b2
      ximag = -ximag/b2
C--z=1/z
      b2 = 1./b2
Cvb   PRINT *,'real imag b2 :',xreal,ximag,b2
      CALL fvonz(xreal,ximag,f1)
C--f1=f(1/z)
      ximag = -ximag
      CALL fvonz(xreal,ximag,f2)
C--f2=f(1/zquer)
C     WRITE(6,*)'Imaginaerteil von (f1+f2) muesste 0 sein'
      f = DREAL((f1+f2)/2.d0)
      ff = f
      zwei = DCMPLX(2.d0,0.d0)
C     WRITE(6,*)'Im((f1+f2)/2)',dimag((f1+f2)/zwei)
      RETURN
      END
      SUBROUTINE dsigklei(ipro,s,qh,qq,qp,qm,ifs)
C---------------------------------------------------------------
C! Routine for generating final states in the reaction
C
C       e+ e-  ---> mu+ mu- H
C
C Modifications:
C     o Correct axial and vector couplings for quarks 
C       and neutrinos             - P. Janot, 04-Nov-1999
C     o Include t-channel for Hee - P. Janot, 04-Nov-1999
C---------------------------------------------------------------
      IMPLICIT REAL*8 (A-H,O-Z)
      DIMENSION qh(4),qp(4),qm(4),qq(4),rp(4), wmax(2)
      REAL*8 a,b,x,xp,x1,x2,a1,a2,xap,fap,xmh,eb
C      REAL*4 pymass
      COMPLEX*16 q0,rz,rw,qz,qw
      include 'param.inc'
      include 'lundcom.inc'
      include 'consts.inc'
      include 'zzdeck.inc'
      include 'miscl.inc'
      LOGICAL first
      DATA first /.TRUE./
      DATA empir0/0./
      DATA ievt /0/, ttot/0D0/, wmax /1D0,1D0/
      rz(x)=1./DCMPLX(x-amz**2,amz*gmz)
      rw(x)=1./DCMPLX(x-amw**2,amw*gmw)
      qz(x1,x2,x3,x4) = rz(x3)*rz(x4)/(rz(x1)*rz(x2))
      qw(x1,x2,x3,x4) = rw(x3)*rw(x4)/(rz(x1)*rz(x2))
C
C Derived constants
C
      IF ( first ) THEN
        g     = 1./SQRT(sw2*(1.-sw2))
        tpi   = 8.*ATAN(1.)
        ca1   = -g/4.
        cv1   = -g/4.*(1.-4.*sw2)
C        cv2   = (g/4.*(4.*sw2-1.))**2        ! All this works 
C        ca2   = (-g/4.)**2                   ! 
C        c1    = (cv2+ca2)**2 + 4.*cv2*ca2    !    *only*
C        c2    = (cv2+ca2)**2 - 4.*cv2*ca2    !
C        cx    = DMAX1(c1,c2)                 !  for leptons
        first = .FALSE.
      ENDIF
C
    1 ee = SQRT(s)
      IF ( ipro .EQ. 1 .OR. ipro .EQ. 5 .OR. ipro .EQ. 7 ) THEN
        p1 = pmas(25,1)
        p2 = pmas(25,2)
        CALL bwgene(0D0,ee,p1,p2,ymh,djdum)
        xmh = ymh
      ELSE
        p1 = pmas(35,1)
        p2 = pmas(35,2)
        CALL bwgene(0D0,ee,p1,p2,ymh,djdum)
        xmh = ymh
      ENDIF
      eb   = SQRT(s)/2.
      a    = 4.*xmh**2/s
      b    = amz*gmz/s
      xp   = 1. + (xmh**2-amz**2)/s
      x1   = SQRT(a)
      x2   = 1.+a/4.
      a1   = DATAN((x1-xp)/b)
      a2   = DATAN((x2-xp)/b)
      xap  = DMIN1( DMAX1(x1+b,xp) , x2 )
C      fap  = ((12.+2.*a)-12.*xap+xap**2)*SQRT(xap**2-a)
      CALL anomal(ipro,s)
      fap  = (AAA*((12.+2.*a)-12.*xap+xap**2)
     &     + (BBB-AAA)*(xap**2-a))
     &     * SQRT(xap**2-a)
C
C Generation of the Z decay
C
      IF ( ifs .LE. 0 ) THEN 
        choix = RNDM2(choix)
        DO ifs = 1,11
          IF ( choix .LT. braz(ifs) ) GOTO 2
        ENDDO
      ENDIF
C
C Check if the centre-of-mass energy after ISR is large enough
C
 2    CONTINUE
      kff    = kcode(ifs)
      xmdec  = PYMASS(kff)
      IF ( s .LT. (xmh+2.*xmdec)**2 ) GOTO 1
C
C Axial and vector couplings for all fermions (not only leptons!)
C
C 1.- Charged leptons
      IF     ( ifs .EQ. 1 .OR. ifs .EQ. 3 .OR. ifs .EQ. 5 ) THEN 
        ca2   = -g/4.
        cv2   = -g/4.*(1.-4.*sw2)
C 2.- Neutrinos
      ELSEIF ( ifs .EQ. 2 .OR. ifs .EQ. 4 .OR. ifs .EQ. 6 ) THEN 
        ca2   =  g/4.
        cv2   =  g/4.
C 3.- Down-type quarks
      ELSEIF ( ifs .EQ. 7 .OR. ifs .EQ. 9 .OR. ifs .EQ. 11 ) THEN 
        ca2   =  -g/4.
        cv2   =  -g/4.*(1.-4./3.*sw2)
C 4.- Up-type quarks
      ELSEIF ( ifs .EQ. 8 .OR. ifs .EQ. 10 ) THEN 
        ca2   =   g/4.
        cv2   =   g/4.*(1.-8./3.*sw2)
      ENDIF
      c1 = (cv1-ca1)**2*(cv2-ca2)**2+(cv1+ca1)**2*(cv2+ca2)**2
      c2 = (cv1-ca1)**2*(cv2+ca2)**2+(cv1+ca1)**2*(cv2-ca2)**2
      cx = DMAX1(c1,c2)
C
C Generation of x value (including anomalous couplings)
C
  100 r  = RNDM2(dum1)
      x  = xp + b * TAN ( r*a1 + (1.-r)*a2 )
C      w3 = ((12.+2.*a)-12.*x+x*x) * SQRT(x*x-a) / fap
      w3 = (AAA*((12.+2.*a)-12.*x+x*x)
     &   + (BBB-AAA)*(x*x-a))
     &   * SQRT(x*x-a)
     &   / fap
      empir0= DMAX1(empir0,w3)
      IF ( w3 .LE. empir ) GOTO 102
      WRITE(6,101) w3,empir
  101 FORMAT('DSIGKLEI: Weight W3 =',f6.3,' is larger than',f6.3,';',/,
     .       '         You have to increase the value of ''EMPIR'' .')
      STOP
  102 IF ( w3 .LT. (RNDM2(dum2)*empir) ) GOTO 100
C
C Generation of higgs solid angle
C
  200 ch = -1. + 2.*RNDM2(dum3)
C      w4 = 1.-ch*ch*(x*x-a)/(8.*(1.-x)+x*x+a)
      w4 = 1.-BBB*ch*ch*(x*x-a)
     &   / (AAA*(8.*(1.-x)+x*x+a)
     &   + (BBB-AAA)*(x*x-a))
      IF ( w4 .GT. 1D0 ) WRITE(6,*) '+++ Warning +++ w4 = ',w4
      IF ( w4 .LT. RNDM2(dum4) ) GOTO 200
      fh = tpi*RNDM2(dum5)
C
C Construct Higgs momentum
C
      IF ( eb*x .LT. xmh ) THEN
        qh(4) = xmh
        qvec = 0.
      ELSE
        qh(4) = eb*x
        qvec  = SQRT ((eb*x)**2-xmh**2)
      ENDIF
      sh    = SQRT (1.-ch**2)
      qh(1) = qvec * sh * SIN(fh)
      qh(2) = qvec * sh * COS(fh)
      qh(3) = qvec * ch
C
C Construct sum of muon momenta
C
      DO 300 i=1,4
  300 qq(i) = -qh(i)
      qq(4) = 2.*eb + qq(4)
      xmt2 = qq(4)**2-qq(3)**2-qq(2)**2-qq(1)**2
      IF ( xmt2 .LE. 0. ) THEN
        xmt2 = 1.1E-6
        qq(4) = SQRT(qq(3)**2+qq(2)**2+qq(1)**2+xmt2)
      ENDIF
C
C Generate mu+ momentum (PEP) and energy (EP) in the mu+mu- CM frame
C
      ep = SQRT (s*(1.-x)+xmh**2)/2.
      pep2= ep*ep-xmdec*xmdec
      pep = -1.
      IF ( pep2 .GT. 0. ) pep = SQRT(pep2)
C
C Try again if this configuration kinematically not o.k.
C
      IF ( pep .LE. 0. ) GOTO 100
C
C Generate the muon directions in the mu+mu- CM frame
C
  400 cm    = -1. + 2.*RNDM2(dum6)
      sm    = SQRT (1.-cm*cm)
      fm    = tpi*RNDM2(dum7)
      rp(1) = pep * sm * SIN(fm)
      rp(2) = pep * sm * COS(fm)
      rp(3) = pep * cm
      rp(4) = ep
C
C Boost mu+ momenta to lab frame and construct mu- momentum
C
      qp(4)=(qq(4)*rp(4)+qq(1)*rp(1)+qq(2)*rp(2)+qq(3)*rp(3))/(2.*ep)
      qr   =(rp(4)+qp(4))/(2.*ep+qq(4))
      DO 500 i=1,3
      qp(i)=rp(i)+qq(i)*qr
  500 qm(i)=qq(i)-qp(i)
      qm(4)=qq(4)-qp(4)
C
C Calculate weight of muon angular variables 
C Warning (05/03/2000): the z-direction was opposite to the LEP 
C convention until today!
C
      ppdqp = eb * ( qp(4) + qp(3) )
      pmdqp = eb * ( qp(4) - qp(3) )
      ppdqm = eb * ( qm(4) + qm(3) )
      pmdqm = eb * ( qm(4) - qm(3) )
      w5 = ( c1*ppdqm*pmdqp + c2*ppdqp*pmdqm )
     .   / ( cx*(ppdqp+ppdqm)*(pmdqp+pmdqm))
      IF ( w5 .LT. RNDM2(dum8) ) GOTO 400
C
C If requested:
C  o Include t-channel for electrons (also called ZZ fusion)
C  o Include t-channel for nu_e's    (also called WW fusion)
C
      IF ( ipro .GT. 2 ) THEN 
C
        ievt = ievt + 1
C
        s1 =  4.*eb**2
        s2 =  2.*prosca(qp,qm)
        t1 = -2.*ppdqp
        t2 = -2.*pmdqm
        u1 = -2.*ppdqm
        u2 = -2.*pmdqp
C
C Interference with ZZ fusion
C
        IF ( ifs .EQ. 1 ) THEN 
C
          q0 = qz(s1,s2,t1,t2)
          he =         t1*t2
     &       +         s1*s2 * CDABS(q0)**2
     &       + c1/c2 * u1*u2 * CDABS(1D0+q0)**2
          hm =         t1*t2
     &       + c1/c2 * u1*u2
          w6 = he / hm / 100D0 / wmax(1)
C
C Inteference with WW fusion
C
        ELSEIF ( ifs .EQ. 2 ) THEN
C
          q0 = qw(s1,s2,t1,t2)
          he =         t1*t2
     &       + c1/c2 * u1*u2
     &       * CDABS(1D0+2D0*cw2*cw2*(cv2+ca2)/(cv1+ca1)*q0)**2
          hm =         t1*t2
     &       + c1/c2 * u1*u2
          w6 = he / hm / 150D0 / wmax(2)
C
        ENDIF
C
C Check if the weight is not too large
C
        IF ( w6 .GT. 1D0 ) THEN 
C          WRITE(6,*) '++++ DSIGKLEI Warning ++++'
C          WRITE(6,*) 'w6 = ',ievt,w6
          wmax(ifs) = wmax(ifs) * w6
        ENDIF
C
        IF ( w6 .LT. RNDM2(dum9) ) GOTO 100
C
      ENDIF
C
C Store the maximum weight
C
      empirm = empir0
C
C Et voila.
C
      qh(4) = xmh
      qq(4) = SQRT(xmt2)
C
      RETURN
      END
      SUBROUTINE dsigma(ipro,ss,qh,qa,hh,izpol,iret)
C-----------------------------------------------------------------------
C!  Angular distributions for Higgs production
C
C   Patrick Janot  --  27 Aug 1991
C-----------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION qh(4), qa(4), hh(4), gg(4)
      include 'param.inc'
      include 'hhprod.inc'
      include 'lundcom.inc'
      include 'datapi.inc'
      include 'sigmas.inc'
      include 'miscl.inc'
      ada(am1,am2,am3) = (am1**2-am2**2-am3**2)**2-4.*am2**2*am3**2
C
      iret = 0
      ee = SQRT(ss)
      izpol = 0
C
      IF ( ipro .GE. 5 .AND. ipro .LE. 8 ) GOTO 1000
C
C HZ & HA
C
C Take into account the widths of the Z and of the Higgses
C
      IF ( ipro .EQ. 1 .OR. ipro .EQ. 2 ) THEN
C
        IF ( iklei .EQ. 1 ) THEN
          izpol = 0
          CALL dsigklei(ipro,ss,qh,qa,hh,gg,izpol)
          GOTO 999
C
        ELSE
C
          plmbda = ada(ee,0D0,0D0)
          wmax   = SQRT(plmbda)*(plmbda+12.*ss*amz**2)
 101      CONTINUE
          IF ( ipro .EQ. 1 ) THEN
            p1 = pmas(25,1)
            p2 = pmas(25,2)
            CALL bwgene(0D0,ee    ,p1  ,p2,  am1,djdum)
            CALL bwgene(0D0,ee-am1,amz ,gmz, am2,djdum)
          ELSE
            p1 = pmas(35,1)
            p2 = pmas(35,2)
            CALL bwgene(0D0,ee    ,p1  ,p2,  am1,djdum)
            CALL bwgene(0D0,ee-am1,amz       ,gmz       ,am2,djdum)
          ENDIF
          almbda = ada(ee,am1,am2)
          IF ( am2 + am1 .GT. ee .OR. almbda .LT. 0. ) GOTO 101
          weight = SQRT(almbda)*(almbda+12.*ss*amz**2)
          IF ( weight/wmax .LT. RNDM2(weight) ) GOTO 101
C
        ENDIF
C
      ELSEIF ( ipro .EQ. 3 .OR. ipro .EQ. 4 ) THEN
C
        wmax   = SQRT(ada(ee,0D0,0D0))**3
 102    CONTINUE
        IF ( ipro .EQ. 3 ) THEN
          IF ( RNDM2(DBLE(ipro)) .GT. 0.5D0 ) THEN
            p1 = pmas(25,1)
            p2 = pmas(25,2)
            r1 = pmas(36,1)
            r2 = pmas(36,2)
            CALL bwgene(0D0,ee    ,p1,p2,am1,djdum)
            CALL bwgene(0D0,ee-am1,r1,r2,am2,djdum)
          ELSE
            p1 = pmas(36,1)
            p2 = pmas(36,2)
            r1 = pmas(25,1)
            r2 = pmas(25,2)
            CALL bwgene(0D0,ee    ,p1,p2,am2,djdum)
            CALL bwgene(0D0,ee-am2,r1,r2,am1,djdum)
          ENDIF
        ELSE
          IF ( RNDM2(DBLE(ipro)) .GT. 0.5D0 ) THEN
            p1 = pmas(35,1)
            p2 = pmas(35,2)
            r1 = pmas(36,1)
            r2 = pmas(36,2)
            CALL bwgene(0D0,ee    ,p1,p2,am1,djdum)
            CALL bwgene(0D0,ee-am1,r1,r2,am2,djdum)
          ELSE
            p1 = pmas(36,1)
            p2 = pmas(36,2)
            r1 = pmas(35,1)
            r2 = pmas(35,2)
            CALL bwgene(0D0,ee    ,p1,p2,am2,djdum)
            CALL bwgene(0D0,ee-am2,r1,r2,am1,djdum)
          ENDIF
        ENDIF
        almbda = ada(ee,am1,am2)
        IF ( am2 + am1 .GT. ee .OR. almbda .LT. 0. ) GOTO 102
        weight = SQRT(almbda)**3
        IF ( weight/wmax .LT. RNDM2(weight) ) GOTO 102
C
      ELSEIF ( ipro .EQ. 9 ) THEN
C
        wmax   = SQRT(ada(ee,0D0,0D0))**3
 103    CONTINUE
        IF ( RNDM2(DBLE(ipro)) .GT. 0.5D0 ) THEN
          p1 = pmas(37,1)
          p2 = pmas(37,2)
          CALL bwgene(0D0,ee    ,p1,p2,am1,djdum)
          CALL bwgene(0D0,ee-am1,p1,p2,am2,djdum)
        ELSE
          p1 = pmas(37,1)
          p2 = pmas(37,2)
          CALL bwgene(0D0,ee    ,p1,p2,am2,djdum)
          CALL bwgene(0D0,ee-am2,p1,p2,am1,djdum)
        ENDIF
        almbda = ada(ee,am1,am2)
        IF ( am2 + am1 .GT. ee .OR. almbda .LT. 0. ) GOTO 103
        weight = SQRT(almbda)**3
        IF ( weight/wmax .LT. RNDM2(weight) ) GOTO 103
C
      ELSEIF ( ipro .EQ. 10 ) THEN
C   
        p1 = pmas(25,1)
        p2 = pmas(25,2)
        CALL bwgene(0D0,ee    ,p1  ,p2,  am1,djdum)
        am2 = 0D0
C
      ENDIF
C
      qh(4) = (ss+am1**2-am2**2) / (2.*ee)
      qa(4) = (ss-am1**2+am2**2) / (2.*ee)
      pmom2 = (ss-(am1+am2)**2)  * (ss-(am1-am2)**2)
      pmom  = SQRT(pmom2)/(2.*ee)
C
 100  c = 2.*rndm2(c) - 1.
      IF ( ipro .GE. 3 .AND. ipro .LT. 10 ) THEN
        weight = 1D0-c**2
        wmax = 1D0
        IF ( weight/wmax .LT. RNDM2(dummy) ) GOTO 100
      ELSE IF ( ipro .EQ. 10 ) THEN
        weight = 1D0+c**2
        wmax = 2D0
        IF ( weight/wmax .LT. RNDM2(dummy) ) GOTO 100
      ELSE
        weight = 1.+c**2 + qa(4)**2/am2**2 * (1.-c**2)
        wmax   = 1. + (qa(4)/am2)**2
        IF ( weight/wmax .LT. RNDM2(dummy) ) GOTO 100
        IF ( (1.+c**2)/weight .GT. RNDM2(dummy) ) THEN
          izpol = 1
        ELSE
          izpol = 0
        ENDIF
      ENDIF
      p = 2.*pi*rndm2(p)
      s = SQRT(1.-c**2)
C
      qh(4) = am1
      qh(3) = pmom * c
      qh(2) = pmom * s * SIN(p)
      qh(1) = pmom * s * COS(p)
      qa(4) = am2
      qa(3) =-qh(3)
      qa(2) =-qh(2)
      qa(1) =-qh(1)
      GOTO 999
C
C WW/ZZ fusion now.
C Include interference with hZ/HZ whenever requested
C
 1000 CONTINUE
C
      IF ( interf(ipro) .EQ. 0 ) THEN 
        CALL dsigwwh(ipro,ee,qh,qa,hh,iret)
      ELSEIF ( ipro .EQ. 7 .OR. ipro .EQ. 8 ) THEN  
        izpol = 1
        CALL dsigklei(ipro,ss,qh,qa,hh,gg,izpol)
        CALL ucopy(hh(1),qa(1),8)
        CALL ucopy(qh(1),hh(1),8)
        CALL ucopy(gg(1),qh(1),8)
      ELSEIF ( ( ipro .EQ. 5 .AND. amh .LT. ee-amz ) .OR.
     &         ( ipro .EQ. 6 .AND. gmh .LT. ee-amz ) ) THEN 
        izpol = 2
        CALL dsigklei(ipro,ss,qh,qa,hh,gg,izpol)
        CALL ucopy(hh(1),qa(1),8)
        CALL ucopy(qh(1),hh(1),8)
        CALL ucopy(gg(1),qh(1),8)
      ELSE
        w0 = chnnz(ipro) / (chnnz(ipro)+chnni(ipro)+chnns(ipro))
        IF ( w0 .LT. RNDM2(w0) ) THEN 
          CALL dsigwwh(ipro,ee,qh,qa,hh,iret)
        ELSE
          izpol = 2
          CALL dsigklei(ipro-4,ss,qh,qa,hh,gg,izpol)
          CALL ucopy(hh(1),qa(1),8)
          CALL ucopy(qh(1),hh(1),8)
          CALL ucopy(gg(1),qh(1),8)
        ENDIF
      ENDIF
C
 999  RETURN
      END
      SUBROUTINE dsigwwh(ipro,ee,p1,p2,hh,iret)
C -------------------------------------------------------------------
C! Generate unweighted events in the WW/ZZ fusion into h channels
C
C Input:    ee,   the effective centre-of-mass energy
C           ipro, = 5 for WW --> h
C                 = 6 for WW --> H
C                 = 7 for ZZ --> h
C                 = 8 for ZZ --> H
C
C Output    p1,   the neutrino/e- momentum
C           p2,   the anti-neutrino/e+ momentum
C           hh,   the Higgs momentum
C
C Patrick Janot -- 3 Sep 1995
C
C Modifications: 
C    o Patrick Janot - 4 Nov 1999
C      Include the possibility of interference with the Higgstrahlung
C      process, only for the Hnn final state [problematic for the Hee
C      final state, for which the interference term is negative and
C      larger in absolute value than the squared term -> done in 
C      dsigklei]   
C      
C -------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'lundcom.inc'
      include 'hhprod.inc'
      include 'datapi.inc'
      include 'consts.inc'
      include 'miscl.inc'
      DIMENSION p1(4), p2(4), pm(4), pp(4), hh(4), ptot(4)
      REAL*8 qbt,y0,tau,betah,x1,x2,xtau,px,pmax,eb,dv,dv1,dv2
      REAL*8 densf,densit,c1,c2,s1,s2,fi1,fi2,c1df,c2df
      REAL*8 x,ratio1,ratio2,ratio3,born1,born2,bwh
      DATA rmax /1D0/, ievt/0/, ineg/0/
      evolwz(x) = -(1.+x)*DLOG(x)-2.*(1.-x)
C
      ss   = ee**2
      eb   = ee/2.
C
C The incoming momenta
C
      pm(1) = 0.
      pm(2) = 0.
      pm(3) = eb
      pm(4) = eb
      pp(1) = 0.
      pp(2) = 0.
      pp(3) =-eb
      pp(4) = eb
C
C Compute relevant couplings
C
      e = SQRT(4.*pi*alpha(0))
C
C Case 1: WW scattering
C
      IF ( ipro .EQ. 5 .OR. ipro .EQ. 6 ) THEN
C
        rmv  = amw
        gmv  = gmw
        v1   = e/sw/SQRT(8.)
        v2   = v1
        a1   = v1
        a2   = v1
        gvvh = e/sw*rmv
C
C Case 2: ZZ scattering
C
      ELSEIF ( ipro .EQ. 7 .OR. ipro .EQ. 8 ) THEN
C
        rmv  = amz
        gmv  = gmz
        a1   =-e/4./sw/cw
        v1   = a1*(1.-4.*sw2)
        a2   = a1
        v2   = v1
        gvvh = e*rmv/sw/cw
C
      ENDIF
C
C This goes into the matrix element squared
C
      con1 = gvvh**2*((v1-a1)**2*(v2-a2)**2+(v1+a1)**2*(v2+a2)**2)
      con2 = gvvh**2*((v1-a1)**2*(v2+a2)**2+(v1+a1)**2*(v2-a2)**2)
C
C And this goes into the fusion/higgs-strahlung interference term
C
      IF ( ipro .EQ. 5 .OR. ipro .EQ. 6 ) THEN
        cnit = e**6*amz**2*(-2.+4.*sw2)/(16.*sw**6*cw**2)
      ELSEIF ( ipro .EQ. 7 .OR. ipro .EQ. 8 ) THEN
        cnit = con1
      ENDIF
C
C Cross section comes out in pb
C
      picob = alpha2 / alpha(0)**2
     .               / (2.*ss*twopi**5)
C
C Generate the mass of the Higgs
C
      IF     ( ipro .EQ. 5 .OR. ipro .EQ. 7 ) THEN
C
        rmh  = pmas(25,1)
        rgh  = pmas(25,2)
        cou  = sab2
C
      ELSEIF ( ipro .EQ. 6 .OR. ipro .EQ. 8 ) THEN
C
        rmh  = pmas(35,1)
        rgh  = pmas(35,2)
        cou  = cab2
C
      ENDIF
C
      rmh2 = rmh**2
      rmgh = rmh*rgh
      CALL bwgene(0D0,ee,rmh,rgh,rm1,djdum)
      rm2 = rm1**2
C
C Approximate the maximal weight for a better efficiency
C
      ratio1 = rmh2/ecm**2
      ratio2 = rm2/ss
      ratio3 = ss/rmh**2
      born2  = rmh/rgh
      born1  = -born2 * (1.-ratio3)
C
      wmax = 10. * rmv/amw * cross(ipro) / reduc(ipro)    
C
      IF ( 1.-ratio2 .GT. 1D-2 ) THEN
        evol = evolwz(ratio2)
      ELSE
        evol = (1.-ratio2)**2/6. + (1.-ratio2)**3/6.
      ENDIF
C
      IF ( ratio3 .GT. 1D-1 ) THEN
        evol = evol
     .       * ( DATAN(born1)+DATAN(born2) )
     .       / pi
      ELSE
        evol = evol
     .       * ( (rmh/rgh)    / (1.+(rmh/rgh)**2)    * ratio3
     .       +   (rmh/rgh)**3 / (1.+(rmh/rgh)**2)**2 * ratio3**2 )
     .       / pi
      ENDIF
      evol = evol / evolwz(ratio1)
C
      IF ( evol .LT. 1D-2 ) THEN 
C        WRITE(6,*) 'Warning: evol = ',evol,ee
        iret = 999
        GOTO 999
      ENDIF
C    .  evol = cou * sigmawwh(ss,ipro) / cross(ipro)

C
      wmax = wmax * evol
      
C
C Compute the delta-V angular distribution cutoff
C
      rmv2  = rmv**2
      gmv2  = gmv**2
      betah = 1.-rm2/ss
      dv    = 2.*rmv2/(ss*betah)
      dv1   = 1.+dv
      dv2   = dv*(2.+dv)
C
C Constant part of the local density
C
      densf = (dv2/4./pi)**2 * 64./ss
C
C Generation of fermion scattering angles
C
    2 c1  = dv1-dv2/(2.d0*RNDM2(1d0)+dv)
      c2  = dv1-dv2/(2.d0*RNDM2(3d0)+dv)
      fi1 = twopi*RNDM2(2d0)
      fi2 = twopi*RNDM2(4d0)
C
C Construct fermion directions
C
      s1    = SQRT(1.-c1*c1)
      s2    = SQRT(1.-c2*c2)
      p1(1) = s1*SIN(fi1)
      p1(2) = s1*COS(fi1)
      p1(3) = c1
      p1(4) = 1.
      p2(1) =-s2*SIN(fi2)
      p2(2) =-s2*COS(fi2)
      p2(3) =-c2
      p2(4) = 1.
C
C Compute acollinearity parameter
C
      tau = prosca(p1,p2)/2.
C
C Generate x1 value such that x1 and x2 distributions are the same
C
      y0   = 1.-betah*tau
      pmax = betah*betah/(4.*y0)
    1 CONTINUE
      x1   = betah*RNDM2(5d0)
      xtau = 1./(1.-tau*x1)
      x2   = (betah-x1)*xtau
      px   = x1*x2*xtau
      IF ( px .LT. RNDM2(6d0)*pmax ) GOTO 1
C
C Construct fermion momenta
C
      DO k=1,4
        p1(k) = p1(k)*eb*x1
        p2(k) = p2(k)*eb*x2
      ENDDO
C
C The higgs momentum follows from momentum conservation
C
      DO k=1,4
        hh(k) = pm(k)+pp(k)-p1(k)-p2(k)
      ENDDO
C
C Check on algorithm by computing the Higgs mass
C
      chk = (hh(4)**2-hh(3)**2-hh(2)**2-hh(1)**2)/rm2 - 1.
      IF ( ABS(chk) .GT. 1D-3 ) WRITE(*,*) ' +++ mh chk:',chk
      hh(4) = rm1
C
C Compute the local density
C
      IF( betah*tau .GT. 1D-3 ) THEN
        qbt  = (-(1.+y0)*DLOG(y0)+2.*y0-2.)/(tau*tau*tau)
      ELSE
        qbt  = betah**3*(10.+10.*betah*tau+9.*betah**2*tau**2)/60.
      ENDIF
      c1df   = -c1+dv1
      c2df   = -c2+dv1
      densit = densf/(qbt*c1df*c1df*c2df*c2df)
C
C Compute exact matrix element squared for
C   o e+ e- ---> nu_e-bar nu_e W+ W- --> nu_e-bar nu-e h
C   o e+ e- ---> e+ e-         Z  Z  --> e+ e-         h
C
      t1    = -2.*prosca(pm,p1)
      t2   =  -2.*prosca(pp,p2)
      u1    = -2.*prosca(pm,p2)
      u2    = -2.*prosca(pp,p1)
      s1    =  2.*prosca(p1,p2)
      vv1   =  t1-rmv2
      vv2   =  t2-rmv2
      ss1   =  s1-amz*amz
      ss2   =  ss-amz*amz
      tfi2  = ( con1*u1*u2 + con2*ss*s1 )
     .      / ( (vv1*vv1+rmv2*gmv2) * (vv2*vv2+rmv2*gmv2) )
C     .      / ( vv1*vv1 * vv2*vv2)
C
C Compute also the exact interference term with Higgs-strahlung
C
      IF ( interf(ipro) .EQ. 1 ) THEN 
        amz2 = amz*amz
        gmz2 = gmz*gmz
        tif2  = 2D0*cnit*u1*u2
     &        * (
     &        (ss1*ss2-amz2*gmz2)*(vv1*vv2-rmv2*gmv2)
     &        + amz*rmv*gmz*gmv*(ss1+ss2)*(vv1+vv2)
     &        )
     &        / ( (ss1*ss1+amz2*gmz2) * (ss2*ss2+amz2*gmz2) )
     &        / ( (vv1*vv1+rmv2*gmv2) * (vv2*vv2+rmv2*gmv2) )
C
        tfi2 = tfi2 + tif2
C
      ENDIF
C
C Compute the weight
C
      weight = cou * picob * tfi2  / densit
     .       * (1.+deltar)**3
     &       * (1.-5./3.*deltar)
C
C Make a few checks & counts
C
      ievt = ievt + 1
      IF ( weight/wmax .GT. rmax ) THEN 
        rmax = weight/wmax
C        WRITE(6,*) 'DSIGWWH Warning - rmax = ',ievt,rmax
      ENDIF
C
      ipr = ipro-4
      wtot (ipr) = wtot (ipr) +  weight
      wtot2(ipr) = wtot2(ipr) +  weight**2
      ntry (ipr) = ntry (ipr) + 1
C
      IF ( weight .LT. 0D0 ) THEN 
C        ineg = ineg + 1
C        WRITE(6,*) 'Negative matrix element squared: ',ievt,weight
        iret = 998
        GOTO 999
      ENDIF
C
C Apply the rejection algorithm
C
      IF ( weight/wmax .LT. RNDM2(weight) ) GOTO 2
C
      nacc(ipr) = nacc(ipr) + 1
C
 999  RETURN
      END
      FUNCTION f0(jhig,hmass)
C--------------------------------------------------------------------
C! Utility routine to compute gamma gamma and gluon gluon widths
C
C Input:    jhig,   Higgs type
C           hmass,  spin 0 boson mass
C
C Output:   f0,    the value of the function
C
C Patrick Janot -- 03 oct 1991
C--------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      COMPLEX*16 ftau, f0
      EXTERNAL ftau
C
      tau = 4.*hmass**2/amhig(jhig)**2
      f0 = tau*(1.-tau*ftau(tau))
C
      RETURN
      END
      FUNCTION f1(jhig,bmass)
C--------------------------------------------------------------------
C! Utility routine to compute gamma gamma and gluon gluon widths
C
C Input:    jhig,   Higgs type
C           bmass,  spin 1 boson mass
C
C Output:   f1,    the value of the function
C
C Patrick Janot -- 03 oct 1991
C--------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      COMPLEX*16 ftau, f1
      EXTERNAL ftau
C
      tau = 4.*bmass**2/amhig(jhig)**2
      f1 = 2. + 3.*tau + 3.*tau*(2.-tau)*ftau(tau)
C
      RETURN
      END
      FUNCTION f12(jhig,fmass)
C--------------------------------------------------------------------
C! Utility routine to compute gamma gamma and gluon gluon widths
C
C Input:    jhig,   Higgs type
C           fmass,  fermion mass
C
C Output:   f12,    the value of the function
C
C Patrick Janot -- 03 oct 1991
C--------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'datapi.inc'
      COMPLEX*16 ftau, f12
      EXTERNAL ftau
C
      IF ( jhig .LE. 2 ) epsh = 1D0
      IF ( jhig .EQ. 3 ) epsh = 0D0
      tau = 4.*fmass**2/amhig(jhig)**2
      f12 = -2.*tau * (epsh + (1.-tau*epsh)*ftau(tau))
C
      RETURN
      END
      FUNCTION FFFF(A,Z,Y,AAA,BBB)
C--------------------------------------------------------------------
C! Analytical expression for the e+e- --> hZ cross section
C
C  From Ronald Kleiss.
C
C  Modifications:
C
C    o Patrick Janot - 29 Oct 99:  add the anomalous coupling 
C                                  contribution
C--------------------------------------------------------------------
      IMPLICIT COMPLEX*16(A-H,O-Z)
      REAL*8 A,A2,Y,AAA,BBB,DELTA
C
      z2 = z*z
      z3 = z*z2
      z4 = z*z3
      a2 = a*a
C
      c0 = ( z4 - 2.*a*z2 + a2 ) /z4 *
     .     ( z4 - 24.*z3 + (48.+10.*a)*z2 - 24.*a*z + a2)
      c1 = a2*a2/(3.*z)
      c2 = - a*a2*( 24.*z - a )/(2.*z2)
      c3 = a2*( 8.*z2*(a+6.) - 24.*a*z + a2 ) /z3
      c4 = -a2*( 24.*z3 + 8.*z2*(a+6.) - 24.*a*z + a2 )/z4
      c5 = z3 - 24.*z2 + 8.*z*(a+6.) + 24.*a
      c6 = z2/2. - 12.*z + 4.*(a+6.)
      c7 = z/3. - 8.
      c8 = 1./4.
C
C Anomalous couplings
C
      delta = BBB - AAA
C
      c0 = c0 * AAA
     &        + ( z4 - 2.*a*z2 + a2 )
     &        * ( z4 - 2.*a*z2 + a2 )
     &        /   z4
     &        * delta
      c1 = c1 * AAA
     &        + a2*a2/(3.*z)
     &        * delta
      c2 = c2 * AAA
     &        + a2*a2/(2.*z2)
     &        * delta
      c3 = c3 * AAA
     &        + a2*(a2-4.*a*z2) /z3
     &        * delta
      c4 = c4 * AAA
     &        - a2*(a2-4.*a*z2) /z4
     &        * delta
      c5 = c5 * AAA
     &        + (z3-4.*a*z)
     &        * delta
      c6 = c6 * AAA
     &        + (z2/2.-2.*a)
     &        * delta
      c7 = c7 * AAA
     &        + z/3.
     &        * delta
      c8 = c8 * AAA
     &        + 1./4.
     &        * delta
C
      FFFF = c0*CDLOG(y-z) + c4*DLOG(y) +
     . (c1+y*(c2+y*(c3+y*y*(c5+y*(c6+y*(c7+y*c8))))))/(y*y*y)
C
      RETURN
      END
      DOUBLE PRECISION FUNCTION FI( ambos , amfer , chii , chio )
C------------------------------------------------------------------
C! Function used in the determination of chi' --> chi gamma
C
C V. Bertin for CHA001
C------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
C      REAL*4 ambos,amfer,chii,chio
C
C     PRINT *,'FI mbos mfer chii chio :',ambos,amfer,chii,chio
      result = 0.D0
C
C     IF ( chii .GE. ambos + amfer ) THEN
C      PRINT *,'ambos amfer chii :',ambos,amfer,chii
C      GOTO 999
C     ENDIF
      IF ( chio .LT. 1D-9 ) chio = 1D-9
      IF ( chii .LT. 1D-9 ) chii = 1D-9
C
      hi = (ambos**2 - amfer**2 - chii**2) / (2.* chii**2)
      wi = hi**2 - (amfer / chii)**2
c     PRINT *,'hi wi :',hi,wi
      IF ( wi .LT. 0.D0 )  THEN
       ximag = DSQRT(-wi)
       xreal = -hi
       CALL dilokp(xreal,ximag,f1,f2)
      ELSE
       xrpi = -hi + DSQRT(wi)
       IF ( ABS(xrpi) .LE. 1.D-15) xrpi = 1.D-15
       xrpi = 1. / xrpi
       xrmi = -hi - DSQRT(wi)
       IF ( ABS(xrmi) .LE. 1.D-15) xrmi = 1.D-15
       xrmi = 1. / xrmi
       f1 = ddilog(xrpi)
       f2 = ddilog(xrmi)
      ENDIF
c     PRINT *,'f1   f2 :',f1,f2
C
      ho = (ambos**2 - amfer**2 - chio**2) / (2.* chio**2)
      wo = ho**2 - (amfer / chio)**2
c     PRINT *,'ho wo :',ho,wo
      IF ( wo .LT. 0.D0 ) THEN
       ximag = DSQRT(-wo)
       xreal = -ho
       CALL dilokp(xreal,ximag,f3,f4)
      ELSE
       xrpi = -ho + DSQRT(wo)
       IF ( ABS(xrpi) .LE. 1.D-15) xrpi = 1.D-15
       xrpi = 1. / xrpi
       xrmi = -ho - DSQRT(wo)
       IF ( ABS(xrmi) .LE. 1.D-15) xrmi = 1.D-15
       xrmi = 1. / xrmi
       f3 = ddilog(xrpi)
       f4 = ddilog(xrmi)
      ENDIF
c     PRINT *,'f3   f4 :',f3,f4
C
      result = (-f1 -f2 + f3 + f4) / (chii**2 - chio**2)
c     PRINT *,'result :',result
C
 999  CONTINUE
      fi = result
      RETURN
      END
      DOUBLE PRECISION FUNCTION FI2( ambos , amfer , chii , chio )
C------------------------------------------------------------------
C! Function used in the determination of chi' --> chi gamma
C
C V. Bertin for CHA001
C------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
C      REAL*4 ambos,amfer,chii,chio
      COMPLEX*16 lambi,lambo,him,hom,hip,hop,ri2
C
      IF ( chio .LT. 1D-9 ) chio = 1D-9
      IF ( chii .LT. 1D-9 ) chii = 1D-9
      IF ( amfer.LT. 1D-9 ) amfer= 1D-9
C
c     PRINT *,'FI2 mbos mfer chii chio :',ambos,amfer,chii,chio
      parai = amfer**2 + ambos**2 - chii**2
      parao = amfer**2 + ambos**2 - chio**2
      rambi = parai**2 - (2.*ambos*amfer)**2
      rambo = parao**2 - (2.*ambos*amfer)**2
      zero = 0.d0
c     PRINT *,'rambi rambo zero:',rambi,rambo,zero
      lambi = DCMPLX(rambi,zero)
      lambo = DCMPLX(rambo,zero)
      lambi = CDSQRT(lambi)
      lambo = CDSQRT(lambo)
c     PRINT *,'lambi lambo :',lambi,lambo
c     PRINT *,'parai parao :',parai,parao
      hip = parai + lambi
      him = parai - lambi
      hop = parao + lambo
      hom = parao - lambo
      IF (CDABS(him) .LT. 1.d-12) him = DCMPLX(1.d-12,0.d0)
      IF (CDABS(hom) .LT. 1.d-12) hom = DCMPLX(1.d-12,0.d0)
c     PRINT *,'hip him hop hom :',hip,him,hop,hom
C
c     ri2 = (CDLOG(him / hip) * lambi / (2.* chii**2)
c    .     - CDLOG(hom / hop) * lambo / (2.* chio**2))
c    .     / (chii**2 - chio**2)
c    .    + (ambos**2 - amfer**2) / (2. * chii**2 * chio**2)
c    .     * LOG(amfer**2 / ambos**2)
      faci = .5d0 / chii**2 / (chii**2 - chio**2)
      faco = .5d0 / chio**2 / (chii**2 - chio**2)
      ri2 = CDLOG(him / hip) * lambi * faci
     .    - CDLOG(hom / hop) * lambo * faco
      fi2 = DREAL(ri2)
     .    + (ambos**2 - amfer**2) / (2.d0 * chii**2 * chio**2)
     .     * LOG(amfer**2 / ambos**2)
c     PRINT *,'ri2 fi2:',ri2,fi2
      RETURN
      END
      SUBROUTINE filujt(ich,ichi2,jchi,ifn)
C------------------------------------------------------------------
C! Fill LUJETS common block after a neutral/charg-ino decay.
C
C  Input:    ich,   the position of the neutral/charg-ino in LUJET
C            ichi2, the index of the final charg/neutral-ino
C            jchi,  its charge (absolute value)
C            ifn,   the index of the final boson
C                     = 0 for a photon
C                     = 1 for a Z
C                     = 2 for a W
C
C  Patrick Janot -- 31 Aug 1995
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'lundcom.inc'
      include 'param.inc'
      include 'hhdeck.inc'
      DIMENSION ijoin(2)
      DIMENSION p1(4),p2(4),p3(4)
C
      ichi = k7lu(ich,2)/ABS(k7lu(ich,2))
C
      IF ( jchi .EQ. 0 ) THEN
        kf1 = 50+ichi2
      ELSE
        kf1 = jchi * (54+ichi2)
      ENDIF
C
      kf2 = 22 + ifn
      IF ( kf2 .EQ. 24 ) THEN
        IF ( ichi .EQ. 0 ) THEN
          kf2 = -jchi * kf2
        ELSE
          kf2 =  ichi * kf2
        ENDIF
      ENDIF
C
      n7lu = n7lu + 1
      ipochi = n7lu
      CALL hhlu1(ipochi,kf1,pvect4(1,1),pvect4(2,1),
     .                      pvect4(3,1),pvect4(4,1),pvect4(5,1))
      k7lu(ich,4) = ipochi
      k7lu(ipochi,3) = ich
C
      n7lu = n7lu + 1
      ipobos = n7lu
      CALL hhlu1(ipobos,kf2,pvect4(1,2),pvect4(2,2),
     .                      pvect4(3,2),pvect4(4,2),pvect4(5,2))
      k7lu(ich,5) = ipobos
      k7lu(ipobos,3) = ich
C
C Special path for W and Z
C
      IF ( ifn .GT. 0 ) CALL wzdecy(ipobos)
      k7lu(ich,1) = 11
C
      IF ( idbg .GE. 5 ) CALL pylist(1)
  999 RETURN
      END
      DOUBLE PRECISION FUNCTION FINTCAR(a,b,c)
C--------------------------------------------------------------------
C! Quick integration for the Higgs pole masses computation
C
C  Improves on the previous method of Carena/Wagner by a
C  factor > 100.
C
C  Patrick Janot -- 21 Sep 1995
C--------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON /CEM/ y1,y2,p2
      EXTERNAL fintsub
      DIMENSION x(1)
      p2 = a
      y1 = b
      y2 = c
      xlo = 0D0
      xhi = 1D0
      fintcar  = DGMLT1(fintsub,xlo,xhi,1,6,x)
      RETURN
      END
      SUBROUTINE fintsub(m,u1,f1,x)
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON /CEM/ y1,y2,p2
      DIMENSION u1(*),f1(*),x(1)
      DO l = 1,m
        x(1) = u1(l)
        f1(l) = LOG(ABS(x(1)*y1+(1-x(1))*y2-x(1)*(1-x(1))*p2)
     .                /(x(1)*(y1-y2)+y2))
      ENDDO
      RETURN
      END
      DOUBLE PRECISION FUNCTION FINTAN(p2,y1,y2)
C--------------------------------------------------------------------
C! Analytical expression for the integration done in FINT
C
C  Not used in the code
C
C  From M. Carena and C. Wagner, 21 Sep 1995
C--------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,M,O-Z)
      delta=(y1-y2)/p2
      erre=(abs((1.+delta)**2-4.*y1/p2))**.5
      fintan=-1.+.5*((y1+y2)/(y1-y2)-delta)*log(y2/y1)
     *    +.5*erre*log(abs((delta**2-(1.+erre)**2)) /
     *      abs((delta**2-(1.-erre)**2)) )
      RETURN
      END
      DOUBLE PRECISION FUNCTION FJ( ambos , amfer , chii , chio )
C------------------------------------------------------------------
C! Function used in the determination of chi' --> chi gamma
C
C V. Bertin for CHA001
C------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
C      REAL*4 ambos,amfer,chii,chio
C
c     PRINT *,'FJ mbos mfer chii chio :',ambos,amfer,chii,chio
      fj = fi(amfer,ambos,chii,chio)
      RETURN
      END
      DOUBLE PRECISION FUNCTION FK( ambos , amfer , chii , chio )
C------------------------------------------------------------------
C! Function used in the determination of chi' --> chi gamma
C
C V. Bertin for CHA001
C------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
C      REAL*4 ambos,amfer,chii,chio
C
c     PRINT *,'FK mbos mfer chii chio :',ambos,amfer,chii,chio
      fk = (1.
     .    + amfer**2 * FI(ambos,amfer,chii,chio)
     .    + ambos**2 * FJ(ambos,amfer,chii,chio)
     .    - chii**2 * FI2(ambos,amfer,chii,chio))
     .   / (chio**2 - chii**2)
      RETURN
      END
      SUBROUTINE fsub2(m,u2,f2,x)
C--------------------------------------------------------------------
C! Three routines for the hZ/hA cross sections including h/A widths
C
C  Patrick Janot -- 01 Sep 1995
C--------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON /BWC/ s,am1,am2,w1,w2,v1,v2
      EXTERNAL fsub1
      DIMENSION u2(*),f2(*),x(2)
      DO l = 1,m
        x(2)  = u2(l)
        am2W2 = am2**2+am2*w2*tan(x(2))
        am2w  = DSQRT(DMAX1(0D0,am2W2))
        x1lo  = -DATAN2(am1,w1)
        x1hi  =  DATAN2((DSQRT(s)-am2W)**2-am1**2,am1*w1)
        IF ( x1lo .LT. x1hi ) THEN
          f2(l) = DGMLT1(fsub1,x1lo,x1hi,1,6,x)
        ELSE
          f2(l) = 0.
        ENDIF
      ENDDO
      RETURN
      END
      SUBROUTINE fsub1(m,u1,f1,x)
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON /BWC/ s,am1,am2,w1,w2,v1,v2
      DIMENSION u1(*),f1(*),x(2)
      DO l = 1,m
        x(1) = u1(l)
        u = (am1**2+am1*w1*DTAN(x(1)))/s
        v = (am2**2+am2*w2*DTAN(x(2)))/s
        ss = DMAX1(1.+u*u+v*v-2.*(u*v+u+v),0D0)
        f1(l) = ss*DSQRT(ss)
      ENDDO
      RETURN
      END
      SUBROUTINE fsub(m,u1,f1,x)
      IMPLICIT REAL*8(A-H,O-Z)
C      REAL*4 sigklei,u,ss
      COMMON /BHV/ s,am1,w1
      DIMENSION u1(*),f1(*),x(1)
      ss = s
      DO l = 1,m
        x(1) = u1(l)
        u = DSQRT(am1**2+am1*w1*DTAN(x(1)))
        f1(l) = sigklei(u,ss)
      ENDDO
      RETURN
      END
      SUBROUTINE fsubcha(m,u1,f1,x)
C--------------------------------------------------------------------
C! A routine for the chi+ decays into W* chi
C
C  Patrick Janot -- 23 Sep 1995
C--------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON / chachi / a,g,xx,xip,clr2,cltr
      DIMENSION u1(*),f1(*),x(1)
      DO l = 1,m
        x(1) = u1(l)
        u = (a/xip)**2+a/xip*g/xip*TAN(x(1))
        part1 = -clr2 * u**2
     .        + .5 *((1. + xx**2)* clr2 - 12.* cltr * xx)* u
     .        + .5 *(1.-xx**2)**2 * clr2
        part2 = u**2 - 2.*(1.+xx**2)*u + (1.-xx**2)**2
        f1(l) = part1 * DSQRT(part2)
      ENDDO
      RETURN
      END
      SUBROUTINE fsubgz(m,u1,f1,x)
C--------------------------------------------------------------------
C! A routine for the h --> Z gamma decay including Z width
C
C  Patrick Janot -- 23 Sep 1995
C--------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON /BHV/ s,am1,w1
      DIMENSION u1(*),f1(*),x(1)
      DO l = 1,m
        x(1) = u1(l)
        u = am1**2+am1*w1*TAN(x(1))
        f1(l) = (1.-u/s)**3
      ENDDO
      RETURN
      END
      SUBROUTINE fsubint3(m,u3,f3,x)
C--------------------------------------------------------------------
C  Interference WW/ZZ fusion with Higgs-strahlung. Second integration
C                                                     (Z mass)
C
C  Patrick Janot -- 7 Nov 1999 
C--------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
      include 'param.inc'
      DIMENSION u3(*),f3(*),x(3)
      REAL*8 rmh, rgh, rmv, ss
      COMMON / sigint / rmh, rgh, rmv, ss
      EXTERNAL fsubint2, dgmlt2
C
      DO l = 1, m
        x(3) = u3(l)      
        xmh  = DSQRT(rmh**2+rmh*rgh*DTAN(x(3)))
        x2lo = -amz/gmz
        x2hi =  ((SQRT(ss)-xmh)**2-amz**2)/(amz*gmz)
C
        x2lo = -LOG(1D0+x2lo**2)
        IF ( x2hi .LT. 0D0 ) THEN 
          x2hi  = -LOG(1D0+x2hi**2)
          f3(L) = DGMLT2(fsubint2,x2lo,x2hi,1,6,x)
        ELSE
          x2hi  = +LOG(1D0+x2hi**2)
          f3(L) = DGMLT2(fsubint2,x2lo,0D0,1,6,x)
     &          + DGMLT2(fsubint2,0D0,x2hi,1,6,x)
        ENDIF
C
      ENDDO
C
      RETURN
      END
      
      SUBROUTINE fsubint2(m,u2,f2,x)
C--------------------------------------------------------------------
C  Interference WW/ZZ fusion with Higgs-strahlung. First integration
C                                                  (Higgs direction)
C
C  Marumi Kado -- Jan. 1998 
C--------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
      EXTERNAL fsubint1, dgmlt1
      DIMENSION u2(*),f2(*),x(3)
C
      DO l = 1, m
        x(2) = u2(l)
        x1lo = -1D0
        x1hi = +1D0
        f2(L)=DGMLT1(fsubint1,x1lo,x1hi,1,6,x)
      ENDDO
C
      RETURN
      END

      SUBROUTINE fsubint1(m,u1,f1,x)
C--------------------------------------------------------------------
C  Interference WW/ZZ fusion with Higgs-strahlung. The function
C
C  Marumi Kado -- Jan 1998 
C--------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
      include 'param.inc'
      DIMENSION u1(*),f1(*),x(3)
      REAL*8 rmh, rgh, rmv, ss
      COMMON / sigint / rmh, rgh, rmv, ss
C
      DO l = 1,m
        x(1)=u1(l)
        xmh = DSQRT(rmh**2+rmh*rgh*DTAN(x(3)))
        IF     ( x(2) .LT. 0D0 ) THEN 
          u = -SQRT(EXP(-x(2))-1D0)
        ELSEIF ( x(2) .GT. 0D0 ) THEN 
          u = +SQRT(EXP(+x(2))-1D0)
        ELSE
          u = 0D0
        ENDIF
        ctet = x(1)
C
        ecm = SQRT(ss)
        snu = amz**2 + amz*gmz*u
        eh  = (ss+xmh**2-snu)/(2D0*ecm)
        enu = ecm - eh
        ph  = sqrt(eh**2-xmh**2)
C     
        s1 = ecm*(enu+ph*ctet)
        s2 = ecm*(enu-ph*ctet)
C
        cchi =  1D0 - 2D0*ss*snu/(s1*s2)
        schi2 = 1D0 - cchi**2
C
        h1 = 1D0 + 2D0*rmv**2/s1
        h2 = 1D0 + 2D0*rmv**2/s2
        t1 = h1 + cchi*h2
        t2 = h2 + cchi*h1
        r  = h1**2 + h2**2 + 2D0*cchi*h1*h2 - schi2
        xl = LOG ( ( h1*h2 + cchi + SQRT(r) )
     &           / ( h1*h2 + cchi - SQRT(r) ) )
C
        gi = ph
     &     * (ss-amz**2) / ( (ss-amz**2)**2 + amz**2*gmz**2 )
     &     * ( 2D0
     &       - (h1+1D0) * LOG((h1+1D0)/(h1-1D0))
     &       - (h2+1D0) * LOG((h2+1D0)/(h2-1D0))
     &       + (h1+1D0) * (h2+1D0) * xl / SQRT(r) )
C
        IF     ( x(2) .LT. 0D0 ) THEN 
          f1(l) = -gi
       ELSEIF ( x(2) .GT. 0D0 ) THEN 
          f1(l) = +gi
        ELSE
          f1(l) = 0D0
        ENDIF
C
      ENDDO
C
      RETURN
      END
      SUBROUTINE fsubwz2(m,u2,f2,x)
C--------------------------------------------------------------------
C! Two routines for the h-->WW/ZZ decays including W/Z widths
C
C  Patrick Janot -- 15 Sep 1995
C--------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON /BWC/ s,am1,am2,wd1,wd2,vg1,vg2
      EXTERNAL fsubwz1
      DIMENSION u2(*),f2(*),x(2)
      DO l = 1,m
        x(2)  = u2(l)
        am2W2 = am2**2+am2*wd2*DTAN(x(2))
        x1lo  = -DATAN2(am1,wd1)
        x1hi  =  DATAN2((SQRT(s)-SQRT(am2W2))**2-am1**2,am1*wd1)
        f2(l) = DGMLT1(fsubwz1,x1lo,x1hi,1,6,x)
      ENDDO
C
      RETURN
      END

      SUBROUTINE fsubwz1(m,u1,f1,x)
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON /BWC/ s,am1,am2,wd1,wd2,vg1,vg2
      DIMENSION u1(*),f1(*),x(2)
      DO l = 1,m
        x(1) = u1(l)
        u = (am1**2+am1*wd1*DTAN(x(1)))
        v = (am2**2+am2*wd2*DTAN(x(2)))
        ss = MAX((s-u-v)**2-4.*u*v,0D0)
C        f1(l) =  DSQRT(ss) * (u*v+ss/12.)
C
C Anomalous couplings:
C
        f1(l) = DSQRT(ss) * ( 
     &           2D0
     &        * (2D0*u + 2D0*vg2*(u+v-s) - vg1*s)
     &        * (2D0*v + 2D0*vg2*(u+v-s) - vg1*s)
     &        + (s - 2D0*(u+v)*(5D-1+vg2)-vg1*s)**2
     &        )
     &        / 12D0
      ENDDO
C
      RETURN
      END
      SUBROUTINE fsubzst(m,u1,f1,x)
C--------------------------------------------------------------------
C! A routine for the chi' decays into Z* chi
C
C  Patrick Janot -- 23 Sep 1995
C--------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
      COMMON / chachi / a,g,xx,xip,clr2,cltr
      DIMENSION u1(*),f1(*),x(1)
      DO l = 1,m
        x(1) = u1(l)
        u = (a/xip)**2+a/xip*g/xip*TAN(x(1))
        part1 = -1.*u**2
     .        + .5*(1.+6.*xx+xx**2)*u
     .        + .5*(1.-xx**2)**2
        part2 = u**2
     .        - 2.*(1.+xx**2)*u
     .        + (1.-xx**2)**2
        f1(l) = part1 * DSQRT(part2)
      ENDDO
      RETURN
      END
      FUNCTION ftau(tau)
C------------------------------------------------------------------
C! Famous function appearing in H --> gamma gamma partial widths
C
C  Patrick Janot -- 03 oct 1991
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'datapi.inc'
      COMPLEX*16 ftau
C
      IF ( tau .GE. 1D0 ) THEN
        ftaur = (DASIN(SQRT(1./tau)))**2
        ftaui = 0.
      ELSEIF ( tau .GE. 1D-5 ) THEN
        b0 = SQRT(1.-tau)
        ftaur = (-(LOG((1.-b0)/(1+b0)))**2 +pi2)/4.
        ftaui = pi/2.*LOG((1.+b0)/(1.-b0))
      ELSE
        ftaur = (-LOG(tau/4.)+pi2)/4.
        ftaui = pi/2.*LOG(4./tau)
      ENDIF
C
      ftau = DCMPLX(ftaur,ftaui)
C
      RETURN
      END
      SUBROUTINE fvonz(xreal,ximag,f1)
C------------------------------------------------------------------
C! Function used in the determination of chi' --> chi gamma
C
C V. Bertin for CHA001
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      REAL*8 xreal,ximag,f,pi
      COMPLEX*16 f1,z,g,a,f3,f4
C
      pi = 3.1415926535897932
CVb   PRINT *,'FVONZ : xreal ximag :',xreal,ximag
C--Mitchell 7.1 und 7.2
      f4 = 1.
      f3 = 0.
      z = DCMPLX(xreal,ximag)
Cvb   PRINT *,'z cdbas(z) :',z,CDABS(z)
      IF ((CDABS(z)) .GT. 1.d0) THEN
C--Michell 4.1
       f3 = -pi*pi/6.d0-.5*CDLOG(-z)*CDLOG(-z)
       f4 = -1.
       z = 1./z
      ENDIF
Cvb   PRINT *,'f3 f4 z :',f3,f4,z
      n = 1
      g = 0.
      a = .25*z
10    g = g+a
      a = a*z*n*n/DBLE(n+2)/DBLE(n+2)
      IF((CDABS(a)) .LT. .0000001) GOTO 20
      n = n+1
      GOTO 10
20    f1 = (z*(3.+g)+2.*(1.-z)*CDLOG(1.-z))/(1.+z)
      f1 = f1*f4+f3
Cvb   PRINT *,'f1 :',f1
      RETURN
      END
      DOUBLE PRECISION FUNCTION gcaren(X,Y)
C--------------------------------------------------------------------
C! gcaren(x,y) = 2. - (x+y)/(x-y)*LOG(x/y)
C
C  20 Sep 1995
C--------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,L,M,O-Z)
      gcaren = 2. - (x+y)/(x-y)*LOG(x/y)
      RETURN
      END
      DOUBLE PRECISION FUNCTION gf1(x,y)
C--------------------------------------------------------------------
C! gf1(x,y) = log(x/y)/(x-y)
C
C  3 Dec 1994
C--------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      u = x/y
      e = u-1.
      IF ( ABS(e) .GT. 1D-6 ) THEN
        gf1 = log(u)/(u-1) /y
      ELSE
        gf1 = (1.-e/2.+e**2/3.) /y
      ENDIF
      END
      DOUBLE PRECISION FUNCTION gf2(x,y)
C--------------------------------------------------------------------
C! gf2 = (2-(x+y)/(x-y)*log(x/y))/(x-y)**2
C
C  3 Dec 1994
C--------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      u = x/y
      e = u-1.
      IF ( ABS(e) .GT. 1D-4 ) THEN
        gf2 = (2.-(u+1)/(u-1)*log(u))/(u-1)**2 /y**2
      ELSE
        gf2 = -(1.-e)/6. /y**2
      ENDIF
      END
      DOUBLE PRECISION FUNCTION gf3(x,y,z)
C--------------------------------------------------------------------
C! gf3(x,y) = [x.gf1(x,z) - y.gf1(y,z)]/(x-y)
C
C  3 Dec 1994
C--------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      u = x/y
      e = u-1.
      IF ( ABS(e) .GT. 1D-6 ) THEN
       gf3 = (u*gf1(x,z)-gf1(y,z))/(u-1.)
      ELSE
        v = x/z
        f = v-1.
        IF ( ABS(f) .GT. 1D-6 ) THEN
          gf3 = (1.-z*gf1(x,z))/(x-z)
        ELSE
          gf3 = (1./2.-v/3.)/z
        ENDIF
      ENDIF
      END
      DOUBLE PRECISION FUNCTION Gfunc(X,Y)
      implicit real*8 (a-h,m,o-z)
      Gfunc = x + y - 2.*x*y/(x-y)*log(x/y)
      return
      end
      SUBROUTINE GFUNCAR(ma,tanb,mq,mur,md,mtop,At,Ab,mu,mglu,
     &                   vh,stop1,stop2,sbot1,sbot2,deltamt,deltamb)
C--------------------------------------------------------------------
C! Compute D-terms contributing to the Higgs masses
C
C  From M. Carena and C. Wagner, 21 Sep 1995
C--------------------------------------------------------------------
      IMPLICIT REAL*8 (A-H,L,M,O-Z)
      include 'ccaren.inc'
      include 'miscl.inc'
      DIMENSION DIAH(2),VH(2,2),VH1(2,2),VH2(2,2),
     *          vh3t(2,2),vh3b(2,2),
     *          hmix(2,2),al(2,2),m2(2,2)

      if(dabs(mu).lt.0.000001) mu = 0.000001
      mq2 = mq**2
      mur2 = mur**2
      md2 = md**2
      tanbA = tanb
      sinbA = tanbA/(tanbA**2+1.)**.5
      cosbA = sinbA/tanbA

      sinb = tanb/(tanb**2+1.)**.5
      cosb = sinb/tanb
Cpaj  pi = 3.14159
Cpaj  g2 = (0.0336*4.*pi)**.5
Cpaj  g12 = (0.0101*4.*pi)
Cpaj  g1 = g12**.5
Cpaj  mz = 91.18
Cpaj  v = 174.1
Cpaj  mw = (g2**2*v**2/2.)**.5
Cpaj  alpha3 = 0.12/(1.+23/12./pi*0.12*log(mtop**2/mz**2))
Cpaj  mb = 3.
      pi = ppi                 ! paj
      mb = rmbot               ! paj
      alpha1 = alpha_1         ! paj
      alpha2 = alpha_2         ! paj
      alpha3 = alpha_3         ! paj
      g1 = (alpha1*4.*pi)**.5  ! paj
      g2 = (alpha2*4.*pi)**.5  ! paj
      g3 = (alpha3*4.*pi)**.5  ! paj
      g12 = g1**2
      g32 = g3**2
      if(mq.gt.mur) mst = mq
      if(mur.gt.mq.or.mur.eq.mq) mst = mur

      msusyt = (mst**2  + mtop**2)**.5

      if(mq.gt.md) msb = mq
      if(md.gt.mq.or.md.eq.mq) msb = md

      msusyb = (msb**2 + mb**2)**.5

      tt = log(msusyt**2/mtop**2)
      tb = log(msusyb**2/mtop**2)

Cpaj  rmtop = mtop/(1.+4.*alpha3/3./pi)
Cpaj  ht = rmtop/(174.*sinb)
Cpaj  htst = rmtop/174.1
Cpaj  hb = mb/174./cosb
Cpaj  g32 = alpha3*4.*pi
      ht = rmtop/( v  *sinb)   ! paj
      htst = rmtop/ v          ! paj
      hb = mb/ v  /cosb        ! paj
      bt2 = -(8.*g32 - 9.*ht**2/2. - hb**2/2.)/(4.*pi)**2
      bb2 = -(8.*g32 - 9.*hb**2/2. - ht**2/2.)/(4.*pi)**2
      al2 = 3./8./pi**2*ht**2
      bt2st = -(8.*g32 - 9.*htst**2/2.)/(4.*pi)**2
      alst = 3./8./pi**2*htst**2
      al1 = 3./8./pi**2*hb**2

      al(1,1) = al1
      al(1,2) = (al2+al1)/2.
      al(2,1) = (al2+al1)/2.
      al(2,2) = al2

      mtop4 = rmtop**4.*(1.+2.*bt2*tt- al2*tt)
      mtop2 = mtop4**.5
      mbot4 = mb**4.*(1.+2.*bb2*tb - al1*tb)
      mbot2 = mbot4**.5
      mbot  = mbot2**.5
C
Cpaj  vi = 174.1*(1. + 3./32./pi**2*htst**2*
Cpaj
C
C A few iterations to make deltamb and deltamb converge (PaJ - 10/12/99)
C
      niter = 5
      DO iter = 1, niter
C
        IF ( ma .LE. mtop ) THEN
          mm = mtop
        ELSE
          mm = ma
        ENDIF
Cpaj
        vi =   v  *(1. + 3./32./pi**2*htst**2*
     *       log(mtop**2/mm**2))
        h1i = vi* cosbA
        h2i = vi*sinbA
        h1t = h1i*(1.+3./8./pi**2*hb**2*log(mm**2/msusyt**2))**.25
        h2t = h2i*(1.+3./8./pi**2*ht**2*log(mm**2/msusyt**2))**.25
        h1b = h1i*(1.+3./8./pi**2*hb**2*log(mm**2/msusyb**2))**.25
        h2b = h2i*(1.+3./8./pi**2*ht**2*log(mm**2/msusyb**2))**.25

        tanbst = h2t/h1t
        sinbt = tanbst/(1.+tanbst**2)**.5
        cosbt = sinbt/tanbst
        
        tanbsb = h2b/h1b
        sinbb = tanbsb/(1.+tanbsb**2)**.5
        cosbb = sinbb/tanbsb
C
        stop12 = (mq2 + mur2)*.5 + mtop2
     *       +1./8.*(g2**2+g1**2)*(h1t**2-h2t**2)
     *       +(((g2**2-5.*g1**2/3.)/4.*(h1t**2-h2t**2) +
     *       mq2 - mur2)**2*0.25 + mtop2*(At-mu/tanbst)**2)**.5
        stop22 = (mq2 + mur2)*.5 + mtop2
     *       +1./8.*(g2**2+g1**2)*(h1t**2-h2t**2)
     *       - (((g2**2-5.*g1**2/3.)/4.*(h1t**2-h2t**2) +
     *       mq2 - mur2)**2*0.25
     *       + mtop2*(At-mu/tanbst)**2)**.5
Cpaj-------------------------------------------------------------
C Compute also the mixing angle for later use
        cosmix = -((g2**2-5.*g1**2/3.)/4.*(h1t**2-h2t**2)+mq2-mur2)
        sinmix = -2.*mtop*(At-mu/tanbst)
        tmix   = ppi/8.
        IF ( cosmix .NE. 0D0 .OR. sinmix .NE. 0D0 )
     &       tmix = DATAN2(sinmix,cosmix)/2.
Cpaj-------------------------------------------------------------
        stop1 = stop12**.5
        if(stop22.lt.0.) THEN 
          stop2 = -(ABS(stop22))**.5
          goto 4237
        ELSE
          stop2 = stop22**.5
        ENDIF
C
        sbot12 = (mq2 + md2)*.5
     *       - 1./8.*(g2**2+g1**2)*(h1b**2-h2b**2)
     *       + (((g1**2/3.-g2**2)/4.*(h1b**2-h2b**2) +
     *       mq2 - md2)**2*0.25 + mbot2*(Ab-mu*tanbsb)**2)**.5
        sbot22 = (mq2 + md2)*.5
     *       - 1./8.*(g2**2+g1**2)*(h1b**2-h2b**2)
     *       - (((g1**2/3.-g2**2)/4.*(h1b**2-h2b**2) +
     *       mq2 - md2)**2*0.25 + mbot2*(Ab-mu*tanbsb)**2)**.5
Cpaj-------------------------------------------------------------
C Compute also the mixing angle for later use
        cosmix = -((g1**2/3.-g2**2)/4.*(h1b**2-h2b**2)+mq2-md2)
        sinmix = -2.*mbot*(Ab-mu*tanbsb)
        bmix   = ppi/8.
        IF ( cosmix .NE. 0D0 .OR. sinmix .NE. 0D0 )
     &       bmix = DATAN2(sinmix,cosmix)/2.
Cpaj-------------------------------------------------------------
        sbot1 = sbot12**.5
        if(sbot22.lt.0.) THEN 
          sbot2 = -(ABS(sbot22))**.5
          goto 4237
        ELSE
          sbot2 = sbot22**.5
        ENDIF
C
Cccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c 11/11/99
c
c     Here is the definition of deltamb and deltamt, which
c     are the vertex corrections to the bottom and top quark
c     mass, keeping the dominant QCD and top Yukawa coupling
c     induced corrections.
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c        
        IF ( icar .EQ. 2 ) THEN 
C
          deltamb = -2.*alpha3/3./pi*mglu*(ab-mu*tanb)*
     *               TCaren(sbot1,sbot2,mglu)
     *              + ht**2/(4.*pi)**2*(at-mu/tanb)*mu*tanb*
     *               TCaren(stop1,stop2,mu)
C          
          deltamt = -2.*alpha3/3./pi*(at-mu/tanb)*mglu*
     *               TCaren(stop1,stop2,mglu)
C
        ELSE
C
          deltamb = 0D0
          deltamt = 0D0
C
        ENDIF
        
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c 11/11/99
c
c   Here the new values of the top and bottom quark masses at
c   the scale MS are defined, to be used in the effective 
c   potential approximation. They are just the old ones, but
c   including the finite corrections deltamt and deltamb.
c   The deltamb corrections can become large and are resummed
c   to all orders, as suggested in the two recent works by M. Carena, 
c   S. Mrenna and C.E.M. Wagner, as well as in the work by M. Carena,
c   D. Garcia, U. Nierste and C.E.M. Wagner, to appear. The top
c   quark mass corrections are small and are kept in the perturbative
c   formulation.  The function TCaren(X,Y,Z) is necessary for the 
c   calculation. The entries are masses and NOT their squares !
c
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
C
        mtop4 = rmtop**4.*(1.+2.*bt2*tt- al2*tt - 4.*deltamt)
c     * /(1.+deltamt)**4.
        mtop2 = mtop4**.5
	mbot4 = mb**4.*(1.+2.*bb2*tb - al1*tb)
     *       /(1.+deltamb)**4.
	mbot2 = mbot4**.5
	mbot  = mbot2**.5
C
      ENDDO
C
C For consistency in the stop and sbottom mass computation:
C
      rmtop = rmtop * (1D0 - deltamt)
      rmbot = rmbot / (1D0 + deltamb)
C
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

      vh1(1,1) = 1./tanbst
      vh1(2,1) = -1.
      vh1(1,2) = -1.
      vh1(2,2) = tanbst
      vh2(1,1) = tanbst
      vh2(1,2) = -1.
      vh2(2,1) = -1.
      vh2(2,2) = 1./tanbst
cccccccccccccccccccccccccccccccc
ccc   D-terms
cccccccccccccccccccccccccccccccc
Cpaj  stw=.2315

      f1t=(mq2-mur2)/(stop12-stop22)*(.5-4./3.*stw)*
     *         log(stop1/stop2)
     *        +(.5-2./3.*stw)*log(stop1*stop2/(mq2+mtop2))
     *        + 2./3.*stw*log(stop1*stop2/(mur2+mtop2))

      f1b=(mq2-md2)/(sbot12-sbot22)*(-.5+2./3.*stw)*
     *        log(sbot1/sbot2)
     *        +(-.5+1./3.*stw)*log(sbot1*sbot2/(mq2+mbot2))
     *        - 1./3.*stw*log(sbot1*sbot2/(md2+mbot2))

      f2t=mtop2**.5*(at-mu/tanbst)/(stop12-stop22)*
     *         (-.5*log(stop12/stop22)
     *        +(4./3.*stw-.5)*(mq2-mur2)/(stop12-stop22)*
     *         gcaren(stop12,stop22))

      f2b=mbot2**.5*(ab-mu*tanbsb)/(sbot12-sbot22)*
     *         (.5*log(sbot12/sbot22)
     *        +(-2./3.*stw+.5)*(mq2-md2)/(sbot12-sbot22)*
     *        gcaren(sbot12,sbot22))

      vh3b(1,1) = mbot4/(cosbb**2)*(log(sbot1**2*sbot2**2/
     *  (mq2+mbot2)/(md2+mbot2))
     *  + 2.*(Ab*(Ab-mu*tanbsb)/(sbot1**2-sbot2**2))*
     *  log(sbot1**2/sbot2**2)) +
     *  Mbot4/(cosbb**2)*(Ab*(Ab-mu*tanbsb)/
     *  (sbot1**2-sbot2**2))**2*gcaren(sbot12,sbot22)

      vh3t(1,1) =
     *  mtop4/(sinbt**2)*(mu*(-At+mu/tanbst)/(stop1**2
     * -stop2**2))**2*gcaren(stop12,stop22)

      vh3b(1,1)=vh3b(1,1)+
     *    mz**2*(2*mbot2*f1b-mbot2**.5*ab*f2b)

      vh3t(1,1) = vh3t(1,1) +
     *  mz**2*(mtop2**.5*mu/tanbst*f2t)

      vh3t(2,2) = mtop4/(sinbt**2)*(log(stop1**2*stop2**2/
     *  (mq2+mtop2)/(mur2+mtop2))
     *  + 2.*(At*(At-mu/tanbst)/(stop1**2-stop2**2))*
     *  log(stop1**2/stop2**2)) +
     *  mtop4/(sinbt**2)*(At*(At-mu/tanbst)/
     *  (stop1**2-stop2**2))**2*gcaren(stop12,stop22)

      vh3b(2,2) =
     *  Mbot4/(cosbb**2)*(mu*(-Ab+mu*tanbsb)/(sbot1**2
     * -sbot2**2))**2*gcaren(sbot12,sbot22)

      vh3t(2,2)=vh3t(2,2)+
     *    mz**2*(-2*mtop2*f1t+mtop2**.5*at*f2t)

      vh3b(2,2) = vh3b(2,2) -mz**2*mbot2**.5*mu*tanbsb*f2b

      vh3t(1,2) = -
     *   mtop4/(sinbt**2)*mu*(At-mu/tanbst)/
     * (stop1**2-stop2**2)*(log(stop1**2/stop2**2) + At*
     * (At - mu/tanbst)/(stop1**2-stop2**2)*gcaren(stop12,stop22))

      vh3b(1,2) =
     * - mbot4/(cosbb**2)*mu*(At-mu*tanbsb)/
     * (sbot1**2-sbot2**2)*(log(sbot1**2/sbot2**2) + Ab*
     * (Ab - mu*tanbsb)/(sbot1**2-sbot2**2)*gcaren(sbot12,sbot22))

      vh3t(1,2)=vh3t(1,2) +
     *      mz**2*(mtop2/tanbst*f1t-mtop2**.5*(at/tanbst+mu)/2.*f2t)

      vh3b(1,2)=vh3b(1,2)
     *  +mz**2*(-mbot2*tanbsb*f1b+mbot2**.5*(ab*tanbsb+mu)/2.*f2b)

      vh3t(2,1) = vh3t(1,2)
      vh3b(2,1) = vh3b(1,2)

      tq = log((mq2 + mtop2)/mtop2)
      tu = log((mur2+mtop2)/mtop2)
      tqd = log((mq2 + mb**2)/mb**2)
      td = log((md2+mb**2)/mb**2)


      DO 8910 I = 1,2
      DO 8911 J = 1,2
        vh(i,j) =
     *  6./(8.*pi**2*(h1t**2+h2t**2))
     *  *vh3t(i,j)*0.5*(1.-al(i,j)*tt/2.) +
     *  6./(8.*pi**2*(h1b**2+h2b**2))
     *  *vh3b(i,j)*0.5*(1.-al(i,j)*tb/2.)
 8911 CONTINUE
 8910 CONTINUE

      GOTO 4236
 4237 DO 6868 I =1,2
      DO 6867 J = 1,2
        vh(i,j) = -1.d+15
 6867 CONTINUE
 6868 CONTINUE
C
 4236 RETURN
      END
      SUBROUTINE gsub3(m,u3,f3,x)
C--------------------------------------------------------------------
C! Three routines for the WW/ZZ-->h cross section including h width
C
C  Patrick Janot -- 01 Sep 1995
C--------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,K,O-Z)
      COMMON / sigww / kappah, kappav, v, a, rmh, rgh, ss
      EXTERNAL gsub2
      DIMENSION u3(*),f3(*),x(3)
C
      DO l = 1,m
        x(3)  = u3(l)
        u = DSQRT(rmh**2+rmh*rgh*DTAN(x(3)))
        kappah = u**2/ss
        x2lo  = kappah
        x2hi  = 1.
        f3(l) = DGMLT2(gsub2,x2lo,x2hi,1,6,x)
      ENDDO
C
      RETURN
      END
      SUBROUTINE gsub2(m,u2,f2,x)
      IMPLICIT REAL*8(A-H,K,O-Z)
      EXTERNAL gsub1
      DIMENSION  u2(*),f2(*),x(3)
C
      DO l = 1,m
        x(2)  = u2(l)
        x1lo  = 1.
        x1hi  = 1./x(2)**2
        f2(l) = DGMLT1(gsub1,x1lo,x1hi,1,6,x)
      ENDDO
C
      RETURN
      END
      SUBROUTINE gsub1(m,u1,f1,x)
      IMPLICIT REAL*8(A-H,K,O-Z)
      COMMON / sigww / kappah, kappav, v, a, rmh, rgh, ss
      DIMENSION u1(*),f1(*),x(3)
C
      DO l = 1,m
        x(1) = u1(l)
        y = 1./DSQRT(x(1))
        z = y * (x(2)-kappah) / (kappav * x(2))
        f = ( 2.*x(2)/y**3 - (1.+2.*x(2))/y**2
     .     + (2.+x(2))/(2.*y) - 1./2. )
     .    * ( z/(1.+z) - DLOG(1.+z) )
     .    + x(2)*z**2*(1.-y)/(y**3*(1.+z))
        g = (-x(2)/y**2 + (2.+x(2))/(2.*y) - 1./2.)
     .    * ( z/(1.+z) - DLOG(1.+z) )
        f1(l) = ( (v**2+a**2)**2 * f + 4.*v**2*a**2 * g )
     .        / ( 1. + (y-x(2))/kappav )**2
     .        * y**3 / 2.
      ENDDO
C
      RETURN
      END
      FUNCTION gtau(tau)
C------------------------------------------------------------------
C! Famous function appearing in H --> Z gamma partial width
C
C  Patrick Janot -- 23 Sep 1995
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'datapi.inc'
      COMPLEX*16 gtau
C
      IF ( tau .GE. 1 ) THEN
        gtaur = SQRT(tau-1.) * DASIN(SQRT(1./tau))
        gtaui = 0.
      ELSEIF ( tau .GE. 1E-5 ) THEN
        b0 = SQRT(1.-tau)
        gtaur = b0 * LOG((1.+b0)/(1-b0)) / 2.
        gtaui = -pi * b0 / 2.
      ELSE
        gtaur = -LOG(tau/4.) / 2.
        gtaui = -pi
      ENDIF
C
      gtau = DCMPLX(gtaur,gtaui)
C
      RETURN
      END
      FUNCTION gzi1(jhig,am)
C--------------------------------------------------------------------
C! Utility routine to compute the Z gamma width
C
C Input:    jhig,   Higgs type
C           am  ,   the mass of the the loop particle
C
C Output:   gzi1,   the value of the function
C
C Patrick Janot -- 23 sep 1995
C
C! Modifications: 
C
C     o 01/11/99 : Take care of the mH=mZ singularity - P. Janot
C                  (Double Precision helped a lot. No need for 
C                  a taylor expansion)
C--------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      COMPLEX*16 ftau, gtau, gzi1
      EXTERNAL ftau, gtau
C
      a = 4.*am**2/amhig(jhig)**2
      b = 4.*am**2/amz**2
      IF ( a .LE. b .AND. (b-a)/b .LT. 1D-6 ) a = b * 0.999999
      IF ( a .GT. b .AND. (a-b)/b .LT. 1D-6 ) a = b * 1.000001
C
      gzi1 = a*b       / 2. / (a-b)
     .     + a**2*b**2 / 2. / (a-b)**2 * (ftau(a)-ftau(b))
     .     + a**2*b         / (a-b)**2 * (gtau(a)-gtau(b))
C
      RETURN
      END



      FUNCTION gzi2(jhig,am)
C--------------------------------------------------------------------
C! Utility routine to compute the Z gamma width
C
C Input:    jhig,   Higgs type
C           am  ,   the mass of the the loop particle
C
C Output:   gzi2,   the value of the function
C
C Patrick Janot -- 23 sep 1995
C
C! Modifications: 
C
C     o 01/11/99 : Take care of the mH=mZ singularity - P. Janot
C                  (Double Precision helped a lot. No need for 
C                  a taylor expansion)
C--------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      COMPLEX*16 ftau, gtau, gzi2
      EXTERNAL ftau, gtau
C
      a = 4.*am**2/amhig(jhig)**2
      b = 4.*am**2/amz**2
      IF ( a .LE. b .AND. (b-a)/b .LT. 1D-6 ) a = b * 0.999999
      IF ( a .GT. b .AND. (a-b)/b .LT. 1D-6 ) a = b * 1.000001
C
      gzi2 = -a*b       / 2. / (a-b)    * (ftau(a)-ftau(b))
C
      RETURN
      END
C  PROGRAM HMSUSY
C      WRITTEN BY HOWARD E. HABER
C      LATEST REVISION: AUGUST 23, 1995
C      COMMENTS OR QUESTIONS: SEND E-MAIL TO HABER@SCIPP.UCSC.EDU
C      BASED ON WORK IN COLLABORATION WITH R. HEMPFLING AND A. HOANG
C

C
C   IRC=0 TREE LEVEL ONLY
C   IRC=1 RADIATIVE CORRECTIONS INCLUDED WITH RGE IMPROVEMENT
C   IRC=2 SLIGHT IMPROVEMENT OF TREATMENT OF STOP-SBOTTOM SECTOR
C
C
C   MU PARAMETER SET TO SIGNMU*SUSY
C   IN PRINCIPLE, CAN CHOOSE ANY MU, ALTHOUGH IF DABS(SIGNMU) IS
C         NOT 1, THEN CONTRIBUTIONS OF GAUGINOS AND HIGGSINOS
C         TO THE RADIATIVE CORRECTIONS MUST BE MODIFIED (SLIGHTLY)
C
C
C   SQUARK MASS PARAMETERS SET IN SETUP SUBROUTINE
C   PARAMETERS DEPOSITED IN COMMON/SQPARM/
C      SLM= COMMON SLEPTON MASS
C      SQM= COMMON SQUARK/SLEPTON MASS (APART FROM B AND T SECTORS)
C      SQK= SQUARK SU(2)-DOUBLET SOFT-SUSY-BREAKING MASS
C      SQU= TOP-SQUARK SU(2)-SINGLET SOFT-SUSY-BREAKING MASS
C      SQD= BOTTOM-SQUARK SU(2)-SINGLET SOFT-SUSY-BREAKING MASS
C      SUSY= COMMON GAUGINO/HIGGSINO MASS SCALE
C
C
C   INITIALLY, ALL FIVE SUSY-BREAKING MASSES SET EQUAL.
C   THIS CAN BE CHANGED EITHER RIGHT OR DIRECTLY IN THE
C   SETUP SUBROUTINE.
C
C   SUBROUTINE HSUSY COMPUTES HIGGS MASSES AND COUPLINGS
C   IT COMPUTES STOP AND SBOTTOM SPECTRUM FIRST, AND DEPOSITS THE
C   RESULTS IN COMMON/SQPARM/.  OUTPUT IS:
C        STA2,STB2= TOP-SQUARK SQUARED-MASSES
C        SBA2,SBB2= BOTTOM-SQUARK SQUARED-MASSES
C   NEXT IT COMPUTES THE HIGGS MASSES AND PUTS THEM IN THE ARRAY H()
C        H(1)= HEAVY CP-EVEN MASS
C        H(2)= LIGHT CP-EVEN MASS
C        H(3)= CP-ODD MASS (INPUT AS "AM" ABOVE)
C        H(4)= CHARGED HIGGS MASS
C   MIXING ANGLE FACTORS THAT APPEAR IN HIGGS FEYNMAN RULES
C   DEPOSITED IN  COMMON/ANG/.  TANB IS INPUT, WHILE OUTPUT INCLUDES:
C        SINA= SIN(ALPHA) [ALPHA: CP-EVEN HIGGS MIXING ANGLE]
C        COSA= COS(ALPHA)
C        SIN2A= SIN(2*ALPHA)
C        COS2A= COS(2*ALPHA)
C        SINBPA= SIN(BETA+ALPHA)
C        COSBPA= COS(BETA+ALPHA)
C        SINBMA= SIN(BETA-ALPHA)
C        COSBMA= COS(BETA-ALPHA)
C   THREE-HIGGS COUPLING DEPOSITED IN COMMON/HINT3/.  NOTATION:
C        HL= LIGHT CP-EVEN HIGGS
C        HH= HEAVY CP-EVEN HIGGS
C         A= CP-ODD HIGGS
C        HPHM= CHARGED HIGGS PAIR
C   FOR EXAMPLE, HLHPHM IS THE FEYNMAN RULE FOR THE INTERACTION OF
C   THE LIGHT CP-EVEN HIGGS WITH A CHARGED HIGGS PAIR (WITHOUT A
C   FACTOR OF I), HLHLHL IS THE THREE LIGHT CP-HIGGS COUPLING, ETC.
C
C   IERR=0 IF THE PROGRAM SUCCEEDS.  IF ONE FINDS A NEGATIVE
C   SQUARED MASS FOR ANY SCALAR (USUALLY, SCALAR-TOP, SCALAR-BOTTOM,
C   THE LIGHT CP-EVEN HIGGS OR THE CHARGED HIGGS), THE PROGRAM
C   RETURNS IERR>0.
C
C
C   SLIGHT INACCURACIES OF THE PROGRAM:
C      1. IRC=2 SLIGHTLY IMPROVES NEUTRAL SCALAR MASSES, BUT
C            NO IMPROVEMENT YET FOR CHARGED SCALAR MASSES.
C      2. LEADING LOGS FOR CHARGED SCALAR MASS CORRECT ONLY
C            IF M(A)=O(ZMASS).
C      3. LEADING LOGS FOR HEAVY CP-EVEN SCALAR MASS CORRECT ONLY
C            IF M(A)=O(ZMASS).
C   IN CASES 2 AND 3, THE RELATIVE ERROR MADE IS RATHER MINOR,
C   SINCE HEAVY SCALARS ARE ROUGHLY DEGENERATE WITH THE CP-ODD SCALAR.
C      4. NO DETAILED STUDY YET OF ACCURACY OF RGE-IMPROVEMENT VIA
C            THE USE OF MT(Q) FOR WIDELY SPLIT SQK, SQU, AND SQD
C

C
C   IF CHARGED HIGGS MASS SQUARED IS NEGATIVE, PROGRAM RETURNS ZERO
C
      SUBROUTINE HMSUSY(IRC,AMASS,TMASS,TANB,SUSY,AT,AB,H,ALF,IERR)
      IMPLICIT REAL*8 (A-H,O-Z)

c      COMMON /HPARAM/ ZMASS,WMASS,SINW2,ALPHA,BMASS,
c     &                 G2,GP2,SINB,COSB,COTB
c      COMMON/ANG/ SINA,COSA,SIN2A,COS2A,SINBPA,COSBPA,SINBMA,COSBMA
c      COMMON/SQPARM/ SQM,SQK,SQU,SQD,XMU,STA2,STB2,SBA2,SBB2
c      COMMON/QMASS/ TMPOLE
      include 'haber.inc'
      COMMON/HINT3/HLAA,HHAA,HLHLHL,HHHLHL,HHHHHL,HHHHHH,
     1      HHHPHM,HLHPHM
      COMMON/TEST/ D11,D12,D22,DP

      DIMENSION H(4),DL(7)
      DATA PI/3.1415926535D0/, ZERO/0.D0/
      IERR= 0
      STEP= 0.D0
      IF (AMASS .GE. ZMASS) STEP= 1.D0
      Z2= ZMASS**2
      W2= WMASS**2
      A2= AMASS**2
      B2= BMASS**2
      COSW2= 1.D0-SINW2
      COS2W= COSW2-SINW2
      SINW= DSQRT(SINW2)
      COSW= DSQRT(COSW2)
      EU= 2.D0/3.D0
      ED= -1.D0/3.D0
      SINB2= SINB**2
      COSB2= COSB**2
      COS2B= COSB**2-SINB**2
      COS2B2= COS2B**2
      SIN2B= 2.D0*SINB*COSB
      TANB2= TANB**2
      COTB2= COTB**2
      S11= A2*SINB2+Z2*COSB2
      S22= A2*COSB2+Z2*SINB2
      S12= -(A2+Z2)*SINB*COSB
      SPM= A2+W2
      IF (IRC .NE. 0) GO TO 25
      CALL CPEVEN(S11,S22,S12,H1R,H2R,SIN2A,COS2A)
      DPM= SPM
      GO TO 50
C
C   HERE WE ALLOW FOR
C   MIXING OF Q(L) AND Q(R) ONLY FOR THE S-TOP AND S-BOTTOM
C   OTHERWISE, THERE IS NO MIXING.
C
   25 SUSY2= SUSY**2
      SQM2= SQM*SQM
      SLM2= SLM*SLM

C.. ld comment out : stop and sbottom masses calculated in HZHA
c      SQBL2= SQK**2+BMASS**2-Z2*COS2B*(0.5D0+ED*SINW2)
c      SQBR2= SQD**2+BMASS**2+Z2*COS2B*ED*SINW2
c      SQTL2= SQK**2+TMPOLE**2+Z2*COS2B*(0.5D0-EU*SINW2)
c      SQTR2= SQU**2+TMPOLE**2+Z2*COS2B*EU*SINW2
c      XB= AB-XMU*TANB
c      XT= AT-XMU*COTB
c      AB3= -XB*BMASS/(2.D0*WMASS)
c      AT3= -XT*TMPOLE/(2.D0*WMASS)
c      DIFFT= SQTL2-SQTR2
c      DIFFB= SQBL2-SQBR2
c      TMIX= -4.D0*AT3*WMASS
c      BMIX= -4.D0*AB3*WMASS
c      STA2= 0.5D0*(SQTL2+SQTR2+DSQRT(DIFFT**2+TMIX**2))
c      STB2= 0.5D0*(SQTL2+SQTR2-DSQRT(DIFFT**2+TMIX**2))
c      SBA2= 0.5D0*(SQBL2+SQBR2+DSQRT(DIFFB**2+BMIX**2))
c      SBB2= 0.5D0*(SQBL2+SQBR2-DSQRT(DIFFB**2+BMIX**2))
C.. ld
      IF (SBB2 .LT. ZERO) GO TO 291
      IF (STB2 .LT. ZERO) GO TO 292

      SQT1= DSQRT(STA2)
      SQT2= DSQRT(STB2)
      SQB1= DSQRT(SBA2)
      SQB2= DSQRT(SBB2)

      SQTM2= SQT1*SQT2
      SQBM2= SQB1*SQB2
      SQTBM2= DSQRT(SQTM2*SQBM2)
      SMT2= 0.5D0*(SQK**2+SQU**2)
      SMT= DSQRT(SMT2)
      QP= DSQRT(SMT*TMPOLE)
      TMASS1= TMASSR(QP,TMASS)
      IF (IRC .LE. 0) TMASS1= TMASS
      T2= TMASS1**2
      COLOR= 3.D0
      GENS= 3.D0
      PT= COLOR*(1.D0-4.D0*EU*SINW2+8.D0*EU**2*SINW2**2)
      PB= COLOR*(1.D0+4.D0*ED*SINW2+8.D0*ED**2*SINW2**2)
C      PF= (GENS-1.D0)*(PT+PB)+2.D0*GENS*(1.D0-2.D0*SINW2+4.D0*SINW2**2)
      PL= 2.D0*GENS*(1.D0-2.D0*SINW2+4.D0*SINW2**2)
      PQ= (GENS-1.D0)*(PT+PB)
      P22= -2.D0*(32.D0*SINW2**2-54.D0*SINW2+27.D0)
      P12= -2.D0*(8.D0*SINW2**2-6.D0*SINW2-9.D0)
      P2H= -10.D0+2.D0*SINW2-2*SINW2**2
      P2HP= 8.D0-22.D0*SINW2+10.D0*SINW2**2
      P1H= -9.D0*COS2B2**2+(1.D0-2.D0*SINW2+2.D0*SINW2**2)*COS2B2
      PH12= (P1H-P2H)*STEP
      PH12P= (P1H+P2HP)*STEP
      FACT= COLOR*G2/(16.D0*PI**2*W2)
      FACT1= COLOR*G2*T2/(32.D0*PI**2*COSW2*SINB2)
      FACT2= COLOR*G2*B2/(32.D0*PI**2*COSW2*COSB2)
      FACTT= G2*Z2*SINB2/(96.D0*PI**2*COSW2)
      FACBB= G2*Z2*COSB2/(96.D0*PI**2*COSW2)
      FACTBT= G2*Z2/(96.D0*PI**2*COSW2)
      DLOGW= DLOG(SUSY2/W2)
      DLOGZ= DLOG(SUSY2/Z2)
      DLOGL= DLOG(SLM2/Z2)
      DLOGLW= DLOG(SLM2/W2)
      DLOGQ= DLOG(SQM2/Z2)
      DLOGQW= DLOG(SQM2/W2)
      DLOGT= DLOG(SQTM2/T2)
      DLOGTB= DLOG(SQTBM2/T2)
      DLOGB= DLOG(SQBM2/Z2)
      DLOGA= DLOG(A2/Z2)
      IF (IRC .NE. 3) GO TO 30
      DM11= S11
      DM22= S22+FACT*(2.D0*T2**2/SINB2)*DLOGT
      DM12= S12
      DHZPM= 0.D0
      GO TO 40
   30 TMASS2= TMASSR(SMT,TMASS)
C
C   DNL22:   MT**2 NON-LEADING LOG PIECE
C
      DNL22 = FACT*T2*Z2/(3.D0*SINB2)
      IF (IRC .LE. 0) TMASS2= TMASS
      CALL DELUD(IRC,TANB,TMASS1,AT,AB,DUD11,DUD22,DUD12,DUDPM)
      CALL DELHM(IRC,TANB,TMASS2,AT,AB,DHZ11,DHZ22,DHZ12,DHZPM)
      DM11= S11+DHZ11+DUD11
     1        +(FACT*(2.D0*B2**2/COSB2-Z2*B2)+FACBB*PB)*DLOGB
C     2        +FACBB*(PT*DLOGT+PF*DLOGF+P22*DLOGZ+PH12*DLOGA)
     2    +FACBB*(PT*DLOGT+PL*DLOGL+PQ*DLOGQ+P22*DLOGZ+PH12*DLOGA)
      DM22= S22+DHZ22+DUD22+DNL22
     1        +(FACT*(2.D0*T2**2/SINB2-Z2*T2)+FACTT*PT)*DLOGT
C     2        +FACTT*(PB*DLOGB+PF*DLOGF+P22*DLOGZ+PH12*DLOGA)
     2    +FACTT*(PB*DLOGB+PL*DLOGL+PQ*DLOGQ+P22*DLOGZ+PH12*DLOGA)
      DM12= S12+DHZ12+DUD12
     1        +SINB*COSB*((FACT1-FACTBT*PT)*DLOGT
C     2        +(FACT2-FACTBT*PB)*DLOGB-FACTBT*(PF*DLOGF+P12*DLOGZ)
     2    +(FACT2-FACTBT*PB)*DLOGB-FACTBT*(PL*DLOGL+PQ*DLOGQ+P12*DLOGZ)
     3        +FACTBT*PH12P*DLOGA)
   40 CALL CPEVEN(DM11,DM22,DM12,H1R,H2R,SIN2A,COS2A)
      FACTG= COLOR*G2/(32.D0*PI**2)
      FACTG2= G2/(48.D0*PI**2)
      FACTGP= 5.D0*GP2*W2/(16.D0*PI**2)
      XCG= COLOR*(GENS-1.D0)+GENS
      DPML= SPM+FACTG*(2.D0*T2*B2/(W2*SINB2*COSB2)-T2/SINB2-B2/COSB2
     1         +2.D0*W2/3.D0)*DLOGTB + FACTGP*DLOGW
C     2         +FACTG2*(XCG*DLOGFW-9.D0*DLOGW)
     2         +FACTG2*(XCG*DLOGQW+GENS*DLOGLW-9.D0*DLOGW)
C     CORRQ=-0.5D0*T2*(W2+A2*COSB2-4.D0*B2/COSB2)/(W2*SINB2)
C    1     -(W2*(3.D0-5.D0*SINB2)/9.D0+2.D0*A2*COSB2/3.D0
C    2     -0.5D0*B2*(4.D0-5.D0*SINB2)/COSB2)/SINB2
C    3     -0.5D0*A2*(A2*COSB2/3.D0
C    4         -B2*(4.D0*COSB2+3.D0*SINB2**2)/(2.D0*COSB2))/(W2*SINB2)
C    5     +B2*(A2*SINB2*DLOG(T2/A2)-2.D0*B2*DLOG(T2/B2)/SINB2)
C    6         /(W2*COSB2)
C     CORRSQ=T2**3/(2.D0*W2*SINB2*SMT2)
C    1    +T2**2*(W2*(8.D0-5.D0*SINB2)-Z2*COS2B+A2*COSB2
C    2    -3.D0*B2*(2.D0+SINB2)/COSB2)/(6.D0*W2*SINB2*SMT2)
C     DPMNL= FACTG*(CORRQ+CORRSQ)
C     DPM= DPML+DPMNL+DHZPM
      DPM= DPML+DHZPM
   50 SIGN= 1.D0
      IF (SIN2A .LT. 0.D0) SIGN= -1.D0
      COSA= DSQRT((1.D0+COS2A)/2.D0)
      SINA= SIGN*DSQRT((1.D0-COS2A)/2.D0)
      ALF= SIGN*DACOS(COSA)
      BET= DATAN(TANB)
      SINBPA= DSIN(BET+ALF)
      SINBMA= DSIN(BET-ALF)
      COSBPA= DCOS(BET+ALF)
      COSBMA= DCOS(BET-ALF)
      H(1)= DSQRT(H1R)
C.. ld
C.. put mass**2 in case it is negative so that HZHA can handle that
      H(2) = H2R
C.. ld
      IF (H2R .LT. ZERO) GO TO 293
      H(2)= DSQRT(H2R)
      H(3)= AMASS
      H(4)= 0.D0
C.. ld
C.. put mass**2 in case it is negative so that HZHA can handle that
      H(4) = H2R
C.. ld
      IF (DPM .LT. ZERO) GO TO 294
      H(4)= DSQRT(DPM)
      CALL HLAMBG(IRC,TANB,AMASS,TMASS1,TMASS2,SUSY,AT,AB,DL)
      CALL HIGGS3(IRC,H,DL)
      RETURN
  291 IERR=1
      WRITE(6,991)
      RETURN
  292 IERR=2
      WRITE(6,992)
      RETURN
  293 IERR= 3
      WRITE(6,993)
      RETURN
  294 IERR= 4
      WRITE(6,994)
  991 FORMAT(1X,'ERROR: S-BOTTOM MASS SQUARED EIGENVALUE IS NEGATIVE')
  992 FORMAT(1X,'ERROR: S-TOP MASS SQUARED EIGENVALUE IS NEGATIVE')
  993 FORMAT(1X,'ERROR: LIGHT CP-EVEN SCALAR SQUARED-MASS IS NEGATIVE')
  994 FORMAT(1X,'ERROR: CHARGED HIGGS SQUARED MASS IS NEGATIVE')
      RETURN
      END
      SUBROUTINE CPEVEN(S11,S22,S12,H1,H2,SIN2A,COS2A)
      IMPLICIT REAL*8(A-H,O-Z)
      DISCR= DSQRT((S11-S22)**2+4.D0*S12**2)
      H1= 0.5D0*(S11+S22+DISCR)
      H2= 0.5D0*(S11+S22-DISCR)
      SIN2A= 2.D0*S12/DISCR
      COS2A= (S11-S22)/DISCR
      RETURN
      END
      SUBROUTINE DELHM(IRC,TANB,TMASS,AT,AB,DHZ11,DHZ22,DHZ12,DHZPM)
      IMPLICIT REAL*8(A-H,O-Z)
      include 'haber.inc'
      DATA PI/3.1415926535D0/
      W2= WMASS*WMASS
      Z2= ZMASS*ZMASS
      AT2= AT*AT
      AB2= AB*AB
      XMU2= XMU*XMU
      GSQ= G2+GP2
      EU= 2.D0/3.D0
      ED= -1.D0/3.D0
      COLOR= 3.D0
      CB2= COSB**2
      SB2= SINB**2
      T2= TMASS**2
      B2= BMASS**2
      T4= TMASS**4/SB2
      B4= BMASS**4/CB2
      T2D= TMASS**2/SB2
      B2D= BMASS**2/CB2
      XB= AB-XMU*TANB
      XT= AT-XMU*COTB
      YB= AB+XMU*COTB
      YT= AT+XMU*TANB
      XB2= XB**2
      XT2= XT**2
      Q2= SQK**2
      U2= SQU**2
      D2= SQD**2
      UF= FF(U2,Q2)
      DF= FF(D2,Q2)
      UG= FG(Q2,U2)
      DG= FG(Q2,D2)
      UKP= UF+2.D0*EU*SINW2*(Q2-U2)*UG
      DKP= DF-2.D0*ED*SINW2*(Q2-D2)*DG
      UH= FH(Q2,U2)
      DH= FH(Q2,D2)
      UB= BP(Q2,U2)
      DB= BP(Q2,D2)
      FPBPU= UF+UB
      FPBPD= DF+DB
      FMBPU= UF-UB
      FMBPD= DF-DB
      HUD= HP(Q2,U2,D2)
      FUD= FP(Q2,U2,D2)
      FACT= COLOR*G2/(16.D0*PI**2*W2)
      DHZPM= 0.5D0*FACT*(XMU2*(T2D**2*UF+B2D**2*DF+2.D0*T2D*B2D*HUD)
     1       -T2D*B2D*(AB2*DF+AT2*UF+2.D0*AT*AB*HUD+FUD*(XMU2-AT*AB)**2)
     2       -W2*B2D*(XMU2*FPBPD-AB2*FMBPD)
     3       -W2*T2D*(XMU2*FPBPU-AT2*FMBPU))
      IF (IRC .NE. 2) GO TO 10
      TQ2= STA2
      BQ2= SBA2
      U2= STB2
      D2= SBB2
      UF= FF(U2,TQ2)
      DF= FF(D2,BQ2)
      UG= FG(TQ2,U2)
      DG= FG(BQ2,D2)
      UKP= UF+2.D0*EU*SINW2*(TQ2-U2)*UG
      DKP= DF-2.D0*ED*SINW2*(BQ2-D2)*DG
      UH= FH(TQ2,U2)
      DH= FH(BQ2,D2)
      UB= BP(TQ2,U2)
      DB= BP(BQ2,D2)
   10 DHZ11= FACT*(B4*AB*XB*(2.D0*DH+AB*XB*DG)+T4*XMU2*XT2*UG+
     1       Z2*(B2*AB*(XB*DKP-AB*DB)+T2*XMU*COTB*(XT*UKP-XMU*COTB*UB)))
      DHZ22= FACT*(T4*AT*XT*(2.D0*UH+AT*XT*UG)+B4*XMU2*XB2*DG+
     1       Z2*(T2*AT*(XT*UKP-AT*UB)+B2*XMU*TANB*(XB*DKP-XMU*TANB*DB)))
      DHZ12= -FACT*(B4*XMU*XB*(DH+AB*XB*DG)+T4*XMU*XT*(UH+AT*XT*UG)
     1       -0.5D0*Z2*(T2*COTB*((XMU2+AT2)*UB-XT*YT*UKP)
     2                 +B2*TANB*((XMU2+AB2)*DB-XB*YB*DKP)))
      RETURN
      END
      SUBROUTINE DELUD(IRC,TANB,TMASS,AT,AB,DHZ11,DHZ22,DHZ12,DHZPM)
      IMPLICIT REAL*8(A-H,O-Z)
      include 'haber.inc'
      DATA PI/3.1415926535D0/
      SINB2= SINB**2
      COSB2= COSB**2
      W2= WMASS*WMASS
      Z2= ZMASS*ZMASS
      T2= TMASS**2
      B2= BMASS**2
      Q2= SQK**2
      U2= SQU**2
      D2= SQD**2
      EU= 2.D0/3.D0
      ED= -1.D0/3.D0
      COLOR= 3.D0
      FACT= COLOR*G2*Z2/(16.D0*PI**2*W2)
      FACTG= COLOR*G2/(64.D0*PI**2)
      DHZPM= FACTG*(T2/SINB2+B2/COSB2-2.D0*W2/3.D0)*DLOG(Q2**2/(U2*D2))
      IF (IRC .EQ. 2) GO TO 50
      DHZ11= -FACT*(B2*(0.5D0+2.D0*ED*SINW2)*DLOG(Q2/D2))
      DHZ22= -FACT*(T2*(0.5D0-2.D0*EU*SINW2)*DLOG(Q2/U2))
      DHZ12= 0.5D0*FACT*(B2*TANB*(0.5D0+2.D0*ED*SINW2)*DLOG(Q2/D2)
     1              +T2*COTB*(0.5D0-2.D0*EU*SINW2)*DLOG(Q2/U2))
      RETURN
   50 DHZ11= -FACT*(B2*(0.5D0+2.D0*ED*SINW2)*DLOG(SBA2/SBB2))
      DHZ22= -FACT*(T2*(0.5D0-2.D0*EU*SINW2)*DLOG(STA2/STB2))
      DHZ12= 0.5D0*FACT*(B2*TANB*(0.5D0+2.D0*ED*SINW2)*DLOG(SBA2/SBB2)
     1              +T2*COTB*(0.5D0-2.D0*EU*SINW2)*DLOG(STA2/STB2))
      RETURN
      END
      SUBROUTINE HLAMBG(IRC,TANB,AMASS,TM1,TM2,SUSY,AT,AB,DLT)
      IMPLICIT REAL*8(A-H,O-Z)
      include 'haber.inc'
      COMMON/TEST/ D11,D12,D22,DP
      DIMENSION DL1(7),DL2(7),DL3(7),DL4(7),DLT(7)
      DATA PI/3.1415926535D0/
C
C   DLN(3)= LAMBDA(3)+LAMBDA(4),   N=1,...,4
C   DLN(4)= LAMBDA(5)-LAMBDA(4),   N=1,...,4
C
      GSQ= G2+GP2
      DLT(1)= 0.25D0*GSQ
      DLT(2)= 0.25D0*GSQ
      DLT(3)= -0.25D0*GSQ
      DLT(4)= 0.5D0*G2
      DLT(5)=0.D0
      DLT(6)=0.D0
      DLT(7)=0.D0
      IF (IRC .EQ. 0) RETURN
      STEP= 0.D0
      IF (AMASS .GE. ZMASS) STEP= 1.D0
      A2= AMASS**2
      COSW2= 1.D0-SINW2
      T2= TM1**2
      B2= BMASS**2
      SUSY2= SUSY*SUSY
      SLM2= SLM*SLM
      SQM2= SQM*SQM
      SQTM2= DSQRT(STA2*STB2)
      SQBM2= DSQRT(SBA2*SBB2)
      SQTBM2= DSQRT(SQTM2*SQBM2)
      W2= WMASS*WMASS
      Z2= ZMASS**2
      AT2= AT*AT
      AB2= AB*AB
      XMU2= XMU*XMU
      V2= 4.D0*W2/G2
      CB2= COSB**2
      SB2= SINB**2
      COS2B= COSB**2-SINB**2
      COS2B2= COS2B**2
      EU= 2.D0/3.D0
      ED= -1.D0/3.D0
      COLOR= 3.D0
      GENS= 3.D0
      PT= COLOR*(1.D0-4.D0*EU*SINW2+8.D0*EU**2*SINW2**2)
      PB= COLOR*(1.D0+4.D0*ED*SINW2+8.D0*ED**2*SINW2**2)
C      PF= (GENS-1.D0)*(PT+PB)+2.D0*GENS*(1.D0-2.D0*SINW2+4.D0*SINW2**2)
      PL= 2.D0*GENS*(1.D0-2.D0*SINW2+4.D0*SINW2**2)
      PQ= (GENS-1.D0)*(PT+PB)
      P22= -2.D0*(32.D0*SINW2**2-54.D0*SINW2+27.D0)
      P12= -2.D0*(8.D0*SINW2**2-6.D0*SINW2-9.D0)
      P2H= -10.D0+2.D0*SINW2-2*SINW2**2
      P2HP= 8.D0-22.D0*SINW2+10.D0*SINW2**2
      P1H= -9.D0*COS2B2**2+(1.D0-2.D0*SINW2+2.D0*SINW2**2)*COS2B2
      PH12= (P1H-P2H)*STEP
      PH12P= (P1H+P2HP)*STEP
      FACT= COLOR*G2/(16.D0*PI**2*W2*Z2)
      FACT1= COLOR*G2*T2/(32.D0*PI**2*COSW2*SB2*Z2)
      FACT2= COLOR*G2*B2/(32.D0*PI**2*COSW2*CB2*Z2)
      FACT3= G2/(96.D0*PI**2*COSW2)
      FACTC= COLOR*G2/(32.D0*PI**2)
      FACTC2= G2/(48.D0*PI**2)
      FACTPC= 5.D0*GP2/(16.D0*PI**2)
      DLOGW= DLOG(SUSY2/W2)
      DLOGZ= DLOG(SUSY2/Z2)
C      DLOGF= DLOG(SQM2/Z2)
C      DLOGFW= DLOG(SQM2/W2)
      DLOGL= DLOG(SLM2/Z2)
      DLOGLW= DLOG(SLM2/W2)
      DLOGQ= DLOG(SQM2/Z2)
      DLOGQW= DLOG(SQM2/W2)
      DLOGT= DLOG(SQTM2/T2)
      DLOGTB= DLOG(SQTBM2/T2)
      DLOGB= DLOG(SQBM2/Z2)
      DLOGA= DLOG(A2/Z2)
      XCG= COLOR*(GENS-1.D0)+GENS
      DL1(1)= 0.25D0*GSQ*(1.D0
     1        +(FACT*(2.D0*B2**2/CB2-Z2*B2)/CB2+FACT3*PB)*DLOGB
C     2        +FACT3*(PF*DLOGF+P22*DLOGZ+PT*DLOGT+PH12*DLOGA))
     2    +FACT3*(PL*DLOGL+PQ*DLOGQ+P22*DLOGZ+PT*DLOGT+PH12*DLOGA))
      DL1(2)= 0.25D0*GSQ*(1.D0
     1        +(FACT*(2.D0*T2**2/SB2-Z2*T2)/SB2+FACT3*PT)*DLOGT
C     2        +FACT3*(PF*DLOGF+P22*DLOGZ+PB*DLOGB+PH12*DLOGA))
     2    +FACT3*(PL*DLOGL+PQ*DLOGQ+P22*DLOGZ+PB*DLOGB+PH12*DLOGA))
      DL1(3)= -0.25D0*GSQ*(1.D0
     1        -(FACT1-FACT3*PT)*DLOGT-(FACT2-FACT3*PB)*DLOGB
C     2        +FACT3*(PF*DLOGF+P12*DLOGZ-PH12P*DLOGA))
     2    +FACT3*(PL*DLOGL+PQ*DLOGQ+P12*DLOGZ-PH12P*DLOGA))
      DL1(4)= 0.5D0*G2*(1.D0
     1        +FACTC*(2.D0*T2*B2/(W2**2*SB2*CB2)+2.D0/3.D0
     2        -(T2/SB2+B2/CB2)/W2)*DLOGTB+FACTPC*DLOGW
C     3        +FACTC2*(XCG*DLOGFW-9.D0*DLOGW))
     3         +FACTC2*(XCG*DLOGQW+GENS*DLOGLW-9.D0*DLOGW))
      DL1(5)=0
      DL1(6)=0
      DL1(7)=0
      AFACT= COLOR/(16.D0*PI**2)
      HB2= 0.5D0*G2*B2/(W2*CB2)
      HT2= 0.5D0*G2*TM2**2/(W2*SB2)
      HB4= HB2*HB2
      HT4= HT2*HT2
      Q2= SQK**2
      U2= SQU**2
      D2= SQD**2
      UF= FF(U2,Q2)
      DF= FF(D2,Q2)
      UG= FG(Q2,U2)
      DG= FG(Q2,D2)
      UK= 0.25D0*(GSQ*UF+2.D0*EU*GP2*(Q2-U2)*UG)
      DK= 0.25D0*(GSQ*DF-2.D0*ED*GP2*(Q2-D2)*DG)
      UH= FH(Q2,U2)
      DH= FH(Q2,D2)
      UB= BP(Q2,U2)
      DB= BP(Q2,D2)
      A11= -AFACT*(HT2*XMU2*UB+HB2*AB2*DB)
      A22= -AFACT*(HT2*AT2*UB+HB2*XMU2*DB)
      DL2(1)= 0.5D0*GSQ*A11
      DL2(2)= 0.5D0*GSQ*A22
      DL2(3)= -0.25D0*GSQ*(A11+A22)
      DL2(4)= 0.5D0*G2*(A11+A22)
      DL2(5)= 0.D0
      DL2(6)= 0.D0
      DL2(7)= 0.D0
      DL3(1)= 2.D0*AFACT*(AB2*HB4*DH-HT2*XMU2*UK+HB2*AB2*DK)
      DL3(2)= 2.D0*AFACT*(AT2*HT4*UH-HB2*XMU2*DK+HT2*AT2*UK)
      DL3(3)= AFACT*(XMU2*(HB4*DH+HT4*UH)+HB2*(XMU2-AB2)*DK
     1           +HT2*(XMU2-AT2)*UK)
      DL3(4)= -0.5D0*AFACT*(4.D0*HB2*HT2*(AT*AB-XMU2)*HP(Q2,U2,D2)
     1           +HB2*DF*(2.D0*(HT2*AB2-HB2*XMU2)+G2*(XMU2-AB2))
     2           +HT2*UF*(2.D0*(HB2*AT2-HT2*XMU2)+G2*(XMU2-AT2)))
      DL3(5)= 0.D0
      DL3(6)= -AFACT*XMU*(HB4*AB*DH+HB2*AB*DK-HT2*AT*UK)
      DL3(7)= -AFACT*XMU*(HT4*AT*UH+HT2*AT*UK-HB2*AB*DK)
      DL4(1)= AFACT*(DG*(AB2*HB2)**2+UG*(XMU2*HT2)**2)
      DL4(2)= AFACT*(UG*(AT2*HT2)**2+DG*(XMU2*HB2)**2)
      DL4(3)= 2.D0*AFACT*XMU2*(AT2*HT4*UG+AB2*HB4*DG)
      DL4(4)= -AFACT*(HT2*HB2*(XMU2-AT*AB)**2*FP(Q2,U2,D2))
      DL4(5)= AFACT*XMU2*(AT2*HT4*UG+AB2*HB4*DG)
      DL4(6)= -AFACT*XMU*(AT*XMU2*HT4*UG+AB*AB2*HB4*DG)
      DL4(7)= -AFACT*XMU*(AB*XMU2*HB4*DG+AT*AT2*HT4*UG)
      DO 10 I=1,7
   10 DLT(I)= DL1(I)+DL2(I)+DL3(I)+DL4(I)
      D11= A2*SB2+V2*(DLT(1)*CB2+DLT(5)*SB2+2.D0*SINB*COSB*DLT(6))
      D22= A2*CB2+V2*(DLT(2)*SB2+DLT(5)*CB2+2.D0*SINB*COSB*DLT(7))
      D12= -A2*SINB*COSB+V2*(DLT(3)*SINB*COSB+DLT(6)*CB2+DLT(7)*SB2)
      DP= A2+0.5D0*V2*DLT(4)
      RETURN
      END
C
      FUNCTION FF(X2,Y2)
      IMPLICIT REAL*8(A-H,O-Z)
      DATA SMALL/1.D-4/
      FF= 0.D0
      IF (X2 .LE. 0.D0) GO TO 100
      IF (Y2 .LE. 0.D0) GO TO 100
      XYD= (X2-Y2)/Y2
      DXYD= DABS(XYD)
      IF (DXYD .LT. SMALL) GO TO 50
      FF= 1.D0/(X2-Y2)-X2*DLOG(X2/Y2)/(X2-Y2)**2
      RETURN
   50 FF= (-1.D0/2.D0+XYD/6.D0-XYD**2/12.D0+XYD**3/20.D0)/Y2
      RETURN
  100 WRITE(6,200)
  200 FORMAT(1X,'ERROR: ARGUMENTS OF FF-FUNCTION MUST BE POSITIVE')
      END
C
      FUNCTION FFD(X2,Y2)
      IMPLICIT REAL*8(A-H,O-Z)
      DATA SMALL/1.D-4/
      FFD= 0.D0
      IF (X2 .LE. 0.D0) GO TO 100
      IF (Y2 .LE. 0.D0) GO TO 100
      XYD= (X2-Y2)/Y2
      DXYD= DABS(XYD)
      IF (DXYD .LT. SMALL) GO TO 50
      FFD= 1.D0-X2*DLOG(X2/Y2)/(X2-Y2)
      RETURN
   50 FFD= -XYD/2.D0+XYD**2/6.D0-XYD**3/12.D0+XYD**4/20.D0
      RETURN
  100 WRITE(6,200)
  200 FORMAT(1X,'ERROR: ARGUMENTS OF FFD-FUNCTION MUST BE POSITIVE')
      END
C
      FUNCTION FG(X2,Y2)
      IMPLICIT REAL*8(A-H,O-Z)
      DATA SMALL/1.D-4/
      FG= 0.D0
      IF (X2 .LE. 0.D0) GO TO 100
      IF (Y2 .LE. 0.D0) GO TO 100
      XYD= (X2-Y2)/Y2
      DXYD= DABS(XYD)
      IF (DXYD .LT. SMALL) GO TO 50
      FG= 2.D0/(X2-Y2)**2-(X2+Y2)*DLOG(X2/Y2)/(X2-Y2)**3
      RETURN
   50 FG= (-1.D0/6.D0+XYD/6.D0-3.D0*XYD**2/20.D0
     1           +2.D0*XYD**3/15.D0)/Y2**2
      RETURN
  100 WRITE(6,200)
  200 FORMAT(1X,'ERROR: ARGUMENTS OF FG-FUNCTION MUST BE POSITIVE')
      END
C
      FUNCTION FH(X2,Y2)
      IMPLICIT REAL*8(A-H,O-Z)
      DATA SMALL/1.D-4/
      FH= 0.D0
      IF (X2 .LE. 0.D0) GO TO 100
      IF (Y2 .LE. 0.D0) GO TO 100
      XYD= (X2-Y2)/Y2
      DXYD= DABS(XYD)
      IF (DXYD .LT. SMALL) GO TO 50
      FH= DLOG(X2/Y2)/(X2-Y2)
      RETURN
   50 FH= (1.D0-XYD/2.D0+XYD**2/3.D0-XYD**3/4.D0+XYD**4/5.D0)/Y2
      RETURN
  100 WRITE(6,200)
  200 FORMAT(1X,'ERROR: ARGUMENTS OF FH-FUNCTION MUST BE POSITIVE')
      END
C
      FUNCTION BP(X2,Y2)
      IMPLICIT REAL*8(A-H,O-Z)
      DATA SMALL/1.D-4/
      BP= 0.D0
      IF (X2 .LE. 0.D0) GO TO 100
      IF (Y2 .LE. 0.D0) GO TO 100
      XYD= (X2-Y2)/Y2
      DXYD= DABS(XYD)
      IF (DXYD .LT. SMALL) GO TO 50
      BP= 0.5D0*(X2+Y2-2.D0*X2*Y2*DLOG(X2/Y2)/(X2-Y2))/(X2-Y2)**2
      RETURN
   50 BP= (1.D0/6.D0-XYD/12.D0+XYD**2/20.D0-XYD**3/30.D0)/Y2
      RETURN
  100 WRITE(6,200)
  200 FORMAT(1X,'ERROR: ARGUMENTS OF BP-FUNCTION MUST BE POSITIVE')
      END
C
      FUNCTION FP(Z2,X2,Y2)
      IMPLICIT REAL*8(A-H,O-Z)
      DATA SMALL/1.D-4/
      FP= 0.D0
      IF (X2 .LE. 0.D0) GO TO 100
      IF (Y2 .LE. 0.D0) GO TO 100
      XYD= (X2-Y2)/Y2
      DXYD= DABS(XYD)
      IF (DXYD .LT. SMALL) GO TO 50
      FP= (FF(X2,Z2)-FF(Y2,Z2))/(X2-Y2)
      RETURN
   50 FP= -FG(Y2,Z2)+XYD*FP1(Y2,Z2)+XYD**2*FP2(Y2,Z2)+XYD**3*FP3(Y2,Z2)
      RETURN
  100 WRITE(6,200)
  200 FORMAT(1X,'ERROR: ARGUMENTS OF FP-FUNCTION MUST BE POSITIVE')
      END
C
      FUNCTION HP(Z2,X2,Y2)
      IMPLICIT REAL*8(A-H,O-Z)
      DATA SMALL/1.D-4/
      HP= 0.D0
      IF (X2 .LE. 0.D0) GO TO 100
      IF (Y2 .LE. 0.D0) GO TO 100
      XYD= (X2-Y2)/Y2
      DXYD= DABS(XYD)
      IF (DXYD .LT. SMALL) GO TO 50
      HP= (FFD(X2,Z2)-FFD(Y2,Z2))/(X2-Y2)
      RETURN
   50 HP= FF(Z2,Y2)+XYD*BP(Y2,Z2)+XYD**2*HP2(Y2,Z2)+XYD**3*HP3(Y2,Z2)
      RETURN
  100 WRITE(6,200)
  200 FORMAT(1X,'ERROR: ARGUMENTS OF HP-FUNCTION MUST BE POSITIVE')
      END
C
      FUNCTION FP1(Y2,Z2)
      IMPLICIT REAL*8(A-H,O-Z)
      DATA SMALL/1.D-4/
      YZD= (Y2-Z2)/Z2
      DYZD= DABS(YZD)
      IF (DYZD .LT. SMALL) GO TO 50
      FP1= 0.5D0*(5.D0*Y2+Z2)/(Y2-Z2)**3
     1         -Y2*(2.D0*Z2+Y2)*DLOG(Y2/Z2)/(Y2-Z2)**4
      RETURN
   50 FP1=(-1.D0/12.D0+YZD/15.D0-YZD**2/20.D0+4.D0*YZD**3/105.D0)/Z2**2
      RETURN
      END
C
      FUNCTION FP2(Y2,Z2)
      IMPLICIT REAL*8(A-H,O-Z)
      DATA SMALL/1.D-4/
      YZD= (Y2-Z2)/Z2
      DYZD= DABS(YZD)
      IF (DYZD .LT. SMALL) GO TO 50
      FP2= (Z2**2-8.D0*Z2*Y2-17.D0*Y2**2)/(6.D0*(Y2-Z2)**4)
     1         +Y2**2*(3.D0*Z2+Y2)*DLOG(Y2/Z2)/(Y2-Z2)**5
      RETURN
   50 FP2=(1.D0/20.D0-YZD/30.D0+3.D0*YZD**2/140.D0+YZD**3/70.D0)/Z2**2
      RETURN
      END
      FUNCTION FP3(Y2,Z2)
      IMPLICIT REAL*8(A-H,O-Z)
      DATA SMALL/1.D-4/
      YZD= (Y2-Z2)/Z2
      DYZD= DABS(YZD)
      IF (DYZD .LT. SMALL) GO TO 50
      POLY= Z2**3-7.D0*Z2**2*Y2+29.D0*Z2*Y2**2+37.D0*Y2**3
      FP3= POLY/(12.D0*(Y2-Z2)**5)
     1         -Y2**3*(4.D0*Z2+Y2)*DLOG(Y2/Z2)/(Y2-Z2)**6
      RETURN
   50 FP3=(-1.D0/30.D0+2.D0*YZD/105.D0-3.D0*YZD**2/280.D0
     1         +2.D0*YZD**3/315.D0)/Z2**2
      RETURN
      END
C
      FUNCTION HP2(Y2,Z2)
      IMPLICIT REAL*8(A-H,O-Z)
      DATA SMALL/1.D-4/
      YZD= (Y2-Z2)/Z2
      DYZD= DABS(YZD)
      IF (DYZD .LT. SMALL) GO TO 50
      HP2= (Z2**2-5.D0*Z2*Y2-2.D0*Y2**2)/(6.D0*(Y2-Z2)**3)
     1         +Y2**2*Z2*DLOG(Y2/Z2)/(Y2-Z2)**4
      RETURN
   50 HP2=(-1.D0/12.D0+YZD/30.D0-YZD**2/60.D0+YZD**3/105.D0)/Z2
      RETURN
      END
C
      FUNCTION HP3(Y2,Z2)
      IMPLICIT REAL*8(A-H,O-Z)
      DATA SMALL/1.D-4/
      YZD= (Y2-Z2)/Z2
      DYZD= DABS(YZD)
      IF (DYZD .LT. SMALL) GO TO 50
      POLY= Z2**3-5.D0*Z2**2*Y2+13.D0*Z2*Y2**2+3.D0*Y2**3
      HP3= POLY/(12.D0*(Y2-Z2)**4)
     1         -Y2**3*Z2*DLOG(Y2/Z2)/(Y2-Z2)**5
      RETURN
   50 HP3=(1.D0/20.D0-YZD/60.D0+YZD**2/140.D0-YZD**3/280.D0)/Z2
      RETURN
      END
      FUNCTION ALFS(QK)
      IMPLICIT REAL*8 (A-H,O-Z)
      include 'haber.inc'
      DATA PI/3.1415926535D0/
C
C   ONE-LOOP STRONG COUPLING FOR QK>BMASS
C
      AST= 0.11D0
      ALFS= AST
      IF (QK .GE. TMPOLE) F= 6.D0
      IF (QK .LT. TMPOLE) F= 5.D0
      IF (QK .LT. 5.D0) GO TO 10
      DEN= 1.D0+AST*(11.D0-2.D0*F/3.D0)*DLOG(QK/TMPOLE)/(2.D0*PI)
      ALFS= AST/DEN
      RETURN
   10 WRITE (6,100)
  100 FORMAT(1X,'ERROR: STRONG COUPLING CONSTANT EVALUATED BELOW M(B)')
      RETURN
      END
      FUNCTION TMASSR(Q,TMASS)
      IMPLICIT REAL*8 (A-H,O-Z)
      include 'haber.inc'
      DATA PI/3.1415926535/
      QP= TMPOLE
      HB2= 0.5D0*G2*BMASS**2/(WMASS*COSB)**2
      HT2= 0.5D0*G2*TMASS**2/(WMASS*SINB)**2
      ALS= ALFS(QP)
      ALST= ALS-3.D0*HT2/(64.D0*PI)
     1          +GP2/(48.D0*PI)
     2          -HB2/(64.D0*PI)
      TMASSR= TMASS*(1.D0-ALST*DLOG(Q**2/TMASS**2)/PI)
      RETURN
      END
      SUBROUTINE HIGGS3(IRC,H,DL)
      IMPLICIT REAL*8(A-H,O-Z)
      include 'haber.inc'
      COMMON/HINT3/HLAA,HHAA,HLHLHL,HHHLHL,HHHHHL,HHHHHH,
     1      HHHPHM,HLHPHM
      DIMENSION H(4),DL(7)
      DATA PI/3.1415926535D0/
C
C   DL(3)= LAMBDA(3)+LAMBDA(4)
C   DL(4)= LAMBDA(5)-LAMBDA(4)
C
      V2= 4.D0*WMASS**2/G2
      DL4NL= 2.D0*(H(4)**2-H(3)**2)/V2
      COSW= DSQRT(1.D0-SINW2)
      G= DSQRT(G2)
      SINB2= SINB**2
      COSB2= COSB**2
      SINA2= SINA**2
      COSA2= COSA**2
      SINA3= SINA**3
      COSA3= COSA**3
      COS2B= COSB2-SINB2
      IF (IRC .EQ. 0) GO TO 50
      FACN= 2.D0*WMASS/G
      HLAA= FACN*(DL(1)*SINA*COSB*SINB2-DL(2)*COSA*SINB*COSB2
     1        -(DL(3)+DL(5))*(COSA*SINB**3-SINA*COSB**3)
     2        +2.D0*DL(5)*SINBMA-DL(6)*SINB*(COSB*SINBPA+SINA*COS2B)
     3        -DL(7)*COSB*(COSA*COS2B-SINB*SINBPA))
      HHAA= -FACN*(DL(1)*COSA*COSB*SINB2+DL(2)*SINA*SINB*COSB2
     1        +(DL(3)+DL(5))*(SINA*SINB**3+COSA*COSB**3)
     2        -2.D0*DL(5)*COSBMA-DL(6)*SINB*(COSB*COSBPA+COSA*COS2B)
     3        +DL(7)*COSB*(SINB*COSBPA+SINA*COS2B))
      HHHLHL= -3.D0*FACN*(DL(1)*COSA*SINA2*COSB+DL(2)*SINA*COSA2*SINB
     1        +(DL(3)+DL(5))*(SINA3*SINB+COSA3*COSB-2.D0*COSBMA/3.D0)
     2        -DL(6)*SINA*(COSB*COS2A+COSA*COSBPA)
     3        +DL(7)*COSA*(SINB*COS2A+SINA*COSBPA))
      HHHHHL= 3.D0*FACN*(DL(1)*SINA*COSB*COSA2-DL(2)*COSA*SINB*SINA2
     1        +(DL(3)+DL(5))*(SINA3*COSB-COSA3*SINB+2.D0*SINBMA/3.D0)
     2        -DL(6)*COSA*(COSB*COS2A-SINA*SINBPA)
     3        -DL(7)*SINA*(SINB*COS2A+COSA*SINBPA))
      HHHHHH= -3.D0*FACN*(DL(1)*COSB*COSA3+DL(2)*SINB*SINA3
     1        +(DL(3)+DL(5))*SINA*COSA*SINBPA
     2        +DL(6)*COSA2*(3.D0*SINA*COSB+COSA*SINB)
     3        +DL(7)*SINA2*(3.D0*COSA*SINB+SINA*COSB))
      HLHLHL= 3.D0*FACN*(DL(1)*COSB*SINA3-DL(2)*SINB*COSA3
     1        +(DL(3)+DL(5))*SINA*COSA*COSBPA
     2        -DL(6)*SINA2*(3.D0*COSA*COSB-SINA*SINB)
     3        +DL(7)*COSA2*(3.D0*SINA*SINB-COSA*COSB))
      HHHPHM= HHAA-FACN*COSBMA*DL(4)
      HLHPHM= HLAA-FACN*SINBMA*DL(4)
C
C   THESE COUPLINGS INCLUDE NON-LEADING LOG CONTRIBUTIONS
C   WHICH ARE TAKEN TO BE THE SAME AS IN THE CORRECTIONS
C   TO THE CHARGED HIGGS MASS SQUARED.
C
C     HHHPM= HHAA-FACN*COSBMA*DL4NL
C     HLHPM= HLAA-FACN*SINBMA*DL4NL
      RETURN
   50 FAC= -G*WMASS
      FAN= -0.5D0*G*ZMASS/COSW
      HHHPHM= FAC*COSBMA-FAN*COS2B*COSBPA
      HLHPHM= FAC*SINBMA+FAN*COS2B*SINBPA
      HHHHHH= 3.D0*FAN*COS2A*COSBPA
      HLHLHL= 3.D0*FAN*COS2A*SINBPA
      HHHHHL= -FAN*(2.D0*SIN2A*COSBPA+COS2A*SINBPA)
      HHHLHL= FAN*(2.D0*SIN2A*SINBPA-COS2A*COSBPA)
      HHAA= -FAN*COS2B*COSBPA
      HLAA= FAN*COS2B*SINBPA
      RETURN
      END
      DOUBLE PRECISION FUNCTION hgamma(s)
C---------------------------------------------------------------------
C! Computes the total cross-section for e+e- -> H gamma at tree level
C  (anomalous couplings only)
C
C Patrick Janot -- 18 Oct 1999
C---------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'consts.inc'
      include 'datapi.inc'
C
      ghgg  = amw * sw * (    fbb +     fww ) / 2D0
      ghzg1 = amw / cw * (    fw  -     fb  ) / 2D0
      ghzg2 = amw / cw * (sw2*fbb - cw2*fww ) 
      ghzg  = ghzg2 + ghzg1/2D0
      v   = -5D-1 + 2D0*sw2
      a   = -5D-1
C
C gamma-gamma
C
      cgg = ghgg**2 
C
C gamma-Z
C
      cgz = ghgg * ghzg
     &    * v / (2D0 * sw * cw)
     &    * s * (s-amz**2)
     &    / ( (s-amz**2)**2 + amz**2*gmz**2 )
C
C Z-Z
C
      czz = ghzg**2
     &    * (v**2+a**2) / (16D0 * sw2 * cw2)
     &    * s**2
     &    / ( (s-amz**2)**2 + amz**2*gmz**2 )
C
C Total cross section
C
      hgamma = 8D0 * pi * alpha2 / 3D0
     &       * ( (s-amh**2)/s )**3
     &       * (cgg + cgz + czz)
C
C There we went.
C
      RETURN 
      END


      SUBROUTINE hhdecay(ph,jchan,jhig,iph)
C--------------------------------------------------------------------
C! Main routine to decay Higgses of the MSSM.
C  The Higgs is decayed in its rest frame. Then, a boost is performed
C  for going back to the lab.
C
C  Input:     Passed:   --PH(4),   Higgs quadriimpulsion (in the lab)
C                                  with the format :
C                                  PH(1,2,3)    momentum (in GeV)
C                             -->  PH(4)        Mass        "
C                       --JHIG,    higgs type ( 1:H, 2:h, 3:A )
C                       --IPH,     (optional) Row number in
C                                  the LUJETS common block
C  Output:    Passed:   --JCHAN,   channel selected
C
C   P. Janot  --  26 Aug 1991
C-----------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'hhdeck.inc'
      include 'counts.inc'
      DIMENSION brloc(nchan,nhig)
      DIMENSION wnloc(4,4,nhig),wcloc(2,2,nhig)
C
      IF ( jhig .LE. 0 .OR. jhig .GT. nhig ) THEN
        WRITE(6,*) '+++ HHDECAY +++ Wrong Higgs number', jhig
        STOP 99
      ENDIF
C
      IF ( parwth(jhig) .EQ. 0.) THEN
        WRITE(6,1000) jhig
        STOP 99
      ENDIF
C
C  Simply get rid of the decay channels suddenly closed due
C  to too low a value of the generated Higgs mass (due to the
C  decay width)
C
      CALL ucopy(branch(1,1),brloc(1,1),nchan*nhig*2)
      CALL ucopy(wneut(1,1,1),wnloc(1,1,1),4*4*nhig*2)
      CALL ucopy(wchar(1,1,1),wcloc(1,1,1),2*2*nhig*2)
C
      IF ( ph(4) .LT. 1.       ) branch( 2,jhig) = 0.
      IF ( ph(4) .LT. 2.*amtau ) branch( 3,jhig) = 0.
      IF ( ph(4) .LT. 2.*amc   ) branch( 4,jhig) = 0.
      IF ( ph(4) .LT. 2.*amb   ) branch( 5,jhig) = 0.
      IF ( ph(4) .LT. 2.*amt   ) branch( 6,jhig) = 0.
      IF ( ph(4) .LT. 20.      ) branch( 7,jhig) = 0.
      IF ( ph(4) .LT. 20.      ) branch( 8,jhig) = 0.
      IF ( ph(4) .LT. 2.*ama  .AND. jhig .LE. 2 )
     .                           branch( 9,jhig) = 0.
      IF ( ph(4) .LT. amh+amz  ) branch( 9,3   ) = 0.
      IF ( ph(4) .LT. 2.*amh   ) branch(10,jhig) = 0.
      IF ( ph(4) .LT. 20.      ) branch(11,jhig) = 0.
      IF ( ph(4) .LT. 2*ame    ) branch(12,jhig) = 0.
      IF ( ph(4) .LT. 2*ammu   ) branch(13,jhig) = 0.
      IF ( ph(4) .LT. 2*ams    ) branch(14,jhig) = 0.
C
      IF ( branch(15,jhig) .GT. 0. ) THEN
        frn = 0.
        DO i = 1, 4
          DO j = 1, 4
C GG - neutralino masses are signed ...
            IF ( ph(4).LT.ABS(amneut(i))+ABS(amneut(j)) ) THEN
              wneut(i,j,jhig) = 0.
            ELSE
              frn = frn + wneut(i,j,jhig)
            ENDIF
          ENDDO
        ENDDO
        branch(15,jhig) = branch(15,jhig) * frn
      ENDIF
C
      IF ( branch(16,jhig) .GT. 0. ) THEN
        frc = 0.
        DO i = 1, 2
          DO j = 1, 2
            IF ( ph(4).LT.amchar(i)+amchar(j) ) THEN
              wchar(i,j,jhig) = 0.
            ELSE
              frc = frc + wchar(i,j,jhig)
            ENDIF
          ENDDO
        ENDDO
        branch(16,jhig) = branch(16,jhig) * frc
      ENDIF
C
      frch = 0.
      DO ich = 1, nchan
        frch = frch + branch(ich,jhig)
      ENDDO
C
C  Choice of the channel.
C
      IF ( frch .EQ. 0. ) THEN
        jchan = 1
         GOTO 30
      ENDIF
C
      rnch = RNDM2(dummy)
      rint = 0.D0
      DO jchan = 1 , nchan
        rint = rint + branch(jchan,jhig)/frch
        if ( rnch .lt. rint ) GOTO 30
      ENDDO
  30  CONTINUE
      IF (idbg .GE. 10) WRITE(6,1001) rnch,rint,jchan,jhig
C
      kevt(jchan,jhig) = kevt(jchan,jhig) + 1
C
C  If a SUSY decay (chi chi or chi+ chi-) has been chosen, select
C  which one precisely
C
      rnch = RNDM2(dummy)
      rint = 0.D0
C
      IF ( jchan .EQ. 15 ) THEN
C
        DO i = 1, 4
          DO j = 1, 4
            rint = rint + wneut(i,j,jhig)/frn
            IF ( rnch .LT. rint ) THEN
              xymas(1,15,jhig) = ABS(amneut(i))
              xymas(2,15,jhig) = ABS(amneut(j))
              xywid(1,15,jhig) = 0.
              xywid(2,15,jhig) = 0.
              ichi = 0
              ichn(1) = i
              ichn(2) = j
              IF ( idbg .GE. 10 ) THEN
                WRITE(6,*) 'Neutralino decay : ',i,j
                WRITE(6,*) 'Masses : ',ABS(amneut(i)),ABS(amneut(j))
              ENDIF
              GOTO 40
            ENDIF
          ENDDO
        ENDDO
C
      ELSEIF ( jchan .EQ. 16 ) THEN
C
        DO i = 1, 2
          DO j = 1, 2
            rint = rint + wchar(i,j,jhig)/frc
            IF ( rnch .LT. rint ) THEN
              xymas(1,16,jhig) = ABS(amchar(i))
              xymas(2,16,jhig) = ABS(amchar(j))
              xywid(1,16,jhig) = 0.
              xywid(2,16,jhig) = 0.
              ichi = 1
              ichn(1) = i
              ichn(2) = j
              IF ( idbg .GE. 10 ) THEN
                WRITE(6,*) 'Chargino decay : ',i,j
                WRITE(6,*) 'Masses : ',ABS(amchar(i)),ABS(amchar(j))
              ENDIF
              GOTO 40
            ENDIF
          ENDDO
        ENDDO
C
      ENDIF
C
      CALL ucopy(brloc(1,1),branch(1,1),nchan*nhig*2)
      CALL ucopy(wnloc(1,1,1),wneut(1,1,1),4*4*nhig*2)
      CALL ucopy(wcloc(1,1,1),wchar(1,1,1),2*2*nhig*2)
C
C  Build quadriimpulsions of the 2 decay particles
C
   40 CALL pick4(ph,jchan,jhig)
C
C  Fill LUJETS common block. Lorentz-boost back to the lab.
C
      CALL hhlujt(jchan,jhig,ph,iph)
C
  999 RETURN
C------------------------------------------------------------------
1000  FORMAT(1X,' +++ HHDECAY +++   No accessible decay channel',
     .          ' for Higgs # ',I1)
1001  FORMAT(1X,' +++ HHDECAY +++ RNDM : ',F8.6,' < ? ',F8.6,
     .       ' Channel ',I3,', Type ',I3,' Higgs # ',I1)
      END
      SUBROUTINE HHFINI
C----------------------------------------------------------------------
C! Termination for neutral Higgs decays routines (from SM or MSSM)
C
C   P. Janot -- 24 August 1991
C----------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'hhdeck.inc'
      include 'counts.inc'
      WRITE(6,1000)
      WRITE(6,1001) (jchan, jchan=1,nchan)
      WRITE(6,1002) (jhig,(kevt(jchan,jhig),jchan=1,nchan),jhig=1,nhig)
C----------------------------------------------------------------------
 1000 FORMAT(1x,' Total number of events in various decay channels :'/)
 1001 FORMAT(4x,16(4x,I2))
 1002 FORMAT(3(1x,I2,1x,16(I6)/))
      RETURN
      END
      SUBROUTINE HHINIT
C------------------------------------------------------------------
C!  Initialisation for neutral Higgs decays routines(from SM or MSSM)
C           . h0 lightest scalar
C           . H0 heaviest scalar    mh < 2m_mu or mh > ~ GeV/c2
C           . A  pseudoscalar
C
C  Input:     none
C
C
C  Output:   /HHDECK/   --BRANCH(ichan,ihig), branching ratios
C                       --WIDTH(ihig), total widths
C
C   P. Janot -- 24 August 1991
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'counts.inc'
      include 'miscl.inc'
C
C  Set event numbers to zero
C
      CALL vzero(kevt(1,1),nchan*nhig)
C
C  Set defaults conditions
C
      CALL setdef
C
C  Set user's conditions if requested
C
      CALL usrdef
C
C  Determine parameters, couplings...
C
      CALL parcou
C      IF ( amh .LT. 0. ) RETURN
      IF ( amh .LE. 0. .OR.
     &   ( ism .LE. 0 .AND. ( gmh.LE.0. .OR.
     .     ama .LE. 0. .OR. amhp.LE.0. ) ) ) RETURN
C      IF ( ism .EQ. 0 .AND. amst(1) .LT. 0. ) RETURN
      IF ( ism .EQ. 0 .AND. 
     .    (amst(1) .LT. 0. .OR. amsb(1) .LT. 0.) ) RETURN
C
C  Compute decay widths and branching ratios
C
      CALL combra
C
  999 RETURN
      END
      SUBROUTINE hhlu1(ipa,kf,px,py,pz,pe,pm)
C------------------------------------------------------------------
C! Add one entry to the LUND event record
C
C  Patrick Janot -- 26 Aug 1991
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C      REAL*8 px,py,pz,pe,pm
      include 'lundcom.inc'
C
      qm = pm
      IF ( qm .LT. 0. ) qm = SQRT(pe**2-px**2-py**2-pz**2)
      DO 100 J=1,5
      k7lu(ipa,j)=0
      p7lu(ipa,j)=0.
  100 v7lu(ipa,j)=0.
C...Store parton/particle in K and P vectors.
      k7lu(ipa,1)=1
      k7lu(ipa,2)=kf
      p7lu(ipa,5)=qm
      p7lu(ipa,4)=pe
      p7lu(ipa,1)=px
      p7lu(ipa,2)=py
      p7lu(ipa,3)=pz
C
  999 RETURN
      END
      SUBROUTINE hhlujt(jchan,jhig,ph,iph)
C------------------------------------------------------------------
C! Fill LUJETS common block after a Higgs boson decay.
C
C  Patrick Janot -- 26 Aug 1991
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C      REAL*4 xmm
      include 'lundcom.inc'
      include 'param.inc'
      include 'hhdeck.inc'
      include 'miscl.inc'
      DIMENSION ijoin(2), jjoin(2)
      DIMENSION p1(4),p2(4),p3(4)
C
      IF ( jchan .EQ. 1 ) THEN
        kf1 = 22
        kf2 = 22
      ELSEIF ( jchan .EQ. 2 ) THEN
        kf1 = 21
        kf2 = 21
      ELSEIF ( jchan .EQ. 3 ) THEN
        kf1 = 15
        kf2 =-15
      ELSEIF ( jchan .EQ. 4 ) THEN
        kf1 = 4
        kf2 =-4
      ELSEIF ( jchan .EQ. 5 ) THEN
        kf1 = 5
        kf2 =-5
      ELSEIF ( jchan .EQ. 6 ) THEN
        kf1 = 6
        kf2 =-6
      ELSEIF ( jchan .EQ. 7 ) THEN
        kf1 = 24
        kf2 =-24
      ELSEIF ( jchan .EQ. 8 ) THEN
        kf1 = 23
        kf2 = 23
      ELSEIF ( jchan .EQ. 9 ) THEN
        IF ( jhig .NE. 3 ) THEN
          kf1 = 36
          kf2 = 36
        ELSE
          kf1 = 25
          kf2 = 23
        ENDIF
      ELSEIF ( jchan .EQ. 10 ) THEN
        kf1 = 25
        kf2 = 25
      ELSEIF ( jchan .EQ. 11 ) THEN
        kf1 = 22
        kf2 = 23
      ELSEIF ( jchan .EQ. 12 ) THEN
        kf1 = 11
        kf2 =-11
      ELSEIF ( jchan .EQ. 13 ) THEN
        kf1 = 13
        kf2 =-13
      ELSEIF ( jchan .EQ. 14 ) THEN
        kf1 = 3
        kf2 =-3
      ELSEIF ( jchan .EQ. 15 ) THEN
        IF ( ism .EQ. 1 ) THEN
          kf1 = 51
          kf2 = 51
        ELSE
          kf1 = 50+ichn(1)
          kf2 = 50+ichn(2)
        ENDIF
      ELSEIF ( jchan .EQ. 16 ) THEN
        kf1 = 54+ichn(1)
        kf2 =-54-ichn(2)
      ENDIF
C
      n7lu = n7lu + 1
      ijoin(1) = n7lu
      CALL hhlu1(ijoin(1),kf1,pvect4(1,1),pvect4(2,1),
     .                    pvect4(3,1),pvect4(4,1),pvect4(5,1))
      k7lu(iph,4) = n7lu
      k7lu(ijoin(1),3) = iph
C
      n7lu = n7lu + 1
      ijoin(2) = n7lu
      CALL hhlu1(ijoin(2),kf2,pvect4(1,2),pvect4(2,2),
     .                    pvect4(3,2),pvect4(4,2),pvect4(5,2))
      k7lu(iph,5) = n7lu
      k7lu(ijoin(2),3) = iph
C
C Parton shower preparation in case of a quark system
C
      IF ( (jchan.GE.4 .AND. jchan.LE.6) .OR.
     &                       jchan.EQ.2  .OR.
     &                       jchan.EQ.14      ) THEN
        k7lu(ijoin(1),1) = 2
        njoin = 2
        CALL pyjoin(njoin,ijoin)
        xmm = p7lu(iph,5)
        CALL pyshow(ijoin(1), ijoin(2), xmm)
      ENDIF
C
C Decay and parton shower if WW or ZZ
C
      IF ( jchan .EQ. 7 .OR. jchan .EQ. 8 ) THEN
        CALL wzdecy(ijoin(1))
        CALL wzdecy(ijoin(2))
      ENDIF
C
      k7lu(iph,1) = 11
C
      IF ( idbg .GE. 5 ) CALL pylist(1)
  999 RETURN
      END
      SUBROUTINE hzha(ipro,ecms,qg1,qg2,qh,qa,hh,izpol)
C-----------------------------------------------------------------------
C! Event generation for the various Higgs production processes
C
C  Input :       ipro,   the process Id
C                ecms,   the centre-of-mass energy
C
C  Output:       qg1[4]
C                qg2[4], the initial state photons
C                qh[4]
C                qa[4]
C                hh[4],  the final state particles (Higgs,Z,fermions,gamma)
C                izpol,  the Z polarization, when IKLEI=0, for hZ.
C
C   Patrick Janot -- 27 Aug 1991
C----------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION qg1(4),qg2(4),qh(4),qa(4),rh(4),ra(4),rg2(4)
      DIMENSION gh(4),hh(4)
      LOGICAL first
      DATA first/.TRUE./
      include 'lundcom.inc'
      include 'hhprod.inc'
      include 'miscl.inc'
      include 'sigmas.inc'
C
      s = ecms**2
      e = ecms/2.
      CALL vzero(qg1(1),4*2)
      CALL vzero(qg2(1),4*2)
      CALL vzero(rg2(1),4*2)
      CALL vzero(qh(1),4*2)
      CALL vzero(ra(1),4*2)
      CALL vzero(rh(1),4*2)
      CALL vzero(qa(1),4*2)
      CALL vzero(hh(1),4*2)
      CALL vzero(gh(1),4*2)
C
C  First compute the total cross section with Brems- or beams-strahlung
C  (if requested)
C
      IF ( first ) THEN
C
        CALL vzero(sthr(1),maxpro*2)
C
C Bremsstrahlung- Beamstrahlung
C
        IF ( xrad .GT. 0D0 ) THEN
C
          IF ( iklei .EQ. 0 ) THEN
            sthr( 1) = (pmas(23,1)+pmas(25,1)-10.*pmas(23,2))**2
            sthr( 2) = (pmas(23,1)+pmas(35,1)-10.*pmas(23,2))**2
          ELSE
            sthr( 1) = 0.1
            sthr( 2) = 0.1
          ENDIF
          sthr( 3) = 0.1
          sthr( 4) = 0.1
          sthr( 5) = 0.1
          sthr( 6) = 0.1
          sthr( 7) = 0.1
          sthr( 8) = 0.1
          sthr( 9) = 0.1
          sthr(10) = pmas(25,1)**2
C
          IF (cross(1)*iproyn(1) .GT. 0. .OR.
     &        cross(5)*interf(5)*iproyn(5) .GT. 0. .OR.
     &        cross(7)*interf(7)*iproyn(7) .GT. 0. )
     &      CALL remt1(e,sigma1,sthr(1),1,cross(1),xrad)
C
          IF (cross(2)*iproyn(2) .GT. 0. .OR.
     &        cross(6)*interf(6)*iproyn(6) .GT. 0. .OR.
     &        cross(8)*interf(8)*iproyn(8) .GT. 0. )
     &      CALL remt1(e,sigma2,sthr(2),2,cross(2),xrad)
C
          IF (cross(3)*iproyn(3) .GT. 0.)
     &      CALL remt1(e,sigma3,sthr(3),3,cross(3),xrad)
C
          IF (cross(4)*iproyn(4) .GT. 0.)
     &      CALL remt1(e,sigma4,sthr(4),4,cross(4),xrad)
C
          IF (cross(5)*iproyn(5) .GT. 0.) THEN 
            CALL remt1(e,sigma5,sthr(5),5,cross(5),xrad)
            IF ( interf(5) .EQ. 1 ) THEN 
              cross1 = sigma1(s)
              chnnz(5) = chnnz(5) * cross(1)/cross1
              chnns(5) = cross(5)/reduc(5)-chnnz(5)
              chnni(5) = 0D0
            ENDIF
          ENDIF
C
          IF (cross(6)*iproyn(6) .GT. 0.) THEN 
            CALL remt1(e,sigma6,sthr(6),6,cross(6),xrad)
            IF ( interf(6) .EQ. 1 ) THEN 
              cross2 = sigma2(s)
              chnnz(6) = chnnz(6) * cross(2)/cross2
              chnns(6) = cross(6)/reduc(6)-chnnz(6)
              chnni(6) = 0D0
            ENDIF
          ENDIF
C
          IF (cross(7)*iproyn(7) .GT. 0.) THEN 
            CALL remt1(e,sigma7,sthr(7),7,cross(7),xrad)
            IF ( interf(7) .EQ. 1 ) THEN 
              cross1 = sigma1(s)
              chnnz(7) = chnnz(7) * cross(1)/cross1
              chnns(7) = cross(7)/reduc(7)-chnnz(7)
              chnni(7) = 0D0
            ENDIF
          ENDIF

          IF (cross(8)*iproyn(8) .GT. 0.) THEN 
            CALL remt1(e,sigma8,sthr(8),8,cross(8),xrad)
            IF ( interf(8) .EQ. 1 ) THEN 
              cross2 = sigma2(s)
              chnnz(8) = chnnz(8) * cross(2)/cross2
              chnns(8) = cross(8)/reduc(8)-chnnz(8)
              chnni(8) = 0D0
            ENDIF
          ENDIF
C
          IF (cross(9)*iproyn(9) .GT. 0.)
     &      CALL remt1(e,sigma9,sthr(9),9,cross(9),xrad)
C
          IF (cross(10)*iproyn(10) .GT. 0.)
     &      CALL remt1(e,sigma10,sthr(10),10,cross(10),xrad)
C
          WRITE(6,1000) (chapro(i),cross(i),iproyn(i),i=1,maxpro)
 1000     FORMAT(
     .     //20x,'------------------------------------------------'/
     .       20x,'Final cross sections :'//
     .    10(20x,'     o ',A14,'  : ',E10.4,' fb  (',I1,')'/)/
     .       20x,' (0) = Channel not requested, only Born level.'/
     .       20x,' (1) = Channel requested, ISR included.'/
     .       20x,'------------------------------------------------'//)
        ENDIF
        first = .FALSE.
      ENDIF
C
C  Event generation
C
      IF ( xrad .GT. 0. ) THEN
 1      CALL remt2(qg1,rg2,ipro)
        s1 = s * (1.-qg1(4)/e)
        e1 = SQRT(s1)/2.
        s2 = s1 * (1.-rg2(4)/e1)
        IF ( s2 .LE. sthr(ipro) ) THEN
          WRITE(6,*) ' *** Warning *** Not enough energy for production'
     .               ,s2,' < ',sthr(ipro),' !'
          GOTO 1
        ENDIF
        CALL dsigma(ipro,s2,qh,qa,hh,izpol,iret)
        IF ( iret .NE. 0 ) GOTO 1
C
        IF ( ipro .LT. 5 .OR. ipro .GE. 9 ) THEN
          qhm = qh(4)
          qam = qa(4)
          qh(4) = SQRT(qh(1)**2+qh(2)**2+qh(3)**2+qh(4)**2)
          qa(4) = SQRT(qa(1)**2+qa(2)**2+qa(3)**2+qa(4)**2)
        ELSE
          hhm = hh(4)
          hh(4) = SQRT(hh(1)**2+hh(2)**2+hh(3)**2+hh(4)**2)
        ENDIF
C
        CALL remt3(qh, rh, 2)
        CALL remt3(qa, ra, 2)
        CALL remt3(rh, qh, 1)
        CALL remt3(ra, qa, 1)
        IF ( ( ipro.GE.5 .AND. ipro.LE.8 ) .OR.
     .       ( ipro.LE.2.AND.iklei.EQ.1 ) ) THEN
          CALL remt3(hh, gh, 2)
          CALL remt3(gh, hh, 1)
        ENDIF
        CALL remt3(rg2,qg2,1)
C
        IF ( ipro .LT. 5 .OR. ipro .GE. 9 ) THEN
          qh(4) = qhm
          qa(4) = qam
        ELSE
          hh(4) = hhm
        ENDIF
C
      ELSE
 11     call dsigma(ipro,s,qh,qa,hh,izpol,iret)
        IF ( iret .NE. 0 ) GOTO 11
      ENDIF
C
C  End of event generation
C
      RETURN
      END
      SUBROUTINE LORENZ(BETAX,BETAY,BETAZ,P)
C-------------------------------------------------------------------
C!  Perform a Lorentz transformation of the quadriimpulsion P
C   from frame 1 to frame 2
C
C   Input:     Passed:    --BETAX,Y,Z    2's velocity / 1
C                         --P,           quadriimpulsion in 1
C
C   Output:    Passed:    --P,           quadriimpulsion in 2
C
C   P. Janot  --  20 oct 1988
C-------------------------------------------------------------------
      IMPLICIT REAL*8 (A-Z)
      DIMENSION P(4)
      BETA2 = BETAX**2 + BETAY**2 + BETAZ**2
      IF(BETA2 .EQ. 0.) RETURN
      GAMMA = 1./SQRT(1.-BETA2)
      ONE   = BETAX*P(1) + BETAY*P(2) + BETAZ*P(3)
      TWO   = (GAMMA-1.)*ONE/BETA2- GAMMA*P(4)
      P(1)  = P(1) + BETAX*TWO
      P(2)  = P(2) + BETAY*TWO
      P(3)  = P(3) + BETAZ*TWO
      P(4)  = GAMMA*(-ONE+P(4))
      RETURN
      END
      SUBROUTINE neutra
C------------------------------------------------------------------
C!  Compute neutralino masses from the MSSM parameters
C
C  Input:    /PARAM/ MSSM parameters
C
C  Output:   /PARAM/ amneut(4),   the neutralino masses.
C
C  P. Janot -- 4 December 1994
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      REAL*4 y(4,4), wr(4), zr(4,4), rr(4,4), work(4)
      INTEGER ir(4)
      include 'param.inc'
      include 'datapi.inc'
      include 'consts.inc'
      include 'miscl.inc'
C
      susM1 = susM * 5./(8.*(1.-sw2))
      susM2 = susM * 3./(8.*sw2)
C
c The neutralino mass matrix Y(4,4)
C
      y(1,1) =  susM1
      y(1,2) =  0.
      y(1,3) = -amz*sw*cb
      y(1,4) =  amz*sw*sb
      y(2,1) =  y(1,2)
      y(2,2) =  susM2
      y(2,3) =  amz*cw*cb
      y(2,4) = -amz*cw*sb
      y(3,1) =  y(1,3)
      y(3,2) =  y(2,3)
      y(3,3) =  0.
      y(3,4) = -susMU
      y(4,1) =  y(1,4)
      y(4,2) =  y(2,4)
      y(4,3) =  y(3,4)
      y(4,4) =  0.
C
C  Diagonalization of the neutralino mass matrix;
C  ZR corresponds to the matrix N transpose; the eigenvalues
C  are contained in WR, and might be <0 or >0 depending of
C  the CP quantum number of the neutralino
C
      CALL eisrs1 (4,4,y,wr,zr,ierr,work)
      CALL ucopy(zr(1,1),rr(1,1),16)
      DO 20 i = 1, 4
        work(i) = ABS(wr(i))
   20 CONTINUE
C
C Sort the neutralinos in increasing mass order
C
      CALL sortzv(work,ir,4,1,0,0)
      DO 10 i = 1, 4
        k         = ir(i)
        amneut(i) = wr(k)
        DO 11 j = 1, 4
          zr(j,i) = rr(j,k)
   11   CONTINUE
   10 CONTINUE
C      CALL ucopy(zr(1,1),fieldn(1,1),4*4)
C
C Matrices Q''(ij) and S''(ij)
C
      DO 1 i=1,4
        DO 2 j=1,4
          tw=sw/cw
          qqmat(i,j) = ( zr(3,i)*(zr(2,j)-tw*zr(1,j))
     .               +   zr(3,j)*(zr(2,i)-tw*zr(1,i)) )
          ssmat(i,j) = ( zr(4,i)*(zr(2,j)-tw*zr(1,j))
     .               +   zr(4,j)*(zr(2,i)-tw*zr(1,i)) )
          fieldn(i,j) = zr(i,j)
    2   CONTINUE
    1 CONTINUE
C
      RETURN
      END
      SUBROUTINE PARCOU
C------------------------------------------------------------------
C!  Compute parameters, couplings
C
C  Input:    /PARAM/ SM and MSSM parameters
C
C  Output:   /PARAM/ Relevant couplings, masses, angles...
C
C   P. Janot -- 24 August 1991
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C      REAL*4 pyalem
      include 'param.inc'
      include 'hhdeck.inc'
      include 'lundcom.inc'
      include 'datapi.inc'
      include 'consts.inc'
      include 'miscl.inc'
C
C Quark and lepton masses
C
      amb   = pmas(5,1)
      amc   = pmas(4,1)
      amtau = pmas(15,1)
      ame   = pmas(11,1)
      ammu  = pmas(13,1)
      amu   = .005
      amd   = .010
      ams   = .150
C
C Z mass and width, W width.
C
      amz    = pmas(23,1)
      gmz    = pmas(23,2)
      gmw    = pmas(24,2)
C
C QED coupling constant at the Z
C
      alpha(0)  = pyalem(amz**2)
      write(*,*) "alpha(0)"
      write(*,*) alpha(0)
      alpha2    = 0.38937966E12 * alpha(0)**2
C
C QCD coupling constant at the Z
C
      fnz       = 5D0
      alphas(0) = pi*runalf(amz,fnz)
C
C Sin**2 theta_E effective fine tuned from G_F, alpha_QED, mZ
C
      sw2    = 0.5*(1.-SQRT(1.-4.*pi*alpha(0)/(SQRT(2.)*G_F*amz**2)))
      sw     = SQRT(sw2)
      cw2    = 1.-sw2
      cw     = SQRT(cw2)
C
C Miscallaneous
C
      gweak2(0) = 4. * pi * alpha(0) / sw2
      deltar    = 3.*G_F*amt**2/(8*pi**2*SQRT(2.))
      paru(102) = sw2
C
C W mass from mZ, weinberg angle and delta(rho)
C
      amw    = amz*cw/SQRT(1.-deltar)
C
C Running Delta rho to compute cross sections
C
      alpha_3 = runalf(amt,5D0) * pi
      factork = 16.11
     .       - 1.04*(4.-amu/amt-amd/amt-ams/amt-amc/amt-amb/amt)
      amtcor = (1. + 4./3. *alpha_3/pi + factork*(alpha_3/pi)**2 )
      deltar = deltar / amtcor**2
C
C Anomalous couplings
C
      dkappag = amw**2 * (    fw +     fb) / 2D0
      dkappaz = amw**2 * (cw2*fw - sw2*fb) / 2D0 / cw2
      dg1z    = amw**2 *      fw           / 2D0 / cw2
C
C Relevant print out
C
      IF ( idbg .GE. 0 ) THEN
        WRITE(6,1003) amz,gmz,amt,g_f,xlamda5
        WRITE(6,1004) 1./alpha(0),sw2,deltar,amw,alphas(0)
C
        IF ( fbb .NE. 0D0 .OR. fww .NE. 0D0 .OR.
     &       fb  .NE. 0D0 .OR. fw  .NE. 0D0 ) THEN
          WRITE(6,3001) fbb*1D6,fww*1D6,fb*1D6,fw*1D6
          WRITE(6,3002) dkappag,dkappaz,dg1z
        ENDIF
      ENDIF
C
      IF ( ism .EQ. 0 ) THEN
C
C First check if we are in SUGRA framework
C
        IF ( susM0 .GE. 0. .AND. susM12 .GE. 0 ) THEN 
C
          susM    = 0.82 * susM12 * 8. * sw2 / 3.  ! SU(2)*U(1) gauginos
          susM3   = 2.87 * susM12                  ! SU(3)      gauginos
C
          call scalar(tb)
          IF ( susMSNU .LT. 0. ) THEN 
            WRITE(6,*) 'problems calculating soft SUSY breaking terms'
            WRITE(6,*) 'sorry...'
            RETURN
          ENDIF
C
        ELSE
C
          susM    = susM2 * 8. * sw2 / 3.  ! SU(2)*U(1)    gauginos
          IF ( susGLU .LE. 0D0 ) THEN 
            susM3 = susM2 * 3.5            ! SU(3) unified gauginos
          ELSE
            susM3 = susGLU                 ! SU(3) gauginos non-unified
          ENDIF
C
        ENDIF
C
C Now ... The MSSM !
C
        beta = ATAN(tb)
        cb = COS(beta)
        sb = SIN(beta)
        amsq = susSMQ
C
C Compute the S-top and S-bottom masses. Other squark splitting is
C assumed to be negligible.
C
        IF ( icar .NE. 1 .AND. icar .NE. 2 ) THEN 
          CALL squarks
          IF ( amst(1) .LE. 0. .OR. amsb(1) .LE. 0. ) THEN
            IF ( idbg .GE. 0 )
     .           WRITE(6,12001) amst(1),amst(2),amsb(1),amsb(2)
            RETURN
          ENDIF
        ENDIF
C
C Compute the neutralino masses
C
        CALL neutra
C
C Compute the chargino masses
C
        CALL chargi
C
C Compute the neutral and charged Higgs masses, together with the
C mixing angle alpha in the CP-even sector
C
        CALL shiggs
C
        IF ( amh .LE. 0. .OR.
     &     ( ism .LE. 0 .AND. ( gmh.LE.0. .OR.
     .       ama .LE. 0. .OR.  amhp.LE.0. ) ) ) THEN
          IF ( idbg .GE. 0 ) WRITE(6,11001) amh,gmh,ama,amhp
          RETURN
        ENDIF
C
        IF ( icar .EQ. 1 .OR. icar .EQ. 2 ) THEN 
          IF ( amst(1) .LE. 0. .OR. amsb(1) .LE. 0. ) THEN
            IF ( idbg .GE. 0 )
     .           WRITE(6,12001) amst(1),amst(2),amsb(1),amsb(2)
            RETURN
          ENDIF
        ENDIF
C
C Compute the chargino and neutralino couplings to the Higgs bosons
C
        CALL chaneucp
C
C Compute the decay branching ratios of the neutralinos in chi Z*
C in chi gamma and in chi+/- W*+/-
C
        CALL chidec
C
      ELSE IF( ism.EQ.-1 ) THEN
C
C Two Higgs doublet Type II model
C
        beta = ATAN( tb )
        ta   = TAN( alfa )
        cb   = COS(beta)
        sb   = SIN(beta)
        ca   = COS(alfa)
        sa   = SIN(alfa)
        c2b  = (1.-tb**2)/(1.+tb**2)
        s2b  = 2.*tb/(1.+tb**2)
        sab2 = SIN(beta-alfa)**2
        cab2 = COS(beta-alfa)**2
        cab  = COS(alfa+beta)
        sab  = SIN(alfa+beta)
        c2a  = COS(2.*alfa)
C
C Bug fix (03/02/2000) so as to allow the neutral Higgs bosons
C to decay into b bbar even in type II models!
C
        corhbb = 1D0
        corgbb = 1D0
C
      ELSE
C
C The Standard Model !
C
        ama  = 0.
        gmh  = 0.
        beta =  pi/4.
        alfa = -pi/4.
        tb   = 1.
        ta   =-1.
        c2b  = 0.
        s2b  = 1.
        sab2 = 1.
        cab2 = 0.
        cb = COS(beta)
        sb = SIN(beta)
        ca = COS(alfa)
        sa = SIN(alfa)
        cab = 1.
        sab = 0.
        c2a = 0.
C
      ENDIF
C
      amhig(1) = gmh
      amhig(2) = amh
      amhig(3) = ama
      pmas(24,1) = amw
      pmas(25,1) = amh
      pmas(35,1) = gmh
      pmas(36,1) = ama
      chaf(25,1) = 'h       '
      chaf(35,1) = 'H       '
      chaf(36,1) = 'A       '
C     mdme(337,1) = 0
      mdme(174,1) = 0
      mdme(153,1) = 0
C
C The QED/QCD running coupling constants at mh, mH, mA
C
        DO jhig = 1, nhig
          fnh = 3D0
          xmh          = amhig(jhig)
          IF ( xmh .LE. 1. ) xmh = 1.
          IF ( amc .LE. xmh/2. ) fnh = fnh + 1.
          IF ( amb .LE. xmh/2. ) fnh = fnh + 1.
          IF ( amt .LE. xmh/2. ) fnh = fnh + 1.
C
          alphas(jhig) = pi*runalf(xmh,fnh)
          alpha (jhig) = pyalem(xmh**2)
          gweak2(jhig) = 4D0 * pi * alpha(jhig) / sw2
C
        ENDDO
C
C Print out...
C
      IF ( idbg .GE. 0 ) THEN
        IF ( ism .EQ. 0 ) THEN
          WRITE(6,1000) amarun,tb,susM2,susMU,susAt,susAB,susSMQ,
     .                               susSMU,susSMD,susSML,susSME
          WRITE(6,1001) amh,gmh,ama,amhp
          WRITE(6,2001) amst(1),amst(2),topmix,
     .                  amsb(1),amsb(2),botmix
          WRITE(6,2003) amneut
          WRITE(6,2004) amchar
          WRITE(6,2014) susM3
C
          IF ( amchar(1) .LT. ABS(amneut(1)) ) WRITE(6,2008)
C
          DO ineut = 1, 4
            WRITE(6,2005) ineut,amneut(ineut)
            DO ic = 1, nchneut
              IF ( brneut(ic,ineut) .GT. 0. )
     .        WRITE(6,2007) channeut(ic,ineut),100.*brneut(ic,ineut)
            ENDDO
          ENDDO
C
          DO ichar = 1, 2
            WRITE(6,2006) ichar,amchar(ichar)
            DO ic = 1, nchchar
              IF ( brchar(ic,ichar) .GT. 0. )
     .        WRITE(6,2007) chanchar(ic,ichar),100.*brchar(ic,ichar)
            ENDDO
          ENDDO
C
          sasb = sa/sb
          cacb = ca/cb
          casb = ca/sb
          sacb = sa/cb
          WRITE(6,2002) sin(beta-alfa),cos(beta-alfa),
     .                  sasb,cacb*corgbb,cacb,
     &                  casb,sacb*corhbb,sacb,
     &                  1./tb,tb
        ELSE IF( ism.EQ.-1 ) THEN
          WRITE(6,4000) amh,gmh,ama,amhp,tb,alfa
C
          sasb = sa/sb
          cacb = ca/cb
          casb = ca/sb
          sacb = sa/cb
          WRITE(6,4001) sin(beta-alfa),cos(beta-alfa),
     .                  sasb,cacb,casb,sacb,1./tb,tb
        ENDIF
      ENDIF
C
C Fill internal mass table
C
      CALL vzero(xymas(1,1,1),2*nchan*nhig*2)
      DO jhig = 1 , nhig
        xymas(1,1,jhig) = 0.
        xymas(1,2,jhig) = 0.
        xymas(1,3,jhig) = amtau
        xymas(1,4,jhig) = amc
        xymas(1,5,jhig) = amb
        xymas(1,6,jhig) = amt
        xymas(1,7,jhig) = amw
        xymas(1,8,jhig) = amz
        xymas(1,9,jhig) = ama
        xymas(1,10,jhig) = amh
        xymas(1,11,jhig) = 0.
        xymas(1,12,jhig) = ame
        xymas(1,13,jhig) = ammu
        xymas(1,14,jhig) = ams
        xymas(1,15,jhig) = 0.
        xymas(1,16,jhig) = 0.
        DO jchan = 1 , nchan
          xymas(2,jchan,jhig) = xymas(1,jchan,jhig)
        ENDDO
        xymas(2,11,jhig) = amz
      ENDDO
      xymas(1,9,3) = amh
      xymas(2,9,3) = amz
C------------------------------------------------------------------
 1000 FORMAT(1x,50('-')//
     .       1x,'With the following input parameters of the MSSM :'/
     .       1x,'   . A mass[Run] : ',F10.3,' GeV/c**2'/
     .       1x,'   . Tan beta    : ',F10.3/
     .       1x,'   . M2          : ',F10.3,' GeV/c**2'/
     .       1x,'   . mu          : ',F10.3,' GeV/c**2'/
     .       1x,'   . At          : ',F10.3/
     .       1x,'   . Ab          : ',F10.3/
     .       1x,'   . mQ          : ',F10.3,' GeV/c**2'/
     .       1x,'   . mU          : ',F10.3,' GeV/c**2'/
     .       1x,'   . mD          : ',F10.3,' GeV/c**2'/
     .       1x,'   . mL          : ',F10.3,' GeV/c**2'/
     .       1x,'   . mE          : ',F10.3,' GeV/c**2'/)
 1001 FORMAT(1x,'we found the following Higgs masses :'/
     .       1x,'   . h mass      : ',F10.3,' GeV/c**2'/
     .       1x,'   . H mass      : ',F10.3,' GeV/c**2'/
     .       1x,'   . A mass[Pole]: ',F10.3,' GeV/c**2'/
     .       1x,'   . H+/- mass   : ',F10.3,' GeV/c**2'/)
 1002 FORMAT(1x,'and sin**2(beta-alfa) = ',F8.6//
     .       1x,50('-')//)
 1003 FORMAT(1x,50('-')//
     .       1x,'With the following input parameters :'/
     .       1x,'   . Z mass      : ',F10.4,' GeV/c**2'/
     .       1x,'   . Z width     : ',F10.4,' GeV'/
     .       1x,'   . top mass    : ',F10.4,' GeV/c**2'/
     .       1x,'   . G_F         : ',E10.4,' GeV**-2'/
     .       1x,'   . Lamba_QCD(5): ',F10.4,' GeV'/)
 1004 FORMAT(1x,'we found the following values :'/
     .       1x,'   . alpha(mZ)   : 1/',F8.3/
     .       1x,'   . sin**2(Th_W): ',F10.4/
     .       1x,'   . delta(rho)  : ',E10.4/
     .       1x,'   . W mass      : ',F10.4,' GeV/c**2'/
     .       1x,'   . alphas(mZ)  : ',F10.4,/)
 2001 FORMAT(1x,' and ... the following squark masses :'/
     .       1x,'   . t~1 mass    : ',F10.3,' GeV/c**2'/
     .       1x,'   . t~2 mass    : ',F10.3,' GeV/c**2'/
     .       1x,'   . Mixing      : ',F10.4,' rd.'/
     .       1x,'   . b~1 mass    : ',F10.3,' GeV/c**2'/
     .       1x,'   . b~2 mass    : ',F10.3,' GeV/c**2'/
     .       1x,'   . Mixing      : ',F10.4,' rd.'/)
 2003 FORMAT(1x,' and ... the following neutralino masses :'/
     .       1x,'   . chi(1) mass : ',F10.3,' GeV/c**2'/
     .       1x,'   . chi(2) mass : ',F10.3,' GeV/c**2'/
     .       1x,'   . chi(3) mass : ',F10.3,' GeV/c**2'/
     .       1x,'   . chi(4) mass : ',F10.3,' GeV/c**2'/)
 2004 FORMAT(1x,' and ... the following chargino masses :'/
     .       1x,'   . chi1+- mass : ',F10.3,' GeV/c**2'/
     .       1x,'   . chi2+- mass : ',F10.3,' GeV/c**2'/)
 2014 FORMAT(1x,' and ... the following gluino mass :'/
     .       1x,'   . Gluino mass : ',F10.3,' GeV/c**2'/)
 2005 FORMAT(/1x,'Branching ratios for the neutralino ',
     .           I1, ' (mass ',F8.3, ' GeV/c**2)')
 2006 FORMAT(/1x,'Branching ratios for the chargino ',
     .           I1, ' (mass ',F8.3, ' GeV/c**2)')
 2007 FORMAT(10x,'o ',A21,1x,20('.'),1x,F9.5,'%')
 2008 FORMAT(//'                  +++ WARNING +++'/
     .       '      The ligthest neutralino is NOT the LSP ! '/
     .       '      The program  will stop  when  trying to '/
     .       '           decay the lightest chargino'/
     .       '                  +++ WARNING +++'//)
 2002 FORMAT(/1x,' and ... the following couplings :'/
     .       1x,'   . hWW or hZZ  : ',F8.3,/
     .       1x,'   . HWW or HZZ  : ',F8.3,/
     .       1x,'   . Htt         : ',F8.3,/
     .       1x,'   . Hbb         : ',F8.3,/
     .       1x,'   . Htautau     : ',F8.3,/
     .       1x,'   . htt         : ',F8.3,/
     .       1x,'   . hbb         : ',F8.3,/
     .       1x,'   . htautau     : ',F8.3,/
     .       1x,'   . Att         : ',F8.3,/
     .       1x,'   . Abb         : ',F8.3,/)
 3001 FORMAT(1x,50('-')//
     .       1x,'The anomalous couplings are :'/
     .       1x,'   . fBB / Lambda**2 : ',F10.4,' TeV**-2'/
     .       1x,'   . fWW / Lambda**2 : ',F10.4,' TeV**-2'/
     .       1x,'   . fB  / Lambda**2 : ',F10.4,' TeV**-2'/
     .       1x,'   . fW  / Lambda**2 : ',F10.4,' TeV**-2'/)
 3002 FORMAT(1x,'corresponding to :'/
     .       1x,'   . D_Kappa_g   : ',F10.4/
     .       1x,'   . D_Kappa_Z   : ',F10.4/
     .       1x,'   . D_g_1^Z     : ',F10.4/)
11001 FORMAT(1x,' +++ Execution will stop due to ',
     .                'negative Higgs mass(es) +++'/
     .       1x,'   . h mass      : ',F12.3,' GeV/c**2'/
     .       1x,'   . H mass      : ',F12.3,' GeV/c**2'/
     .       1x,'   . A mass[Pole]: ',F12.3,' GeV/c**2'/
     .       1x,'   . H+/- mass   : ',F12.3,' GeV/c**2'/)
12001 FORMAT(1x,' +++ Execution will stop due to ',
     .                'negative squark mass(es) +++'/
     .       1x,'   . t~1 mass    : ',F12.3,' GeV/c**2'/
     .       1x,'   . t~2 mass    : ',F12.3,' GeV/c**2'/
     .       1x,'   . b~1 mass    : ',F12.3,' GeV/c**2'/
     .       1x,'   . b~2 mass    : ',F12.3,' GeV/c**2'/)
 4000 FORMAT(1x,'With the following parameters for the Higgs sector:'/
     .       1x,'   . h mass      : ',F10.3,' GeV/c**2'/
     .       1x,'   . H mass      : ',F10.3,' GeV/c**2'/
     .       1x,'   . A mass[Pole]: ',F10.3,' GeV/c**2'/
     .       1x,'   . H+/- mass   : ',F10.3,' GeV/c**2'/
     .       1x,'   . tan(beta)   : ',F10.3/
     .       1x,'   . alpha       : ',F10.3/)
 4001 FORMAT(/1x,' we found the following couplings :'/
     .       1x,'   . hWW or hZZ  : ',F8.3,/
     .       1x,'   . HWW or HZZ  : ',F8.3,/
     .       1x,'   . Htt         : ',F8.3,/
     .       1x,'   . Hbb         : ',F8.3,/
     .       1x,'   . htt         : ',F8.3,/
     .       1x,'   . hbb         : ',F8.3,/
     .       1x,'   . Att         : ',F8.3,/
     .       1x,'   . Abb         : ',F8.3,/)
  999 RETURN
      END
      DOUBLE PRECISION FUNCTION phspgz(a,b,c)
C ------------------------------------------------------------------
C! Phase space factor for  h --> gamma Z
C
C  Inputs:        a   is the Higgs mass squared
C                 b   is the Z mass
C                 c,  is the Z width
C
C  Output:        phspgz, the hZ cross section with width effects
C
C  Patrick Janot -- 01 Sep 1995
C -------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
C      REAL*4 a,b,c
      COMMON /BHV/ s,am1,w1
      EXTERNAL fsubgz
      DIMENSION x(1)
      include 'datapi.inc'
      s      = a
      am1    = b
      w1     = c
      xlo    = -DATAN2(am1,w1)
      xhi    =  DATAN2(s-am1**2,am1*w1)
      phspgz = DGMLT1(fsubgz,xlo,xhi,1,6,x)
     .       / (piby2+DATAN2(am1,w1))
      RETURN
      END
      SUBROUTINE pick4(ph,jchan,jhig)
C------------------------------------------------------------------
C!  Derive the quadrimomenta of the Higgs decay particles
C
C  P. Janot --  26 Aug 1991
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'hhdeck.inc'
      include 'datapi.inc'
      DIMENSION ptot(4),pp(4),qq(4),cormas(2)
      REAL*8 betax, betay, betaz, p4
      CALL vzero(pvect4(1,1),10*2)
C
      amloc = ph(4)
C
C Generate mass according to the width
C
      DO idc = 1 , 2
        IF ( idbg .GE. 10 ) WRITE(6,*) idc
     .      ,xymas(idc,jchan,jhig)
     .      ,xywid(idc,jchan,jhig)
        IF ( xywid(idc,jchan,jhig) .GT. 0.05 ) THEN
          x = xymas(idc,jchan,jhig)
          g = xywid(idc,jchan,jhig)
          IF ( idc .EQ. 1 ) THEN
            xmin = 2.*ame
            xmax = amloc-xmin
          ELSE
            xmin = 2.*ame
            xmax = amloc-cormas(1)
          ENDIF
          CALL bwgene(xmin,xmax,x,g,cormas(idc),djdummy)
        ELSE
          cormas(idc) = xymas(idc,jchan,jhig)
        ENDIF
      ENDDO
C
C  Special treatment for WW, ZZ, Z gamma
C
      IF ( jchan .EQ. 7 .OR. jchan .EQ. 8 ) THEN
        xma2 = xmax**2
        xh2  = amloc**2
        xm12 = cormas(1)**2
    1   xm22 = cormas(2)**2
        weight = SQRT((xh2-xm12-xm22)**2-4.*xm12*xm22)
     .         * (xm12*xm22 + ((xh2-xm12-xm22)**2-4.*xm12*xm22)/12.)
        wmax   = (xh2-xm12) * (xm12*xma2 + (xh2-xm12)**2/12.)
        IF ( weight/wmax .LT. RNDM2(weight) ) THEN
          CALL bwgene(xmin,xmax,x,g,cormas(2),djdummy)
          GOTO 1
        ENDIF
C
        IF ( RNDM2(xm1) .GT. 0.5D0 ) THEN
          xm1 = cormas(1)
          cormas(1) = cormas(2)
          cormas(2) = xm1
        ENDIF
C
      ENDIF
C
C  Special treatment for Z gamma
C
      IF ( jchan .EQ. 11 ) THEN
    2   weight = (1.-cormas(2)**2/amloc**2)**3
        IF ( weight .LT. RNDM2(weight) ) THEN
          CALL bwgene(xmin,xmax,x,g,cormas(2),djdummy)
          GOTO 2
        ENDIF
      ENDIF
C
C Compute quadri-momenta
C
      pvect4(4,1) = (amloc**2+cormas(1)**2-cormas(2)**2)
     .            / (amloc*2.)
      pvect4(4,2) = (amloc**2+cormas(2)**2-cormas(1)**2)
     .            / (amloc*2.)
      pmom2       = (amloc**2-(cormas(1)+cormas(2))**2)
     .            * (amloc**2-(cormas(1)-cormas(2))**2)
      pmom = SQRT(pmom2)/(2.*amloc)
      c = 2.*rndm2(c) - 1.
      p = 2.*pi*rndm2(p)
      s = SQRT(1.-c**2)
      pvect4(3,1) = pmom * c
      pvect4(2,1) = pmom * s * SIN(p)
      pvect4(1,1) = pmom * s * COS(p)
      pvect4(3,2) =-pvect4(3,1)
      pvect4(2,2) =-pvect4(2,1)
      pvect4(1,2) =-pvect4(1,1)
C
C  Boost back to the lab
C
      CALL ucopy(pvect4(1,1),pp(1),4*2)
      CALL ucopy(pvect4(1,2),qq(1),4*2)
      betax = -ph(1)
      betay = -ph(2)
      betaz = -ph(3)
      p4 = SQRT(amloc**2+betax**2+betay**2+betaz**2)
      betax = betax/p4
      betay = betay/p4
      betaz = betaz/p4
      CALL lorenz(betax,betay,betaz,pp)
      CALL lorenz(betax,betay,betaz,qq)
      CALL ucopy(pp(1),pvect4(1,1),4*2)
      CALL ucopy(qq(1),pvect4(1,2),4*2)
      pvect4(5,1) = cormas(1)
      pvect4(5,2) = cormas(2)
C
      amas1 = (pvect4(4,1)**2
     .      -  pvect4(1,1)**2
     .      -  pvect4(2,1)**2
     .      -  pvect4(3,1)**2)
      amas2 = (pvect4(4,2)**2
     .      -  pvect4(1,2)**2
     .      -  pvect4(2,2)**2
     .      -  pvect4(3,2)**2)
      pxtot = pvect4(1,1) + pvect4(1,2)
      pytot = pvect4(2,1) + pvect4(2,2)
      pztot = pvect4(3,1) + pvect4(3,2)
      entot = pvect4(4,1) + pvect4(4,2)
      amtot = entot**2-pxtot**2-pytot**2-pztot**2
C
      IF ( idbg .GE. 10 ) THEN
        WRITE(6,1000) (pvect4(i,1),i=1,4),SQRT(amas1)
        WRITE(6,1000) (pvect4(i,2),i=1,4),SQRT(amas2)
        WRITE(6,1000) pxtot,pytot,pztot,entot,SQRT(amtot)
      ENDIF
C
  999 RETURN
C------------------------------------------------------------------
1000  FORMAT(1X,5(2X,F9.3))
      END
      SUBROUTINE pole(ihiggs,mchi,ma,tanb,mq,mur,mdr,mtop,at,ab,mu,mglu,
     *                mh,mhp,hm,hmp,amp,mhch,sa,ca,sab,cab,
     *                stop1,stop2,sbot1,sbot2,tanbA)
C------------------------------------------------------------------
C! Computes the Higgs pole masses and mixing angles.
C
C    Inputs: ihiggs(explained below),mchi,ma,tanb,mq,mur,mdr,mtop,
C    at,ab,mu
C
C    where mchi is the largest chargino mass, ma is the running
C    CP-odd Higgs mass, tanb is the value of the ratio of vacuum
C    expectaion values at the scale mtop,mq is the third generation
C    left handed squark mass parameter, mur is the third generation
C    right handed stop mass parameter, mdr is the third generation
C    right handed sbottom mass parameter, mtop is the pole top quark
C    mass; at,ab are the soft supersymmetry breaking trilinear
C    couplings of the stop and sbottoms, respectively, and mu is the
C    supersymmetric mass parameter.
C
C
C    Output: mh and mhp which are the lightest CP-even Higgs running
C    and pole masses, respectively; hm and hmp are the heaviest CP-even
C    Higgs running and pole masses, repectively; sa and ca are the
C    sin(alpha) and cos(alpha) where alpha is the Higgs mixing angle.
C    amp is the CP-odd Higgs pole mass. stop1,stop2,sbot1 and sbot2
C    are the stop and sbottom mass eigenvalues. Finally, tanbA is
C    the value of tanb at the CP-odd Higgs mass scale.
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCcC
ccccccccccccccccccccccccccccccccccccccccccc
ccccc  The parameter ihiggs=0,1,2,3 corresponds to the
ccccc  number of Higgses whose pole mass is computed
ccccc   by the subroutine vac(...). If ihiggs=0 only running
ccccc   masses are given, what makes the running of the program
ccccc   much faster and it is quite generally a good approximation
ccccc   (for a theoretical discussion see Ref. below).
ccccc    If ihiggs=1, only the pole
ccccc   mass for h is computed. If ihiggs=2, then h and H, and
ccccc   if ihiggs=3, then h,H,A polarizations are computed
ccccccccccccccccccccccccccccccccccccccccccccccc
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c       Program based on the work by M. Carena, M. Quiros
c       and C.E.M. Wagner, "Effective potential methods and
c       the Higgs mass spectrum in the MSSM", CERN-TH/95-157.
c
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
C
      IMPLICIT REAL*8(A-H,M,O-Z)
C
C P. Janot: First initialize all output values to -1., to avoid
C           potential problems in loops (05/03/2000)
C
      mh     = -1D0
      mhp    = -1D0
      hm     = -1D0
      hmp    = -1D0
      amp    = -1D0
      mhch   = -1D0
      stop1  = -1D0
      stop2  = -1D0
      sbot1  = -1D0
      sbot2  = -1D0
C
      CALL vac(ihiggs,mchi,ma,tanb,mq,mur,mdr,mtop,at,ab,mu,mglu,
     *        mh,mhp,hm,hmp,amp,mhch,stop1,stop2,sbot1,sbot2,
     *        sa,ca,sab,cab,stop1w,stop2w,tanbA)
C
      sinb = tanb/(tanb**2+1.)**.5
      cosb = 1./(tanb**2+1.)**.5
      sinbma = sinb*ca - cosb*sa
C
      RETURN
      END
      DOUBLE PRECISION FUNCTION prosca(p1,p2)
C-------------------------------------------------------------------
C! Compute the scalar product p1.p2
C-------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION p1(4), p2(4)
      prosca = p1(4)*p2(4) - p1(3)*p2(3) - p1(2)*p2(2) - p1(1)*p2(1)
      RETURN
      END
      SUBROUTINE radcor(xmq,jhig,fnq,r1,r2)
C-------------------------------------------------------------------
C! Compute the 1st order rad. cor. to H --> qqbar(g) width
C
C  Inputs:       --xmq,   the running quark mass
C                --jhig,  the Higgs type
C                --fnq,   the quark flavour index
C
C  Ouptuts:      --r1,    the alpha_s    correction
C                --r2,    the alpha_s**2 correction
C
C  P. Janot  --  22 nov 1989.
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      include 'param.inc'
      REAL*8 betaq, abeta
      IF( jhig.LE.3 ) THEN
        xmh = amhig(jhig)
      ELSE IF( jhig.EQ.4 ) THEN
        xmh = amhp
      END IF
      IF ( xmh .LT. 2.*xmq ) THEN
C       betaq = DSQRT(1D0-4D0*xmq**2/xmh**2)
C       abeta = (1.+betaq**2)
C    .        * (4.*DDILOG( (1D0-betaq)/(1D0+betaq))
C    .        +  2.*DDILOG(-(1D0-betaq)/(1D0+betaq))
C    .        -  3.*DLOG(2D0/(1D0+betaq))*DLOG((1D0+betaq)/(1D0-betaq))
C    .        -  2.*DLOG(betaq)*DLOG((1D0+betaq)/(1D0-betaq)))
C    .        -  3.*betaq*DLOG(4D0/(1D0-betaq**2))
C    .        -  4.*betaq*DLOG(betaq)
C       IF ( jhig .LE. 2 ) THEN
C         r1 = abeta/betaq
C    .       + (3.+34.*betaq**2-13.*betaq**4)
C    .       / (16.*betaq**3)
C    .       * DLOG((1D0+betaq)/(1D0-betaq))
C    .       + 3. * (-1.+7.*betaq**2) / (8.*betaq**2)
C       ELSEIF (jhig .EQ. 3 ) THEN
C         r1 = abeta/betaq
C    .       + (19.+2.*betaq**2+3.*betaq**4)
C    .       / (16.*betaq)
C    .       * DLOG((1D0+betaq)/(1D0-betaq))
C    .       + 3. * (7.-betaq**2) / 8.
C       ELSE
C         r1 = 3.*ALOG(xmq/xmh)
C       ENDIF
C       r1 = r1 - 3.*ALOG(xmq/xmh)
C       r2 = 0.
        r1 = 0.
        r2 = 0.
      ELSE
        r1 = 17./3.-40.*(xmq/xmh)**2
        r2 = 35.9399-1.3586*fnq
      ENDIF
C
  999 RETURN
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
      REAL*4 xrad
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
C     alf = pyalem(SNGL(s))
      alf = 1./137.035989
      xpl  = DLOG(xpt(s))
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
    3 FORMAT(' Iteration # ',i3,'  Integral =',e15.6)
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
  200 R=M*RNDM2(1D0)
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
  301 QQ =  C1*QIN(2) + S1*QIN(3)
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
      SUBROUTINE rghm(mchi,ma,tanb,mq,mur,md,mtop,au,ad,mu,mglu,
     *    mhp,hmp,mhch,sa,ca,sab,cab,tanbA)
C---------------------------------------------------------------------
C!  Computes the Higgs running masses and mixing angles
C
C   From M. Carena and C. Wagner, 21 Sep 1995
C---------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,L,M,O-Z)
      include 'ccaren.inc'
      include 'miscl.inc'
      DIMENSION VH(2,2),M2(2,2),M2P(2,2)
C
Cpaj  mz = 91.18
Cpaj  alpha1 = 0.0101
Cpaj  alpha2 = 0.0337
Cpaj  alpha3Z = 0.12
Cpaj  v = 174.1
Cpaj  pi = 3.14159
      alpha1 = alpha_1     ! paj
      alpha2 = alpha_2     ! paj
      alpha3 = alpha_3     ! paj
      pi = ppi             ! paj
      tanbA = tanb
      tanbt = tanb
C
C     mbottom(mtop) = 3. GeV
Cpaj  mb = 3.
      mb = rmbot           ! paj
Cpaj  alpha3 = alpha3Z/(1. +(11. - 10./3.)/4./pi*alpha3Z*
Cpaj *log(mtop**2/mz**2))
C
C     rmtop= running top quark mass
Cpaj  rmtop = mtop/(1.+4.*alpha3/3./pi)
      tq   = log((mq**2+mtop**2)/mtop**2)
      tu   = log((mur**2 + mtop**2)/mtop**2)
      td   = log((md**2 + mtop**2)/mtop**2)
      tglu = log(mglu**2/mtop**2)            ! 11/11/99
      sinb = tanb/((1. + tanb**2)**.5)
      cosb = sinb/tanb
      if(ma.gt.mtop)
     *tanbA = tanb*(1.-3./32./pi**2*
     *(rmtop**2/v**2/sinb**2-rmbot**2/v**2/cosb**2)*
     *log(ma**2/mtop**2))
      if(ma.lt.mtop.or.ma.eq.mtop) tanbt = tanbA
      sinb = tanbt/((1. + tanbt**2)**.5)
      cosb = 1./((1. + tanbt**2)**.5)
      cos2b = (tanbt**2 - 1.)/(tanbt**2 + 1.)
      g1 = (alpha1*4.*pi)**.5
      g2 = (alpha2*4.*pi)**.5
      g3 = (alpha3*4.*pi)**.5
C
      hu = rmtop/v/sinb
      hd = rmbot/v/cosb
      CALL Gfuncar(ma,tanbA,mq,mur,md,mtop,Au,Ad,mu,mglu,
     &             vh,stop1,stop2,sbot1,sbot2,deltamt,deltamb)
C
      if(mq.gt.mur) tp = tq - tu
      if(mq.lt.mur.or.mq.eq.mur) tp = tu - tq
      if(mq.gt.mur) tdp = tu
      if(mq.lt.mur.or.mq.eq.mur) tdp = tq
      if(mq.gt.md) tpd = tq - td
      if(mq.lt.md.or.mq.eq.md) tpd = td - tq
      if(mq.gt.md) tdpd = td
      if(mq.lt.md.or.mq.eq.md) tdpd = tq

      if(mq.gt.md) dlambda1 = 6./96./pi**2*g1**2*hd**2*tpd
      if(mq.lt.md.or.mq.eq.md) dlambda1 = 3./32./pi**2*
     * hd**2*(g1**2/3.+g2**2)*tpd

      if(mq.gt.mur) dlambda2 =12./96./pi**2*g1**2*hu**2*tp
      if(mq.lt.mur.or.mq.eq.mur) dlambda2 = 3./32./pi**2*
     * hu**2*(-g1**2/3.+g2**2)*tp

ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c 11/11/99 - C. Wagner and M. Carena
c
c  dlambdap1 and dlambdap2 are the new log corrections due to
c  the presence of the gluino mass. They are in general very small, 
c  and only present if there is a hierarchy of masses between the
c  two stops.
c
c
c 17/11/99 - G. Ganis. There is no singularity for mq = mur.
c                      Thus, treat this case as the others.
c 23/11/99 - C. Wagner No correction if mglu > mq and mur 
c 11/12/99 - P. Janot  Initialize dlambdap2 for icar=1! 
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc    

      dlambdap2 = 0D0
      IF ( icar .EQ. 2 ) THEN 
        IF ( mglu .GT. mur .AND. mglu .GT. mq ) THEN 
          dlambdap2 = 0D0 
        ELSE
          IF     ( mq .GT. mur ) THEN 
            IF     ( mglu .GT. mur ) THEN 
              dlambdap2 = -4./(16.*pi**2)**2*hu**4*(tq**2-tglu**2)
            ELSE
              dlambdap2 = -4./(16.*pi**2)**2*hu**4*(tq**2-tu**2)
            ENDIF
          ELSE
            IF     ( mglu .GT. mq) THEN 
              dlambdap2 = -4./(16.*pi**2)**2*hu**4*(tu**2-tglu**2)
            ELSE
              dlambdap2 = -4./(16.*pi**2)**2*hu**4*(tu**2-tq**2)
            ENDIF
          ENDIF
        ENDIF
      ENDIF
cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

      dlambda3 = 0.
      dlambda4 = 0.

      if(mq.gt.md) dlambda3 = -1./32./pi**2*g1**2*hd**2*tpd
      if(mq.lt.md.or.mq.eq.md) dlambda3 = 3./64./pi**2*hd**2*
     *(g2**2-g1**2/3.)*tpd

      if(mq.gt.mur) dlambda3 = dlambda3 -
     *1./16./pi**2*g1**2*hu**2*tp
      if(mq.lt.mur.or.mq.eq.mur) dlambda3 = dlambda3 +
     * 3./64./pi**2*hu**2*(g2**2+g1**2/3.)*tp

      if(mq.lt.mur) dlambda4 = -3./32./pi**2*g2**2*hu**2*tp
      if(mq.lt.md) dlambda4 = dlambda4 - 3./32./pi**2*g2**2*
     *hd**2*tpd
C
      lambda1 = ((g1**2 + g2**2)/4.)*
     * (1.-3.*hd**2*(tpd + tdpd)/8./pi**2)
     *+(3.*hd**4./16./pi**2) *tpd*(1.
     *+ (3.*hd**2/2. + hu**2/2.
     *- 8.*g3**2) * (tpd + 2.*tdpd)/16./pi**2)
     *+(3.*hd**4./8./pi**2) *tdpd*(1.  + (3.*hd**2/2. + hu**2/2.
     *- 8.*g3**2) * tdpd/16./pi**2) + dlambda1
      lambda2 = ((g1**2 + g2**2)/4.)*(1.-3.*hu**2*
     *(tp + tdp)/8./pi**2)
     *+(3.*hu**4./16./pi**2) *tp*(1.
     *+ (3.*hu**2/2. + hd**2/2.
     *- 8.*g3**2) * (tp + 2.*tdp)/16./pi**2)
     *+(3.*hu**4./8./pi**2) *tdp*(1. + (3.*hu**2/2. + hd**2/2.
     *- 8.*g3**2) * tdp/16./pi**2) + dlambda2
     &                             + dlambdap2     ! 11/11/99
      lambda3 = ((g2**2 - g1**2)/4.)*(1.-3.*
     *(hu**2)*(tp + tdp)/16./pi**2 -3.*
     *(hd**2)*(tpd + tdpd)/16./pi**2) +dlambda3
      lambda4 = (- g2**2/2.)*(1.
     *-3.*(hu**2)*(tp + tdp)/16./pi**2
     *-3.*(hd**2)*(tpd + tdpd)/16./pi**2) +dlambda4
      lambda5 = 0.
      lambda6 = 0.
      lambda7 = 0.

      m2(1,1) = 2.*v**2*(lambda1*cosb**2+2.*lambda6*
     *cosb*sinb + lambda5*sinb**2) + ma**2*sinb**2

      m2(2,2) = 2.*v**2*(lambda5*cosb**2+2.*lambda7*
     *cosb*sinb + lambda2*sinb**2) + ma**2*cosb**2
      m2(1,2) = 2.*v**2*(lambda6*cosb**2+(lambda3+lambda4)*
     *cosb*sinb + lambda7*sinb**2) - ma**2*sinb*cosb

      m2(2,1) = m2(1,2)
ccccccccccccccccccccccccccccccccccccccccccccccccc
ccc  this is the contribution from light charginos/neutralinos
ccccccccccccccccccccccccccccccccccccccccccccccccc

        mssusy=(.5*(mq**2+mur**2)+mtop**2)**.5

        if(mchi.gt.mssusy)goto 3790
        if(mchi.lt.mtop) mchi=mtop

        tchar=log(mssusy**2/mchi**2)

        deltal12=(9./64./pi**2*g2**4+5./192./pi**2*g1**4)*tchar
        deltal3p4=(3./64./pi**2*g2**4+7./192./pi**2*g1**4
     *       +4./32/pi**2*g1**2*g2**2)*tchar

        deltam112=2.*deltal12*v**2*cosb**2
        deltam222=2.*deltal12*v**2*sinb**2
        deltam122=2.*deltal3p4*v**2*sinb*cosb

        m2(1,1)=m2(1,1)+deltam112
        m2(2,2)=m2(2,2)+deltam222
        m2(1,2)=m2(1,2)+deltam122
        m2(2,1)=m2(2,1)+deltam122

 3790   continue

ccccccccccccccccccccccccccccccccccccccccccc
ccc  end of charginos/neutralinos
ccccccccccccccccccccccccccccccccccccccccccc

      do 9800 i = 1,2
      do 9801 j = 1,2
      m2p(i,j) = m2(i,j) + vh(i,j)
 9801 continue
 9800 continue

      Trm2p = m2p(1,1) + m2p(2,2)
      detm2p = m2p(1,1)*m2p(2,2) - m2p(1,2)*m2p(2,1)

      mh2p = (Trm2p - (Trm2p**2 - 4.* detm2p)**.5)/2.
      HM2p = (Trm2p + (Trm2p**2 - 4.* detm2p)**.5)/2.
      HMp = Hm2p**.5
      IF ( mh2p .GT. 0. ) THEN
        mhp = SQRT(mh2p)
      ELSE
        mhp = -SQRT(-mh2p)
      ENDIF
C
      sin2alpha = 2.*m2p(1,2)/(Trm2p**2-4.*detm2p)**.5
      cos2alpha = (m2p(1,1)-m2p(2,2))/(Trm2p**2-4.*detm2p)**.5
C JBV
C      if(cos2alpha.gt.0.) alpha = asin(sin2alpha)/2.
C      if(cos2alpha.lt.0.) alpha = -pi/2.-asin(sin2alpha)/2.
      alpha = atan2(sin2alpha,cos2alpha)/2
C JBV
      sa = sin(alpha)
      ca = cos(alpha)
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c   11/11/99 C. Wagner and M.carena
c
c        Here the values of sab and cab are defined, in order
c        to define the new couplings of the lightest and 
c        heavy CP-even Higgs to the bottom quark.
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc	

      sab = sa*(1.-deltamb/(1.+deltamb)*(1.+ca/sa/tanb))
      cab = ca*(1.-deltamb/(1.+deltamb)*(1.-sa/ca/tanb)) 

ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

      sqbma = (sinb*ca - cosb*sa)**2
C
C Bretelle for the charged Higgses
C
      MS = ((mq**2 + mur**2)/2. + mtop**2)**.5
      t = log(MS**2/mtop**2)
      aud = (-6.*mu**2/MS**2 - ( mu**2- Ad*Au)**2/MS**4
     *+ 3.*(Au + Ad)**2/MS**2)/6.
      carlos4 = (- g2**2/2.)*(1.-3.*(hu**2 + hd**2)*t/16./pi**2)
     *-(6.*hu**2*hd**2/16./pi**2) * (t + aud/2. + (hu**2 + hd**2
     *- 8.*g3**2) * (aud*t + t**2)/16./pi**2)
     *+(3.*hu**4/96./pi**2) * (3.*mu**2/MS**2 - mu**2*Au**2/
     *MS**4)*
     *(1+ (6.*hu**2 -2.* hd**2/2.
     *-  16.*g3**2) *t/16./pi**2)
     *+(3.*hd**4/96./pi**2) * (3.*mu**2/MS**2 - mu**2*Ad**2/
     *MS**4)*
     *(1+ (6.*hd**2 -2.* hu**2/2.
     *-  16.*g3**2) *t/16./pi**2)
      carlos5 = -(3.*hu**4* mu**2*Au**2/96./pi**2/MS**4) *
     * (1- (2.*hd**2 -6.* hu**2 + 16.*g3**2) *t/16./pi**2)
     *-(3.*hd**4* mu**2*Ad**2/96./pi**2/MS**4) *
     * (1- (2.*hu**2 -6.* hd**2 + 16.*g3**2) *t/16./pi**2)
      mhch2 = mA**2 + (carlos5 - carlos4)* v**2
      IF ( mhch2 .GE. 0. ) THEN
        mhch = SQRT(mhch2)
      ELSE
        mhch = -SQRT(-mhch2)
      ENDIF
C
 2242 RETURN
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
      DOUBLE PRECISION FUNCTION runalf(xmu,fn)
C-------------------------------------------------------------------
C! Running strong coupling constant
C
C  Inputs:       --xmu,   Scale mass
C                --fn,    flavour number
C
C  Ouptuts:      --runalf,  The value of the alphas(xmu)/pi
C
C  P. Janot  --  22 nov 1989.
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      COMMON / alcoef / beta1,beta2,beta3,gama1,gama2,gama3
C
C Lambda_QCD at nf # 5 (from lambda_QCD(5), given in data cards)
C
      xlamda4 = xlamda5 * (amb/xlamda5)**(2./25.)
     .        * (LOG(amb**2/xlamda5**2))**(963./14375.)
      xlamda3 = xlamda4 * (amc/xlamda4)**(2./27.)
     .        * (LOG(amc**2/xlamda4**2))**(107./2025.)
      xlamda6 = xlamda5 * (xlamda5/amt)**(2./21.)
     .        * (LOG(amt**2/xlamda5**2))**(-321./3381.)
C
C Lambda_QCD at fn
C
      nf = fn
      IF ( nf .LT. 3 ) xlamda = xlamda3
      IF ( nf .EQ. 3 ) xlamda = (fn-3.)*xlamda4+(4.-fn)*xlamda3
      IF ( nf .EQ. 4 ) xlamda = (fn-4.)*xlamda5+(5.-fn)*xlamda4
      IF ( nf .EQ. 5 ) xlamda = (fn-5.)*xlamda6+(6.-fn)*xlamda5
      IF ( nf .GE. 6 ) xlamda =        xlamda6
C
C  Coefficients
C
      beta1 = -11./2. + fn/3.
      beta2 = -51./4. + 19./12.*fn
      beta3 = (-2857. + 5033.*fn/9. - 325.*fn**2/27.) / 64.
      gama1 = 2.
      gama2 = 101./12. -5./18.*fn
      gama3 = (3747. - (160.*1.2020569+2216./9.)*fn -140.*fn**2/27.)
     .            / 96.
C
C  alpha_s / pi
C
      xmu2 = xmu
      IF ( xmu2 .LT. .5 ) xmu2 = .5
      as = 1./(-beta1*LOG(xmu2/xlamda))
      aal = LOG(2.*LOG(xmu2/xlamda))
      runalf = as * (1. - as*beta2/beta1*aal
     .       + as**2 * ( (beta2/beta1)**2 * ( aal**2 -aal - 1. )
     .       + beta3/beta1))
C
      RETURN
      END
      DOUBLE PRECISION FUNCTION RUNMAS(xmq,jhig,facrad)
C-------------------------------------------------------------------
C! Compute the running mass term in H --> qqbar(g) width
C
C  Inputs:       --xmq,   quark mass
C                --jhig,  Higgs type
C
C  Ouptuts:      --runmas,  The value of the running mass
C                --facrad,  The finite QCD corrections
C
C  P. Janot  --  22 nov 1989.
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      include 'param.inc'
      include 'consts.inc'
      include 'datapi.inc'
      COMMON / alcoef / beta1,beta2,beta3,gama1,gama2,gama3
C
      IF( jhig.LE.3 ) THEN
        xmh    = amhig (jhig)
      ELSE IF( jhig.EQ.4 ) THEN
        xmh    = amhp
      END IF
C
C  Flavour number (assume mH >> mu, md, ms)
C
      fnh = 3D0
      IF ( amc .LE. xmh/2. ) fnh = fnh + 1D0
      IF ( amb .LE. xmh/2. ) fnh = fnh + 1D0
      IF ( amt .LE. xmh/2. ) fnh = fnh + 1D0
C
      alphah = runalf(xmh,fnh)
C
C Compute mq(mH)
C
      cmulth =  beta2/beta1*(gama1/beta1-gama2/beta2)
      cmulth2 = .5*( (beta2/beta1)**2*(gama1/beta1-gama2/beta2)**2
     .              -(beta2/beta1)**2*(gama1/beta1-gama2/beta2)
     .              +(beta3/beta1)   *(gama1/beta1-gama3/beta3) )
      xmhp = (-beta1*alphah)**(-gama1/beta1)
     .     * (1. + cmulth*alphah + cmulth2*alphah**2 )
C
C Compute mq(Mq)
C
      IF     ( xmq .EQ. ams ) THEN
C PJ 97/11/03: set the scale at 1 GeV for the s quark mass
        xmqq = 1.0
        fnq = 3D0
        factork = 16.11
     .          - 1.04*(2.-amu/ams-amd/ams)
      ELSEIF ( xmq .EQ. amc ) THEN
        xmqq = xmq
        fnq = 4D0
        factork = 16.11
     .          - 1.04*(3.-amu/amc-amd/amc-ams/amc)
      ELSEIF ( xmq .EQ. amb ) THEN
        xmqq = xmq
        fnq = 5D0
        factork = 16.11
     .          - 1.04*(4.-amu/amb-amd/amb-ams/amb-amc/amb)
      ELSEIF ( xmq .EQ. amt ) THEN
        xmqq = xmq
        fnq = 6D0
        factork = 16.11
     .          - 1.04*(4.-amu/amt-amd/amt-ams/amt-amc/amt-amb/amt)
      ELSE
        xmqq = xmq
      ENDIF
      alphaq = runalf(xmqq,fnq)
C     alphaq = runalf(2.*xmqq,fnq)
      cmultq =  beta2/beta1*(gama1/beta1-gama2/beta2)
      cmultq2 = .5*( (beta2/beta1)**2*(gama1/beta1-gama2/beta2)**2
     .              -(beta2/beta1)**2*(gama1/beta1-gama2/beta2)
     .              +(beta3/beta1)   *(gama1/beta1-gama3/beta3) )
      xmqp = (-beta1*alphaq)**(-gama1/beta1)
     .     * (1. + cmultq*alphaq + cmultq2*alphaq**2 )
     .     * (1. + 4./3. *alphaq + factork*alphaq**2 )
C
      runmas = (xmhp/xmqp)**2
C
C Finite radiative corrections due to gluon radiations
C
      runrun = runmas
      CALL radcor(xmq*SQRT(runrun),jhig,fnq,r1,r2)
      facrad = 1. + r1 * alphah + r2 * alphah**2
C     facqed = 1. + r1 * alpha(jhig) + r2 * alpha(jhig)**2
C
C  Here we go
C
 999  RETURN
C-------------------------------------------------------------------
      END
      subroutine scalar(tanb)
C----------------------------------------------------------------------------
C
C  calculate some (soft susy breaking) scalar mass from m0, m1/2 and tan(beta)
C
C  Laurent Duflot -- Jan 1998
C  
C-----------------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,M,O-Z)
C      real tanb,m0,m12,msnu
      include 'param.inc'

C      real m0_2,m12_2,cos2b,mz_2
C--------------------------------------------------------------------
C
      m0 = susM0
      m12 = susM12
C
      cos2b = cos(2.*DATAN(tanb))
      m0_2 = m0*m0
      m12_2 = m12*m12
      mz_2 = amz*amz

C... compute squark and slepton masses from m0 m1/2 and tan(beta)
C Formulae from Z. Phys. C67 (1995) 647 eq. (19)
C This is an explicit solution of the RGE, valid for small to moderate 
C tan(beta). The top and bottom Yukawa effects are not taken into account 
C as this would require a SUGRA-like model, where the higgs spectrum is known 
C at the large scale
C
      susSMQ  = m0_2 + 6.5*m12_2     ! stop Left  with no Yukawa running
      susSMU  = m0_2 + 6.1*m12_2     ! stop right with no Yukawa running
C
      susSMD  = m0_2 + 6.0*m12_2     ! sbottom right
      susSML  = m0_2 + 0.52*m12_2    ! slepton left
      susSME  = m0_2 + 0.15*m12_2    ! slepton right
C         
      msnu = susSML + 0.5*cos2b*mz_2 ! sneutrino
C
      if ( susSMQ .lt. 0. .or. 
     &     susSMU .lt. 0. .or.
     &     susSMD .lt. 0. .or.
     &     susSML .lt. 0. .or.
     &     susSME .lt. 0.      ) then
         write(6,*) ' negative squark mass**2 ! '
         write(6,*) '  SUSY break. ',susSMQ,susSMU,susSMD,
     &        susSML,susSME
         msnu = -1000.
         return
      endif
      susSMQ = sqrt(susSMQ)
      susSMU = sqrt(susSMU)
      susSMD = sqrt(susSMD)
      susSML = sqrt(susSML)
      susSME = sqrt(susSME)
      
      if ( msnu .lt. 0 ) then
         write(6,*) ' negative sneutrino mass**2 !',msnu
         msnu = -sqrt(abs(msnu))
      else
         msnu = sqrt(msnu)
      endif
      susMSNU = msnu
      
      return 
      end
      SUBROUTINE SETDEF
C-------------------------------------------------------------
C!  Set default conditions to HHDECAY
C
C   Input:   / LUNCOM /  --PMAS, mass array from LUND
C
C   Output:  / H0DECK /  --ichan = 1 for requested channels
C                        --channel = channel names
C                        --idbg        debug level
C
C   P. Janot  --  26 Aug 1991
C-------------------------------------------------------------
      include 'param.inc'
      include 'hhdeck.inc'
      include 'hcdeff.inc'
C
      idbg = 0
C
      DO jhig  = 1, nhig
        DO jchan = 1, nchan
          ichan(jchan,jhig) = 1
        ENDDO
      ENDDO
C
      channel( 1,1) = 'H --> gam gam '
      channel( 2,1) = 'H --> glu glu '
      channel( 3,1) = 'H --> tau+tau-'
      channel( 4,1) = 'H --> c c bar '
      channel( 5,1) = 'H --> b b bar '
      channel( 6,1) = 'H --> t t bar '
      channel( 7,1) = 'H --> W+ W-   '
      channel( 8,1) = 'H --> Z0 Z0   '
      channel( 9,1) = 'H --> A A     '
      channel(10,1) = 'H --> h h     '
      channel(11,1) = 'H --> Z0 gam  '
      channel(12,1) = 'H --> e+ e-   '
      channel(13,1) = 'H --> mu+ mu- '
      channel(14,1) = 'H --> s sbar  '
      channel(15,1) = 'H --> chi chi '
      channel(16,1) = 'H --> chi+chi-'
C
      channel( 1,2) = 'h --> gam gam '
      channel( 2,2) = 'h --> glu glu '
      channel( 3,2) = 'h --> tau+tau-'
      channel( 4,2) = 'h --> c c bar '
      channel( 5,2) = 'h --> b b bar '
      channel( 6,2) = 'h --> t t bar '
      channel( 7,2) = 'h --> W+ W-   '
      channel( 8,2) = 'h --> Z0 Z0   '
      channel( 9,2) = 'h --> A A     '
      channel(10,2) = '              '
      channel(11,2) = 'h --> Z0 gam  '
      channel(12,2) = 'h --> e+ e-   '
      channel(13,2) = 'h --> mu+ mu- '
      channel(14,2) = 'h --> s sbar  '
      channel(15,2) = 'h --> chi chi '
      channel(16,2) = 'h --> chi+chi-'
C
      channel( 1,3) = 'A --> gam gam '
      channel( 2,3) = 'A --> glu glu '
      channel( 3,3) = 'A --> tau+tau-'
      channel( 4,3) = 'A --> c c bar '
      channel( 5,3) = 'A --> b b bar '
      channel( 6,3) = 'A --> t t bar '
      channel( 7,3) = 'A --> W+ W-   '
      channel( 8,3) = 'A --> Z0 Z0   '
      channel( 9,3) = 'A --> Z0 h    '
      channel(10,3) = '              '
      channel(11,3) = 'A --> Z0 gam  '
      channel(12,3) = 'A --> e+ e-   '
      channel(13,3) = 'A --> mu+ mu- '
      channel(14,3) = 'A --> s sbar  '
      channel(15,3) = 'A --> chi chi '
      channel(16,3) = 'A --> chi+chi-'
C
      DO i=1,ndch 
        hccon(i)= 1                                                 
      ENDDO                                                          
C
      HCCHAN( 1)= 'H+ --> nu e+     '                                   
      HCCHAN( 2)= 'H+ --> nu mu+    '                                   
      HCCHAN( 3)= 'H+ --> nu tau+   '                                   
      HCCHAN( 4)= 'H+ --> u d bar   '                                   
      HCCHAN( 5)= 'H+ --> u s bar   '                                   
      HCCHAN( 6)= 'H+ --> u b bar   '                                   
      HCCHAN( 7)= 'H+ --> c d bar   '                                   
      HCCHAN( 8)= 'H+ --> c s bar   '                                   
      HCCHAN( 9)= 'H+ --> c b bar   '                                   
      HCCHAN(10)= 'H+ --> W+ b d bar'                                   
      HCCHAN(11)= 'H+ --> W+ b s bar'                                   
      HCCHAN(12)= 'H+ --> W+ b b bar'                                   
      HCCHAN(13)= 'H+ --> h nu l+   '                                   
      HCCHAN(14)= 'H+ --> h q q1 bar'                                   
      HCCHAN(15)= 'H+ --> A nu l+   '                                   
      HCCHAN(16)= 'H+ --> A q q1 bar'                                   
      HCCHAN(17)= 'H+ --> H nu l+   '                                   
      HCCHAN(18)= 'H+ --> H q q1 bar'                                   
      HCCHAN(19)= 'H+ --> chi0 chi+ '                                   
C                                                                       
C - Number of subchannels                                               
C
      DO I=1,12                                                         
        NSCH(I)= 1                                                      
      END DO                                                            
      DO I=13,17,2                                                      
        NSCH(I)  = 3                                                    
        NSCH(I+1)= 9                                                    
      END DO                                                            
      NSCH(19)= 1                                                       
C
  999 RETURN
C-----------------------------------------------------------------------
      END
      SUBROUTINE shiggs
C------------------------------------------------------------------
C!  Compute SUSY Higgs masses from the MSSM parameters
C
C  Input:    /PARAM/ MSSM parameters
C
C  Output:   /PARAM/ amh, gmh, amhp, the squared Higgs masses
C                    alfa, the mixing angle in the CP-even sector
C
C  P. Janot -- 3 December 1994
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'datapi.inc'
      include 'consts.inc'
      include 'miscl.inc'
      include 'ccaren.inc'
      include 'haber.inc'

C      REAL*4 pyalem
      REAL*8 haber_tmass,haber_tanb,haber_ma,
     &  haber_susy,haber_H(4),haber_at,haber_ab,haber_alf
      integer haber_irc 
C
C Compute running top/bottom masses and coupling constants
C at the top mass scale
C
      alpha_1 = pyalem(amt**2) / cw2
      alpha_2 = alpha_1 / sw2 * cw2
      alpha_3 = runalf(amt,5D0) * pi
      factork = 16.11
     .        - 1.04*(4.-amu/amt-amd/amt-ams/amt-amc/amt-amb/amt)
C
      IF ( icar .NE. 1 ) THEN 
        runamt = amt
     .         / ( 1.
     &         + 4./3. *alpha_3/pi
     &                                     )
      ELSE
        runamt = amt
     .         / ( 1.
     &         + 4./3. *alpha_3/pi
     &         + factork*(alpha_3/pi)**2 ! Should be used only with 2-loops
     &                                     )
      ENDIF
C
      amhig(1) = amt
      runamb = amb * SQRT(runmas(amb,1,radb))
      amhig(1) = 0.
C
      corhbb = 1D0          ! Bug found by Gerardo  Ganis
      corgbb = 1D0          ! Was not set for icar = 0 or 3.
C
      IF ( icar .LE. 0 ) THEN
C
C This is the EPA approximation
C
        ama   = amarun
C
        amst1 = amst(1)**2
        amst2 = amst(2)**2
        amsb1 = amsb(1)**2
        amsb2 = amsb(2)**2
C
        crat    = susAt - susMU/tb
        crab    = susAb - susMU*tb
C
C The Delta_ij correction terms entering the mass matrix
C
        delta11 = runamb**4/cb2
     .          * (LOG(amsb1*amsb2/runamb**4)
     .          + 2.*susAb*crab   * gf1(amsb1,amsb2)
     .          + (susAb*crab)**2 * gf2(amsb1,amsb2) )
     .          + runamt**4/sb2
     .          * (susMU*crat)**2 * gf2(amst1,amst2)
C
        delta22 = runamt**4/sb2
     .          * (LOG(amst1*amst2/runamt**4)
     .          + 2.*susAt*crat   * gf1(amst1,amst2)
     .          + (susAt*crat)**2 * gf2(amst1,amst2) )
     .          + runamb**4/cb2
     .          * (susMU*crab)**2 * gf2(amsb1,amsb2)
C
        delta12 =-runamt**4/sb2
     .          * susMU*crat *    ( gf1(amst1,amst2)
     .          + susAt*crat *      gf2(amst1,amst2) )
     .          - runamb**4/cb2
     .          * susMU*crab *    (  gf1(amsb1,amsb2)
     .          + susAb*crab *       gf2(amsb1,amsb2) )
C
C The scale of the correction
C
        corr    = 3.*G_F/(SQRT(2.)*pi**2)
C
        delta11 = corr*delta11/2.
        delta22 = corr*delta22/2.
        delta12 = corr*delta12/2.
C
C The eigen values of the CP-even Higgs mass matrix
C
        a11   =  amz**2*cb2 + ama**2*sb2  + delta11
        a22   =  amz**2*sb2 + ama**2*cb2  + delta22
        a12   = -(amz**2 + ama**2)*s2b/2. + delta12
        delta = SQRT( (a11-a22)**2 + 4.*a12**2 )
        amh   = (a11 + a22 - delta)/2.
        gmh   = (a11 + a22 + delta)/2.
C
C Check the MSSM consistency
C
        IF ( amh .LE. 0. .OR. gmh .LE. 0. ) THEN
          IF ( idbg .GE. 0 ) THEN
            WRITE(6,1000) ama,tb,susM2,susMU,susAt,susAB,susSMQ,susSMU,
     .                           susSMD,susSML,susSME
            WRITE(6,2000) amh,gmh
          ENDIF
          RETURN
C         STOP 99
        ENDIF
C
        IF ( amh .GE. 0 ) THEN
          amh   = SQRT(amh)
        ELSE
          amh   = -SQRT(-amh)
        ENDIF
        gmh   = SQRT(gmh)
C
C The CP-even mixing angle alpha (no ambiguities with ATAN2)
C
        costa = (a11-a22) / delta
        sinta = 2.*a12    / delta
        alfa = piby8
        IF ( costa .NE. 0D0 .OR. sinta .NE. 0D0 )
     &       alfa = DATAN2(sinta,costa)/2.
C
C Radiative correction to the charged Higgs mass (coded only in
C the case of a large stop mixing)
C
        deltahp = -corr * runamt**4*susMU**2/4.
     .          * gf3(amst1,amst2,susSMQ**2)
        amhp = SQRT(amw**2+ama**2+deltahp)
C
      ELSEIF ( icar .EQ. 1 .OR. icar .EQ. 2 ) THEN
C
C Here is the two-loops improved renormalization group calculation.
C
        cma   = amarun
        ctb   = tb
        cmq   = susSMQ
        cmur  = susSMU
        cmdr  = susSMD
        cmglu = susM3
        cmtop = amt
        cAu   = susAt
        cAd   = susAb
        cmu   = susMU
        cmchi = amchar(2)
C
        rmtop = runamt
        rmbot = runamb
C
        ppi = pi
        mz  = amz
        v   = 1. / SQRT(g_f * SQRT(2.)) / SQRT(2.)
        sint = sw2
        stw  = sw2
C
C Note: now (11/11/99) the gluino mass cmglu is an input
C
        CALL pole(3,cmchi,cma,ctb,cmq,cmur,cmdr,cmtop,cAu,cAd,cmu,cmglu,
     *              cmh,cmhp,chm,chmp,camp,cmhch,csa,cca,csab,ccab,
     *              cstop1,cstop2,csbot1,csbot2,ctanbA)
C
        amh  = cmhp
        gmh  = cHMp
        ama  = camp
C
        amhp = cmhch
        costa = cca
        sinta = csa
        alfa = piby4
        IF ( costa .NE. 0D0 .OR. sinta .NE. 0D0 )
     &       alfa = DATAN2(sinta,costa)
C
C 11/11/99: Take the stop and sbottom masses from subhpole
C           since they now include the top threshold and 
C           some genuine two loop gluino corrections.
C
        amst(1) = cstop2
        amst(2) = cstop1
        amsb(1) = csbot2
        amsb(2) = csbot1
C
        topmix  = tmix
        botmix  = bmix
C
C 11/11/99: corrections to the hbb and htt couplings
C
        corhbb = csab/csa
        corgbb = ccab/cca
C
      ELSEIF ( icar .GE. 3 ) THEN 
C
C
C  Rad. correction from Haber et al.
C
         haber_irc = 2

         TMPOLE = amt
         BMASS = amb
         ALPHA_EM = pyalem(amt**2)
         SINW2 = sw2
         COSW2 = 1.D0 - SINW2
         ZMASS = amz
         WMASS = amw
         E2 = 4.D0*PI*ALPHA_EM
         G2 = E2/SINW2
         GP2 = E2/COSW2
         SINB = tb/sqrt(1.+tb**2)
         COSB = 1./sqrt(1.+tb**2)
         COTB = 1./tb

         SQM = susSMQ  ! could be anything !!!!
         SQK = susSMQ
         SQU = susSMU
         SQD = susSMD
         SLM = susSML
         XMU = susMu

         sta2  = amst(2)**2
         stb2  = amst(1)**2
         sba2  = amsb(2)**2
         sbb2  = amsb(1)**2

         if ( haber_irc .eq. 0 ) then
            HABER_TMASS = TMPOLE
         else
            HABER_TMASS = runamt
         endif

         HABER_MA = amarun
         HABER_TANB = tb
         HABER_SUSY = SQM   ! could be anything !!!
         HABER_AT = susAt
         HABER_Ab = susAb
         call HMSUSY(HABER_IRC,HABER_MA,HABER_TMASS,HABER_TANB,
     &               HABER_SUSY,HABER_AT,HABER_AB,HABER_H,
     &               HABER_ALF,ierr)
         if ( ierr .ne. 0 ) then
            write(6,*) 'error in HMSUSY ',ierr
            if ( ierr .eq.1 ) write(6,*) ' stop mass**2 ',STB2
            if ( ierr .eq.2 ) write(6,*) ' sbot mass**2 ',SBB2
            if ( ierr .eq.3 ) write(6,*) ' h    mass**2 ',HABER_H(2)
            if ( ierr .eq.4 ) write(6,*) ' H+   mass**2 ',HABER_H(4)
            return
         endif 

         amh  = HABER_H(2)
         gmh  = HABER_H(1)
         ama  = HABER_H(3)
         amhp = HABER_H(4)
         alfa = HABER_ALF
         sinta = SINA
         costa = COSA
C
      ENDIF
C
C Various couplings and angles.
C
      ta   = TAN(alfa)
      sab2 = SIN(beta-alfa)**2
      cab2 = COS(beta-alfa)**2
      ca = COS(alfa)
      sa = SIN(alfa)
      c2a = COS(2.*alfa)
      cab = COS(alfa+beta)
      sab = SIN(alfa+beta)
C
      RETURN
C------------------------------------------------------------------
 1000 FORMAT(1x,50('-')//
     .       1x,'With the following input parameters of the MSSM :'/
     .       1x,'   . A mass      : ',F8.3,' GeV/c**2'/
     .       1x,'   . Tan beta    : ',F8.3/
     .       1x,'   . M2          : ',F8.3,' GeV/c**2'/
     .       1x,'   . mu          : ',F8.3,' GeV/c**2'/
     .       1x,'   . At          : ',F8.3/
     .       1x,'   . Ab          : ',F8.3/
     .       1x,'   . mQ          : ',F8.3,' GeV/c**2'/
     .       1x,'   . mU          : ',F8.3,' GeV/c**2'/
     .       1x,'   . mD          : ',F8.3,' GeV/c**2'/
     .       1x,'   . mL          : ',F8.3,' GeV/c**2'/
     .       1x,'   . mE          : ',F8.3,' GeV/c**2'/)
 2000 FORMAT(' A non physical set of Higgs masses has',
     .       ' been obtained :'/
     .       1x,'   . mh**2       : ',E10.4,' (GeV/c**2)**2'/
     .       1x,'   . mH**2       : ',E10.4,' (GeV/c**2)**2'/
     .       1x,' +++++++  STOP execution here +++++++'//)
      END
      DOUBLE PRECISION FUNCTION sigintf(s,ipro)
C -------------------------------------------------------------------
C! Compute the Born level cross section of the interference between
C  hZ and WW/ZZ fusion into h or H
C
C Input:    s,    the effective centre-of-mass energy squared
C           ipro, = 5 for WW --> h
C                 = 6 for WW --> H
C                 = 7 for ZZ --> h
C                 = 8 for ZZ --> H
C
C Marumi Kado --  Jan 1998
C
C Modifications
C   o Patrick Janot -- 7 Nov 1999
C     Improve integration efficiency
C   o Patrick Janot -- 21 Nov 1999
C     Incorporate the IBA and the top corrections.
C -------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,O-Z)
      include 'param.inc'
      include 'consts.inc'
      include 'datapi.inc'
      include 'lundcom.inc'
      include 'miscl.inc'
C      
      EXTERNAL fsubint3, dgmlt3
      REAL*8 x3lo,x3hi,x(3),DGMLT3
C
      REAL*8 rmh, rgh, rmv, ss
      COMMON / sigint / rmh, rgh, rmv, ss
C
      ve   = -1D0 + 4D0*sw2
      ae   = -1D0
C
      con1 = (G_F**3*amz**8*alpha2)/(SQRT(2.)*pi**3*s*alpha(0)**2)
C
C Case 1: WW scattering
C
      IF ( ipro .EQ. 5 .OR. ipro .EQ. 6 ) THEN
C
        rmv  = amw
        con2 = (ve+ae) * cw**2 * (amw/amz)**2 / 8D0
        con1 = con1
     &       * (1.+deltar)**3         ! Improved Born Approximation
     &       * (1.-5./6.*deltar)      ! WWH coupling top correction
     &       * (1.-4./3.*deltar)      ! ZZH coupling top correction
C
C Case 2: ZZ scattering
C
      ELSEIF ( ipro .EQ. 7 .OR. ipro .EQ. 8 ) THEN
C
        rmv  = amz
        con2 = ((ve**2+ae**2)**2+4D0*ve**2*ae**2) / 64D0
        con1 = con1
     &       * (1.+deltar)**3         ! Improved Born Approximation
     &       * (1.-8./3.*deltar)      ! (ZZH coupling)**2 top correction
C
      ENDIF
C
      con3 = 1D0 / (4D0 * SQRT(s))
C
      IF     ( ipro .EQ. 5 .OR. ipro .EQ. 7 ) THEN
C
        rmh  = pmas(25,1)
        rgh  = pmas(25,2)
C
      ELSEIF ( (ipro .EQ. 6 .OR. ipro .EQ. 8) .AND. ism .NE. 1 ) THEN
C
        rmh  = pmas(35,1)
        rgh  = pmas(35,2)
C
      ELSE
C
       sigintf = 0D0
       GOTO 999
C
      ENDIF
C
C First integration (Higgs Breit Wigner)
C
      ss      = s
      x3lo    = -DATAN2(rmh,rgh)
      x3hi    =  DATAN2(ss-rmh**2,rmh*rgh)
      sigintf = con1 * con2 * con3 
     &         * DGMLT3(fsubint3,x3lo,x3hi,1,6,x)
     .         / (piby2+DATAN2(rmh,rgh))
C
  999 RETURN
      END

      DOUBLE PRECISION FUNCTION sigklei(xmh,s)
C--------------------------------------------------------------------
C! Total cross section for the process e+e- ---> H Z*
C
C Subroutine from R. Kleiss, slightly modified to run with
C my COMMONs
C
C Input : s = beam-beam invariant mass square (GeV**2);
C         xmh = Higgs mass
C
C Modifications :
C   Patrick Janot
C       -- 28 Jan 1992 : Implement improved Born approximation
C       -- 12 Dec 1993 : Implement eeZ Vertex correction
C       -- 12 Dec 1993 : Implement eeZ Vertex correction
C--------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'consts.inc'
C
      COMPLEX*16 xq,z1,z2,ffff
      REAL*8 a,x1
C
C Derived constants
C
      g    = 1./SQRT(sw2*(1.-sw2))
      cga  = -g/4.
      cgv  = g*(4.*sw2-1.)/4.
      a    = 4.*xmh**2/s
      b    = amz*gmz/s
      xp   = 1.+(xmh**2-amz**2)/s
      x1   = SQRT(a)
      x2   = 1.+ a/4.
C The eeZ and mumuZ couplings
      con1 = alpha(0) * (g*(cgv**2+cga**2))**2 / (36.*s)
C The "first" Z Breit-Wigner
      con2 = (amz**2/s)/( (1.-amz**2/s)**2 + b**2 )
C The full width correction
      con3 = alpha(0)*amz*(cgv**2+cga**2)/(3.*gmz)
C The Improved Born approximation
      con4 = (1.+deltar)**2
C The ZZH vertex correction with a top loop
     .     * (1.-8./3.*deltar)
C And the weak correction associated to the e+e- --> Z vertex
     .     * (1.-8.*deltar*(1.-sw2)*cgv*cga/(cgv**2+cga**2))
C The overall multiplicative constant
      cons = con1 * con2 * con4 / con3
C
C The integral factor using an external function
C
      xq      = DCMPLX(xp,b)
      z1      = xq + CDSQRT( xq*xq - a )
      z2      = 2.*xq - z1
C
C Compute a few anomalous coupling
C
      CALL anomal(1,s)
C
C The integration
C
      IF ( CDABS(z1) .NE. 0. ) THEN
        sigklei = 1./(8.*b) * IMAG( 1./(z1-z2)
     .          * ( FFFF(a,z1,2D0,AAA,BBB)
     .            - FFFF(a,z1,x1,AAA,BBB)
     .            - FFFF(a,z2,2D0,AAA,BBB)
     .            + FFFF(a,z2,x1,AAA,BBB) ))
      ELSE
        sigklei = 0.
      ENDIF
C
C     sigklei = sigklei * cons / brai(3)
      sigklei = sigklei * cons
C
  999 RETURN
      END

      DOUBLE PRECISION FUNCTION sigmawwh(s,ipro)
C -------------------------------------------------------------------
C! Compute the IBA cross section of the WW/ZZ fusion into h
C
C Input:    s,    the effective centre-of-mass energy squared
C           ipro, = 5 for WW --> h
C                 = 6 for WW --> H
C                 = 7 for ZZ --> h
C                 = 8 for ZZ --> H
C
C Patrick Janot -- 2 Sep 1995
C -------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'lundcom.inc'
      include 'consts.inc'
      include 'datapi.inc'
      include 'miscl.inc'
      EXTERNAL gsub3, dgmlt3
      REAL*8 x3lo,x3hi,x(3),DGMLT3
C
      REAL*8 kappah, kappav, v, a, rmh, rgh, ss
      COMMON / sigww / kappah, kappav, v, a, rmh, rgh, ss
C
C Case 1: WW scattering
C
      IF ( ipro .EQ. 5 .OR. ipro .EQ. 6 ) THEN
C
        rmv  = amw
        v    = SQRT(2.)
        a    = SQRT(2.)
        gvvh = gweak2(0) * rmv**2
        con3     = (1.+deltar)**3 * (1.-5./3.*deltar)
C
C Case 2: ZZ scattering
C
      ELSEIF ( ipro .EQ. 7 .OR. ipro .EQ. 8 ) THEN
C
        rmv  = amz
        v    = -1.+4.*sw2
        a   = -1.
        gvvh = gweak2(0) * rmv**2 / cw**2
        con3     = (1.+deltar)**3 * (1.-8./3.*deltar)
C
      ENDIF
C
C The Higgs mass
C
      IF     ( ipro .EQ. 5 .OR. ipro .EQ. 7 ) THEN
C
        rmh  = pmas(25,1)
        rgh  = pmas(25,2)
C
      ELSEIF ( (ipro .EQ. 6.OR.ipro .EQ. 8) .AND. ism .NE. 1 ) THEN
C
        rmh  = pmas(35,1)
        rgh  = pmas(35,2)
C
      ELSE
C
        sigmawwh = 0D0
        GOTO 999
C
      ENDIF
C
      con1     = G_F**3*rmv**4 / (64.*SQRT(2.)*pi**3)
     .         * alpha2 / alpha(0)**2
C
      con2     = alpha2 * gvvh
     .         / (1024. * pi * sw2**2 * amw**4 )
C
      kappav   = rmv**2/s
C
      ss     = s
      x3lo    = -DATAN2(rmh,rgh)
      x3hi    =  DATAN2(ss-rmh**2,rmh*rgh)
      sigmawwh = con2 * con3 * DGMLT3(gsub3,x3lo,x3hi,1,6,x)
     .                       / (piby2+DATAN2(rmh,rgh))
C
  999 RETURN
      END
      DOUBLE PRECISION FUNCTION sigma1(s)
C--------------------------------------------------------------------
C! TEN functions sigma1(s) --> sigma10(s) for the cross sections
C  of the processes 1 to 10.
C
C--------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      sigma1 = crocom(1,s)
      RETURN
      END
C
      DOUBLE PRECISION FUNCTION sigma2(s)
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      sigma2 = crocom(2,s)
      RETURN
      END
C
      DOUBLE PRECISION FUNCTION sigma3(s)
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      sigma3 = crocom(3,s)
      RETURN
      END
C
      DOUBLE PRECISION FUNCTION sigma4(s)
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      sigma4 = crocom(4,s)
      RETURN
      END
C
      DOUBLE PRECISION FUNCTION sigma5(s)
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      sigma5 = crocom(5,s)
      RETURN
      END
C
      DOUBLE PRECISION FUNCTION sigma6(s)
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      sigma6 = crocom(6,s)
      RETURN
      END
C
      DOUBLE PRECISION FUNCTION sigma7(s)
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      sigma7 = crocom(7,s)
      RETURN
      END
C
      DOUBLE PRECISION FUNCTION sigma8(s)
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      sigma8 = crocom(8,s)
      RETURN
      END
C
      DOUBLE PRECISION FUNCTION sigma9(s)
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      sigma9 = crocom(9,s)
      RETURN
      END
C
      DOUBLE PRECISION FUNCTION sigma10(s)
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      sigma10 = crocom(10,s)
      RETURN
      END
C
      DOUBLE PRECISION FUNCTION sigmat(s)
      IMPLICIT DOUBLE PRECISION (A-H, O-Z)
      sigmat = sigma1(s)+sigma2(s)+sigma3(s)+sigma4(s)+sigma5(s)
     &       + sigma6(s)+sigma7(s)+sigma8(s)+sigma9(s)+sigma10(s)
      RETURN
      END
      SUBROUTINE squarks
C------------------------------------------------------------------
C!  Compute squark masses from the MSSM parameters
C
C  Input:    /PARAM/ MSSM parameters
C
C  Output:   /PARAM/ amst(2), amsb(2), the stop and sbottom masses
C
C   P. Janot -- 4 December 1994
C   Modification 
C   P. Janot -- 3 June 1998 : Compute Delta rho (stop,sbottom)
C------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'datapi.inc'
      include 'consts.inc'
      include 'miscl.inc'
      COMMON / mydeltar / rhostop
      REAL*8 st12,st22,sb12,sb22
C
C Compute running top/bottom masses and coupling constants
C at the top mass scale
C
      alpha_3 = runalf(amt,5D0) * pi
      factork = 16.11
     .        - 1.04*(4.-amu/amt-amd/amt-ams/amt-amc/amt-amb/amt)
      runamt = amt
     .       / ( 1.
     &         + 4./3. *alpha_3/pi
C    &         + factork*(alpha_3/pi)**2   ! only with full two loop logs
     &                                   )
      amhig(1) = amt
      runamb = amb * SQRT(runmas(amb,1,radb))
      amhig(1) = 0.
C
C
      c2b = COS(2.*beta)
      s2b = SIN(2.*beta)
      cb2 = COS(beta)**2
      sb2 = SIN(beta)**2
C
C The Stop mass matrix
C
      t11 = runamt**2 + susSMQ**2 + (4.*amw**2-1.*amz**2)*c2b/6.
      t22 = runamt**2 + susSMU**2 - (4.*amw**2-4.*amz**2)*c2b/6.
C     t12 = runamt * (susAt + susMU/tb)    ! Sign mistake (Marcela)
      t12 = runamt * (susAt - susMU/tb)
C
C The Stop masses
C
      delta   = SQRT( (t11-t22)**2 + 4.*t12**2 )
      amst(1) = (t11 + t22 - delta)/2.
      amst(2) = (t11 + t22 + delta)/2.
C
C The mixing angle in the Stop sector
C
      cosmix = -(t11-t22)
      sinmix = -2.*t12
      topmix = piby8
      IF ( cosmix .NE. 0D0 .OR. sinmix .NE. 0D0 )
     &     topmix = DATAN2(sinmix,cosmix)/2.
C
C The Sbottom mass matrix
C
      b11 = runamb**2 + susSMQ**2 - (2.*amw**2+1.*amz**2)*c2b/6.
      b22 = runamb**2 + susSMD**2 + (2.*amw**2-2.*amz**2)*c2b/6.
C     b12 = runamb * (susAb + susMU*tb)  ! Sign mistake (Marcela)
      b12 = runamb * (susAb - susMU*tb)
C
C The Sbottom masses
C
      delta   = SQRT( (b11-b22)**2 + 4.*b12**2 )
      amsb(1) = (b11 + b22 - delta)/2.
      amsb(2) = (b11 + b22 + delta)/2.
C
C The mixing angle in the Sbottom sector
C
      cosmix = -(b11-b22)
      sinmix = -2.*b12
      botmix = piby8
      IF ( cosmix .NE. 0D0 .OR. sinmix .NE. 0D0 )
     &     botmix = DATAN2(sinmix,cosmix)/2.
C
C Check the MSSM consistency
C
      IF ( amst(1)*amst(2) .LE. 0. .OR.
     .     amsb(1)*amsb(2) .LE. 0. ) THEN
        IF ( idbg .GE. 0 ) THEN
          WRITE(6,1000) ama,tb,susM,susMU,susAt,susAB,susSMQ,susSMU,
     .                         susSMD,susSML,susSME
          WRITE(6,2000) amst(1),amst(2),amsb(1),amsb(2)
        ENDIF
        RETURN
      ENDIF
C
C Compute the Delta(rho) contribution
C
      st12  = amst(2)
      st22  = amst(1)
      xst11 =    t12       / SQRT(t12**2.+(t11-st12)**2.)
      xst12 = - (t11-st12) / SQRT(t12**2.+(t11-st12)**2.)
      xst21 =    t12       / SQRT(t12**2.+(t11-st22)**2.)
      xst22 = - (t11-st22) / SQRT(t12**2.+(t11-st22)**2.)
C
      sb12  = amsb(2)
      sb22  = amsb(1)
      xsb11 =    b12       / SQRT(b12**2.+(b11-sb12)**2.)
      xsb12 = - (b11-sb12) / SQRT(b12**2.+(b11-sb12)**2.)
      xsb21 =    b12       / SQRT(b12**2.+(b11-sb22)**2.)
      xsb22 = - (b11-sb22) / SQRT(b12**2.+(b11-sb22)**2.)
C
      rhostop = alpha(0)
     &        * (3./(16.*pi*sw2*amw**2.))
     &        * (xst11**2.*xsb11**2.*gfunc(st12,sb12)
     &         + xst11**2.*xsb12**2.*gfunc(st12,sb22)
     &         + xst12**2.*xsb11**2.*gfunc(st22,sb12)
     &         + xst12**2.*xsb12**2.*gfunc(st22,sb22)
     &         - xst11**2.*xst12**2.*gfunc(st12,st22)
     &         - xsb11**2.*xsb12**2.*gfunc(sb12,sb22))      
C
C Now the masses
C
      amst(1) = SQRT(amst(1))
      amst(2) = SQRT(amst(2))
      amsb(1) = SQRT(amsb(1))
      amsb(2) = SQRT(amsb(2))
C
      RETURN
C------------------------------------------------------------------
 1000 FORMAT(1x,50('-')//
     .       1x,'With the following input parameters of the MSSM :'/
     .       1x,'   . A mass      : ',F8.3,' GeV/c**2'/
     .       1x,'   . Tan beta    : ',F8.3/
     .       1x,'   . M           : ',F8.3,' GeV/c**2'/
     .       1x,'   . mu          : ',F8.3,' GeV/c**2'/
     .       1x,'   . At          : ',F8.3/
     .       1x,'   . Ab          : ',F8.3/
     .       1x,'   . mQ          : ',F8.3,' GeV/c**2'/
     .       1x,'   . mU          : ',F8.3,' GeV/c**2'/
     .       1x,'   . mD          : ',F8.3,' GeV/c**2'/
     .       1x,'   . mL          : ',F8.3,' GeV/c**2'/
     .       1x,'   . mE          : ',F8.3,' GeV/c**2'/)
 2000 FORMAT(' A non physical set of stop and sbottom masses has',
     .       ' been obtained :'/
     .       1x,'   . mStop1^2    : ',E10.4,' (GeV/c**2)**2'/
     .       1x,'   . mStop2^2    : ',E10.4,' (GeV/c**2)**2'/
     .       1x,'   . mSbottom1^2 : ',E10.4,' (GeV/c**2)**2'/
     .       1x,'   . mSbottom2^2 : ',E10.4,' (GeV/c**2)**2'/
     .       1x,' +++++++  STOP execution here +++++++'//)
      END
      DOUBLE PRECISION FUNCTION strange(x,bet)
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
      ELSEIF ( x .EQ. 0D0 ) THEN
        strange = 0D0
      ENDIF
      RETURN
      END
      FUNCTION TCaren(X,Y,Z) 
      IMPLICIT REAL*8(A-H,L,M,O-Z)
C
      IF ( ABS(x-y) .LT. 1D-5 ) x = y + 1D-5
      IF ( ABS(z-y) .LT. 1D-5 ) z = y - 1D-5
      IF ( ABS(x-z) .LT. 1D-5 ) z = x + 1D-5
C
      TCaren = (X**2*Y**2*log(X**2/Y**2)
     &       +  X**2*Z**2*log(Z**2/X**2)
     &       +  Y**2*Z**2*log(Y**2/Z**2))
     &       /  ( (X**2-Y**2)
     &          * (Y**2-Z**2)
     &          * (X**2-Z**2) )
      return
      end	
      SUBROUTINE USRDEF
C-------------------------------------------------------------
C!  The user puts here his running conditions for HZHA
C
C   Input:  Data cards file or user program
C
C  P. Janot  --  24 aug 1991
C-------------------------------------------------------------
C
      include 'fscard.inc'
C                                                                       
      include 'param.inc'
      include 'hhdeck.inc'
      include 'hccomm.inc'
      include 'hcdeff.inc'
C
      LOGICAL NOHCD
      INTEGER ITAB(nchan*nhig)
      REAL*4 TAB(nchan*nhig)
      EQUIVALENCE (TAB(1),ITAB(1))
C
C  --  Channel number (if only one is selected)
      kchan = ghhg(1)
      IF ( kchan .NE. 0) THEN
        CALL vzero(ichan(1,1),nchan*nhig)
        ichan(kchan,1) = 1
        ichan(kchan,2) = 1
        ichan(kchan,3) = 1
      ENDIF
C  --  Debug level
      idbg   = ghhg(2)
C
C  Define a set of decay channels FOR H
      DO 1 kchan=1,nchan
        ichan(kchan,1) = gch1(kchan)
        ichan(kchan,2) = gch2(kchan)
        ichan(kchan,3) = gch3(kchan)
 1    CONTINUE
C
C  Print out of final values chosen for running HHDECAY
C
  130 CONTINUE
C      WRITE(6,1004) IDBG
      IF ( IDBG .LT. 2 )  GOTO 11
      WRITE(6,1005)
      DO 10 khig =1,nhig
      DO 10 kchan=1,nchan
        IF( ichan(kchan,khig) .EQ. 1 )
     .  WRITE(6,1006) kchan,channel(kchan,khig)
   10 CONTINUE
   11 CONTINUE
C
C - Charged Higgses:
C - Enable/Disable accordingly to data cards (default ON)
C - 
      DO I=1,NDCH
        HCCON(I)= 1
      END DO
C
      NOHCD = .TRUE.
      DO I=1,NDCH
        J= GCHC(I)
        IF( J.EQ.0 .OR. J.EQ.1 ) THEN
          HCCON(I)= J
        ELSE
          HCCON(I)= 1
        END IF
        IF( HCCON(I).GT.0 ) NOHCD= .FALSE.
      ENDDO
C
      IF( NOHCD ) THEN
        WRITE(*,'(A,A,A)') ' +++ USRDEF Warning +++ ',
     .       ' Found GCHC with no channel ON ',
     .       ' Action: ignore it '
        DO I=1,NDCH
          HCCON(I)= 1
        END DO
      ELSE
C - Superseed GHCC ...
        IHCD(1)= 0
        IHCD(2)= 0
      END IF 
C - Some of the decays are not implemented ... force them out
      DO I=10,12
        HCCON(I)= -1
      END DO
      HCCON(19)= -1
C
      RETURN
C-------------------------------------------------------------------
  999 FORMAT(1X,'    +++ USRDEF +++ Warning +++'/
     .       1X,'Not enough space for creating KHIG bank')
 1000 FORMAT(/1X,'+++ USRDEF Warning +++ '/
     .       1X,'Word #  1 in data card GHHG (',I6,
     .          ') should be between 0 and ',I2)
 1003 FORMAT(1X,'+++ USRDEF Warning +++ '/
     .       1X,' Length of data card GCHA (',I6,
     .          ') should be 0 or ',I2)
 1004 FORMAT(1X,'Initial conditions for running H0DECAY are :'/
     .       5X,'. Debug level             : ',I2//)
1005  FORMAT(1X,'Requested decay channels are :'/)
1006  FORMAT(5X,I2,'. ',A12)
1009  FORMAT(1X,'+++ USRDEF Warning +++ '/
     .       1X,'Word #',I2,' in data card GHGG (',D14.6,
     .          ') should be between .6 and 2.5')
      END
      SUBROUTINE vac(ihiggs,mchi,ma,tanb,mq,mur,mdr,
     *  mtop,at,ab,mu,mglu,mh,mhp,hm,hmp,amp,mhch,stop1,stop2,
     *  sbot1,sbot2,sa,ca,sab,cab,stop1w,stop2w,tanbA)
C--------------------------------------------------------------------
C! Computes vacuum polarization effects to Higgs masses
C
C  From M. Carena and C. Wagner.
C
C  Modifications :
C      1. Patrick Janot -- 20 Sep 1995
C         Improves the iteration procedure to fasten the routine
C         (--> factor 100 improvement roughly)
C--------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,M,O-Z)
      include 'ccaren.inc'
      DIMENSION delta(2,2),coupt(2,2),T(2,2),sstop2(2),
     *ssbot2(2),B(2,2),coupb(2,2),
     *hcoupt(2,2),hcoupb(2,2),
     *acoupt(2,2),acoupb(2,2)
      DIMENSION polar(3),pr(3)
C
      delta(1,1) = 1.
      delta(2,2) = 1.
      delta(1,2) = 0.
      delta(2,1) = 0.
C
Cpaj  v = 174.1
Cpaj  mz=91.18
Cpaj  pi=3.14159
Cpaj  alpha3z=.12
Cpaj  alpha3=1./(1./alpha3z+23./6./pi*log(mtop/mz))
Cpaj  rmtop = mtop/(1.+4*alpha3/3./pi)
Cpaj  rmbot = 3.
      pi = ppi           ! paj
C
      CALL rghm(mchi,ma,tanb,mq,mur,mdr,mtop,at,ab,
     *   mu,mglu,mh,hm,mhch,sa,ca,sab,cab,tanbA)
C
      mhp = mh
      hmp = hm
      amp = ma
      IF ( mh .LE. 0. ) GOTO 3333
C
      sinb = tanb/(tanb**2+1.)**.5
      cosb = 1./(tanb**2+1.)**.5
      cos2b = sinb**2 - cosb**2
      sinbpa = sinb*ca + cosb*sa
      cosbpa = cosb*ca - sinb*sa
      mq2 = mq**2
      mur2 = mur**2
      mdr2 = mdr**2
      mst11 = Rmtop**2 + mq2  - 0.35*MZ**2*cos2b
      mst22 = Rmtop**2 + mur2 - 0.15*MZ**2*cos2b
      if(mst11.lt.0.) goto 3333
      if(mst22.lt.0.) goto 3333
      msb11 = Rmbot**2 + mq2  + 0.42*MZ**2*cos2b
      msb22 = Rmbot**2 + mdr2 + 0.08*MZ**2*cos2b
      if(msb11.lt.0.) goto 3333
      if(msb22.lt.0.) goto 3333
      wmst11 = Rmtop**2 + mq2
      wmst22 = Rmtop**2 + mur2
      mst12 = Rmtop*(At - mu/tanb)
      msb12 = Rmbot*(Ab - mu*tanb)
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C
C            Stop Eigenvalues calculation
C
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      Stop12 = 0.5*(mst11+mst22) +
     * 0.5*((mst11+mst22)**2 -
     * 4.*(mst11*mst22 - mst12**2))**.5
      Stop22 = 0.5*(mst11+mst22) -
     * 0.5*((mst11+mst22)**2 - 4.*(mst11*mst22 - mst12**2))**.5
      if(Stop22.lt.0.) goto 3333
      sstop2(1) = stop12
      sstop2(2) = stop22
      stop1 = Stop12**.5
      stop2 = Stop22**.5
      stop1w = stop1
      stop2w = stop2
C
      IF ( mst12 .EQ. 0. ) THEN
        xst11 = 1.
        xst12 = 0.
        xst21 = 0.
        xst22 = 1.
      ELSE
        xst11 = mst12/(mst12**2+(mst11-stop12)**2)**.5
        xst12 = - (mst11-stop12)/(mst12**2+(mst11-stop12)**2)**.5
        xst21 = mst12/(mst12**2+(mst11-stop22)**2)**.5
        xst22 = - (mst11-stop22)/(mst12**2+(mst11-stop22)**2)**.5
      ENDIF
C
      T(1,1) = xst11
      T(2,2) = xst22
      T(1,2) = xst12
      T(2,1) = xst21
C
      Sbot12 = 0.5*(msb11+msb22) +
     * 0.5*((msb11+msb22)**2 -
     * 4.*(msb11*msb22 - msb12**2))**.5
      Sbot22 = 0.5*(msb11+msb22) -
     * 0.5*((msb11+msb22)**2 - 4.*(msb11*msb22 - msb12**2))**.5
      if(Sbot22.lt.0.) goto 3333
      sbot1 = Sbot12**.5
      sbot2 = Sbot22**.5
      ssbot2(1) = sbot12
      ssbot2(2) = sbot22
C
      IF ( msb12 .EQ. 0. ) THEN
        xsb11 = 1.
        xsb12 = 0.
        xsb21 = 0.
        xsb22 = 1.
      ELSE
        xsb11 = msb12/(msb12**2+(msb11-sbot12)**2)**.5
        xsb12 = - (msb11-sbot12)/(msb12**2+(msb11-sbot12)**2)**.5
        xsb21 = msb12/(msb12**2+(msb11-sbot22)**2)**.5
        xsb22 = - (msb11-sbot22)/(msb12**2+(msb11-sbot22)**2)**.5
      ENDIF
C
      B(1,1) = xsb11
      B(2,2) = xsb22
      B(1,2) = xsb12
      B(2,1) = xsb21
C
Cpaj  sint = 0.2320
      sqr = 2.**.5
Cpaj  vp = 174.1*sqr
      vp = v*sqr
C
cccccccccccccccccccccccccccccccccccc
ccc    starting of light higgs
cccccccccccccccccccccccccccccccccccc
cccccccccccccccccccccccccccccccccccccccc
      if(ihiggs.eq.0)goto 3524
ccccccccccccccccccccccccccccccccccccccccc
      DO 4646 i = 1,2
      DO 4576 j = 1,2
        coupt(i,j) =
Cpaj * sint*mz**2*2.*sqr/174./3.*sinbpa*(delta(i,j) +
     * sint*mz**2*2.*sqr/ v  /3.*sinbpa*(delta(i,j) +
     * (3. - 8.*sint)/4./sint*T(1,i)*T(1,j))
Cpaj * -rmtop**2/174.1**2*vp/sinb*ca*delta(i,j)
     * -rmtop**2/  v  **2*vp/sinb*ca*delta(i,j)
     * -rmtop/vp/sinb*(At*ca + mu*sa)*(T(1,i)*T(2,j) +
     *T(1,j)*T(2,i))
 4576 CONTINUE
 4646 CONTINUE
C
      DO 1646 i = 1,2
      DO 1576 j = 1,2
        coupb(i,j) =
Cpaj * -sint*mz**2*2.*sqr/174./6.*sinbpa*(delta(i,j) +
     * -sint*mz**2*2.*sqr/ v  /6.*sinbpa*(delta(i,j) +
     * (3. - 4.*sint)/2./sint*B(1,i)*B(1,j))
Cpaj * +rmbot**2/174.1**2*vp/cosb*sa*delta(i,j)
     * +rmbot**2/  v  **2*vp/cosb*sa*delta(i,j)
     * +rmbot/vp/cosb*(Ab*sa + mu*ca)*(B(1,i)*B(2,j) +
     * B(1,j)*B(2,i))
 1576 CONTINUE
 1646 CONTINUE
C
      prun = mh
      eps = 1D-6*prun
      iter = 0
7007  iter = iter + 1
      DO 7980 i3 = 1,3
        pr(i3)=prun+(i3-2)*eps/2
        p2=pr(i3)**2
        polt = 0.
        DO 7979 i = 1,2
        DO 7978 j = 1,2
         polt = polt + coupt(i,j)**2*3.*
     *   fintcar(p2,sstop2(i),sstop2(j))/16./pi**2
 7978   CONTINUE
 7979   CONTINUE
        polb = 0.
        DO 9979 i = 1,2
        DO 9978 j = 1,2
          polb = polb + coupb(i,j)**2*3.*
     *    fintcar(p2,ssbot2(i),ssbot2(j))/16./pi**2
 9978   CONTINUE
 9979   CONTINUE
        rmtop2 = rmtop**2
        mtop2=mtop**2
C
        poltt =
Cpaj * 3.*rmtop**2/8./pi**2/174.1**2*
     * 3.*rmtop**2/8./pi**2/  v  **2*
     * ca**2/sinb**2 *
     *   (-2.*mtop**2+.5*p2)*
     *  fintcar(p2,mtop2,mtop2)
C
        pol = polt + polb + poltt
        polar(i3) = p2 - mh**2 - pol
 7980 CONTINUE
      deriv = (polar(3)-polar(1))/eps
      drun = - polar(2)/deriv
      prun = prun + drun
      p2 = prun**2
      IF ( ABS(drun) .LT. 1D-6 ) GOTO 7777
      if ( iter .gt. 100 ) then
         write(6,*) ' VAC : too many iterations (h) '
         goto 7777
      endif
      GOTO 7007
 7777 CONTINUE
C
       mhp = p2**.5
C
cccccccccccccccccccccccccccccccccccccccc
ccc   end of light higgs
cccccccccccccccccccccccccccccccccccccccc
 3340 IF(ihiggs.EQ.1)GOTO 3524
ccccccccccccccccccccccccccccccccccccccccc
ccc starting of heavy higgs
cccccccccccccccccccccccccccccccccccccccccc
C
      DO 1446 I = 1,2
      DO 1476 J = 1,2
        hcoupt(i,j) =
Cpaj * -sint*mz**2*2.*sqr/174./3.*cosbpa*(delta(i,j) +
     * -sint*mz**2*2.*sqr/ v  /3.*cosbpa*(delta(i,j) +
     * (3. - 8.*sint)/4./sint*T(1,i)*T(1,j))
Cpaj * -rmtop**2/174.1**2*vp/sinb*sa*delta(i,j)
     * -rmtop**2/  v  **2*vp/sinb*sa*delta(i,j)
     * -rmtop/vp/sinb*(At*sa - mu*ca)*(T(1,i)*T(2,j) +
     *T(1,j)*T(2,i))
 1476 CONTINUE
 1446 CONTINUE
C
      DO 1146 I = 1,2
      DO 1176 J = 1,2
        hcoupb(i,j) =
Cpaj * sint*mz**2*2.*sqr/174./6.*cosbpa*(delta(i,j) +
     * sint*mz**2*2.*sqr/ v  /6.*cosbpa*(delta(i,j) +
     * (3. - 4.*sint)/2./sint*B(1,i)*B(1,j))
Cpaj * -rmbot**2/174.1**2*vp/cosb*ca*delta(i,j)
     * -rmbot**2/  v  **2*vp/cosb*ca*delta(i,j)
     * -rmbot/vp/cosb*(Ab*ca - mu*sa)*(B(1,i)*B(2,j) +
     * B(1,j)*B(2,i))
        hcoupb(i,j)=0.
 1176 CONTINUE
 1146 CONTINUE
C
      prun = hm
      eps = 1D-6*prun
      iter = 0
 1001 iter = iter + 1
      DO 1780 i3 = 1,3
        pr(i3)=prun+(i3-2)*eps/2
        hp2=pr(i3)**2
C
        hpolt = 0.
        do 1779 i = 1,2
        do 1778 j = 1,2
        hpolt = hpolt + hcoupt(i,j)**2*3.*
     *  fintcar(hp2,sstop2(i),sstop2(j))/16./pi**2
 1778 CONTINUE
 1779 CONTINUE
C
      hpolb = 0.
      DO 1979 I = 1,2
      DO 1978 J = 1,2
        hpolb = hpolb + hcoupb(i,j)**2*3.*
     *  fintcar(hp2,ssbot2(i),ssbot2(j))/16./pi**2
 1978 CONTINUE
 1979 CONTINUE
C
      rmtop2 = rmtop**2
      mtop2  = mtop**2
C
      hpoltt =
Cpaj * 3.*rmtop**2/8./pi**2/174.1**2*
     * 3.*rmtop**2/8./pi**2/  v  **2*
     *  sa**2/sinb**2 *
     *   (-2.*mtop**2+.5*hp2)*
     *  fintcar(hp2,mtop2,mtop2)
C
      hpol = hpolt + hpolb + hpoltt
      polar(i3) =hp2-hm**2-hpol
 1780 CONTINUE
      deriv = (polar(3)-polar(1))/eps
      drun = - polar(2)/deriv
      prun = prun + drun
      hp2 = prun**2
      IF ( ABS(drun) .LT. 1D-6 ) GOTO 1111
      if ( iter .gt. 100 ) then
         write(6,*) ' VAC : too many iterations (H) '
         goto 1111
      endif
      GOTO 1001
 1111 CONTINUE
C
      hmp = hp2**.5
ccccccccccccccccccccccccccccccccccccccccccc
ccc  end of heavy higgs
cccccccccccccccccccccccccccccccccccccccccccc
      if(ihiggs.eq.2)goto 3524
cccccccccccccccccccccccccccccccccccccccccccc
ccc  beginning of pseudoscalar higgs
cccccccccccccccccccccccccccccccccccccccccccc
C
      DO 3446 i = 1,2
      DO 3476 j = 1,2
        acoupt(i,j) =
     * -rmtop/vp/sinb*(At*cosb + mu*sinb)*
     *  (T(1,i)*T(2,j) -T(1,j)*T(2,i))
 3476 CONTINUE
 3446 CONTINUE
C
      DO 3146 I = 1,2
      DO 3176 J = 1,2
        acoupb(i,j) =
     * rmbot/vp/cosb*(Ab*sinb + mu*cosb)*
     *  (B(1,i)*B(2,j) -B(1,j)*B(2,i))
 3176 CONTINUE
 3146 CONTINUE
C
      prun = ma
      eps = 1D-6*prun
      iter = 0
 6006 iter = iter + 1
      DO 3780 i3 = 1,3
        pr(i3)=prun+(i3-2)*eps/2
        ap2=pr(i3)**2
        apolt = 0.
        DO 3779 I = 1,2
        DO 3778 J = 1,2
          apolt = apolt + acoupt(i,j)**2*3.*
     *    fintcar(ap2,sstop2(i),sstop2(j))/16./pi**2
 3778   CONTINUE
 3779   CONTINUE
        apolb = 0.
        DO 3979 I = 1,2
        DO 3978 J = 1,2
          apolb = apolb + acoupb(i,j)**2*3.*
     *    fintcar(ap2,ssbot2(i),ssbot2(j))/16./pi**2
 3978   CONTINUE
 3979   CONTINUE
        rmtop2 = rmtop**2
        mtop2=mtop**2
        apoltt =
Cpaj *  3.*rmtop**2/8./pi**2/174.1**2*
     *  3.*rmtop**2/8./pi**2/  v  **2*
     *  cosb**2/sinb**2 *
     *   (-.5*ap2)*
     *  fintcar(ap2,mtop2,mtop2)
        apol = apolt + apolb + apoltt
        polar(i3) = ap2 - ma**2 -apol
 3780 CONTINUE
      deriv = (polar(3)-polar(1))/eps
      drun = - polar(2)/deriv
      prun = prun + drun
      ap2 = prun**2
      IF ( ABS(drun) .LT. 1D-6 ) GOTO 6666
      if ( iter .gt. 100 ) then
         write(6,*) ' VAC : too many iterations (A) '
         goto 6666
      endif
      GOTO 6006
 6666 CONTINUE
C
      amp = ap2**.5
C
ccccccccccccccccccccccccccccccccccccccccccc
ccc end of pseudoscalar higgs
cccccccccccccccccccccccccccccccccccccccccccc
        if(ihiggs.eq.3)goto 3524
cccccccccccccccccccccccccccccccccccccccccccc
C
3524  RETURN
3333  RETURN
      END
      FUNCTION weakcor(jhig,fmas,fchrg,rt,rw)
C-------------------------------------------------------------------
C! Compute the one loop electroweak correction to h --> ffbar
C  (for f = e,mu,tau,u,d,s,c,b, but not t). Valid for mH < 2mW
C  but clearly useless for mH > 2mW !
C
C Patrick Janot -- 28 Sept 1995
C-------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'param.inc'
      include 'datapi.inc'
      include 'consts.inc'
      IF ( fmas .EQ. amb ) THEN
        Cf = 1D0
      ELSE
        Cf = 7D0-4D0*alphas(jhig)
      ENDIF
C
      qed     =      alpha(jhig)/pi * 3D0/2D0 * fchrg**2
     .             * (3D0/2D0-LOG(amhig(jhig)**2/fmas**2))
C
      weak    =      G_F / (8D0*pi**2*SQRT(2D0))
     .             * ( Cf*amt**2*rt
     .               + amw**2*rw*(3D0*LOG(cw2)/sw2-5D0)
     .               + amz**2*rw*(.5D0-3D0*(1D0-4D0*sw2*fchrg)**2) )
C
      weakcor = 1D0 + qed + weak
C
      RETURN
      END
      SUBROUTINE wzdecy(iwz)
C-------------------------------------------------------------------
C! Decay a virtual W or Z, and prepare the parton shower
C  in case of a hadronic decay
C
C  Input :    iwz, the position of the virtual boson in /LUJET/
C
C  Patrick Janot -- 01 Sep 1995
C-------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'lundcom.inc'
      DIMENSION ijoin(2)
C
    1 CALL pydecy(iwz)
C
C In case of a hadronic decay
C
      IF ( IABS(k7lu(n7lu,2)) .LE. 8 ) THEN
C
C Reject too low masses to avoid infinite loops in LUEXEC
C
        xmm = p7lu(iwz,5)
        IF ( xmm .LE. 0.300 ) THEN
          k7lu(n7lu,1) = 1
          n7lu = n7lu - 2
          GOTO 1
        ENDIF
C
C From Torbjorn : Reset mstj(92) to avoid a duplication of the
C parton shower and a subsequent crash in LUEXEC
C
        mstj(92) = 0
C
C Prepare the parton shower
C
        ijoin(1) = k7lu(iwz,4)
        ijoin(2) = k7lu(iwz,5)
        njoin = 2
        CALL pyjoin(njoin,ijoin)
        CALL pyshow(ijoin(1), ijoin(2), xmm)
C
      ENDIF
C
      RETURN
      END
      FUNCTION zcaren(x)
C-----------------------------------------------------------------------
C! Function used for the Higgs masses computation
C
C  From M. Carena and C. Wagner
C-----------------------------------------------------------------------
      IMPLICIT REAL*8(A-H,M,O-Z)
      SA = 1. - 4.*X
      if(Sa.lt.0.) sa1 = abs(sa)
      if(SA.lt.0.) Zcaren = 2.*SA1**.5*atan(1./SA1**.5)
      if(SA.gt.0.) Zcaren = SA**.5*log((1.+SA**.5)/(1.-SA**.5))
      RETURN
      END
      SUBROUTINE zdecay(qz,izpol,q1,q2,ifs)
C-----------------------------------------------------------------------
C! Decay a Z boson with a polarization IZPOL
C
C  Not used if IKLEI = 1
C
C  Patrick Janot -- 25 Nov 1991
C-----------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'datapi.inc'
      include 'zzdeck.inc'
      DIMENSION qz(4),q1(4),q2(4),p1(4),p2(4)
      REAL*8 betax, betay, betaz, a(3,3)
C      REAL*4 pymass
C
      CALL vzero(q1(1),4*2)
      CALL vzero(q2(1),4*2)
      CALL vzero(p1(1),4*2)
      CALL vzero(p2(1),4*2)
C
C Choice of the decay channel
C
      choix = RNDM2(choix)
      DO ifs = 1,11
       IF ( choix .LT. braz(ifs) ) GOTO 1
      ENDDO
C
C Choice of the polar decay angle in the Z frame
C
    1 costet = 2.*RNDM2(costet) - 1.
      IF ( izpol .EQ. 1 ) THEN
        weight = (1.+costet**2)/2.
      ELSE
        weight = (1.-costet**2)/2.
      ENDIF
      IF ( weight .LT. RNDM2(weight) ) GOTO 1
      sintet = SQRT(1.-costet**2)
C
C Choice of the azimuthal decay angle in the Z frame
C
      phi = 2.*pi*RNDM2(phi)
      sinphi = SIN(phi)
      cosphi = COS(phi)
C
C 4-vectors of the decay products in the Z frame
C
      kff = kcode(ifs)
      pz    = SQRT(qz(1)**2+qz(2)**2+qz(3)**2)
      ez    = SQRT(qz(4)**2+pz**2)
      pt = SQRT(qz(1)**2+qz(2)**2)
      ene = qz(4)/2.
      pm = pymass(kff)
      pmm = SQRT(ene**2-pm**2)
      q1(4) = ene
      q1(3) = pmm*costet
      q1(2) = pmm*sintet*sinphi
      q1(1) = pmm*sintet*cosphi
      q2(4) = q1(4)
      q2(3) =-q1(3)
      q2(2) =-q1(2)
      q2(1) =-q1(1)
C
C Boost in the lab frame
C
      betax = 0.
      betay = 0.
      betaz =-pz/ez
      CALL lorenz(betax,betay,betaz,q1)
      CALL lorenz(betax,betay,betaz,q2)
C
C Rotation to the standard reference frame
C
      a(1,3) = qz(1)/pz
      a(2,3) = qz(2)/pz
      a(3,3) = qz(3)/pz
      a(1,2) = qz(2)/pt
      a(2,2) =-qz(1)/pt
      a(3,2) = 0.
      a(1,1) = a(2,2)*a(3,3)-a(3,2)*a(2,3)
      a(2,1) = a(3,2)*a(1,3)-a(1,2)*a(3,3)
      a(3,1) = a(1,2)*a(2,3)-a(2,2)*a(1,3)
      p1(1) = a(1,1)*q1(1)+a(1,2)*q1(2)+a(1,3)*q1(3)
      p1(2) = a(2,1)*q1(1)+a(2,2)*q1(2)+a(2,3)*q1(3)
      p1(3) = a(3,1)*q1(1)+a(3,2)*q1(2)+a(3,3)*q1(3)
      p2(1) = a(1,1)*q2(1)+a(1,2)*q2(2)+a(1,3)*q2(3)
      p2(2) = a(2,1)*q2(1)+a(2,2)*q2(2)+a(2,3)*q2(3)
      p2(3) = a(3,1)*q2(1)+a(3,2)*q2(2)+a(3,3)*q2(3)
      CALL ucopy(p1(1),q1(1),3*2)
      CALL ucopy(p2(1),q2(1),3*2)
      q1(4) = SQRT(q1(1)**2+q1(2)**2+q1(3)**2+pm**2)
      q2(4) = SQRT(q2(1)**2+q2(2)**2+q2(3)**2+pm**2)
C
      RETURN
      END

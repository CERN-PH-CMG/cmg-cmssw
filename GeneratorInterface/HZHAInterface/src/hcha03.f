      FUNCTION ALPHAS(Q,N)
C-----------------------------------------------------------------------
C!
C!    INPUT:          Q   scale (GeV)                         (REAL*8)
C!                    N   option                              (INTEGER)
C!    OUTPUT:         ALPHAS    running quark mass             (REAL*8)
C!
C!    Comments:       Cleaning of Djouadi code.
C!    Author:         A.Djouadi
C!    Cleaned by      G.Ganis         July 2, 96
C-----------------------------------------------------------------------
      IMPLICIT   NONE
      REAL*8     ALPHAS
C - Parameters
C - Arguments
      INTEGER    N
      REAL*8     Q
C - Common blocks
      REAL*8     XLB1, XLB2
      COMMON    /ALSLAM/ XLB1(6),XLB2(6)
      INTEGER    N0
      REAL*8     XLAMBDA, AMC, AMB, AMT
      COMMON    /ALS/ XLAMBDA, AMC, AMB, AMT, N0
C - Auxiliary variables
      INTEGER    I, J, NF
      REAL*8     XLB(6), X, ALS1, ALS2, B0, B1,
     .           PI
C - Macro
      B0(I)=33.D0-2.D0*I
      B1(I)=6.D0*(153.D0-19.D0*I)/B0(I)**2
      ALS1(I,X)=12.D0*PI/(B0(I)*LOG((X/XLB(I))**2))
      ALS2(I,X)=12.D0*PI/(B0(I)*LOG((X/XLB(I))**2))
     .          *(1.D0-B1(I)*LOG(LOG((X/XLB(I))**2))
     .           /LOG((X/XLB(I))**2))
C - Statements
      PI=4.D0*DATAN(1.D0)
      IF(N.EQ.1)THEN
        DO I=1,6
          XLB(I)= XLB1(I)
        END DO
      ELSE
        DO I=1,6
          XLB(I)= XLB2(I)
        END DO
      ENDIF
      IF( Q.LT.AMC ) THEN
        NF=3
      ELSEIF( Q.LE.AMB )THEN
        NF=4
      ELSEIF( Q.LE.AMT )THEN
        NF=5
      ELSE
        NF=6
      ENDIF
      IF( N.EQ.1 ) THEN
        ALPHAS= ALS1(NF,Q)
      ELSE
        ALPHAS= ALS2(NF,Q)
      ENDIF
C -
      RETURN
      END
      SUBROUTINE ALSINI(ACC)
C-----------------------------------------------------------------------
C!
C!    INPUT:          ACC   accuracy                          (REAL*8)
C!
C!    Comments:       Cleaning of Djouadi code.
C!    Author:         A.Djouadi
C!    Cleaned by      G.Ganis         July 2, 96
C-----------------------------------------------------------------------
      IMPLICIT   NONE
C - Parameters
C - Arguments
      REAL*8     ACC
C - Common blocks
      REAL*8     XLB1, XLB2
      COMMON    /ALSLAM/ XLB1(6),XLB2(6)
      INTEGER    N0
      REAL*8     XLAMBDA, AMC, AMB, AMT
      COMMON    /ALS/ XLAMBDA, AMC, AMB, AMT, N0
C - Auxiliary variables
      INTEGER    I
      REAL*8     XLB(6), PI, XITER
C - Statements
      PI=4.D0*DATAN(1.D0)
      XLB1(1)=0D0
      XLB1(2)=0D0
      XLB2(1)=0D0
      XLB2(2)=0D0
      IF(N0.EQ.3)THEN
        XLB(3)=XLAMBDA
        XLB(4)=XLB(3)*(XLB(3)/AMC)**(2.D0/25.D0)
        XLB(5)=XLB(4)*(XLB(4)/AMB)**(2.D0/23.D0)
        XLB(6)=XLB(5)*(XLB(5)/AMT)**(2.D0/21.D0)
      ELSEIF(N0.EQ.4)THEN
        XLB(4)=XLAMBDA
        XLB(5)=XLB(4)*(XLB(4)/AMB)**(2.D0/23.D0)
        XLB(3)=XLB(4)*(XLB(4)/AMC)**(-2.D0/27.D0)
        XLB(6)=XLB(5)*(XLB(5)/AMT)**(2.D0/21.D0)
      ELSEIF(N0.EQ.5)THEN
        XLB(5)=XLAMBDA
        XLB(4)=XLB(5)*(XLB(5)/AMB)**(-2.D0/25.D0)
        XLB(3)=XLB(4)*(XLB(4)/AMC)**(-2.D0/27.D0)
        XLB(6)=XLB(5)*(XLB(5)/AMT)**(2.D0/21.D0)
      ELSEIF(N0.EQ.6)THEN
        XLB(6)=XLAMBDA
        XLB(5)=XLB(6)*(XLB(6)/AMT)**(-2.D0/23.D0)
        XLB(4)=XLB(5)*(XLB(5)/AMB)**(-2.D0/25.D0)
        XLB(3)=XLB(4)*(XLB(4)/AMC)**(-2.D0/27.D0)
      ENDIF
      DO I=1,6
        XLB1(I)=XLB(I)
      END DO
      IF(N0.EQ.3)THEN
        XLB(3)=XLAMBDA
        XLB(4)=XLB(3)*(XLB(3)/AMC)**(2.D0/25.D0)
     .              *(2.D0*DLOG(AMC/XLB(3)))**(-107.D0/1875.D0)
        XLB(4)=XITER(AMC,XLB(3),3,XLB(4),4,ACC)
        XLB(5)=XLB(4)*(XLB(4)/AMB)**(2.D0/23.D0)
     .              *(2.D0*DLOG(AMB/XLB(4)))**(-963.D0/13225.D0)
        XLB(5)=XITER(AMB,XLB(4),4,XLB(5),5,ACC)
        XLB(6)=XLB(5)*(XLB(5)/AMT)**(2.D0/21.D0)
     .             *(2.D0*DLOG(AMT/XLB(5)))**(-321.D0/3381.D0)
        XLB(6)=XITER(AMT,XLB(5),5,XLB(6),6,ACC)
      ELSEIF(N0.EQ.4)THEN
        XLB(4)=XLAMBDA
        XLB(5)=XLB(4)*(XLB(4)/AMB)**(2.D0/23.D0)
     .              *(2.D0*DLOG(AMB/XLB(4)))**(-963.D0/13225.D0)
        XLB(5)=XITER(AMB,XLB(4),4,XLB(5),5,ACC)
        XLB(3)=XLB(4)*(XLB(4)/AMC)**(-2.D0/27.D0)
     .              *(2.D0*DLOG(AMC/XLB(4)))**(107.D0/2025.D0)
        XLB(3)=XITER(AMC,XLB(4),4,XLB(3),3,ACC)
        XLB(6)=XLB(5)*(XLB(5)/AMT)**(2.D0/21.D0)
     .             *(2.D0*DLOG(AMT/XLB(5)))**(-321.D0/3381.D0)
        XLB(6)=XITER(AMT,XLB(5),5,XLB(6),6,ACC)
      ELSEIF(N0.EQ.5)THEN
        XLB(5)=XLAMBDA
        XLB(4)=XLB(5)*(XLB(5)/AMB)**(-2.D0/25.D0)
     .               *(2.D0*DLOG(AMB/XLB(5)))**(963.D0/14375.D0)
        XLB(4)=XITER(AMB,XLB(5),5,XLB(4),4,ACC)
        XLB(3)=XLB(4)*(XLB(4)/AMC)**(-2.D0/27.D0)
     .               *(2.D0*DLOG(AMC/XLB(4)))**(107.D0/2025.D0)
        XLB(3)=XITER(AMC,XLB(4),4,XLB(3),3,ACC)
        XLB(6)=XLB(5)*(XLB(5)/AMT)**(2.D0/21.D0)
     .               *(2.D0*DLOG(AMT/XLB(5)))**(-321.D0/3381.D0)
        XLB(6)=XITER(AMT,XLB(5),5,XLB(6),6,ACC)
      ELSEIF(N0.EQ.6)THEN
        XLB(6)=XLAMBDA
        XLB(5)=XLB(6)*(XLB(6)/AMT)**(-2.D0/23.D0)
     .               *(2.D0*DLOG(AMT/XLB(6)))**(321.D0/3703.D0)
        XLB(5)=XITER(AMT,XLB(6),6,XLB(5),5,ACC)
        XLB(4)=XLB(5)*(XLB(5)/AMB)**(-2.D0/25.D0)
     .               *(2.D0*DLOG(AMB/XLB(5)))**(963.D0/14375.D0)
        XLB(4)=XITER(AMB,XLB(5),5,XLB(4),4,ACC)
        XLB(3)=XLB(4)*(XLB(4)/AMC)**(-2.D0/27.D0)
     .               *(2.D0*DLOG(AMC/XLB(4)))**(107.D0/2025.D0)
        XLB(3)=XITER(AMC,XLB(4),4,XLB(3),3,ACC)
      ENDIF
      DO I=1,6
       XLB2(I)=XLB(I)
      END DO
C -
      RETURN
      END
      FUNCTION GETCKM(I,J)
C-----------------------------------------------------------------------
C! Get VCKM matrix element for flavours I,J
C!
C!    Input:          I   JETSET code for up-like flavour
C!                    J   JETSET code for  down-like flavour
C!
C!    Output:         GETCKM, VCKM element
C!                    1. if I,J is not consistent
C!
C!    Gerardo Ganis              3 May 1996
C-----------------------------------------------------------------------
      IMPLICIT   NONE
      REAL*8     GETCKM
C - Parameters & Constants
C - Arguments
      INTEGER    I, J
C - Common blocks
C - JETSET parameter commons
      INTEGER    KCHG
      REAL*8     PMAS, PARF, VCKM
      COMMON /PYDAT2/ KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
C - Auxiliary variables
      INTEGER    IU, ID, L, K
      LOGICAL    UP, DW
C - Data statements
C - Macros
      UP(K)=     ABS(K).EQ.2 .OR. ABS(K).EQ.4 .OR. ABS(K).EQ.6
      DW(K)=     ABS(K).EQ.1 .OR. ABS(K).EQ.3 .OR. ABS(K).EQ.5
C - Statements
      GETCKM= 1.D0
C - Check consistency
      IF( .NOT.UP(I) .AND. .NOT.DW(I) ) RETURN
      IF( .NOT.UP(J) .AND. .NOT.DW(J) ) RETURN
      IF( UP(I) .AND. UP(J) ) RETURN
      IF( DW(I) .AND. DW(J) ) RETURN
C - Get matrix index
      IF( UP(I) ) THEN
        IU= I/2
        ID= (J+1)/2
      ELSE
        IU= J/2
        ID= (I+1)/2
      END IF
C - Get matrix element
      GETCKM= DBLE(VCKM(IU,ID))
C
      RETURN
      END

      SUBROUTINE GETHUD( ID, CHAN)
C-----------------------------------------------------------------------
C! Decay H+ in neutral higgs + up-down pair. Get phase space from JETSET
C! and choose accordingly to the matrix element with a Hit_Or_Miss metho
C!
C!    Input:            ID    /PYJETS/ position for H+
C!                      CHAN  decay channel
C!    Output:           in /PYJETS/
C!
C!    Called by HCDECY
C!
C!    Gerardo Ganis              3 May 1996
C!                       Mod    21 Nov 1999
C-----------------------------------------------------------------------
      IMPLICIT   NONE
C - Parameters & Constants
      INTEGER    NTRYMX
      PARAMETER( NTRYMX= 1000 )
C - Arguments
      INTEGER    ID, CHAN
C - Common blocks
      include 'hcdeff.inc'
C - JETSET common for event record
      INTEGER    NJMAX, NTMAX
      PARAMETER( NJMAX= 4000, NTMAX= 5)
      INTEGER    NJET, NPAD, KJET
      REAL*8     PJET, VJET
      COMMON /PYJETS/ NJET, NPAD, KJET(NJMAX,NTMAX), PJET(NJMAX,NTMAX),
     .                      VJET(NJMAX,NTMAX)
C - Auxiliary variables
      REAL*8     M(4), Q(4,4), ME, RAN(1),
     .           HHUDME
      REAL*8     MOLD
      INTEGER    I, J, K, IP(4), KJOLD, NTRY
      LOGICAL    OK
      EXTERNAL   HHUDME
C - Data statements
C - Macros
C - Statements
      OK= .FALSE.
      KJOLD= KJET(ID,1)
C - Get the evnt
      NTRY= 0
      DO WHILE( .NOT.OK .AND. NTRY.LT.NTRYMX )
        NTRY= NTRY+1
C - Decay the particle accordingly to phase space ...
        CALL PYDECY( ID )
C - fill relevant information ...
        IP(1)= KJET(ID,4)
        IP(2)= KJET(ID,4)+1
        IP(3)= KJET(ID,4)+2
        IP(4)= ID
        DO I=1,4
          M(I)= DBLE(PJET(IP(I),5))
        END DO
        DO I=1,4
          DO J=I,4
            Q(I,J)= DBLE( PJET(IP(I),4)*PJET(IP(J),4) -
     .                    PJET(IP(I),1)*PJET(IP(J),1) -
     .                    PJET(IP(I),2)*PJET(IP(J),2) -
     .                    PJET(IP(I),3)*PJET(IP(J),3) )
            IF( I.NE.J ) Q(J,I)= Q(I,J)
          END DO
        END DO
C - Get matrix element
        ME= HHUDME(0,M,Q)
        IF( ME.GT.MEMAX(CHAN) ) THEN
          MEMAX(CHAN)= ME
          UPMAX(CHAN)= UPMAX(CHAN)+1
        END IF
C - Hit or miss
        CALL HCRNDM(RAN,1)
        IF( RAN(1).LT. ME/MEMAX(CHAN) ) THEN
          OK= .TRUE.
        ELSE
          IF( NTRY.LT.NTRYMX ) THEN
            NJET= NJET-3
            KJET(ID,1)= KJOLD
            KJET(ID,4)= 0
            KJET(ID,5)= 0
          END IF
        END IF
      END DO
C - Notify if unsuccessful
      IF( .NOT.OK ) THEN
        WRITE(*,'(A,A,I6,A,A)') 
     .   ' +++ GETHUD Warning: unable to hit an event ',
     .   ' after ',NTRYMX,' trials ', 
     .   ' => take the last generated with phase space '
      END IF
C - Over
      RETURN
      END

      SUBROUTINE HCBRAT(BR,WD)
C-----------------------------------------------------------------------
C! Calculate H+ branching fractions and widths
C!
C!    Input:     common blocks /HCCOMM/ and /HCMAXX/
C!    Output:    BR(i)    branching fraction
C!               WD(i)    width in GeV
C!               i=0,36   WD(0)= total width, BR(0)= 1.
C!
C!    Called by HCSETJ
C!
C!    Gerardo Ganis              3 May 1996
C-----------------------------------------------------------------------
      IMPLICIT   NONE
C - Parameters & Constants
C - Common blocks
      include 'hccomm.inc'
      include 'hcdeff.inc'
C - Arguments
      REAL*8     BR( 0:NCH ), WD( 0:NCH )
C - Auxiliary variables
      INTEGER     QCDL, QCDQ,
     .            I, J, N
      REAL*8      MHC, MH0, SA, CA, SB, CB, SAB, CAB, GRED,
     .            HUDWDT, HHUDWD
      EXTERNAL    HUDWDT, HHUDWD
C - Data statements
      DATA        QCDL /0/, QCDQ /1/
C - Macros
C - Statements
      MHC= MH(4)
      SA= TAF  /SQRT(1.D0+TAF**2)
      CA= 1.D0/SQRT(1.D0+TAF**2)
      SB= TBT  /SQRT(1.D0+TBT**2)
      CB= 1.D0/SQRT(1.D0+TBT**2)
      SAB= SA*CB-CA*SB
      CAB= SA*SB+CA*CB
C - Initialize
      DO I=0,NCH
        BR(I)= 0.D0
        WD(I)= 0.D0
        IF( I.GT.0 ) UPMAX(I)= 0
      END DO
      N= 0
C - 1-3 nu lepton
      DO I=12,16,2
        N=N+1
        WD(N)= HUDWDT( MHC, I, I-1, TBT, QCDL )
      END DO
C - 4-9 up down
      DO I=2,4,2
        DO J=1,5,2
          N= N+1
          WD(N)= HUDWDT( MHC, I, J, TBT, QCDQ )
        END DO
      END DO
C - 10-12 W b d bar ( not yet done )
      DO I=1,5,2
        N=N+1
        WD(N)= 0.D0
      END DO
C - Decays in h f f'
      MH0= MH(2)
      GRED= CAB
      DO I=12,16,2
        N= N+1
        WD(N)= HHUDWD(MHC,MH0,GRED,I,I-1,MEMAX(N))
      END DO
      DO I=2,6,2
        DO J=1,5,2
          N= N+1
          WD(N)= HHUDWD(MHC,MH0,GRED,I,J,MEMAX(N))
        END DO
      END DO
C - Decays in A f f'
      MH0= MH(3)
      GRED= 1.D0
      DO I=12,16,2
        N= N+1
        WD(N)= HHUDWD(MHC,MH0,GRED,I,I-1,MEMAX(N))
      END DO
      DO I=2,6,2
        DO J=1,5,2
          N= N+1
          WD(N)= HHUDWD(MHC,MH0,GRED,I,J,MEMAX(N))
        END DO
      END DO
C - Decays in H f f'
      MH0= MH(1)
      GRED= SAB
      DO I=12,16,2
        N= N+1
        WD(N)= HHUDWD(MHC,MH0,GRED,I,I-1,MEMAX(N))
      END DO
      DO I=2,6,2
        DO J=1,5,2
          N= N+1
          WD(N)= HHUDWD(MHC,MH0,GRED,I,J,MEMAX(N))
        END DO
      END DO
C - 49 chi0 chi+ (not done yet)
      WD(49)= 0.D0
C - Sum in index 0
      DO I=0,NCH
        WD(0)= WD(I) + WD(0)
      END DO
      IF( WD(0).NE.0 ) THEN
        DO I=0,NCH
          BR(I)= WD(I) / WD(0)
        END DO
      END IF
C
      RETURN
      END

      FUNCTION HCCONV( SS, MH, GH )                                      
C-----------------------------------------------------------------------
C!    Calculates convolution of H+H- kinematical factors with           
C!    2 Breit-Wigners                                                   
C!                                                                      
C!    Input:  SS  (REAL*8)     center_of_mass_energy squared (GeV**2)   
C!            MH  (REAL*8)     H+ mass                       (GeV/c**2) 
C!            GH  (REAL*8)     H+ width                      (GeV/c**2) 
C!                                                                      
C!    Output: HCCONV (REAL*8)  convolution factor                       
C!                                                                      
C!    Gerardo Ganis              3 May 1996                             
C-----------------------------------------------------------------------
      IMPLICIT   NONE                                                   
      REAL*8     HCCONV                                                 
C - Arguments                                                           
      REAL*8     SS, MH, GH                                              
C - Working common (share the hA routines)
      REAL*8 s,am1,am2,w1,w2
      COMMON /BWC/ s,am1,am2,w1,w2
C - Auxiliary variables                                                 
      REAL*8     X2MIN, X2MAX, X(2) 
C - Constants                                                           
      REAL*8     PI, piby2                                                     
      PARAMETER( PI= 3.141592653589793238, piby2=pi/2D0 )                             
C - External subroutine                                                 
      EXTERNAL   fsub2                                                 
C - External functions                                                  
      REAL*8     DGMLT2                                                 
      EXTERNAL   DGMLT2                                                 
C                                                                       
C - Fill the common block                                               
C                                                                       
      s      = ss                                                        
      am1    = mh
      am2    = mh                                                       
      w1     = gh                                                       
      w2     = gh
C
C - Calculate the integral                                              
C
      X2MIN  =  -DATAN2(am2,w2)
      X2MAX  =   DATAN2(s-am2**2,am2*w2)
      HCCONV= DGMLT2( fsub2, x2min, x2max, 1, 6, x)
     .       / (piby2+DATAN2(am1,w1))                                   
     .       / (piby2+DATAN2(am2,w2))
C                                                                       
      RETURN                                                            
      END                                                               
      SUBROUTINE HCCUBR(BR,BRC)
C-----------------------------------------------------------------------
C-----------------------------------------------------------------------
      IMPLICIT   NONE
C - Parameters & Constants
C - Common blocks
      include 'hccomm.inc'
      include 'hcdeff.inc'
C - Arguments
      REAL*8     BR(0:NCH), BRC(NDCH)
C - Auxiliary variables
      INTEGER    I, J, N
C - Data statements
C - Macros
C - Statements
      DO I=1,NDCH
        HCCBR1(I)= 0.D0
           BRC(I)= 0.D0
        DO J=1,NMXC
          HCCBR2(I,J)= 0.D0
        END DO
      END DO
C - Cumulate ...
      N= 1
      HCCBR1(N)= BR(1)*HCCON(N)
         BRC(N)= BR(1)
      DO I=2,12
        N= N+1
        HCCBR1(N)= HCCBR1(N-1) + BR(I)*HCCON(N)
           BRC(N)= BRC(N)      + BR(I)
      END DO
      N= N+1
      HCCBR1(N)= HCCBR1(N-1)
      DO I=13,15
        HCCBR1(N)= HCCBR1(N) + BR(I)*HCCON(N)
           BRC(N)= BRC(N)    + BR(I)
      END DO
      N= N+1
      HCCBR1(N)= HCCBR1(N-1)
      DO I=16,24
        HCCBR1(N)= HCCBR1(N) + BR(I)*HCCON(N)
           BRC(N)= BRC(N)    + BR(I)
      END DO
      N= N+1
      HCCBR1(N)= HCCBR1(N-1)
      DO I=25,27
        HCCBR1(N)= HCCBR1(N) + BR(I)*HCCON(N)
           BRC(N)= BRC(N)    + BR(I)
      END DO
      N= N+1
      HCCBR1(N)= HCCBR1(N-1)
      DO I=28,36
        HCCBR1(N)= HCCBR1(N) + BR(I)*HCCON(N)
           BRC(N)= BRC(N)    + BR(I)
      END DO
      N= N+1
      HCCBR1(N)= HCCBR1(N-1)
      DO I=37,39
        HCCBR1(N)= HCCBR1(N) + BR(I)*HCCON(N)
           BRC(N)= BRC(N)    + BR(I)
      END DO
      N= N+1
      HCCBR1(N)= HCCBR1(N-1)
      DO I=40,48
        HCCBR1(N)= HCCBR1(N) + BR(I)*HCCON(N)
           BRC(N)= BRC(N)    + BR(I)
      END DO
      N= N+1
      HCCBR1(N)= HCCBR1(N-1) + BR(NCH)*HCCON(N)
         BRC(N)=               BR(NCH)
      HCCBR1(0)= HCCBR1(N)
      IF( HCCBR1(0).GT.0.D0 ) THEN
        DO I=1,NDCH
          HCCBR1(I)= HCCBR1(I)/HCCBR1(0)
        END DO
      END IF
C - Subchannel cumulative BR
      HCCBR2(13,1)= BR(13)
      HCCBR2(13,2)= HCCBR2(13,1)
      DO I=14,15
        HCCBR2(13,I-12)= HCCBR2(13,I-13) + BR(I)
      END DO
      HCCBR2(14,1)= BR(16)
      HCCBR2(14,2)= HCCBR2(14,1)
      DO I=17,24
        HCCBR2(14,I-15)= HCCBR2(14,I-16) + BR(I)
      END DO
      HCCBR2(15,1)= BR(25)
      HCCBR2(15,2)= HCCBR2(15,1)
      DO I=26,27
        HCCBR2(15,I-24)= HCCBR2(15,I-25) + BR(I)
      END DO
      HCCBR2(16,1)= BR(28)
      HCCBR2(16,2)= HCCBR2(16,1)
      DO I=29,36
        HCCBR2(16,I-27)= HCCBR2(16,I-28) + BR(I)
      END DO
      HCCBR2(17,1)= BR(37)
      HCCBR2(17,2)= HCCBR2(17,1)
      DO I=38,39
        HCCBR2(17,I-36)= HCCBR2(17,I-37) + BR(I)
      END DO
      HCCBR2(18,1)= BR(40)
      HCCBR2(18,2)= HCCBR2(18,1)
      DO I=41,48
        HCCBR2(18,I-39)= HCCBR2(18,I-40) + BR(I)
      END DO
C - Normalize them to 1.
      DO I=13,18
        DO J=1,NSCH(I)
          IF( HCCBR2(I,NSCH(I)).GT.0.D0 )
     .        HCCBR2(I,J)= HCCBR2(I,J)/HCCBR2(I,NSCH(I))
        END DO
      END DO
C
      RETURN
      END

      SUBROUTINE HCDECC(IDCP,IDCM)
C-----------------------------------------------------------------------
C! Choose decay channels for Charged Higgses
C!
C!    Input:         ihcd(2) in common block /HCCOMM/
C!    Output:        IDCP   channel number for H+ decay
C!                   IDCM   channel number for H- decay
C!
C!    Gerardo Ganis  --  5 May 1996
C-----------------------------------------------------------------------
      IMPLICIT   NONE
C - Parameters & Constants
C - Common blocks
      include 'hccomm.inc'
      include 'hcdeff.inc'
C - Arguments
      INTEGER    IDCP, IDCM
C - Auxiliary variables
      REAL*8     RAN(1)
      INTEGER    I, J, K, CHAN(2), SUBC(2), IDC(2)
      LOGICAL    FIRST, NOTYET
C - Data statements
      DATA       FIRST /.TRUE./
C - Macros
      NOTYET(I)= (I.GE.13.AND.I.LE.15) .OR. I.EQ.49
C - Statements
      IF( FIRST ) THEN
        DO I=1,NCH
          HCDSTA(I)= 0
        END DO
      END IF
      DO I=1,2
        SUBC(I)= 1
        IF( IHCD(I).GT.0 .AND. IHCD(I).LE.NDCH ) THEN
          CHAN(I)= IHCD(I)
          IF( FIRST .AND. NOTYET( CHAN(I) ) )
     .                           WRITE(*,101) HCCHAN(CHAN(I))
        ELSE
          IF( IHCD(I).NE.0 ) THEN
            IF( FIRST ) WRITE(*,100) I, IHCD(I)
            IHCD(I)= 0
          END IF
          CALL HCRNDM(RAN,1)
          DO J=1,NDCH
            IF( RAN(1).LT.HCCBR1(J) ) GOTO 1
          END DO
 1        CONTINUE
          CHAN(I)= J
        END IF
C - Choose subchannel (if the case)
        IF( NSCH(CHAN(I)).GT.1 ) THEN
          CALL HCRNDM(RAN,1)
          DO J=1,NSCH(CHAN(I))
            IF( RAN(1).LT.HCCBR2(CHAN(I),J) ) GOTO 2
          END DO
 2        CONTINUE
          SUBC(I)= J
        END IF
      END DO
C - Get internal channel number
      DO I=1,2
        IDC(I)= 0
        DO J=1,CHAN(I)-1
          DO K=1,NSCH(J)
            IDC(I)= IDC(I)+1
          END DO
        END DO
        IDC(I)= IDC(I)+SUBC(I)
      END DO
C - Define output variables
      IDCP= IDC(1)
      IDCM= IDC(2)
      CALL HCRNDM(RAN,1)
      IF( RAN(1).GT. 0.5D0 ) THEN
        IDCP= IDC(2)
        IDCM= IDC(1)
      END IF
C - Statistics
      HCDSTA(0)= HCDSTA(0)+2
      HCDSTA(IDC(1))= HCDSTA(IDC(1))+1
      HCDSTA(IDC(2))= HCDSTA(IDC(2))+1
C
 100  FORMAT(//,'*******************************************',/,
     .   'WARNING: channel for decay ',I1,' unknown (',I6,')',/,
     .          '*** Set it to 0 for the rest of the run ***',//)
 101  FORMAT(//,
     .    '***************************************************',/,
     .    '*** The channel chosen has not been implemented ***',/,
     .    '*** properly yet; it does not contribute to the ***',/,
     .    '*** total width, and it decays the H+- through  ***',/,
     .    '*** phase-space only. BE AWARE OF THIS.         ***',/,
     .    '*** Channel :',A17,           '                 ***',/,
     .    '***************************************************',//)
C
      IF( FIRST ) FIRST= .FALSE.
C
      RETURN
      END

      SUBROUTINE HCDECY(ID,CHAN,STATUS)
C-----------------------------------------------------------------------
C! Decay H+ particle in /PYJETS/ position ID through channel CHAN
C!
C!    Input:          ID     /PYJETS/ position of decaying particle
C!                    CHAN   decay channel
C!    Output:         in /PYJETS/
C!                    STATUS     =0     successful
C!                               =1     decay not kinematically allowed
C!                               =2,3   decays not correctly defined in
C!                                      JETSET commons
C!
C!    Called by ASKUSE
C!
C!    Gerardo Ganis              3 May 1996
C-----------------------------------------------------------------------
      IMPLICIT   NONE
C - Parameters & Constants
C - Arguments
      INTEGER    ID, CHAN, STATUS
C - Common blocks
      include 'hcdeff.inc'
C - JETSET common for event record
      INTEGER    NJMAX, NTMAX
      PARAMETER( NJMAX= 4000, NTMAX= 5)
      INTEGER    NJET, NPAD, KJET
      REAL*8     PJET, VJET
      COMMON /PYJETS/ NJET, NPAD, KJET(NJMAX,NTMAX), PJET(NJMAX,NTMAX),
     .                      VJET(NJMAX,NTMAX)
C - JETSET parameter commons
      INTEGER     MSTU, MSTJ
      REAL*8      PARU, PARJ
      COMMON /PYDAT1/ MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      INTEGER     KCHG
      REAL*8      PMAS, PARF, VCKM
      COMMON /PYDAT2/ KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      INTEGER     MDCY, MDME, KFDP
      REAL*8      BRAT
      COMMON /PYDAT3/ MDCY(500,3),MDME(2000,2),BRAT(2000),
     .                KFDP(2000,5)
C - Auxiliary variables
      INTEGER     I, J, K, L, MDME1OLD(NCH), NJOIN, IJOIN(2),
     .            KDC, IFC, NDC,
     .            MDCY25, MDCY36, MDCY35, MDCY37
      REAL*8      MHC
      REAL*8      MASS, QMX
      LOGICAL     D2B, D2BQ, D3BQ
C - Data statements
C - Macros
      D2B(I)=     I.GE.1 .AND. I.LE.12
      D2BQ(I)=    I.GE.4 .AND. I.LE.12
      D3BQ(I)=   (I.GE.16 .AND. I.LE.24) .OR.
     .           (I.GE.28 .AND. I.LE.36)
      MASS(I,J)= SQRT((PJET(I,4)+PJET(J,4))**2 -
     .                (PJET(I,1)+PJET(J,1))**2 -
     .                (PJET(I,2)+PJET(J,2))**2 -
     .                (PJET(I,3)+PJET(J,3))**2 )
C - Statements
      STATUS= 0
C - Get pointers to tables ...
      KDC= ABS(KJET(ID,2))
      MHC= DBLE(PJET(ID,5))
      IFC= MDCY(KDC,2)
      NDC= MDCY(KDC,3)
C - Check whether the channel is kinematically allowed
      IF( MHC.LT.THRES(CHAN) ) THEN
        STATUS= 1
        RETURN
      END IF
C - Check whether decay channel is defined
      IF( IFC.EQ.0 .OR. CHAN.GT.NDC .OR. CHAN.LT.1 ) THEN
        STATUS= 2
        RETURN
      END IF
      IF( NDC.NE.NCH ) THEN
        STATUS= 3
        RETURN
      END IF
C - Inhibit all decays except the one wanted
      K=0
      DO I=IFC,IFC+NDC-1
        K= K+1
C - Keep track of the present status
        MDME1OLD(K)= MDME(I,1)
        IF( K.NE.CHAN ) THEN
          MDME(I,1)= -1
        ELSE
          MDME(I,1)=  1
        END IF
      END DO
C - Inhibit H, h and A decays
      MDCY25= MDCY(25,1)
      MDCY35= MDCY(35,1)
      MDCY36= MDCY(36,1)
      MDCY(25,1)= 0
      MDCY(35,1)= 0
      MDCY(36,1)= 0
C - Action now depends on the decay chosen ...
      IF( D2B(CHAN) ) THEN
C - Decay it
        CALL PYDECY(ID)
C - 2-body decay ... if quarks prepare fragmentation ...
        IF( D2BQ(CHAN) ) THEN
          NJOIN= 2
          IJOIN(1)= KJET(ID,4)
          IJOIN(2)= KJET(ID,5)
          QMX= MASS(IJOIN(1),IJOIN(2))
          CALL PYJOIN(NJOIN,IJOIN)
          CALL PYSHOW(IJOIN(1),IJOIN(2),QMX)
C - ... fragmentate ...
          MDCY37= MDCY(37,1)              ! Inhibit other H+ decay
          MDCY(37,1)= 0
          CALL PYEXEC
          MDCY(37,1)= MDCY37
        END IF
      ELSE
C - 3-body decay with neutral Higgs
        CALL GETHUD( ID, CHAN )
        IF( D3BQ(CHAN) ) THEN
          NJOIN= 2
          IJOIN(1)= KJET(ID,4)+1
          IJOIN(2)= KJET(ID,5)
          QMX= MASS(IJOIN(1),IJOIN(2))
          CALL PYJOIN(NJOIN,IJOIN)
          CALL PYSHOW(IJOIN(1),IJOIN(2),QMX)
C - ... fragmentate ...
          MDCY37= MDCY(37,1)             ! Inhibit other H+ decay
          MDCY(37,1)= 0
          CALL PYEXEC
          MDCY(37,1)= MDCY37
        END IF
      END IF
C - Restore old status
      K= 0
      DO I=IFC,IFC+NDC-1
        K= K+1
        MDME(I,1)= MDME1OLD(K)
      END DO
      MDCY(25,1)= MDCY25
      MDCY(35,1)= MDCY35
      MDCY(36,1)= MDCY36
C
      RETURN
      END

      SUBROUTINE HCFILH
C-----------------------------------------------------------------------
C! Fill some histo for H+H- production
C!
C!    Called by ASKUSE
C!
C!    Gerardo Ganis              3 May 1996
C-----------------------------------------------------------------------
      IMPLICIT   NONE
C - Parameters & Constants
      REAL*8     PI
      PARAMETER( PI= 3.141593 )
C - Arguments
C - Common blocks
      include 'hcdeff.inc'
      include 'miscl.inc'
C - JETSET common for event record
      INTEGER    NJMAX, NTMAX
      PARAMETER( NJMAX= 4000, NTMAX= 5)
      INTEGER    NJET, NPAD, KJET
      REAL*8     PJET, VJET
      COMMON /PYJETS/ NJET, NPAD, KJET(NJMAX,NTMAX), PJET(NJMAX,NTMAX),
     .                      VJET(NJMAX,NTMAX)
C - JETSET parameter commons
      INTEGER     MSTU, MSTJ
      REAL*8      PARU, PARJ
      COMMON /PYDAT1/ MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      INTEGER     KCHG
      REAL*8      PMAS, PARF, VCKM
      COMMON /PYDAT2/ KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      INTEGER     MDCY, MDME, KFDP
      REAL*8      BRAT
      COMMON /PYDAT3/ MDCY(500,3),MDME(2000,2),BRAT(2000),
     .                KFDP(2000,5)
C - Auxiliary variables
      INTEGER     I, J, K, L
      REAL*8      MASS, QMX, MH, GH, MHP, MHM, THP, THM, CTHS,
     .            PYP, EVIS, EB, ETP, ETM, EPP, EPM
      LOGICAL     FIRST, VISIB
C - Data statements
      DATA        FIRST /.TRUE./
C - Macros
      MASS(I,J)= SQRT((PJET(I,4)+PJET(J,4))**2 -
     .                (PJET(I,1)+PJET(J,1))**2 -
     .                (PJET(I,2)+PJET(J,2))**2 -
     .                (PJET(I,3)+PJET(J,3))**2 )
      VISIB(I) = ABS(I).NE.12 .AND. ABS(I).NE.14 .AND. ABS(I).NE.16
C - Statements
      IF( FIRST ) THEN
        MH= PMAS(37,1)
        GH= PMAS(37,2)
C - Giulio: commented HBOOK calls
C        CALL HBOOK2(10001,'generated widths',
C     .            100,MH-20*GH,MH+20*GH,100,MH-20*GH,MH+20*GH,0.)
C        CALL HBOOK1(10002,' cos theta * ',50,-1.,1.,0.)
C        CALL HBOOK1(10003,' evis ',60,0.,200.,0.)
C        CALL HBOOK1(10004,' tau- energy (Ebeam units)',50,0.,1.,0.)
C        CALL HBOOK1(10005,' tau+ energy (Ebeam units)',50,0.,1.,0.)
C        CALL HBOOK1(10006,' pi- + pi+ ene/eb - lab frame',50,0.,2.,0.)
C        CALL HBOOK1(10007,' pi- ene/eb - lab frame',50,0.,1.,0.)
C        CALL HBOOK1(10008,' pi+ ene/eb - lab frame',50,0.,1.,0.)
        FIRST= .FALSE.
      END IF
C - get HIGGS
      EB  = ECM/2.
      EVIS= 0.
      DO I=1,NJET
        IF( KJET(I,2).EQ.37 ) THEN
           THP= PYP(I,13)
           MHP= PJET(I,5)
           DO J= KJET(I,4), KJET(I,5)
             IF( KJET(J,2).EQ.-15 ) THEN
               ETP= PJET(J,4)
               DO K= KJET(J,4), KJET(J,5)
                 IF( KJET(K,2).EQ.211 ) EPP= PJET(K,4)
               END DO
             END IF
           END DO
        END IF
        IF( KJET(I,2).EQ.-37 ) THEN
           THM= PYP(I,13)
           MHM= PJET(I,5)
           DO J= KJET(I,4), KJET(I,5)
             IF( KJET(J,2).EQ. 15 ) THEN
               ETM= PJET(J,4)
               DO K= KJET(J,4), KJET(J,5)
                 IF( KJET(K,2).EQ.-211 ) EPM= PJET(K,4)
               END DO
             END IF
           END DO
        END IF
        IF( KJET(I,1).EQ.1 ) THEN
          IF( VISIB(KJET(I,2)) ) EVIS= EVIS + PJET(I,4)
        END IF
      END DO
C - Calculate cos(theta*)
      CTHS= COS(.5*(THM+PI-THP))/COS(.5*(THM-PI+THP))
C - Fill histos
C - Giulio: Commented histos
C      CALL HF2(10001,MHP,MHM,1.)
C      CALL HF1(10002,CTHS,1.)
C      CALL HF1(10003,EVIS,1.)
C      CALL HF1(10004,ETP/EB,1.)
C      CALL HF1(10005,ETM/EB,1.)
C      CALL HF1(10006,(EPP+EPM)/EB,1.)
C      CALL HF1(10007,EPP/EB,1.)
C      CALL HF1(10008,EPM/EB,1.)
C
      RETURN
      END

      SUBROUTINE HCH_EKELBOUNDS(X,Y1,Y2)
C-----------------------------------------------------------------------------
C!    G.Ganis              Nov 21, 1999

C! Gets integration bounds on the second variable as a function of the
C! first one
C!                           El -> Ekmin Ekmax
C!
C!     Input:     X          first variable
C!    Output:    Y1          lower bound on the second variable
C!               Y2          upper bound on the second variable
C-----------------------------------------------------------------------------
      IMPLICIT   NONE
C - Parameters & Constants
      REAL*8     SMALL
      PARAMETER( SMALL= 1D-16 )
C - Arguments
      INTEGER    OPT
      REAL*8     X, Y1, Y2
C - Common blocks
      REAL*8     WM, MMS, GGS, E3MIN, E3MAX, E4MIN, E4MAX, WMEX
      COMMON /PARPS3N/ WM(4), MMS, GGS, E3MIN, E3MAX,
     .           E4MIN, E4MAX, WMEX
C - Auxiliary variables
      REAL*8     M, M1, M2, M3, MI, MJ, ML, MK,
     .           E1, E1MIN, E1MAX, P1, E2MIN, E2MAX, 
     .           G1, BG1, M23, E2S, P2S 
C - Data statements
C - Macros
C - Statements
      MJ= WM(1)
      MI= WM(2)
      MK= WM(3)
      ML= WM(4)
      E1= X
      M = MJ
      M3= MI
      Y1= -1.D0
      Y2= -1.D0
      E1MIN= E4MIN
      E1MAX= E4MAX
      M1   = ML
      M2   = MK
C - Get limits
      IF( E1.GE.E1MIN .AND. E1.LE.E1MAX ) THEN
        E2MIN= -1.D0
        E2MAX= -1.D0
C - Mass of the {2,3} system
        M23= M*SQRT( MAX((1D0-M1/M)**2-2D0*(E1-M1)/M,SMALL)  )
C - Boost
        P1= (E1+M1)*(E1-M1)
        IF( P1.GT.0.D0 ) THEN
          P1= SQRT(P1)
        ELSE
          P1= 0.D0
        END IF
        G1= (M-E1) / M23
        BG1= P1 / M23
C - In the rest frame of the {2,3} system
        E2S= M23/2D0 *( 1D0 + (M2**2 - M3**2)/M23**2 )
        P2S= SQRT( MAX(ABS((E2S- M2)*(E2S+M2)),SMALL) )
C - Boost back
        E2MIN= G1*E2S - BG1*P2S 
        E2MAX= G1*E2S + BG1*P2S 
      ELSE
        WRITE(*,19) '_HCH_EKELBOUNDS_ : E1 out of range ',E1,E1MIN,E1MAX
      END IF
C - Output
      Y1= E2MIN
      Y2= E2MAX
C - Over!
      RETURN
 19   FORMAT(1X,A40,G12.4,' not in [',G12.4,',',G12.4,']')
      END

      SUBROUTINE HCH_GETIBOUND(X,Y1,Y2)
C-----------------------------------------------------------------------------
C!    G.Ganis              Nov,21 1999
C! Gets integration bounds on the second variable as a function of the
C! first one   
C!                         dEl dxS: El -> Smin Smax
C!
C!    Input:      X          first variable
C!    Output:    Y1          lower bound on the second variable
C!               Y2          upper bound on the second variable
C-----------------------------------------------------------------------------
      IMPLICIT   NONE
C - Parameters & Constants
C - Arguments
      REAL*8     X, Y1, Y2
C - Common blocks
      REAL*8     WM, MMS, GGS, E3MIN, E3MAX, E4MIN, E4MAX, WMEX
      COMMON /PARPS3N/ WM(4), MMS, GGS, E3MIN, E3MAX,
     .           E4MIN, E4MAX, WMEX
C - Auxiliary variables
      REAL*8     EMIN, EMAX
C - Data statements
C - Macros
C - Statements
      CALL HCH_EKELBOUNDS(X,EMIN,EMAX)
      Y1= WM(2)**2 - WM(1)**2 + 2.D0*WM(1)*(X+EMIN)
      Y2= WM(2)**2 - WM(1)**2 + 2.D0*WM(1)*(X+EMAX)
C - Over!
      RETURN
      END

      LOGICAL FUNCTION HCPS3(MH,M1,M2,M3,E2,E3,CT23)
C----------------------------------------------------------------------
C!    Check 3-body phase space
C!
C!    Input:           MH  = decaying particle mass (GeV)
C!                     M1, M2, M3 = masses of decay products (GeV)
C!                     E2, E3 = energies of particles with mass M2, M3 (
C!    Output:          HCPS3= .TRUE. if good phase space configuration
C!                     CT23 = cosine of the angle between 2 and 3 direct
C!                     ( = -10 if the check is .FALSE. )
C!
C!    Called by HHUDWD
C!
C!    Gerardo Ganis              3 May 1996
C----------------------------------------------------------------------
      IMPLICIT    NONE
C - Arguments
      REAL*8      MH, M1, M2, M3,
     .            E2, E3, CT23
C - Auxiliary variables
      REAL*8      P2, P3, A, B
C
      HCPS3= .FALSE.
      CT23= -10.D0
C - Check low bound
      IF( E2 .LT. M2 )         RETURN
      IF( E3 .LT. M3 )         RETURN
C - Check sum
      IF( E2+E3 .GT. MH-M1 ) RETURN
C - From phase space definition
      P2= SQRT( E2**2- M2**2 )
      P3= SQRT( E3**2- M3**2 )
      A  = MH**2-M1**2+M2**2+M3**2-2*MH*(E2+E3)+2*E2*E3
      B  = 2*P2*P3
      IF( ABS(A).GE. B  )   RETURN
C
      CT23= A/B
      HCPS3= .TRUE.
C
      RETURN
      END

      SUBROUTINE HCRNDM( RANDOM, LEN )
C----------------------------------------------------------------------
C! Get LEN random numbers in [0,1]
C!
C!    Input:     LEN     = wanted number of randoms
C!    Output:    RANDOM  = array dimensioned at least LEN with randoms
C!
C!    Called by many
C!
C!    Gerardo Ganis              3 May 1996
C----------------------------------------------------------------------
      IMPLICIT    NONE
      REAL*8    RANDOM(*)
      INTEGER   LEN
      REAL*8    RNDMCMSSW
      INTEGER   I, DUM
C
      DUM= 0
      DO I=1,LEN
        RANDOM(I)= RNDMCMSSW(DUM)
      END DO
C
      RETURN
      END

      SUBROUTINE HCSETJ( DBG )
C-----------------------------------------------------------------------
C! Define decay channels and branching fractions for Charged Higgs ( KC=
C! in JETSET framework
C!
C!    Input:           none
C!    Output:          in JETSET common blocks
C!
C!    Called by ASKUSI
C!
C!    Gerardo Ganis              3 May 1996
C-----------------------------------------------------------------------
      IMPLICIT   NONE
C - Parameters & Constants
      REAL*8     HBAR
      PARAMETER( HBAR= 6.583173D-25 )
      INTEGER    IHC
      PARAMETER( IHC= 37 )
C - Arguments
      INTEGER    DBG
C - Common blocks
      include 'hccomm.inc'
      include 'hcdeff.inc'
C - JETSET parameter commons
      INTEGER    KCHG
      REAL*8     PMAS, PARF, VCKM
      COMMON /PYDAT2/ KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
      INTEGER    MDCY, MDME, KFDP
      REAL*8     BRAT
      COMMON /PYDAT3/ MDCY(500,3),MDME(2000,2),BRAT(2000),
     .                KFDP(2000,5)
C - Auxiliary variables
      REAL*8     BR( 0:NCH ), WD( 0:NCH ), FRAC, BRC(NDCH)
      INTEGER    KC, ILD, ILDMAX, K, KUP, KDW, I
      CHARACTER  STAR*2
C - Data statements
C - Macros
C - Statements
C - Get Branching Fractions
      CALL HCBRAT(BR,WD)
C - Lifetime in seconds
      TAUHP= 0.D0
      IF( WD(0).GT.0.D0 ) TAUHP= HBAR / WD(0)
C - Set parameters
      PMAS(IHC,1)= SNGL(MH(4))           ! Mass ...
      PMAS(IHC,2)= SNGL(WD(0))        ! Width ...
      PMAS(IHC,3)= SNGL(MIN(MH(4),10.*WD(0))) ! Deviations from mass ...
      MDCY(IHC,1)= 1                  ! Allow for H+ decay ...
C - Get first free position in MDME, BRAT, KFDP
      ILDMAX= 0
      DO KC=1,500
        IF( KC.NE.IHC ) THEN
          ILD= MDCY(KC,2) + MDCY(KC,3) - 1
          IF( ILD.GT.ILDMAX ) ILDMAX= ILD
        END IF
      END DO
      IF( (2000-ILDMAX).LT.NCH ) THEN
        WRITE(*,*) '_HCSETJ_ - Not enough space to define decays'
        RETURN
      END IF
C - Define entry point in the decay table
      MDCY(IHC,2)= ILDMAX+1
C - Define total number of decays defined
      MDCY(IHC,3)= NCH
C _ Enable all the decays
      K= 0
      DO I=MDCY(IHC,2),MDCY(IHC,2)+MDCY(IHC,3)-1
        K=K+1
        MDME( I, 1)= 1
C - Decays into ud
        IF( K.LE.3 ) THEN
          MDME( I, 2)= 0     ! space phase ...
          KUP=   K*2 + 10
          KDW= -(K*2 +  9)
          KFDP( I, 1)= KUP
          KFDP( I, 2)= KDW
          KFDP( I, 3)= 0
          THRES(K)= DBLE(PMAS(KUP,1)+PMAS(ABS(KDW),1))
        ELSE IF( K.LE.9 ) THEN
          MDME( I, 2)= 0    ! the same, but shower allowed ...
          KUP= ((K-1)/3)*2
          KDW= K - ((K-1)/3)*3
          KDW=-(KDW*2-1)
          KFDP( I, 1)= KUP
          KFDP( I, 2)= KDW
          KFDP( I, 3)= 0
          THRES(K)= DBLE(PMAS(KUP,1)+PMAS(ABS(KDW),1))
        ELSE IF( K.LE.12 ) THEN
          MDME( I, 2)= 0    ! the same, but shower allowed ...
          KDW= K - ((K-1)/3)*3
          KDW=-(KDW*2-1)
          KFDP( I, 1)= 24                  ! W+
          KFDP( I, 2)=  5                  ! b
          KFDP( I, 3)= KDW                 ! d bar
          KFDP( I, 4)= 0
          THRES(K)= DBLE(PMAS(24,1)+PMAS(5,1)+PMAS(ABS(KDW),1))
C - Decays into hud
        ELSE IF( K.LE.15 ) THEN
          MDME( I, 2)= 0     ! space phase ...
          KUP=   (K-12)*2 + 10
          KDW= -((K-12)*2 +  9)
          KFDP( I, 1)= 25
          KFDP( I, 2)= KUP
          KFDP( I, 3)= KDW
          KFDP( I, 4)= 0
          THRES(K)= DBLE(PMAS(25,1)+PMAS(KUP,1)+PMAS(ABS(KDW),1))
        ELSE IF( K.LE.24 ) THEN
          MDME( I, 2)= 0    ! the same, but shower allowed ...
          KUP= ((K-13)/3)*2
          KDW= K - ((K-1)/3)*3
          KDW=-(KDW*2-1)
          KFDP( I, 1)= 25
          KFDP( I, 2)= KUP
          KFDP( I, 3)= KDW
          KFDP( I, 4)= 0
          THRES(K)= DBLE(PMAS(25,1)+PMAS(KUP,1)+PMAS(ABS(KDW),1))
C - Decays into Aud
        ELSE IF( K.LE.27 ) THEN
          MDME( I, 2)= 0     ! space phase ...
          KUP=   (K-24)*2 + 10
          KDW= -((K-24)*2 +  9)
          KFDP( I, 1)= 36
          KFDP( I, 2)= KUP
          KFDP( I, 3)= KDW
          KFDP( I, 4)= 0
          THRES(K)= DBLE(PMAS(36,1)+PMAS(KUP,1)+PMAS(ABS(KDW),1))
        ELSE IF( K.LE.36 ) THEN
          MDME( I, 2)= 0    ! the same, but shower allowed ...
          KFDP( I, 1)= 36
          KUP= ((K-25)/3)*2
          KDW= K - ((K-1)/3)*3
          KDW=-(KDW*2-1)
          KFDP( I, 2)=  KUP
          KFDP( I, 3)=  KDW
          KFDP( I, 4)= 0
          THRES(K)= DBLE(PMAS(36,1)+PMAS(KUP,1)+PMAS(ABS(KDW),1))
C - Decays into Hud
        ELSE IF( K.LE.39 ) THEN
          MDME( I, 2)= 0     ! space phase ...
          KUP=   (K-36)*2 + 10
          KDW= -((K-36)*2 +  9)
          KFDP( I, 1)= 35
          KFDP( I, 2)= KUP
          KFDP( I, 3)= KDW
          KFDP( I, 4)= 0
          THRES(K)= DBLE(PMAS(35,1)+PMAS(KUP,1)+PMAS(ABS(KDW),1))
        ELSE IF( K.LE.48 ) THEN
          MDME( I, 2)= 0    ! the same, but shower allowed ...
          KFDP( I, 1)= 35
          KUP= ((K-37)/3)*2
          KDW= K - ((K-1)/3)*3
          KDW=-(KDW*2-1)
          KFDP( I, 2)=  KUP
          KFDP( I, 3)=  KDW
          KFDP( I, 4)= 0
          THRES(K)= DBLE(PMAS(35,1)+PMAS(KUP,1)+PMAS(ABS(KDW),1))
        ELSE IF( K.EQ.NCH ) THEN
          MDME( I, 2)= 0    ! the same, but shower allowed ...
          KFDP( I, 1)= 51   ! chi0
          KFDP( I, 2)= 55   ! chi+
          KFDP( I, 3)= 0
          THRES(K)= DBLE(PMAS(51,1)+PMAS(55,1))
        END IF
        BRAT(I)= BR(K)
      END DO
C - Calculate cumulative BR
      CALL HCCUBR(BR,BRC)
C - Print out now
      IF( DBG.GE.0 ) THEN
        WRITE(*,90) MH(4),WD(0)
        DO I=1,NDCH
          STAR= '  '
          IF( IHCD(1).EQ.0 .AND. IHCD(2).EQ.0 ) THEN
            IF( HCCON(I).EQ.1 ) STAR= '**'
          ELSE IF( IHCD(1).EQ.0 .AND. IHCD(2).GT.0 ) THEN
            IF( HCCON(I).EQ.1 ) THEN
              IF( I.EQ.IHCD(2) ) THEN
                STAR= '**'
              ELSE
                STAR= '* '
              END IF
            END IF
          ELSE IF( IHCD(1).GT.0 .AND. IHCD(2).EQ.0 ) THEN
            IF( HCCON(I).EQ.1 ) THEN
              IF( I.EQ.IHCD(1) ) THEN
                STAR= '**'
              ELSE
                STAR= ' *'
              END IF
            END IF
          ELSE IF( IHCD(1).GT.0 .AND. IHCD(2).GT.0 ) THEN
            IF( I.EQ.IHCD(1) .AND. I.EQ.IHCD(2) ) THEN
              STAR= '**'
            ELSE IF( I.EQ.IHCD(1) .AND. I.NE.IHCD(2) ) THEN
              STAR= '* '
            ELSE IF( I.EQ.IHCD(2) .AND. I.NE.IHCD(1) ) THEN
              STAR= ' *'
            END IF
          END IF
          FRAC= BRC(I)
          IF( HCCON(I).GE.0 ) 
     .        WRITE(*,91) HCCHAN(I), FRAC*100.D0, STAR
        END DO
        WRITE(*,92)
      END IF
 90   FORMAT(/50('-')//
     .  1X,'The following branching ratios have been computed :',/,
     .  1X,'    H+ mass               : ',F9.4,' GeV/c**2',/,
     .  1X,'    Total decay width     : ',F9.4,' GeV',//)
 91   FORMAT(1X,'Channel ',A17,' --- BR = ',F10.5,' % (',A2,')')
 92   FORMAT(//,1X,' o note: the * indicate the channels open',
     .             ' for the two Higgses, respectively ',//)
      RETURN
      END

      FUNCTION HHUDME(OPT,M,Q)
C-----------------------------------------------------------------------
C! Matrix element for the process H+ -> H0 u d
C
C     Input:     OPT    0= full propagator in, 1= progator out (for integration)
C                M(i) masses in GeV/c**2: i=1  m_H0
C                                          =2  m_u
C                                          =3  m_d
C                                          =4  m_H+
C                Q(i,j) dot products P(i)*metric*P(j)
C                       i,j=1,4 ( H0, u, d, H+ )
C     Output:    HHUDME
C
C     Called by HHUDWD, GETHUD
C
C     Gerardo Ganis              3 May 1996
C                         Mod   21 Nov 1999
C-----------------------------------------------------------------------
      IMPLICIT   NONE
      REAL*8     HHUDME
C - Parameters & Constants
      INTEGER    NV
      PARAMETER( NV= 4 )
      REAL*8     PI
      PARAMETER( PI= 3.141592653589793238 )
C - Arguments
      INTEGER    OPT
      REAL*8     M(NV),Q(NV,NV)
C - Common blocks
C - JETSET parameter commons
      INTEGER     MSTU, MSTJ
      REAL*8      PARU, PARJ
      COMMON /PYDAT1/ MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      INTEGER     KCHG
      REAL*8      PMAS, PARF, VCKM
      COMMON /PYDAT2/ KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
C - Auxiliary variables
      REAL*8      MW, GW, SW2, EE,
     .            PW, KK, KAP, MER
      REAL*8      PYALEM
C - Data statements
C - Macros
C - Statements
      MW = DBLE(PMAS(24,1))
      GW = DBLE(PMAS(24,2))
      SW2= DBLE(PARU(102))
      EE = DBLE(PYALEM(SNGL(M(4)**2))) * 4.D0 * PI
C - Check if kinematically allowed
      HHUDME= 0.D0
      IF( M(4).LT.(M(1)+M(2)+M(3)) ) RETURN
C - Propagator
      KK = M(2)**2+M(3)**2+2.D0*Q(2,3)
      PW = 1.D0/((KK-MW**2)**2+(MW*GW)**2)
C - Kappa
      KAP= (M(4)+M(1))*(M(4)-M(1))/(MW**2)
C - reduced matrix element
      MER= 4.D0*(Q(1,3)+Q(3,4))*(Q(1,2)+Q(2,4)) -
     .     2.D0*(M(1)**2+M(4)**2+2.D0*Q(1,4))*Q(2,3) +
     .     2.D0*(KAP**2)*Q(2,3)*(M(2)**2+M(3)**2) +
     .     4.D0*KAP*M(2)*M(3)*(Q(1,2)+Q(2,4)-Q(1,3)-Q(3,4)) -
     .     4.D0*((KAP*M(2)*M(3))**2)
C - Matrix element
      IF( OPT.EQ.1 ) THEN
        HHUDME= (EE/SW2)**2 /8.D0 * MER
      ELSE
        HHUDME= (EE/SW2)**2 /8.D0 * PW * MER
      END IF
C - Over!
      RETURN
      END

      REAL*8 FUNCTION HHUDWD(MH,MH0,GRED,IU,ID,MEMX)
C-----------------------------------------------------------------------
C! Calculate the width for the decay H+ -> H0 u d                       
C!                                                                      
C!    Input:     MH              H+ mass in GeV                         
C!               MH0             H0 mass in GeV                         
C!               GRED            reduced coupling                       
C!                               sin(alpha-beta)    for H0= H           
C!                               cos(alpha-beta)    for H0= h           
C!                               1.                 for H0= A           
C!               IU              JETSET code for up-like fermion        
C!               ID              JETSET code for down-like antifermion  
C!    Output:    HHUDWD          Width in GeV      
C!               MEMX            ME Maximum ... for generation purposes
C!                                                                      
C!    Called by HCBRAT                                                  
C!                                                                      
C!    Gerardo Ganis              3 May 1996                             
C!                         Mod  21 Nov 1999
C-----------------------------------------------------------------------
      IMPLICIT   NONE
C - Parameters & Constants
      INTEGER    NV
      PARAMETER( NV= 4 )
      REAL*8     PI
      PARAMETER( PI= 3.141592653589793238 )
C - Arguments
      INTEGER    IU, ID
      REAL*8     MH, MH0, GRED, MEMX
C - Common blocks
      REAL*8     WM, MMS, GGS, E3MIN, E3MAX, E4MIN, E4MAX, WMEX
      COMMON /PARPS3N/ WM(4), MMS, GGS, E3MIN, E3MAX,
     .           E4MIN, E4MAX, WMEX
C - JETSET parameter commons
      INTEGER    KCHG
      REAL*8     PMAS, PARF, VCKM
      COMMON /PYDAT2/ KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
C - Auxiliary variables
      INTEGER    I, J, K, L, NG
      REAL*8     M(NV), ME, RAN(4), QF, PHASE,
     .           DE2, DE3, E2, E3, CT23, XN,
     .           XMIN, XMAX, X,
     .           GETCKM, DGMLT2
      LOGICAL    QUARK
      EXTERNAL   GETCKM, HHUDWDX2, DGMLT2
C - Data statements
C - Macros
      QUARK(I)=  ABS(I).GE.1 .AND. ABS(I).LE.6
C - Statements
      M(1)= MH0
      M(2)= DBLE(PMAS(IU,1))
      M(3)= DBLE(PMAS(ID,1))
      M(4)= MH
C - Check if the decay is possible
      HHUDWD= 0.D0
      IF( M(4).LT.(M(1)+M(2)+M(3)) ) RETURN
C - Energy ranges .
      E3MIN= M(2)
      E3MAX= M(4)/2.D0*(1.D0+(M(2)**2-(M(1)+M(3))**2)/M(4)**2)
      E4MIN= M(3)
      E4MAX= M(4)/2.D0*(1.D0+(M(3)**2-(M(1)+M(2))**2)/M(4)**2)
C - Fill /PARPS3N/
      WM(1)= M(4)
      WM(2)= M(1)
      WM(3)= M(2)
      WM(4)= M(3)
      MMS  = DBLE(PMAS(24,1))
      GGS  = DBLE(PMAS(24,2))
      WMEX   = -1E10
C - Numerical integration
      XMIN= E4MIN
      XMAX= E4MAX
      XN  = 2.D0*M(4) * MMS*GGS
      HHUDWD= 4*PI*DGMLT2(HHUDWDX2,XMIN,XMAX,3,6,X) / XN
C - Multiply by CKM coupling and colour ( 1 for leptons )
      QF= GETCKM(IU,ID)**2
      IF( QUARK(IU) ) QF= QF*3.D0
C - Phase space factor
      PHASE= 1.D0 /( 8.D0 * ((2.D0*PI)**4) )
      HHUDWD= HHUDWD * QF * PHASE / (2.D0*M(4))
C - Maximum
      MEMX= WMEX
      IF( HHUDWD.GT.0D0 .AND. WMEX.LE.0D0 ) THEN
        WRITE(*,'(A,A,A)') ' +++ HHUDWD Warning : untrustable MAX ',
     .                   ' => Check your inputs ',
     .                   ' => Set equal to the width '
      END IF
C - Over !
      RETURN
      END

      SUBROUTINE HHUDWDX1(M,U1,F1,X)
C------------------------------------------------------------------------------
C------------------------------------------------------------------------------
      IMPLICIT   NONE
C - Parameters & Constants
      INTEGER    NV
      PARAMETER( NV= 4 )
C - Arguments
      INTEGER    M
      REAL*8     U1(*),F1(*),X(2)
C - Common blocks
      REAL*8     WM, MMS, GGS, E3MIN, E3MAX, E4MIN, E4MAX, WMEX
      COMMON /PARPS3N/ WM(4), MMS, GGS, E3MIN, E3MAX,
     .           E4MIN, E4MAX, WMEX
C - Auxiliary variables
      INTEGER    L, I, J 
      REAL*8     XMIN, XMAX, XG, X1, X2, X3, Q(4,4), E2, E3,
     .           MM(4), CT23, SB,
     .           HHUDME
      LOGICAL    HCPS3
      EXTERNAL   HCPS3, HHUDME
C - Data statements
C - Macros
C - Statements
      MM(4) = WM(1)
      MM(1) = WM(2)
      MM(2) = WM(3)
      MM(3) = WM(4)
      DO L=1,M
        X(1)= U1(L)
        SB= TAN(X(1))*MMS*GGS + MMS**2
        E2= (SB-WM(2)**2+WM(1)**2)/2.D0/WM(1) - X(2)
        E3  = X(2)
        F1(L)= 0.D0
        IF( HCPS3(MM(4),MM(1),MM(2),MM(3),E2,E3,CT23) ) THEN
          X2= E2/MM(4)
          X3= E3/MM(4)
          X1= 1.D0-X2-X3
C - Fill Q(i,j)= P(i)*metric*P(j)
          DO I=1,NV
            Q(I,I)= MM(I)**2
          END DO
          Q(1,2)= ((MM(4)**2)*(1.D0-2.D0*X3)+
     .              MM(3)**2-MM(1)**2-MM(2)**2)/2.D0
          Q(1,3)= ((MM(4)**2)*(1.D0-2.D0*X2)+
     .              MM(2)**2-MM(1)**2-MM(3)**2)/2.D0
          Q(2,3)= ((MM(4)**2)*(1.D0-2.D0*X1)+
     .              MM(1)**2-MM(2)**2-MM(3)**2)/2.D0
          Q(1,4)= (MM(4)**2)*X1
          Q(2,4)= (MM(4)**2)*X2
          Q(3,4)= (MM(4)**2)*X3
          DO I=1,NV
            DO J=I+1,NV
              Q(J,I)= Q(I,J)
            END DO
          END DO
C - Decay width
          F1(L)= HHUDME(1,MM,Q)
        END IF
C - For the maximum
        WMEX = MAX(HHUDME(0,MM,Q),WMEX)
      END DO
C - Over!
      RETURN
      END 

      SUBROUTINE HHUDWDX2(M,U2,F2,X)
C------------------------------------------------------------------------------
C------------------------------------------------------------------------------
      IMPLICIT   NONE
C - Parameters & Constants
C - Arguments
      INTEGER    M
      REAL*8     U2(*),F2(*),X(2)
C - Common blocks
      REAL*8     WM, MMS, GGS, E3MIN, E3MAX, E4MIN, E4MAX, WMEX
      COMMON /PARPS3N/ WM(4), MMS, GGS, E3MIN, E3MAX,
     .           E4MIN, E4MAX, WMEX
C - Auxiliary variables
      INTEGER    L
      REAL*8     XMIN, XMAX, DGMLT1
      EXTERNAL   HHUDWDX1, DGMLT1
C - Data statements
C - Macros
C - Statements
      DO L=1,M
        X(2)= U2(L)
        CALL HCH_GETIBOUND(X(2),XMIN,XMAX)
        XMIN= ATAN( (XMIN-MMS**2)/MMS/GGS )
        XMAX= ATAN( (XMAX-MMS**2)/MMS/GGS )
        F2(L)= DGMLT1(HHUDWDX1,XMIN,XMAX,3,6,X)
      END DO
C
      RETURN
      END 

      FUNCTION HUDWDT( MH, IUP, IDW, TBT, OPT)
C-----------------------------------------------------------------------
C! Implementation of H+->ud decay width accordingly to
C! eq.441, pag.405, LEP2 workshop proceed.
C! Modified for finite masses ...
C!
C!    Input:      MH       Charged Higgs mass in GeV/c**2
C!                IUP      JETSET code for up-like fermion
C!                IDW      JETSET code for down-like antifermion
C!                TBT      tan_beta, ratio between vev's (v2/v1)
C!                OPT      QCD radiative correction option (quark only)
C!                         =0  no correction (tree-level)
C!                         =1  tree-level + running quark masses
C!                         ( not available at present )
C!
C!    Output:     HUDWDT   width in GeV
C!
C!    Called by HCBRAT
C!
C!    Gerardo Ganis             21 May 1996
C-----------------------------------------------------------------------
      IMPLICIT    NONE
      REAL*8      HUDWDT
C - Parameters & Constants
      REAL*8      PI
      PARAMETER(  PI= 3.141592653589793238D0 )
C - Arguments
      REAL*8      MH, TBT
      INTEGER     IUP, IDW, OPT
C - JETSET parameter commons
      INTEGER     MSTU, MSTJ
      REAL*8      PARU, PARJ
      COMMON /PYDAT1/ MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      INTEGER     KCHG
      REAL*8      PMAS, PARF, VCKM
      COMMON /PYDAT2/ KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
C - Auxiliary variables
      REAL*8      GF, MW, SW2, HU, HD, RMU, RMD, MU0, MD0,
     .            GETCKM, ALFS, PHASE2, QCD, MMU, MMD, RQMASS,
     .            ALPHAS
      REAL*8      PYALEM, ULALPS
      INTEGER     I, NC
      LOGICAL     QUARK, FIRST
      EXTERNAL    PHASE2, GETCKM
C
      REAL*8      runmas, facdum
      EXTERNAL    runmas
C - Data statements
      DATA        FIRST /.TRUE./
C - Macros
      QUARK(I)=   I.GE.1 .AND. I.LE.6
C - Statements
      MW = DBLE(PMAS(24,1))
      SW2= DBLE(PARU(102))
      GF = DBLE(PYALEM(SNGL(MH**2)))*PI/SQRT(2.D0)/MW**2/SW2
C      ALFS = DBLE(ULALPS(SNGL(MH**2)))
C - Define parameters
      MU0= DBLE(PMAS(IUP,1))       ! Pole mass
      MD0= DBLE(PMAS(IDW,1))       ! Pole mass
      MMU= MU0
      MMD= MD0
      NC = 1
      QCD= 0.D0
      ALFS= 0.D0
      IF( QUARK(IUP) ) THEN
C ----------------------------------------------------------------------
C First order QCD corrections 
C        RMU= DBLE(RUNMAS( SNGL(MU0), 4, FACDUM ))
C        RMD= DBLE(RUNMAS( SNGL(MD0), 4, FACDUM ))
C        MMU= MU0*SQRT(RMU)
C        MMD= MD0*SQRT(RMD)
C ----------------------------------------------------------------------
        IF( OPT.EQ.1 ) THEN
C - Initialize and use Djouadi stuff instead ...
          IF( FIRST ) CALL RAMINI
          MMU= RQMASS(MH,IUP)
          MMD= RQMASS(MH,IDW)
          QCD= 1.D0
          ALFS = ALPHAS(MH,2)
          FIRST= .FALSE.
        END IF
        NC = 3
      END IF
C - Check if kinematically allowed
      HUDWDT= 0.D0
      IF( MH.LT.(MU0+MD0) ) RETURN
C - Define coupling constants (with renormalized masses, if quarks)
      HU= MMU/TBT
      HD= MMD*TBT
C - Total width
      HUDWDT= (NC*GF*MH)*2.D0/SQRT(2.D0) *
     .        ( (HU**2+HD**2)*(1.D0-(MMU**2+MMD**2)/MH**2) -
     .          4.D0*(MMU*MMD/MH)**2 ) *
     .          PHASE2(MH,MMU,MMD) * GETCKM(IUP,IDW)**2 *
     .        ( 1.D0 + 17.D0/3.D0*ALFS/PI*QCD )
C
      RETURN
      END

      FUNCTION PHASE2(M,M1,M2)
C-----------------------------------------------------------------------
C! Two-body phase space ( M -> M1 + M2 )
C!
C!    Input:      M          mass of the decaying particle
C!                M1,M2      masses of decay products
C!
C!    Output      PHASE2     beta(m1,m2)/8/pi
C!
C!    Gerardo Ganis  --  12 May 1996
C-----------------------------------------------------------------------
      IMPLICIT   NONE
      REAL*8     PHASE2
C - Parameters & Constants
      REAL*8      PI
      PARAMETER(  PI= 3.141592653589793238D0 )
C - Arguments
      REAL*8     M, M1, M2
C - Common blocks
C - Auxiliary variables
      REAL*8     B2
C - Data statements
C - Macros
C - Statements
      B2= 1.D0-2.D0*(M1**2+M2**2)/M**2+((M1**2-M2**2)/M**2)**2
C - Two body phase space
      PHASE2= SQRT(MAX(0.D0,B2)) / (8.D0*PI)
C - That's it
      RETURN
      END

      SUBROUTINE RAMINI
C-----------------------------------------------------------------------
C! Initialize stuff to run alpha_S and quark masses ...
C!
C! From A.Djouadi code ...
C!    G.Ganis        May 96
C-----------------------------------------------------------------------
      IMPLICIT   NONE
C - Parameters & Constants
      REAL*8     ACC
      PARAMETER( ACC= 1.0D-10 )
C - Arguments
C - Common blocks
      INTEGER    N0
      REAL*8     XLAMBDA, AMCA, AMBA, AMTA
      COMMON    /ALS/ XLAMBDA, AMCA, AMBA, AMTA, N0
      REAL*8     AMS, AMC, AMB, AMT, AMSB
      COMMON    /MASSES/ AMS, AMC, AMB, AMT, AMSB
C - JETSET parameter commons
      INTEGER     MSTU, MSTJ
      REAL*8      PARU, PARJ
      COMMON /PYDAT1/ MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      INTEGER     KCHG
      REAL*8      PMAS, PARF, VCKM
      COMMON /PYDAT2/ KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
C - Auxiliary variables
C - Data statements
C - Macros
C - Statements
      N0     = 5
C - Get relevant parameters from JETSET commons
      XLAMBDA= DBLE(PARU(112))
      AMS    = DBLE(PMAS(3,1))
      AMC    = DBLE(PMAS(4,1))
      AMB    = DBLE(PMAS(5,1))
      AMT    = DBLE(PMAS(6,1))
      AMSB   = AMS
      AMCA   = AMC
      AMBA   = AMB
      AMTA   = AMT
C - Init alpha_s stuff ...
      CALL ALSINI( ACC )

      RETURN
      END

      FUNCTION RQMASS(Q,KF)
C-----------------------------------------------------------------------
C! Get running quark mass
C!
C!    INPUT:          Q   scale (GeV)                         (REAL*8)
C!                    KF  number of flavours above threshold  (INTEGER)
C!    OUTPUT:         RQMASS  running quark mass              (REAL*8)
C!
C!    Commnets:       Adapted from function RUNP in Djouadi code.
C!    G.Ganis         May 96
C-----------------------------------------------------------------------
      IMPLICIT   NONE
      REAL*8     RQMASS
C - Parameters & Constants
C - Arguments
      INTEGER    KF
      REAL*8     Q
C - Common blocks
C - JETSET parameter commons
      INTEGER     KCHG
      REAL*8      PMAS, PARF, VCKM
      COMMON /PYDAT2/ KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
C - Auxiliary variables
      REAL*8      RUNM
C - Data statements
C - Macros
C - Statements
      IF( KF.EQ.1 .OR. KF.EQ.2 ) THEN
        RQMASS= DBLE(PMAS(KF,1))
        RETURN
      END IF
      IF( KF.GE.3 .AND. KF.LE.6 ) THEN
        RQMASS= RUNM(Q,KF,1)
      END IF
C
      RETURN
      END

      FUNCTION RUNM(Q,NF,IMSBAR)
C-----------------------------------------------------------------------
C!
C!    INPUT:          Q   scale (GeV)                         (REAL*8)
C!                    NF  number of flavours above threshold  (INTEGER)
C!    OUTPUT:         RUNM    running quark mass              (REAL*8)
C!
C!    Comments:       Cleaning of Djouadi code.
C!    Author:         A.Djouadi
C!    Cleaned by      G.Ganis         July 2, 96
C-----------------------------------------------------------------------
      IMPLICIT   NONE
      REAL*8     RUNM
C - Parameters
      INTEGER    NN
      REAL*8     ZETA3, ACC
      PARAMETER( NN= 6, ZETA3= 1.202056903159594D0, ACC= 1.D-8 )
C - Arguments
      INTEGER    NF
      REAL*8     Q
C - Common blocks
      INTEGER    N0A
      REAL*8     XLAMBDA, AMCA, AMBA, AMTA
      COMMON    /ALS/    XLAMBDA, AMCA, AMBA, AMTA, N0A
      REAL*8     AMS, AMC, AMB, AMT, AMSB
      COMMON    /MASSES/ AMS, AMC, AMB, AMT, AMSB
      REAL*8     XMSB, XMHAT, XKFAC
      COMMON    /RUN/    XMSB, XMHAT, XKFAC
C - Auxiliary variables
      INTEGER    I, J, ISTRANGE, IMSBAR, N0
      REAL*8     AM(NN), YMSB(NN),
     .           B0, B1, B2, G0, G1, G2, C0, C1, C2, CQ, TRAN,
     .           X, XK, PI, ALPHAS, DD, AMSU, AMSD, Q0
      SAVE ISTRANGE
C - Macro statements
      B0(I)=(33.D0-2.D0*I)/12D0
      B1(I) = (102D0-38D0/3D0*I)/16D0
      B2(I) = (2857D0/2D0-5033D0/18D0*I+325D0/54D0*I**2)/64D0
      G0(I) = 1D0
      G1(I) = (202D0/3D0-20D0/9D0*I)/16D0
      G2(I) = (1249D0-(2216D0/27D0+160D0/3D0*ZETA3)*I
     .       - 140D0/81D0*I**2)/64D0
      C1(I) = G1(I)/B0(I) - B1(I)*G0(I)/B0(I)**2
      C2(I) = ((G1(I)/B0(I) - B1(I)*G0(I)/B0(I)**2)**2
     .       + G2(I)/B0(I) + B1(I)**2*G0(I)/B0(I)**3
     .       - B1(I)*G1(I)/B0(I)**2 - B2(I)*G0(I)/B0(I)**2)/2D0
      TRAN(X,XK)=1D0+4D0/3D0*ALPHAS(X,2)/PI+XK*(ALPHAS(X,2)/PI)**2
      CQ(X,I)=(2D0*B0(I)*X)**(G0(I)/B0(I))
     .            *(1D0+C1(I)*X+C2(I)*X**2)
C - Data statements
      DATA ISTRANGE/0/
C - Statements
      PI=4.D0*ATAN(1.D0)
      AM(1) = 0
      AM(2) = 0
      RUNM= 0D0
      IF( NF.LE.2 ) RETURN
C -
      IF( IMSBAR.EQ.1 ) THEN
        IF( ISTRANGE.EQ.0 ) THEN
C - Strange pole mass from msbar-mass at 1 GeV
          AMSD = XLAMBDA
          AMSU = 1.D8
123       AMS  = (AMSU+AMSD)/2
          AM(3) = AMS
          XMSB = AMS/CQ(ALPHAS(AMS,2)/PI,3)
     .            *CQ(ALPHAS(1.D0,2)/PI,3)/TRAN(AMS,0D0)
          DD = (XMSB-AMSB)/AMSB
          IF( ABS(DD).GE.ACC ) THEN
            IF(DD.LE.0.D0)THEN
              AMSD = AM(3)
            ELSE
              AMSU = AM(3)
            ENDIF
            GOTO 123
          ENDIF
          ISTRANGE=1
        ELSE                     ! Added on Sep 3, 2002 to fix
          AM(3) = AMS            ! potential undefinition problems ...
        ENDIF
      ELSE
        AMS=AMSB
        AM(3) = AMS
      ENDIF
C -
      AM(4) = AMC
      AM(5) = AMB
      AM(6) = AMT
      XK = 16.11D0
      DO I=1,NF-1
        XK = XK - 1.04D0*(1.D0-AM(I)/AM(NF))
      END DO
C -
      XMSB = AM(NF)/TRAN(AM(NF),0D0)
      XMHAT = XMSB/CQ(ALPHAS(AM(NF),2)/PI,NF)
C     XKFAC = TRAN(AM(NF),0D0)/TRAN(AM(NF),XK)
      XKFAC = TRAN(AM(NF),0D0)
      IF(NF.EQ.3)THEN
        YMSB(3) = XMSB
        YMSB(4) = YMSB(3)*CQ(ALPHAS(AM(4),2)/PI,3)/
     .                    CQ(ALPHAS(AM(3),2)/PI,3)
        YMSB(5) = YMSB(4)*CQ(ALPHAS(AM(5),2)/PI,4)/
     .                    CQ(ALPHAS(AM(4),2)/PI,4)
        YMSB(6) = YMSB(5)*CQ(ALPHAS(AM(6),2)/PI,5)/
     .                    CQ(ALPHAS(AM(5),2)/PI,5)
      ELSEIF(NF.EQ.4)THEN
        YMSB(4) = XMSB
        YMSB(3) = YMSB(4)*CQ(ALPHAS(AM(3),2)/PI,3)/
     .                    CQ(ALPHAS(AM(4),2)/PI,3)
        YMSB(5) = YMSB(4)*CQ(ALPHAS(AM(5),2)/PI,4)/
     .                    CQ(ALPHAS(AM(4),2)/PI,4)
        YMSB(6) = YMSB(5)*CQ(ALPHAS(AM(6),2)/PI,5)/
     .                    CQ(ALPHAS(AM(5),2)/PI,5)
      ELSEIF(NF.EQ.5)THEN
        YMSB(5) = XMSB
        YMSB(4) = YMSB(5)*CQ(ALPHAS(AM(4),2)/PI,4)/
     .                    CQ(ALPHAS(AM(5),2)/PI,4)
        YMSB(3) = YMSB(4)*CQ(ALPHAS(AM(3),2)/PI,3)/
     .                    CQ(ALPHAS(AM(4),2)/PI,3)
        YMSB(6) = YMSB(5)*CQ(ALPHAS(AM(6),2)/PI,5)/
     .                    CQ(ALPHAS(AM(5),2)/PI,5)
      ELSEIF(NF.EQ.6)THEN
        YMSB(6) = XMSB
        YMSB(5) = YMSB(6)*CQ(ALPHAS(AM(5),2)/PI,5)/
     .                    CQ(ALPHAS(AM(6),2)/PI,5)
        YMSB(4) = YMSB(5)*CQ(ALPHAS(AM(4),2)/PI,4)/
     .                    CQ(ALPHAS(AM(5),2)/PI,4)
        YMSB(3) = YMSB(4)*CQ(ALPHAS(AM(3),2)/PI,3)/
     .                    CQ(ALPHAS(AM(4),2)/PI,3)
      ENDIF
      IF(Q.LT.AMC)THEN
        N0=3
        Q0 = AMS
      ELSEIF(Q.LE.AMB)THEN
        N0=4
        Q0 = AMC
      ELSEIF(Q.LE.AMT)THEN
        N0=5
        Q0 = AMB
      ELSE
        N0=6
        Q0 = AMT
      ENDIF
C - Output
      RUNM = YMSB(N0)*CQ(ALPHAS(Q,2)/PI,N0)/
     .                CQ(ALPHAS(Q0,2)/PI,N0)
C -
      RETURN
      END
      FUNCTION SIGHHC( S, TERM )
C-----------------------------------------------------------------------
C!    Calculates total cross-section for e+e- -> H+H-
C!    with width effects
C!
C!    Input:   S  (REAL*8)     center_of_mass_energy squared (GeV**2)
C!          TERM  (INTEGER)    1 =>  only photon exchange
C!                             2 =>  only Z exchange
C!                             3 =>  only Z-photon interference
C!                             0 =>  all terms ( 1+2+3 )
C!    N.B.: Charged Higgs mass and width are taken from JETSET commons
C!
C!    Output: SIGHHC (REAL*8)   total cross-section ( picobarn )
C!
C!    Gerardo Ganis              3 May 1996
C-----------------------------------------------------------------------
      IMPLICIT   NONE
      REAL*8     SIGHHC
C - Arguments
      REAL*8     S
      INTEGER    TERM
C - JETSET commons
      INTEGER     MSTU, MSTJ
      REAL*8      PARU, PARJ
      COMMON /PYDAT1/ MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      INTEGER     KCHG
      REAL*8      PMAS, PARF, VCKM
      COMMON /PYDAT2/ KCHG(500,3),PMAS(500,4),PARF(2000),VCKM(4,4)
C - Auxiliary variables
      REAL*8     XSGA, XSZ0, XSGZ
      REAL*8     MZ, GZ, SW2, ALFEM
      REAL*8     CL, CR, ST2W, PROZ, E4, XS, HHKIN, BETA
      REAL*8     MH, GH
C - Constants
      INTEGER    KHC, KFZ, MAXWDT
      PARAMETER( KHC= 37, KFZ= 23, MAXWDT= 20 )
      REAL*8     PI, TOPB
      PARAMETER( PI= 3.141592653589793238, TOPB= 0.38937966D9 )
C - External functions
      REAL*8     PYALEM
      EXTERNAL   PYALEM
      REAL*8     HCCONV
      EXTERNAL   HCCONV
C
C - Initialize
      SIGHHC= 0.D0
      XS    = 0.D0
      XSGA  = 0.D0
      XSZ0  = 0.D0
      XSGZ  = 0.D0
C - return if s <= 0
      IF( S.LE.0.D0 ) RETURN
C - Get info from JETSET commons
      MH   = DBLE(PMAS( KHC, 1))
      GH   = DBLE(PMAS( KHC, 2))
      MZ   = DBLE(PMAS( KFZ, 1))
      GZ   = DBLE(PMAS( KFZ, 2))
      SW2  = DBLE(PARU( 102 ))
      ALFEM= DBLE(PYALEM( SNGL(S) ))
C - Calculate auxiliary variables
      CL   = -0.5D0 + SW2
      CR   =          SW2
      ST2W = 4.D0*SW2*(1.D0-SW2) / (1.D0-2.D0*SW2)
      PROZ = (S-MZ**2)**2 + (GZ*MZ)**2
      E4   = (4.D0*PI*ALFEM)**2
C - Calculate cross-section terms
      IF( TERM.EQ.0 .OR. TERM.EQ.1 ) THEN
C - photon exchange
        XSGA= 1.D0 / S
      END IF
      IF( TERM.EQ.0 .OR. TERM.EQ.2 ) THEN
C - Z exchange
        XSZ0= 2.D0 * (CL**2+CR**2) / ST2W**2 * S / PROZ
      END IF
      IF( TERM.EQ.0 .OR. TERM.EQ.3 ) THEN
C - photon-Z inteference
        XSGZ=-2.D0 * (CL+CR) / ST2W * (S-MZ**2) / PROZ
      END IF
C - Kinematical factor
      IF( MH .LT. (SQRT(S)/2.D0-MAXWDT*GH) ) THEN
        BETA = SQRT(MAX(1.D0-4*MH**2/S,0.D0))
        HHKIN= BETA**3
      ELSE
        HHKIN= HCCONV( S, MH, GH )
      END IF
C - Get total cross-section ( 48 = 64*3/4 )
      XS = E4/(48.D0*PI) * ( XSGA + XSZ0 + XSGZ ) * HHKIN
C - Convert to picobarn
      SIGHHC = XS * TOPB
C - Return successfully
      RETURN
      END

      FUNCTION XITER(Q,XLB1,NF1,XLB,NF2,ACC)
C-----------------------------------------------------------------------
C!
C!    INPUT:          Q   scale (GeV)                         (REAL*8)
C!                    XLB1                                    (REAL*8)
C!                    NF1                                     (INTEGER)
C!                    XLB                                     (REAL*8)
C!                    NF2                                     (INTEGER)
C!                    ACC                                     (REAL*8)
C!    OUTPUT:         XITER                                   (REAL*8)
C!
C!    Comments:       Cleaning of Djouadi code.
C!    Author:         A.Djouadi
C!    Cleaned by      G.Ganis         July 2, 96
C-----------------------------------------------------------------------
      IMPLICIT   NONE
      REAL*8     XITER
C - Parameters
C - Arguments
      INTEGER    NF1, NF2
      REAL*8     Q, XLB1, XLB, ACC
C - Auxiliary variables
      INTEGER    I, II
      REAL*8     B0, B1, ALS2, AA, BB, XIT,
     .           A, B, X, Y,
     .           PI, ALP, XX, Y1, Y2, DY, XLB2
C - Macro
      B0(I)= 33.D0-2.D0*I
      B1(I)= 6.D0*(153.D0-19.D0*I)/B0(I)**2
      ALS2(I,X,Y)= 12.D0*PI/(B0(I)*LOG(X**2/Y**2))
     .                  *(1.D0-B1(I)*LOG(LOG(X**2/Y**2))
     .                  /LOG(X**2/Y**2))
      AA(I)= 12D0*PI/B0(I)
      BB(I)= B1(I)/AA(I)
      XIT(A,B,X)= A/2.D0*(1D0+SQRT(1D0-4D0*B*LOG(X)))
C - Statements
      PI  = 4.D0*ATAN(1.D0)
      XLB2= XLB
      II  = 0
1     II  = II+1
      X   = LOG(Q**2/XLB2**2)
      ALP = ALS2(NF1,Q,XLB1)
      A   = AA(NF2)/ALP
      B   = BB(NF2)*ALP
      XX  = XIT(A,B,X)
      XLB2= Q*EXP(-XX/2.D0)
      Y1  = ALS2(NF1,Q,XLB1)
      Y2  = ALS2(NF2,Q,XLB2)
      DY  = ABS(Y2-Y1)/Y1
      IF( DY.GE.ACC ) GOTO 1
C - Output
      XITER=XLB2
C -
      RETURN
      END

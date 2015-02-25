      INTEGER    HCCON, NSCH, NCH, NDCH, NMXC
      PARAMETER( NCH= 49, NDCH= 19, NMXC= 9 )
      REAL*8     HCCBR1, HCCBR2, TAUHP
      COMMON /HCDEFF/ NSCH(NDCH), HCCON(NDCH),
     .                HCCBR1(0:NDCH), HCCBR2(NDCH,NMXC), TAUHP
      CHARACTER*17 HCCHAN
      COMMON /HCNAME/ HCCHAN(NDCH)
      INTEGER    UPMAX
      REAL*8     MEMAX, THRES
      COMMON /HCMAXX/ MEMAX(NCH), THRES(NCH), UPMAX(NCH)
      INTEGER    HCDSTA
      COMMON /HCSTAT/ HCDSTA(0:NCH)
C-----------------------------------------------------------------------
C!    HCCHAN           channel names
C!    HCCON            requested decay ( 1= ON, 0= OFF )
C!    HCCBR1           cumulative BR on first channel classif. (data car
C!    HCCBR2           cumulative BR on subchannel classif. (internal)
C!    NSCH             number of subchannels per channel
C!    TAUHP            lifetime in seconds
C-----------------------------------------------------------------------

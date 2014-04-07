      SUBROUTINE znnb_init                                       
C-----------------------------------------------------------------------
C! Initialization routine for HZHA01                                    
C                                                                       
C-----------------------------------------------------------------------
      IMPLICIT REAL*8 (A-H,O-Z)
C                                                                       
      include 'fscard.inc'
      include 'parznn.h'
      include 'lundcom.h'
      include 'miscl.h'
C                                                                       
      loutbe = 6
C
C Welcome !
C
      WRITE(loutbe,1001) 
 1001 FORMAT(//15X,'+---------------------------------------------+'/
     .      15X,'|                                             |'/
     .      15X,'|     W E L C O M E   T O    Z N N B 0 1      |'/
     .      15X,'|     **********************************      |'/
     .      15X,'|      The Z nu nu final state generator      |'/
     .      15X,'|             in e+e- collisions              |'/
     .      15X,'|     **********************************      |'/
     .      15X,'|                                             |'/
     .      15X,'|                                             |'/
     .      15X,'|     Last date of change : 19 June 2012      |'/
     .      15X,'|                                             |'/
     .      15X,'|   . Patrick Janot  --  CERN/PH              |'/
     .      15X,'|                                             |'/
     .      15X,'+---------------------------------------------+'//)
C                                                                       
C Read the generator parameters                                         
C                                                                       
      ecm    =      gene(1)
      amz    =      gene(2)
      gmz    =      gene(3)
      sw2    =      gene(4)
      pmas(23,1) = amz
      pmas(23,2) = gmz
      parj( 102) = sw2
C
C Switch off Z -> nunubar decays
      mdme(183,1) = 0
      mdme(185,1) = 0
      mdme(187,1) = 0
C
C
C...Initialize ZZNB
C
      Z=amz/1000.
      XW=sw2
      W=Z*SQRT(1.-xw)
      W2=W**2
      Z2=Z**2
      WD2=1./W2
      ZD2=1./Z2
      ALFWEM=pyalem(ecm**2)
      COST2=(PI*ALFWEM/XW)**3/(1.-XW)
      FCONV=0.3893796623D+03
C
C Compute cross sections etc.
C

      ecms = ecm
      CALL znnbin(ecms)
C
C...Debug flags
      idb1 = debu(1)
      idb2 = debu(2)
C
C  Initialize events counters
C
       DO 11 i = 1,11
   11  nevent(i) = 0
C
      RETURN
      END

      SUBROUTINE znnb_event                                             
C-----------------------------------------------------------------------
C! Event generation                                                     
C-----------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      DIMENSION qg1(4),qg2(4),qz(4),qn(4),qb(4)
      include "lundcom.h"
      include "miscl.h"
C
C  Initialization
C
      ista = 0
      nitr = 0
      nivx = 0
      ecms = ecm
      weit = 1.
      sbeam = ecm**2
      n7lu = 0
C
C  Now generate the event
C
C
      nevent(1) = nevent(1) + 1
      call znnbge(ecms,qg1,qg2,qz,qn,qb)
C
C  Radiative photon in the initial state ?
C
      n7lu = n7lu + 1
      kfg = 22
      CALL hhlu1(n7lu,kfg,qg1(1),qg1(2),qg1(3),qg1(4))
      k7lu(n7lu,1) = 1
      k7lu(n7lu,3) = 0
C
      n7lu = n7lu + 1
      kfg = 22
      CALL hhlu1(n7lu,kfg,qg2(1),qg2(2),qg2(3),qg2(4))
      k7lu(n7lu,1) = 1
      k7lu(n7lu,3) = 0
C
      nrad = 1
C
C  Fill the Z
C
      kfz = 23
      n7lu = n7lu + 1
      CALL hhlu1(n7lu,kfz,qz(1),qz(2),qz(3),qz(4))
      k7lu(n7lu,1) = 1
      k7lu(n7lu,3) = 0
C
C   Fill the neutrinos
C
      kfn = 12
      n7lu = n7lu + 1
      CALL hhlu1(n7lu,kfn,qn(1),qn(2),qn(3),qn(4))
      k7lu(n7lu,1) = 1
      k7lu(n7lu,3) = 0
C
      kfb = -12
      n7lu = n7lu + 1
      CALL hhlu1(n7lu,kfb,qb(1),qb(2),qb(3),qb(4))
      k7lu(n7lu,1) = 1
      k7lu(n7lu,3) = 0
C
C  Other decays and fragmentation
C
      CALL pyexec
C
C  Listing of the event
C
      IF ( nevent(1) .GE. idb1 .AND.
     .     nevent(1) .LE. idb2 ) CALL pylist(1)
C
      RETURN
      END

      SUBROUTINE zznb_close                                             
C-----------------------------------------------------------------------
C
C   Routine for printout at the end of a run
C
C-----------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      include 'miscl.h'
      include 'crosec.h'
C
C Print event counters
C
       WRITE(LOUTBE,101)
  101  FORMAT(//20X,'EVENTS STATISTICS',
     &         /20X,'*****************')
       WRITE(LOUTBE,102) NEVENT(1),NEVENT(2),NEVENT(3)
  102  FORMAT(/5X,'# OF GENERATED EVENTS                      = ',I10,
     &        /5X,'# OF ACCEPTED  EVENTS                      = ',I10,
     &        /5X,'# OF REJECTED  EVENTS (ISTA # 0 in ASKUSE) = ',I10)
C
      sigto = wtot/FLOAT(ntry)
      dsig  = SQRT((wtot2/FLOAT(ntry) - sigto**2)/FLOAT(ntry))
      WRITE(6,*) '-----------------------------------'
      WRITE(6,*) ' Generated cross section : '
      WRITE(6,*) ' Sigma = ',sigto,' +/- ',dsig,' pb'
      WRITE(6,*) '-----------------------------------'
      RETURN
      END

      SUBROUTINE hzha_init                                       
C-----------------------------------------------------------------------
C! Initialization routine for HZHA01                                    
C                                                                       
C-----------------------------------------------------------------------
      IMPLICIT REAL*8 (A-H,O-Z)
C                                                                       
      include 'fscard.inc'
C
      DIMENSION indneu(4), indcha(2)
      INTEGER PYCOMP
      DATA indneu/51,52,53,54/
      DATA indcha/55,56/
C
      include 'lundcom.inc'
      include 'param.inc'
      include 'consts.inc'
      include 'hhprod.inc'
      include 'hhdeck.inc'
      include 'zzdeck.inc'
      include 'miscl.inc'
      include 'datapi.inc'
      include 'hccomm.inc'
      include 'hcdeff.inc'
      real*8 M12,m0,msnu
C
      loutbe = 6

C
C Welcome !
C
      WRITE(loutbe,1001) 
 1001 FORMAT(//15X,'+---------------------------------------------+'/
     .      15X,'|                                             |'/
     .      15X,'|     W E L C O M E   T O    H Z H A 0 3      |'/
     .      15X,'|     **********************************      |'/
     .      15X,'|      The (SUSY) Higgs boson generator       |'/
     .      15X,'|             in e+e- collisions              |'/
     .      15X,'|     **********************************      |'/
     .      15X,'|                                             |'/
     .      15X,'|                                             |'/
     .      15X,'|     Last date of change : 5 June 2000       |'/
     .      15X,'|                                             |'/
     .      15X,'|   . Patrick Janot  --  CERN/PPE             |'/
     .      15X,'|       for the neutral Higgs bosons          |'/
     .      15X,'|                                             |'/
     .      15X,'|   . Gerardo Ganis  --  MPI/Munich           |'/
     .      15X,'|       for the charged Higgs bosons          |'/
     .      15X,'|                                             |'/
     .      15X,'|  with contributions from                    |'/
     .      15X,'|                                             |'/
     .      15X,'|   . Vincent Bertin                          |'/
     .      15X,'|   . Laurent Duflot                          |'/
     .      15X,'|   . Anne-Isabelle Etienvre                  |'/
     .      15X,'|   . Marumi  Kado                            |'/
     .      15X,'|                                             |'/
     .      15X,'|  and pieces of code from                    |'/
     .      15X,'|                                             |'/
     .      15X,'|   . Ronald Kleiss                           |'/
     .      15X,'|   . Marcela Carena & Carlos Wagner          |'/
     .      15X,'|   . Howie Haber & Mariano Quiros            |'/
     .      15X,'|                                             |'/
     .      15X,'+---------------------------------------------+'//)
C                                                                       
C Read the generator parameters                                         
C                                                                       
      iklei   = NINT(gene(1))                                           
      iproc   = NINT(gene(2))                                           
      xrad    =      gene(3)                                            
      ecm     =      gene(4)                                            
      empir   =      gene(5)                                            
      ism     = NINT(gene(6))                                           
      icar    = NINT(gene(7))                                           
C
C Check consistency                                                     
C                                                                       
      IF( iproc.EQ.9 .AND. ism.EQ.1 ) THEN                              
        WRITE(*,*)                                                      
     .   'No charged Higgs in SM - STOP '                               
        STOP                                                            
      END IF                                                            
C                                                                       
C Read the Standard Model parameters                                    
C                                                                       
      amz     =      gsmo(1)                                            
      gmz     =      gsmo(2)                                            
      g_f     =      gsmo(3)                                            
      amt     =      gsmo(4)                                            
      amh     =      gsmo(5)                                            
      xlamda5 =      gsmo(6)                                            
C                                                                       
C Read the MSSM parameters                                              
C                                                                       
      IF ( gsus(2) .GT. 0D0 .AND. gsgu(2) .GT. 0D0 ) THEN 
C
C but not the two cards at the same time! 
C      
        WRITE(6,*) 'You cannot have both GSUS and GSGU cards'
        STOP 99
C
      ELSEIF( gsus(2) .GT. 0D0 ) THEN
C
        amarun  =      gsus(1)                                            
        tb      =      gsus(2)                                            
        susM2   =      gsus(3)                                            
        susMU   =      gsus(4)                                            
        susAt   =      gsus(5)                                            
        susAb   =      gsus(6)                                            
        susSMQ  =      gsus(7)                                            
        susSMU  =      gsus(8)                                            
        susSMD  =      gsus(9)                                            
        susSML  =      gsus(10)                                           
        susSME  =      gsus(11)                                           
        susM0   =     -1. 
        susM12  =     -1. 
        susMSNU =      1.
        IF ( gglu(1) .GT. 0. ) THEN 
          susGLU  =    gglu(1)
        ELSE
          susGLU  =   -1.
        ENDIF
C
      ELSEIF ( gsgu(2) .GT. 0D0 ) THEN
C
        amarun  =      gsgu(1)
        tb      =      gsgu(2)
        susM0   =      gsgu(3)
        susM12  =      gsgu(4)
        susMU   =      gsgu(5)
        susAt   =      gsgu(6)
        susAb   =      gsgu(7)
C
      ENDIF
C
C Read the anomalous couplings (f / /\**2, in 1/TeV**2).
C
      fbb  =      gfbw(1)
      fww  =      gfbw(2)
      fb   =      gfbw(3)
      fw   =      gfbw(4)
C In 1 / GeV**2
      fbb = fbb / 1E6
      fww = fww / 1E6
      fb  = fb  / 1E6
      fw  = fw  / 1E6
C
      CALL vzero(nevpro(1),maxpro)
      CALL vzero(iproyn(1),maxpro)
      CALL vzero(interf(1),maxpro)
C
      IF ( iproc .GE. 1 .AND. iproc .LE. maxpro ) THEN 
        iproyn(iproc) = 1
      ELSEIF ( iproc .LE. -5 .AND. iproc. GE. -8 ) THEN 
        iproyn(-iproc) = 1
        interf(-iproc) = 1
      ELSEIF ( iproc .EQ. 0 ) THEN  
        DO ipro = 1, maxpro
          iproyn(ipro) = 1
          interf(ipro) = 1
        ENDDO
      ELSE
      ENDIF
C                                                                       
      DO ipro = 1, maxpro
        iproyn(ipro) = pryn(ipro)
        IF ( iproyn(ipro) .EQ. 2 ) THEN 
          interf(ipro) = 1
          iproyn(ipro) = 1
        ENDIF
      ENDDO
C
      DO ifs = 1, 11
        kselec(ifs) = gzdc(ifs)
      ENDDO
C
      IF ( interf(5) .EQ. 1 .OR. interf(6) .EQ. 1 ) THEN 
        interf(5) = 1
        interf(6) = 1
        kselec(2) = 0
      ENDIF
      IF ( interf(7) .EQ. 1 .OR. interf(8) .EQ. 1 ) THEN 
        interf(7) = 1
        interf(8) = 1
        kselec(1) = 0
      ENDIF
C
      braz(1) = brai(1) * kselec(1)
      DO ifs = 2, 11
        braz(ifs) = brai(ifs)*kselec(ifs)+braz(ifs-1)
      ENDDO
C
      fracz = braz(11)
      IF ( fracz .NE. 0. ) THEN 
        DO ifs = 1, 11
          braz(ifs) = braz(ifs)/braz(11)
        ENDDO
      ELSE
        fracz = 1.
        iproyn(1) = 0
        iproyn(2) = 0
        DO ifs = 1, 11
          braz(ifs) = brai(ifs)
        ENDDO
      ENDIF
C
      pmas(23,1) = amz
      pmas(23,2) = gmz
      pmas( 6,1) = amt
C
C Set the b, c and tau pole masses. Please do not change these values,
C They are meant to be valid for the two loop QCD radiative corrections
C (computed in hzha).
C
      pmas( 5,1) = 4.68                                                 
      pmas( 4,1) = 1.64                                                 
      pmas(15,1) = 1.777                                                
C
C Electron and muon masses may also have their improtance in 
C Higgs and neutralino Higgs decays.
C
      pmas(11,1) = 0.511E-3
      pmas(13,1) = 0.105659
C
C Set the W width.
C
      pmas(24,2) = 2.096
C
C - Charged Higgs
C
      ihcd(1)= ghcc(1)
      ihcd(2)= ghcc(2)
      IF( ism.EQ.-1 ) THEN
        WRITE(6,3000)
        gmh=  ghcc(3)
        amh=  ghcc(4)
        ama=  ghcc(5)
        amhp= ghcc(6)
        tb=   ghcc(7)
        alfa= ghcc(8)
      ENDIF
3000  FORMAT(/,'***************************************************',/,
     .         '*** Generic Type II two-higgs-doublet model     ***',/,
     .         '*** Take Higgs masses and angles from data      ***',/,
     .         '*** card GHCC                                   ***',/,
     .         '***************************************************',//)
C
C...Initialize Higgs decay routine
C
      CALL hhinit
      IF ( amh .LE. 0..OR.
     .   ( ism .LE. 0 .AND. 
     .   ( gmh.LE.0. .OR. ama .LE. 0. .OR. amhp.LE.0. .OR.
     &     amst(1) .LT. 0. .OR. amsb(1) .LT. 0. .OR.  
     .      susMSNU .LE. 0. ) ) ) STOP 99
C
C...Compute Born cross sections
C
      CALL vzero(wtot (1),4*2)
      CALL vzero(wtot2(1),4*2)
      CALL vzero(ntry (1),4)
      CALL vzero(nacc (1),4)
C
      CALL vzero(reduc(1),maxpro*2)
      reduc( 1) = fracz * parwth(2) / width(2)
      reduc( 5) = parwth(2) / width(2)
      reduc( 7) = parwth(2) / width(2)
      reduc(10) = parwth(2) / width(2)
      IF ( ism .LE. 0 ) THEN
        reduc(2) = fracz * parwth(1) / width(1)
        reduc(3) = parwth(2)*parwth(3) / (width(2)*width(3))
        reduc(4) = parwth(1)*parwth(3) / (width(1)*width(3))
        reduc(6) = parwth(1) / width(1)
        reduc(8) = parwth(1) / width(1)
        brhc1    = hccbr1(ihcd(1))
        IF( ihcd(1).GT.1 ) brhc1= hccbr1(ihcd(1))-hccbr1(ihcd(1)-1)
        brhc2    = hccbr1(ihcd(2))
        IF( ihcd(2).GT.1 ) brhc2= hccbr1(ihcd(2))-hccbr1(ihcd(2)-1)
        reduc(9) = brhc1*brhc2
        IF( MIN(ihcd(1),ihcd(2)).GT.0 .AND. ihcd(1).NE.ihcd(2) )
     &  reduc(9)= 2D0*reduc(9)
      ENDIF
C
      sbeam = ecm**2
      DO 5 ipro = 1, maxpro
        cross(ipro) = crocom(ipro,sbeam)
    5 CONTINUE
C
      WRITE(loutbe,2000) (chapro(ipro),
     .                    cross (ipro),
     .                    reduc (ipro),ipro=1,maxpro)
 2000 FORMAT(1x,'Born level cross section have been computed:'/
     .    10(1x,'   . ',A18,' : ',E12.4,' fb  (BR = ',F8.6,')'/))
C
      WRITE(loutbe,*) 'Notes : '
      WRITE(loutbe,*) '  o The cross sections include the BR''s;'
      IF ( interf (5) .EQ. 1 ) THEN 
        WRITE(loutbe,*) '  o WW -> h/H include hZ and interference;' 
        WRITE(loutbe,*) '    Therefore hZ/HZ do not include hnn/Hnn;'
      ENDIF
      IF ( interf (7) .EQ. 1 ) THEN 
        WRITE(loutbe,*) '  o ZZ -> h/H include hZ and interference;'
        WRITE(loutbe,*) '    Therefore hZ/HZ do not include hee/Hee;'
      ENDIF
      WRITE(loutbe,*) '  '
C
      DO ipro = 5, 8
        IF ( interf(ipro) .EQ. 1 .AND. iproyn(ipro) .EQ. 1 ) THEN 
          IF ( ipro .EQ. 5 )
     &    WRITE(loutbe,*) 'Breakdown of h nu_e nu_e(bar) cross section'
          IF ( ipro .EQ. 6 )
     &    WRITE(loutbe,*) 'Breakdown of H nu_e nu_e(bar) cross section'
          IF ( ipro .EQ. 7 )
     &    WRITE(loutbe,*) 'Breakdown of h e+ e- cross section'
          IF ( ipro .EQ. 8 )
     &    WRITE(loutbe,*) 'Breakdown of H e+ e- cross section'
          WRITE(loutbe,*) '  . Higgs-strahlung : ',chnnz(ipro),' fb'
          WRITE(loutbe,*) '  . Boson fusion    : ',chnns(ipro),' fb'
          WRITE(loutbe,*) '  . Interference    : ',chnni(ipro),' fb'
          WRITE(loutbe,*) '  '
        ENDIF
      ENDDO
C...
C...End of HZHA stuff
C...
C
C...Fill back the Higgs masses and lifetimes into the JETSET common blocks
C
      mdcy(pycomp(25),1) = 0
      mdcy(pycomp(35),1) = 0
      mdcy(pycomp(36),1) = 0
      pmas(pycomp(25),4) = tauh(2)/3.33E-12
      pmas(pycomp(35),4) = tauh(1)/3.33E-12
      pmas(pycomp(36),4) = tauh(3)/3.33E-12
      pmas(pycomp(37),4) = SNGL(tauhp)/3.33E-12
C
C...Add neutralinos to the particle list
C
      DO ineut = 1, 4
        jchi = indneu(ineut)
        kchi = PYCOMP(jchi)
C
        CHAF(kchi,1)(1:4)='Chi0'
        WRITE(CHAF(kchi,1)(4:4),50) ineut
   50   FORMAT(I1)
        PMAS(kchi,1) = ABS(amneut(ineut))
        IF ( ineut .NE. 1 ) MDCY(kchi,1) = 0
        KCHG(kchi,1) = 0
        KCHG(kchi,3) = 0
      ENDDO
C
C...Add Charginos to the particle list
C
      DO ichar = 1, 2
        jchi = indcha(ichar)
        kchi = PYCOMP(jchi)
C
        CHAF(kchi,1)(1:4)='Chi0'
        WRITE(CHAF(kchi,1)(4:4),51) ichar
   51   FORMAT(I1)
        PMAS(kchi,1) = ABS(amchar(ichar))
        MDCY(kchi,1) = 0
        KCHG(kchi,1) = 3
        KCHG(kchi,3) = 1
      ENDDO
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

      SUBROUTINE hzha_event                                             
C-----------------------------------------------------------------------
C! Event generation                                                     
C-----------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
C                                                                       
      include 'fscard.inc'
C                                                                       
      DIMENSION qg1(4),qg2(4),qh(4),qa(4),ph(4),hh(4),qp(4),qm(4)
      DIMENSION qz(4),q1(4),q2(4),q3(4),ch(4),ijoin(2)
      DIMENSION qin1(4),qin2(4),qout1(4),qout2(4),qout3(4)
      REAL ptrak(4,2)
C      REAL*8 pymass, xmm, rx, ry, rz, dum
      REAL ecms,weit
      include 'lundcom.inc'
      include 'miscl.inc'
      include 'hhprod.inc'
      include 'zzdeck.inc'
      include 'sigmas.inc'
C
C  Initialization ASKUSE''s arguments
C
      ista   = 0
      nitr   = 0
      nivx   = 0
      ecms   = ecm
      cmse   = ecms
      weit   = 1.
      sbeam  = ecm**2
      n7lu   = 0
      idpr   = 0
      ipro   = 0
      jchanh = 0
      jchana = 0
      ifs    = 0
C
C  Choice of the channel (if ipro = 0)
C
      sigtot = 0.
      DO ipro = 1 , maxpro
        sigtot = sigtot + iproyn(ipro)*cross(ipro)
      ENDDO
C
      IF ( sigtot .EQ. 0. ) THEN
        WRITE(6,*) 'Not a single process has a non-vanishing',
     .             ' Cross section. Execution stopped.'
        STOP 99
      ENDIF
C
      rnch = RNDM2(dummy)
      rint = 0D0
      DO ipro = 1 , maxpro
        rint = rint + iproyn(ipro)*cross(ipro) / sigtot
        if ( rnch .LT. rint ) GOTO 30
      ENDDO
  30  CONTINUE
C
C  Now generate the event
C
      IF ( cross(ipro) .LE. 0. ) THEN
        WRITE(6,*) 'Warning : Process ',chapro(ipro),
     .  ' has a vanishing cross-section, Execution stopped'
        STOP 99
      ENDIF
C
      nevent(1) = nevent(1) + 1
C      WRITE(6,*) 'Event # ',nevent(1),ipro
      nevpro(ipro) = nevpro(ipro) + 1
    1 CALL hzha(ipro,cmse,qg1,qg2,qh,qa,hh,izpol)
C
C  Radiative photon in the initial state ?
C
      IF ( xrad .GT. 0. ) THEN
        n7lu = n7lu + 1
        kfg = 22
        CALL hhlu1(n7lu,kfg,qg1(1),qg1(2),qg1(3),qg1(4),0D0)
        k7lu(n7lu,1) = 1
        k7lu(n7lu,3) = 0
        n7lu = n7lu + 1
        kfg = 22
        CALL hhlu1(n7lu,kfg,qg2(1),qg2(2),qg2(3),qg2(4),0D0)
        k7lu(n7lu,1) = 1
        k7lu(n7lu,3) = 0
      ENDIF
C
      IF ( ipro .GE. 5 .AND. ipro .LE. 8 ) GOTO 1000
C
C  Virtual exchange boson (gamma or Z0)
C
      n7lu = n7lu + 1
      kfz = 23
      CALL hhlu1(n7lu,kfz,-qg1(1)-qg2(1),-qg1(2)-qg2(2),
     .                    -qg1(3)-qg2(3),ecm-qg1(4)-qg2(4),-1D0)
      k7lu(n7lu,1) = 11
      k7lu(n7lu,3) = 0
      k7lu(n7lu,4) = n7lu + 1
      k7lu(n7lu,5) = n7lu + 2
      ipo1 = n7lu
C
C  Fill the h (if IPRO = 1 or 3 or 10) or the H (if IPRO = 2 or 4)
C
      IF ( ipro .EQ. 1 .OR. ipro .EQ. 3 .OR. ipro .EQ. 10 ) THEN
        kfh = 25
        jhig = 2
      ELSE IF( ipro .EQ. 9 ) THEN
        kfh = 37                       ! H+
      ELSE
        kfh = 35
        jhig = 1
      ENDIF
      n7lu = n7lu + 1
      qhe = SQRT(qh(1)**2+qh(2)**2+qh(3)**2+qh(4)**2)
      CALL hhlu1(n7lu,kfh,qh(1),qh(2),qh(3),qhe,qh(4))
      k7lu(n7lu,1) = 1
      k7lu(n7lu,3) = ipo1
      ipoh = n7lu
C
C   Fill the Z0 (if IPRO = 1 or 2) or the A (if IPRO = 3 or 4)
C   or the photon (if IPRO = 10)
C
      IF ( ipro .LE. 2 ) THEN
        kfa = 23                       ! Z
      ELSEIF ( ipro .EQ. 9 ) THEN
        kfa =-37                       ! H-
      ELSEIF ( ipro .EQ. 10 ) THEN
        kfa = 22                       ! Gamma
      ELSE
        kfa = 36
      ENDIF
      n7lu = n7lu + 1
      qae = SQRT(qa(1)**2+qa(2)**2+qa(3)**2+qa(4)**2)
      CALL hhlu1(n7lu,kfa,qa(1),qa(2),qa(3),qae,qa(4))
      k7lu(n7lu,1) = 1
      k7lu(n7lu,3) = ipo1
      ipoa = n7lu
C
C  Decay charged higgses if H+H- production
C
      IF( ipro .EQ. 9 ) THEN
        CALL hcdecc(ihpd,ihmd)
        CALL hcdecy(ipoh,ihpd,ist)
        IF( ist.NE.0 ) GOTO 1
        CALL hcdecy(ipoa,ihmd,ist)
        IF( ist.NE.0 ) GOTO 1
        GOTO 100
      END IF
C
C  Decay the Higgses
C
      CALL hhdecay(qh,jchanh,jhig,ipoh)
      IF ( ipro.EQ.3 .OR. ipro.EQ.4 ) CALL hhdecay(qa,jchana,3,ipoa)
      GOTO 100
C
C Now the WW/ZZ fusion
C
 1000 CONTINUE
C
C Fill the neutrinos (WW) or the e+e- pair (ZZ)
C
      IF ( ipro .EQ. 5 .OR. ipro .EQ. 6 ) THEN
        kfn = 12
      ELSEIF ( ipro .EQ. 7 .OR. ipro .EQ. 8 ) THEN
        kfn = 11
C
C 05/06/2000: FSR for ZZ fusion!
C
        DO id = 1, 4
          qin1(id) = qh(id)
          qin2(id) = qa(id)
        ENDDO
C
        CALL addglu(qin1,qin2,qout1,qout2,qout3,0)
C
C Put the leptons back ... 
C
        DO ID = 1, 4
          qh(id) = qout1(id)
          qa(id) = qout2(id)
          q3(id) = qout3(id)
        ENDDO
C
C ... and feed LUND with the [possible] FSR photon
C
        IF ( q3(4) .GT. 0. ) THEN
          n7lu = n7lu + 1
          kfg = 22
          CALL hhlu1(n7lu,kfg,q3(1),q3(2),q3(3),q3(4),0D0)
          k7lu(n7lu,3) = 0
        ENDIF
      ENDIF
C
C Now the fermion pair
C
      q5 = pmas(kfn,1)
C
      n7lu = n7lu + 1
      CALL hhlu1(n7lu, kfn,qh(1),qh(2),qh(3),qh(4),q5)
      k7lu(n7lu,1) = 1
      k7lu(n7lu,3) = 0
C
      n7lu = n7lu + 1
      CALL hhlu1(n7lu,-kfn,qa(1),qa(2),qa(3),qa(4),q5)
      k7lu(n7lu,1) = 1
      k7lu(n7lu,3) = 0
C
C Fill the Higgs
C
      IF ( ipro .EQ. 5 .OR. ipro .EQ. 7 ) THEN
        kfh = 25
        jhig = 2
      ELSEIF ( ipro .EQ. 6 .OR. ipro .EQ. 8 ) THEN
        kfh = 35
        jhig = 1
      ENDIF
C
      n7lu = n7lu + 1
      hhe = SQRT(hh(1)**2+hh(2)**2+hh(3)**2+hh(4)**2)
      CALL hhlu1(n7lu,kfh,hh(1),hh(2),hh(3),hhe,hh(4))
      k7lu(n7lu,1) = 1
      k7lu(n7lu,3) = 0
      ipoh = n7lu
C
C  Decay the Higgs
C
      CALL hhdecay(hh,jchanh,jhig,ipoh)
C
C Is there any Higgses in the decay products?
C
  100 nnhg = 0
      DO i7lu = 1 , n7lu
        IF ( ( k7lu(i7lu,2) .EQ. 25 .OR.
     .         k7lu(i7lu,2) .EQ. 35 .OR.
     .         k7lu(i7lu,2) .EQ. 36 ) .AND.
     .         k7lu(i7lu,1) .EQ. 1 ) THEN
          ph(1) = p7lu(i7lu,1)
          ph(2) = p7lu(i7lu,2)
          ph(3) = p7lu(i7lu,3)
          ph(4) = p7lu(i7lu,5)
          iph = i7lu
          nnhg = nnhg + 1
          IF ( k7lu(i7lu,2) .EQ. 35 ) khig = 1
          IF ( k7lu(i7lu,2) .EQ. 25 ) khig = 2
          IF ( k7lu(i7lu,2) .EQ. 36 ) khig = 3
          CALL hhdecay(ph,jchan,khig,iph)
        ENDIF
      ENDDO
      IF ( nnhg .GT. 0 ) GOTO 100
C
C Any neutralino or chargino in the decay products ?
C
  200 nnchi = 0
      DO i7lu = 1 , n7lu
        IF ( ABS(k7lu(i7lu,2)) .GE. 52 .AND.
     .       ABS(k7lu(i7lu,2)) .LE. 56 .AND.
     .         k7lu(i7lu,1) .EQ. 1 ) THEN
          ch(1) = p7lu(i7lu,1)
          ch(2) = p7lu(i7lu,2)
          ch(3) = p7lu(i7lu,3)
          ch(4) = p7lu(i7lu,4)
          ich = i7lu
          nnchi = nnchi + 1
          IF ( ABS(k7lu(i7lu,2)) .LE. 54 ) THEN
            ichi1 = k7lu(i7lu,2)-50
            ichi = 0
          ELSEIF ( k7lu(i7lu,2) .GE. 55 ) THEN
            ichi1 = k7lu(i7lu,2)-54
            ichi = 1
          ELSE
            ichi1 = -k7lu(i7lu,2)-54
            ichi = -1
          ENDIF
          CALL decchi(ch,ichi1,ichi,ich)
        ENDIF
      ENDDO
      IF ( nnchi .GT. 0 ) GOTO 200
C
C
C  Decay the Z (if any!)
C
      IF ( ipro .LT. 3 ) THEN
        IF ( iklei .EQ. 0 ) THEN
          CALL zdecay(qa,izpol,q1,q2,ifs)
        ELSE
          ifs = izpol
          DO i = 1,3
            q2(i) = hh(i)
            q1(i) = qa(i)-q2(i)
          ENDDO
        ENDIF
        kff = kcode(ifs)
        q5 = pymass(kff)
        q1(4) = SQRT(q1(1)**2+q1(2)**2+q1(3)**2+q5**2)
        q2(4) = SQRT(q2(1)**2+q2(2)**2+q2(3)**2+q5**2)
C
C FSR for charged leptons
C
        IF ( ifs .EQ. 1 .OR. ifs .EQ. 3 .OR. ifs .EQ. 5 ) THEN
C
          DO id = 1, 4
             qin1(id) = q1(id)
             qin2(id) = q2(id)
          ENDDO
C
          CALL addglu(qin1,qin2,qout1,qout2,qout3,0)
C
C Put back the leptons
C
          DO ID = 1, 4
             q1(id) = qout1(id)
             q2(id) = qout2(id)
             q3(id) = qout3(id)
          ENDDO
C
C  Feed LUND with the 4-vectors
C
          k7lu(ipoa,4) = n7lu+1
          IF ( q3(4) .GT. 0. ) THEN
            n7lu = n7lu + 1
            kfg = 22
            CALL hhlu1(n7lu,kfg,q3(1),q3(2),q3(3),q3(4),0D0)
            k7lu(n7lu,3) = ipoa
          ENDIF
C
        ENDIF
C
        n7lu = n7lu + 1
        ijoin(1) = n7lu
        CALL hhlu1(n7lu,kff,q1(1),q1(2),q1(3),q1(4),q5)
        k7lu(ijoin(1),3) = ipoa
C
        n7lu = n7lu + 1
        ijoin(2) = n7lu
        CALL hhlu1(n7lu,-kff,q2(1),q2(2),q2(3),q2(4),q5)
        k7lu(ijoin(2),3) = ipoa
        k7lu(ipoa,5) = n7lu
C
C  Prepare and execute parton shower for quarks
C
        xmm    = qa(4)
        IF ( kff .LT. 6 ) THEN
          k7lu(ijoin(1),1) = 2
          njoin = 2
          CALL pyjoin(njoin,ijoin)
          CALL pyshow(ijoin(1), ijoin(2), xmm)
        ENDIF
C
C Don/t decay the Z twice!
C
        k7lu(ipoa,1) = 11
C
      ENDIF
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
C  Event counters
C
      nevent(2) = nevent(2) + 1
C
C Fill Histos for iproc = 9
C
C      IF( ipro .EQ. 9 ) CALL hcfilh
C
      RETURN
      END

      SUBROUTINE hzha_close                                             
C-----------------------------------------------------------------------
C! Routine for printout at the end of a run
C
C-----------------------------------------------------------------------
      IMPLICIT REAL*8 (A-H,O-Z)
      include 'param.inc'
      include 'hhprod.inc'
      include 'miscl.inc'
      REAL DUMMY(4)
C
      CALL hhfini
C
      WRITE(6,*) '--------------------------------------------------'
      WRITE(6,*) 'Number of events generated in each channel : '
      WRITE(6,*) '--------------------------------------------------'
      DO ipro = 1, maxpro
        WRITE(6,*) chapro(ipro),' : ',nevpro(ipro),' events.'
      ENDDO
      WRITE(6,*) '--------------------------------------------------'
C
C Maximum weight reached
C
      WRITE (LOUTBE,100) EMPIRM
  100 FORMAT (//1X,'+++++MAXIMUM WEIGHT REACHED   ++++++',F10.3)
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
      RETURN
      END

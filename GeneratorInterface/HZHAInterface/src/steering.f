      SUBROUTINE steering(filename)                                                  
C                                                                       
      CHARACTER filename*120
      PARAMETER(nkey=17)                                                
      CHARACTER*4 key                                                   
      DIMENSION key(nkey),locvar(nkey),lenvar(nkey),leinit(nkey)
      DATA key/'TRIG','DEBU','TIME','GENE','GSMO','GSUS',               
     .         'PRYN','GZDC','GCH1','GCH2','GCH3','GHHG',
     .         'GHCC','GCHC','GSGU', 'GFBW', 'GGLU'/ 
      DATA leinit/2,2,1,7,6,11,10,11,16,16,16,2,8,19,7,4,1/
C                                                                       
      include 'fscard.inc'
C                                                                       
      CALL vzero(trig(1),2)                                             
      CALL vzero(debu(1),2)                                             
      CALL vzero(time(1),1)                                             
      CALL vzero(gene(1),7)                                             
      CALL vzero(gsmo(1),6)                                             
      CALL vzero(gsus(1),11)                                            
      CALL vzero(pryn(1),9)                                            
      CALL vzero(gzdc(1),11)                                            
      CALL vzero(gch1(1),16)                                            
      CALL vzero(gch2(1),16)                                            
      CALL vzero(gch3(1),16)                                            
      CALL vzero(ghhg(1),2)                                             
      CALL vzero(ghcc(1),8)                                             
      CALL vzero(gchc(1),10)                                             
      CALL vzero(gsgu(1),4)                                             
      CALL vzero(gfbw(1),4)                                             
      CALL vzero(gglu(1),1)                                             
C
C Define the units for input and output
C
      locvar(1) = 10      ! input  unit
      lenvar(1) = 6       ! output unit
      OPEN(locvar(1), FILE=filename)
      CALL ffread(0,key,locvar,lenvar)
C                                  
C Read the cards
C                                     
      locvar( 1) = LOCF(trig(1))                                        
      locvar( 2) = LOCF(debu(1))                                        
      locvar( 3) = LOCF(time(1))                                        
      locvar( 4) = LOCF(gene(1))                                        
      locvar( 5) = LOCF(gsmo(1))                                        
      locvar( 6) = LOCF(gsus(1))                                        
      locvar( 7) = LOCF(pryn(1))                                        
      locvar( 8) = LOCF(gzdc(1))                                        
      locvar( 9) = LOCF(gch1(1))                                        
      locvar(10) = LOCF(gch2(1))                                        
      locvar(11) = LOCF(gch3(1))                                        
      locvar(12) = LOCF(ghhg(1))                                        
      locvar(13) = LOCF(ghcc(1))                                        
      locvar(14) = LOCF(gchc(1))                                        
      locvar(15) = LOCF(gsgu(1))                                        
      locvar(16) = LOCF(gfbw(1))                                        
      locvar(17) = LOCF(gglu(1))                                       
      CALL ucopy(leinit(1),lenvar(1),nkey)
C
      CALL ffread(nkey,key,locvar,lenvar)                               
      END
C      CALL hzha_init                                                    
C                                                                       
C Event generation                                                      
C                                                                       
C      iev1   = trig(1)                                                  
C      iev2   = trig(2)                                                  
C      timlft = time(1)                                                  
C      DO 1 ievt = iev1, iev2                                            
C        CALL hzha_event                                                 
C        CALL timel(tileft)                                              
C        IF ( tileft .LE. timlft ) GOTO 2                                
C    1 CONTINUE                                                          
C                                                                       
C Close the job                                                         
C                                                                       
C    2 CALL hzha_close                                                   
C                                                                       
C      STOP                                                              
C      END                                                               


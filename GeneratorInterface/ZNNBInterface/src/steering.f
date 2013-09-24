      SUBROUTINE steering(filename)      
C                                                                       
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
      CHARACTER filename*120
      PARAMETER(nkey=4)                                                
      CHARACTER*4 key                                                   
      DIMENSION key(nkey),locvar(nkey),lenvar(nkey),leinit(nkey)
      DATA key/'TRIG','DEBU','TIME','GENE'/
      DATA leinit/2,2,1,4/
C                                                                       
      include 'fscard.inc'
C                                                                       
      CALL vzero(trig(1),2)                                             
      CALL vzero(debu(1),2)                                             
      CALL vzero(time(1),1)                                             
      CALL vzero(gene(1),4)                                             
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
      CALL ucopy(leinit(1),lenvar(1),nkey)
C
      CALL ffread(nkey,key,locvar,lenvar)                               
C
C Initialization
C
C      CALL znnb_init
C                                                                       
C Event generation                                                      
C                                                                       
C      iev1   = trig(1)                                                  
C      iev2   = trig(2) 
C      write(6,*) iev1,iev2
C      timlft = time(1)                                                  
C      DO 1 ievt = iev1, iev2                                            
C        CALL znnb_event                                                 
C        CALL timel(tileft)                                              
C        IF ( tileft .LE. timlft ) GOTO 2                                
C    1 CONTINUE                                                          
C                                                                       
C Close the job                                                         
C                                                                       
C      CALL zznb_close                                                   
C                                                                       
      RETURN                                                            
      END                                                               


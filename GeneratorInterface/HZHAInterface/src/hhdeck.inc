      CHARACTER*14 channel
      CHARACTER*21 channeut, chanchar
      REAL*8 branch, width, parwth, xymas, xywid
      REAL*8 wneut, wchar, widneut, brneut, widchar, brchar
      REAL*8 pvect4, bchpp, bchgg, ph
      PARAMETER(nchneut=8,nchchar=5)
      COMMON / hhdeck / branch(nchan,nhig),width(nhig),
     .                  parwth(nhig),xymas(2,nchan,nhig),
     .                  xywid(2,nchan,nhig)
      COMMON / chaneu / ichn(2),
     .                  wneut(4,4,nhig), wchar(2,2,nhig),
     .                  widneut(4), brneut(nchneut,4),
     .                  widchar(2), brchar(nchchar,2)
      COMMON / chanch / channeut(nchneut,4), chanchar(nchchar,2)
      COMMON / vect4 / pvect4(5,2)
      COMMON / hinput / ichan(nchan,nhig), channel(nchan,nhig)
      COMMON / hisbr / bchpp,bchgg
      DIMENSION ph(4)
C

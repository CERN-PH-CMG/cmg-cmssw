#ifndef DUPLICATESCHECKER_h
#define DUPLICATESCHECKER_h

#include<iostream>
#include<vector>
#include <ext/hash_map>

namespace __gnu_cxx
{
        template<> struct hash< std::string >
        {
                size_t operator()( const std::string& x ) const
                {
                        return hash< const char* >()( x.c_str() );
                }
        };
}


class DuplicatesChecker{
   private :
  typedef __gnu_cxx::hash_map<std::string, bool > RunEventHashMap;
      RunEventHashMap map;
   public :
        DuplicatesChecker(){}
        ~DuplicatesChecker(){}
        void Clear(){map.clear();}
        bool isDuplicate(unsigned int Run, unsigned int Lumi, unsigned int Event){
           char tmp[255];sprintf(tmp,"%i_%i_%i",Run,Lumi,Event);
           RunEventHashMap::iterator it = map.find(tmp);
           if(it==map.end()){
              map[tmp] = true;
              return false;
           }
           return true;
        }
        bool isDuplicate(unsigned int Run, unsigned int Lumi, unsigned int Event,unsigned int cat){
	  char tmp[255];sprintf(tmp,"%i_%i_%i_%i",Run,Lumi,Event,cat);
	  RunEventHashMap::iterator it = map.find(tmp);
	  if(it==map.end()){
	    map[tmp] = true;
	    return false;
	  }
	  return true;
        }
};

#endif

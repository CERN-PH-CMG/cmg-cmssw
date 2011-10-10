
#include <string>
#include <vector>
#include <iostream>
#include <iterator>

using namespace std;

namespace JSONWrapper{

std::string removeWhiteSpace(const std::string& in){
   int n = in.size();
   std::string out = "";
     
   bool isText1 = false;
   bool isText2 = false;
   for(int i=0;i<n;i++){
      if(in[i]=='\"')isText1=!isText1;
      if(in[i]=='\'')isText2=!isText2;

      if(!isText1 && !isText2 && (in[i]==' ' || in[i]=='\n' || in[i]=='\t' || in[i]=='\r'))continue;
      out += in[i];  
   } 
   return out;
}


std::string removeQuotes(const std::string& in){
   int n = in.size();
   std::string out = "";
   if(in[0]=='\"' && in[n-1]=='\"')return in.substr(1,n-2);
   return in;
}


size_t findComma(const std::string& in, int start){
   int n = in.size();
   bool isText1 = false;
   bool isText2 = false;
   int  isBracket = 0;
   int  isBrace   = 0;
   for(int i=start;i<n;i++){
      if(in[i]=='\"')isText1=!isText1;
      if(in[i]=='\'')isText2=!isText2;
      if(in[i]=='[')isBracket++;
      if(in[i]==']')isBracket--;
      if(in[i]=='{')isBrace  ++;
      if(in[i]=='}')isBrace  --;

      if(!isText1 && !isText2 && isBracket==0 && isBrace  ==0 && in[i]==',' )return i;
   }
   return std::string::npos;
}


size_t findEndBrace(const std::string& in, int start){
   int n = in.size();
   bool isText1 = false;
   bool isText2 = false;
   int  isBracket = 0;
   int  isBrace   = 1;
   for(int i=start;i<n;i++){
      if(in[i]=='\"')isText1=!isText1;
      if(in[i]=='\'')isText2=!isText2;
      if(in[i]=='[')isBracket++;
      if(in[i]==']')isBracket--;
      if(in[i]=='{')isBrace  ++;
      if(in[i]=='}')isBrace  --;

      if(!isText1 && !isText2 && isBracket==0 && isBrace  ==0)return i;
   }
   return std::string::npos;
}

size_t findEndBracket(const std::string& in, int start){
   int n = in.size();
   bool isText1 = false;
   bool isText2 = false;
   int  isBracket = 1;
   int  isBrace   = 0;
   for(int i=start;i<n;i++){
      if(in[i]=='\"')isText1=!isText1;
      if(in[i]=='\'')isText2=!isText2;
      if(in[i]=='[')isBracket++;
      if(in[i]==']')isBracket--;
      if(in[i]=='{')isBrace  ++;
      if(in[i]=='}')isBrace  --;

      if(!isText1 && !isText2 && isBracket==0 && isBrace  ==0)return i;
   }
   return std::string::npos;
}


bool isObject(const std::string& in){return in[0]=='{';}
bool isArray (const std::string& in){return in[0]=='[';}
bool isComma (const std::string& in){return in[0]==',';}

class Object
{
        public:
                Object(){}
                Object(const std::string& in, bool isInputFile=false);
                void ParseObject(const std::string& in); 
                void GetObjects(const std::string& in);
                void GetArray  (const std::string& in);
                void Load      (const std::string& in);
                void Print     (int Level=0);
                bool   isTag(std::string searchkey) const {for(unsigned int i=0;i<key.size();i++){if(key[i] == searchkey)return true;}return false; }
                Object operator[] (std::string searchkey) const {for(unsigned int i=0;i<key.size();i++){if(key[i] == searchkey)return obj[i];}return Object(); }
                Object operator[] (int i                ) const {return obj[i]; }
                string toString(){return val;}
                const char* c_str(){return val.c_str();}
                double toDouble(){double tmp; sscanf(val.c_str(),"%lf",&tmp);return tmp;}
                bool   toBool  (){if( (val[0]=='t' || val[0]=='T') && (val[1]=='r' || val[1]=='R') && (val[2]=='u' || val[2]=='U') && (val[3]=='e' || val[3]=='E') )return true; return false; }
                std::vector<Object>& daughters(){return obj;}


        int EndOfObject;
        std::vector<std::string> key;
        std::vector<Object> obj;
        std::string val;
};


Object::Object(const std::string& in, bool IsInputFile){
   if(IsInputFile){
      Load(in);
   }else{
      ParseObject(in);
   }
}

void Object::Load(const std::string& in){
   printf("Loading: %s\n", in.c_str());
   FILE* pFile = fopen(in.c_str(),"r");

   char buffer[1024];
   std::string JsonFile = "";
   while(!feof(pFile)){
      fgets(buffer,1024,pFile);
      JsonFile += buffer;
   }
   fclose(pFile);
   JsonFile = removeWhiteSpace(JsonFile);
   ParseObject(JsonFile);
}


void Object::ParseObject(const std::string& in){
   if(isObject(in)){
      GetObjects(in);
      val = "LIST";
   }else if(isArray(in)){
      GetArray(in);
      val = "ARRAY";
   }else{
      val = removeQuotes(in);
   }
}

void Object::GetObjects(const std::string& in){
   size_t start   = in.find('{');
   size_t mid     = in.find(':', start);
   size_t end     = findEndBrace(in,mid);//in.rfind('}');
   size_t nextval = std::min(findComma(in,mid),end);

   do{
      key.push_back( removeQuotes(in.substr( start+1,(mid-start)-1) )    );
      obj.push_back( Object(in.substr( mid  +1,  (nextval-mid)-1) ) );    

      start   = nextval;
      mid     = in.find(':', start);
      nextval = std::min(findComma(in,mid),end);
      if(start==nextval)break;
   }while(nextval<=end);
}


void Object::GetArray(const std::string& in){
   size_t start   = in.find('[');
   size_t end     = findEndBracket(in,start+1);
   size_t nextval = std::min(findComma(in,start+1),end);

   do{
      key.push_back("obj" );
      obj.push_back( Object(in.substr( start+1,  (nextval-start)-1) ) );

      start   = nextval;
      nextval = std::min(findComma(in,start+1),end);
      if(start==nextval)break;
   }while(nextval<=end);
}

void Object::Print(int Level){
      std::string indent = "";for(int i=0;i<Level;i++)indent += "   ";

      for(unsigned int i=0;i<key.size();i++){
         printf("%sKEY = %20s   VAL = %s\n",indent.c_str(),key[i].c_str(), obj[i].val.c_str());
         obj[i].Print(Level+1);
      }
} 

}

#ifndef PyTMVATool_h
#define PyTMVATool_h

namespace TMVA { class Reader; }
#include <vector>
#include <string>
 
class PyTMVATool {
    public:
        PyTMVATool(const char *name=0, int nvars=1, const char *weightFile=0, const char *options = 0) ;
        PyTMVATool(const PyTMVATool &other) ;
        ~PyTMVATool() ;

        void AddIntVariable(const char *var) ;
        void AddFloatVariable(const char *var) ;
        void DoneInit();

        void setInt(int i, int val)   const { numbers_[i].i = val; }
        void setFloat(int i, float val) const { numbers_[i].f = val; }

        double getVal() const ;
    private:
        typedef union { int i; float f; } number;

        std::string name_, nvars_, weightFile_, options_;
        TMVA::Reader *reader_;
        unsigned int ndeclared_;
        mutable std::vector<number> numbers_;
};
#endif

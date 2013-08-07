#include "CMGTools/TTHAnalysis/interface/PyTMVATool.h"

#include <TMVA/Reader.h>
#include <cassert>

PyTMVATool::PyTMVATool(const char *name, int nvars, const char *weightFile, const char *options) : 
    name_(name), weightFile_(weightFile), options_(options),
    numbers_(nvars) 
{
    reader_ = new TMVA::Reader(options_.c_str());
    ndeclared_ = 0;
}

PyTMVATool::PyTMVATool(const PyTMVATool &other) :
    name_(other.name_), weightFile_(other.weightFile_), options_(other.options_),
    numbers_(other.numbers_.size()) 
{
    reader_ = new TMVA::Reader(options_.c_str());
    ndeclared_ = 0;
}

PyTMVATool::~PyTMVATool() 
{
    delete reader_;
}

void PyTMVATool::AddIntVariable(const char *var) 
{
    reader_->AddVariable(var, &numbers_[ndeclared_].i);
    ndeclared_++;
}

void PyTMVATool::AddFloatVariable(const char *var) 
{
    reader_->AddVariable(var, &numbers_[ndeclared_].f);
    ndeclared_++;
}

void PyTMVATool::DoneInit() 
{
    assert(ndeclared_ == numbers_.size());
    reader_->BookMVA(name_, weightFile_);
}

double PyTMVATool::getVal() const {
    return reader_->EvaluateMVA(name_);
}

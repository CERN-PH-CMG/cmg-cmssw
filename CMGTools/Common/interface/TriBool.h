#ifndef TRIBOOL_H_
#define TRIBOOL_H_

namespace cmg{
	
	//a boolean type that has three states
	enum TriBool{ tbUnset = -1, tbFalse = 0, tbTrue = 1};
	
	bool toBool(const TriBool& b);
	TriBool toTriBool(const bool& b);

}

#endif /*TRIBOOL_H_*/

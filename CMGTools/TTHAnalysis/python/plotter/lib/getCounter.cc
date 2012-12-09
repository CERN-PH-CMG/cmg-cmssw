#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

double getCounter(string filename, string header) {

  // Open file
  ifstream file(filename.c_str());

  if (!file) { // Oops
    cerr << filename << "Not found" <<endl;
  }

  string line;

  while (getline(file, line)) {
    size_t pos = line.find(header);
    if (pos==string::npos) continue;
    string data = line.substr(pos+header.size(),string::npos);
    istringstream iss (data,istringstream::in);
    double value=0;
    iss >> value;
    //    cout << data << " val: " << value <<endl;
    return value;
  }  
  return 0;
}

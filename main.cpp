#include <iostream>
#include <fstream>
#include "src/VirtualMachine.h"

using namespace std;

void usage() {
  cout << "Usage: ./wtf-vm MANIFEST_FILE" << endl;
  exit(1);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    usage();
  }

  Wtf::VirtualMachine vm;
  vm.startVm();

  return 0;
}
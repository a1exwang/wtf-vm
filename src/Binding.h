//
// Created by alexwang on 8/15/16.
//

#ifndef WTF_VM_BINDING_H
#define WTF_VM_BINDING_H


#include <memory>
#include <map>
#include "DataValue.h"
#include "WtfException.h"

namespace Wtf {
  class Binding {
  public:
    Binding(std::shared_ptr<Binding> outerBinding) :outerBinding(outerBinding) {}
    void defineVar(const std::string& name, std::shared_ptr<DataValue> val) {
      if (varTable.find(name) == varTable.end()) {
        // This variable has not been defined
        varTable[name] = val;
      }
      else {
        throw VarRedefined(std::string("Variable '") + name + "' redefined");
      }
    }

    std::shared_ptr<DataValue> findVar(const std::string& name) {
      auto it = varTable.find(name);
      if (it == varTable.end()) {
        if (outerBinding.get() != nullptr) {
          try {
            return outerBinding->findVar(name);
          }
          catch(VarUndefined) {
            throw VarUndefined(std::string("Variable '") + name + "' undefined");
          }
        }
        throw VarUndefined(std::string("Variable '") + name + "' undefined");
      }
      return it->second;
    }

  private:
    std::shared_ptr<Binding> outerBinding;
    std::map<std::string, std::shared_ptr<DataValue>> varTable;
  };
}


#endif //WTF_VM_BINDING_H

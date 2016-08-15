//
// Created by alexwang on 8/15/16.
//

#ifndef WTF_VM_VIRTUALMACHINE_H
#define WTF_VM_VIRTUALMACHINE_H

#include <memory>
#include <map>
#include "MachineInst.h"
#include "DataValue.h"
#include "Binding.h"

namespace Wtf {

  class VirtualMachine {
  public:
    VirtualMachine();
    ~VirtualMachine() { delete root; }
    VirtualMachine(const VirtualMachine&) = delete;
    VirtualMachine& operator=(const VirtualMachine&) = delete;
    VirtualMachine(VirtualMachine&&) = delete;
    VirtualMachine& operator=(VirtualMachine&&) = delete;

  public:
    void exec(std::shared_ptr<const MachineInst> inst, Binding* currentBinding);

    void evalFnValue(CallableValue* fn, Binding* currentBinding);

    void evalInstList(std::vector<std::shared_ptr<const MachineInst>> instList, Binding *currentBinding);

    void startVm() {
      root->call(this, {});
    }

  protected:
    std::shared_ptr<DataValue> popStack() {
      auto ret = *(stack.end() - 1);
      stack.pop_back();
      return ret;
    }

  private:
    std::vector<std::shared_ptr<DataValue>> stack;
    FunctionValue *root;
  };

}
#endif //WTF_VM_VIRTUALMACHINE_H

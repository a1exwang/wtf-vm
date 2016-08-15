//
// Created by alexwang on 8/15/16.
//

#include "DataValue.h"
#include "Binding.h"
#include "VirtualMachine.h"
#include <memory>

void Wtf::FunctionValue::call(Wtf::VirtualMachine *vm, std::vector<std::shared_ptr<Wtf::DataValue>> params) {
  Binding *shadowBinding = new Binding(binding);
  if (params.size() != this->getArgumentNames().size()) {
    throw ArgumentError(std::string("argument size not matched"));
  }
  for (int i = 0; i < getArgumentNames().size(); ++i) {
    shadowBinding->defineVar(getArgumentNames()[i], params[i]);
  }

  vm->evalInstList(instructions, shadowBinding);
  delete shadowBinding;
}

Wtf::FunctionValue::FunctionValue(std::vector<std::shared_ptr<const Wtf::MachineInst>> insts,
                                  std::vector<std::string> args, std::shared_ptr<Wtf::Binding> outerBinding)
    :CallableValue(args, "FunctionValue"),
     outerBinding(outerBinding),
     instructions(insts), binding(std::shared_ptr<Binding>(new Binding(outerBinding))){
}

//
// Created by alexwang on 8/15/16.
//

#include <iostream>
#include "VirtualMachine.h"

void Wtf::VirtualMachine::exec(std::shared_ptr<const Wtf::MachineInst> inst, Wtf::Binding *currentBinding) {
  std::string type = inst->getType();
  if (type == "PushIntInst") {
    stack.push_back(
        std::shared_ptr<IntValue>(new IntValue(
            static_cast<const PushIntInst*>(inst.get())->getValue()
        )));
  }
  else if (type == "PushStrInst") {
    stack.push_back(
        std::shared_ptr<StringValue>(new StringValue(
            static_cast<const PushStrInst*>(inst.get())->getValue()
        )));
  }
  else if (type == "DupInst") {
    stack.push_back(*(stack.end()-1));
  }
  else if (type == "FnCallInst") {
    auto fnName = popStack();
    StringValue *stringValue = static_cast<StringValue*>(fnName.get());
    std::string str = stringValue->getStringValue();
    auto fn = currentBinding->findVar(str);
    evalFnValue(static_cast<CallableValue*>(fn.get()), currentBinding);
  }
}

void Wtf::VirtualMachine::evalFnValue(Wtf::CallableValue *fn, Wtf::Binding *currentBinding) {
  auto argCount = fn->getArgumentCount();
  std::vector<std::shared_ptr<DataValue>> values;
  for (size_t i = 0; i < argCount; ++i) {
    auto p = popStack();
    values.push_back(p);
  }
  fn->call(this, values);
}

void Wtf::VirtualMachine::evalInstList(std::vector<std::shared_ptr<const Wtf::MachineInst>> instList,
                                       Wtf::Binding *currentBinding) {
  for (auto instPtr : instList) {
    exec(instPtr, currentBinding);
  }
}

Wtf::VirtualMachine::VirtualMachine() {
  std::vector<std::shared_ptr<const MachineInst>> insts;

  insts.push_back(std::shared_ptr<PushStrInst>(new PushStrInst("Hello world")));
  insts.push_back(std::shared_ptr<PushStrInst>(new PushStrInst("puts")));
  insts.push_back(std::shared_ptr<FnCallInst>(new FnCallInst()));

  std::vector<std::string> args;
  root = new FunctionValue(insts, args, nullptr);

  auto fnPuts = [] (VirtualMachine* vm, std::vector<std::shared_ptr<DataValue>> params) -> void {
    auto str = static_cast<StringValue*>(params[0].get());
    std::cout << str->getStringValue() << std::endl;
  };
  root->getBinding()->defineVar("puts", std::shared_ptr<NativeFnValue>(new NativeFnValue(1, fnPuts)));

}

//
// Created by alexwang on 8/15/16.
//

#ifndef WTF_VM_DATAVALUE_H
#define WTF_VM_DATAVALUE_H
#include <string>
#include <cstdlib>
#include <vector>
#include <memory>
#include <sstream>
#include "MachineInst.h"
#include "WtfException.h"

namespace Wtf {
  class VirtualMachine;
  class Binding;
  class DataValue {
  public:
    DataValue(const std::string &type) :type(type) { }
    std::string getType() const {
      return this->type;
    }
    virtual bool needGC() const = 0;
    virtual std::string toString() const = 0;
    virtual ~DataValue() { }
  private:
    std::string type;
  };

  class IntValue :public DataValue {
  public:
    IntValue(int v) :DataValue("IntValue"), intValue(v) { }
    virtual std::string toString() const override {
      return std::to_string(intValue);
    }
    int getIntValue() const {
      return intValue;
    }
    bool needGC() const override { return false; }

  private:
    int intValue;
  };

  class StringValue :public DataValue {
  public:
    StringValue(const std::string &str) :DataValue("StringValue"), stringValue(str) { }
    virtual std::string toString() const override {
      return stringValue;
    }
    std::string getStringValue() const {
      return stringValue;
    }
    bool needGC() const override { return true; }
  private:
    std::string stringValue;
  };

  class CallableValue :public DataValue {
  public:
    CallableValue(std::vector<std::string> argumentNames, const std::string &type)
        :DataValue(type), argumentNames(argumentNames) { }
    std::vector<std::string> getArgumentNames() const {
      return argumentNames;
    }
    size_t getArgumentCount() const {
      return argumentNames.size();
    }
    bool needGC() const { return false; }

    virtual void call(VirtualMachine *vm, std::vector<std::shared_ptr<DataValue>> params) = 0;
  private:
    std::vector<std::string> argumentNames;
  };

  class NativeFnValue :public CallableValue {
  public:
    NativeFnValue(int argumentCount, std::function<void (VirtualMachine*, std::vector<std::shared_ptr<DataValue>>(params))> nativeFn)
        :CallableValue(makeArgumentNames(argumentCount), "NativeFnValue"), nativeFn(nativeFn) {
    }

    std::vector<std::string> makeArgumentNames(int c) {
      std::vector<std::string> ret;
      std::stringstream ss;
      for (int i = 0; i < c; ++i) {
        ss << "p" << i;
        ret.push_back(ss.str());
        ss.clear();
      }
      return ret;
    }

    virtual void call(VirtualMachine *vm, std::vector<std::shared_ptr<DataValue>> params) override {
      nativeFn(vm, params);
    }
    virtual std::string toString() const override {
      std::stringstream ss;
      ss << "NativeFunction<" << this << ">";
      return ss.str();
    }

    std::function<void (VirtualMachine*, std::vector<std::shared_ptr<DataValue>>(params))> nativeFn;
  };

  class FunctionValue :public CallableValue {
  public:
    FunctionValue(std::vector<std::shared_ptr<const MachineInst>> insts,
                  std::vector<std::string> args,
                  std::shared_ptr<Binding> outerBinding);

    std::shared_ptr<Binding> getBinding() {
      return binding;
    }

    virtual std::string toString() const override {
      std::stringstream ss;
      ss << "Function<" << this << ">";
      return ss.str();
    }
    virtual void call(VirtualMachine *vm, std::vector<std::shared_ptr<DataValue>> params) override;

    std::vector<std::shared_ptr<const MachineInst>> getInstructions() const {
      return instructions;
    }
    bool needGC() const override { return true; }
  private:
    std::shared_ptr<Binding> outerBinding;
    std::vector<std::shared_ptr<const MachineInst>> instructions;
    std::shared_ptr<Binding> binding;
  };
}


#endif //WTF_VM_DATAVALUE_H

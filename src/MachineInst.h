//
// Created by alexwang on 8/15/16.
//

#ifndef WTF_VM_MACHINECODE_H
#define WTF_VM_MACHINECODE_H

#include <string>

namespace Wtf {
  class MachineInst {
  public:
    MachineInst(const std::string& type) :type(type) { }
    const std::string &getType() const { return type; }
    virtual std::string serialize() const = 0;
    virtual ~MachineInst() { }
  private:
    std::string type;
  };

  class PushIntInst :public MachineInst {
  public:
    PushIntInst(int value) :MachineInst("PushIntInst"), value(value) {}
    int getValue() const {
      return value;
    }
    virtual std::string serialize() const override {
      return "PushIntInst";
    }
  private:
    int value;
  };

  class PushStrInst :public MachineInst {
  public:
    PushStrInst(const std::string& str) :MachineInst("PushStrInst"), str(str) {}
    std::string getValue() const {
      return str;
    }
    virtual std::string serialize() const override {
      return "PushStrInst";
    }
  private:
    std::string str;
  };

  class DupInst :public MachineInst {
  public:
    DupInst() :MachineInst("DupInst") { }

    virtual std::string serialize() const override {
      return "DupInst";
    }
  };

  class FnCallInst :public MachineInst {
  public:
    FnCallInst() :MachineInst("FnCallInst") { }

    virtual std::string serialize() const override {
      return "FnCallInst";
    }
  };


}
#endif //WTF_VM_MACHINECODE_H

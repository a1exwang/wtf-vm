//
// Created by alexwang on 8/15/16.
//

#ifndef WTF_VM_EXCEPTION_H
#define WTF_VM_EXCEPTION_H

#include <string>
#include <typeinfo>

namespace Wtf {

  class WtfException {
  public:
    WtfException(const std::string& msg) :msg(msg) { }
    std::string getMessage() const {
      return std::string("<") + std::string(typeid(*this).name()) + std::string("> ") + msg;
    }
    virtual ~WtfException() { };
  private:
    std::string msg;
  };

  class VarRedefined :public WtfException {
    using WtfException::WtfException;
  };

  class VarUndefined :public WtfException {
    using WtfException::WtfException;
  };
  class ArgumentError :public WtfException {
    using WtfException::WtfException;
  };

}


#endif //WTF_VM_EXCEPTION_H

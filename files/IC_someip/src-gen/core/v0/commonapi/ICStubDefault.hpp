/*
* This file was generated by the CommonAPI Generators.
* Used org.genivi.commonapi.core 3.2.14.v202310241605.
* Used org.franca.core 0.13.1.201807231814.
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* http://mozilla.org/MPL/2.0/.
*/
#ifndef V0_COMMONAPI_IC_STUB_DEFAULT_HPP_
#define V0_COMMONAPI_IC_STUB_DEFAULT_HPP_


#include <CommonAPI/Export.hpp>

#include <v0/commonapi/ICStub.hpp>
#include <cassert>
#include <sstream>

# if defined(_MSC_VER)
#  if _MSC_VER >= 1300
/*
 * Diamond inheritance is used for the CommonAPI::Proxy base class.
 * The Microsoft compiler put warning (C4250) using a desired c++ feature: "Delegating to a sister class"
 * A powerful technique that arises from using virtual inheritance is to delegate a method from a class in another class
 * by using a common abstract base class. This is also called cross delegation.
 */
#    pragma warning( disable : 4250 )
#  endif
# endif

namespace v0 {
namespace commonapi {

/**
 * Provides a default implementation for ICStubRemoteEvent and
 * ICStub. Method callbacks have an empty implementation,
 * remote set calls on attributes will always change the value of the attribute
 * to the one received.
 *
 * Override this stub if you only want to provide a subset of the functionality
 * that would be defined for this service, and/or if you do not need any non-default
 * behaviour.
 */
class COMMONAPI_EXPORT_CLASS_EXPLICIT ICStubDefault
    : public virtual ICStub {
public:
    COMMONAPI_EXPORT ICStubDefault()
        : remoteEventHandler_(this),
          interfaceVersion_(IC::getInterfaceVersion()) {
    }

    COMMONAPI_EXPORT const CommonAPI::Version& getInterfaceVersion(std::shared_ptr<CommonAPI::ClientId> _client) {
        (void)_client;
        return interfaceVersion_;
    }

    COMMONAPI_EXPORT ICStubRemoteEvent* initStubAdapter(const std::shared_ptr< ICStubAdapter> &_adapter) {
        CommonAPI::Stub<ICStubAdapter, ICStubRemoteEvent>::stubAdapter_ = _adapter;
        return &remoteEventHandler_;
    }

    COMMONAPI_EXPORT virtual void setGear(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _gear, setGearReply_t _reply) {
        (void)_client;
        (void)_gear;
        int32_t result = 0;
        _reply(result);
    }
    COMMONAPI_EXPORT virtual void getBattery(const std::shared_ptr<CommonAPI::ClientId> _client, getBatteryReply_t _reply) {
        (void)_client;
        int32_t battery = 0;
        int32_t result = 0;
        _reply(battery, result);
    }
    COMMONAPI_EXPORT virtual void setMode(const std::shared_ptr<CommonAPI::ClientId> _client, int32_t _mode, setModeReply_t _reply) {
        (void)_client;
        (void)_mode;
        int32_t result = 0;
        _reply(result);
    }
    COMMONAPI_EXPORT virtual void fireBatteryStatusChangedEvent(const int32_t &_batValue) {
        ICStub::fireBatteryStatusChangedEvent(_batValue);
    }
    COMMONAPI_EXPORT virtual void fireGearStatusChangedEvent(const std::string &_gearValue) {
        ICStub::fireGearStatusChangedEvent(_gearValue);
    }
    COMMONAPI_EXPORT virtual void fireLrSignStatusChangedEvent(const int32_t &_signValue) {
        ICStub::fireLrSignStatusChangedEvent(_signValue);
    }
    COMMONAPI_EXPORT virtual void firePdcStatusChangedEvent(const int32_t &_distValue) {
        ICStub::firePdcStatusChangedEvent(_distValue);
    }


protected:
    class COMMONAPI_EXPORT_CLASS_EXPLICIT RemoteEventHandler: public virtual ICStubRemoteEvent {
    public:
        COMMONAPI_EXPORT RemoteEventHandler(ICStubDefault *_defaultStub)
            : 
              defaultStub_(_defaultStub) {
        }


    private:
        ICStubDefault *defaultStub_;
    };
protected:
    ICStubDefault::RemoteEventHandler remoteEventHandler_;

private:


    CommonAPI::Version interfaceVersion_;
};

} // namespace commonapi
} // namespace v0


// Compatibility
namespace v0_1 = v0;

#endif // V0_COMMONAPI_IC_STUB_DEFAULT

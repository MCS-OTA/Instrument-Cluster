// "v0.0.0"
#include "ICStubImpl.h"
#include "speed.h"
#include "battery.h"
#include <string>
#include <thread>

ICStubImpl::ICStubImpl(Gear* gear, Battery * battery, QObject *parent)
    : QObject(parent), gear_(gear),battery_(battery) {

    //    CommonAPI::Runtime::get()->registerStateChangeListener([this](CommonAPI::AvailabilityState state) {
//        if (state == CommonAPI::AvailabilityState::AVAILABLE) {
//            onClientConnected();
//        } else if (state == CommonAPI::AvailabilityState::NOT_AVAILABLE) {
//            onClientDisconnected();
//        }
//    });
    std::thread([this]() {
        const char* SOCKET_PATH = "/tmp/notify.sock";
        unlink(SOCKET_PATH);  // 기존 소켓 파일 삭제

        int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        sockaddr_un addr;
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, SOCKET_PATH);
        bind(server_fd, (sockaddr*)&addr, sizeof(addr));
        listen(server_fd, 1);

        std::cout << "IC: waiting for notify update ..." << std::endl;

        while (true) {
            int client_fd = accept(server_fd, nullptr, nullptr);
            if (client_fd < 0) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }

            char buffer[128] = {0};
            int len = read(client_fd, buffer, sizeof(buffer));
            if (len > 0) {
                int ask_value = std::stoi(buffer);
                std::cout << "🔔 [IC] Python → 업데이트 알림 수신: " << ask_value << std::endl;
                askNotify(ask_value);  // HU로 이벤트 전달
            }
            close(client_fd);
        }
    }).detach();  // 백그라운드 스레드
}
ICStubImpl::~ICStubImpl(){

}

void ICStubImpl::setGear(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _gear, setGearReply_t _reply){
    std::cout << "gear :" << _gear << "\n";
    if(_gear == "Start"){
        emit signalStart();
        _reply(0);
    }else if(_gear == "P" || _gear == "D" || _gear == "N" || _gear == "R"){
        emit signalGear(_gear);
        _reply(0);
    }else{
        _reply(-1);
    }
}
void ICStubImpl::getBattery(const std::shared_ptr<CommonAPI::ClientId> _client, getBatteryReply_t _reply) {
    int32_t batteryValue = battery_->batteryValue();
    if(batteryValue >= 0 || batteryValue <= 100){
        _reply(batteryValue, 0);
    }else{
        _reply(batteryValue, -1);
    }
}
void ICStubImpl::setMode(const std::shared_ptr<CommonAPI::ClientId> _client, int32_t _mode, setModeReply_t _reply){
    std::cout << "mode :" << _mode << "\n";
    emit signalMode(_mode);
    if(_mode == 0 || _mode == 1 || _mode == 2 || _mode == 42){
        _reply(0);
    }else{
        _reply(-1);
    }
}

void ICStubImpl::answerNotify(const std::shared_ptr<CommonAPI::ClientId> _client, int32_t _answer, answerNotifyReply_t _reply){
    std::cout << "[IC] Received answer from HU: " << _answer << std::endl;

    // 1. ota_prime_ecu에 로컬 소켓 등으로 전달
    int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/tmp/ota_back.sock");

    if (::connect(socket_fd, (sockaddr*)&addr, sizeof(addr)) == 0) {
        std::string msg = std::to_string(_answer);
        send(socket_fd, msg.c_str(), msg.size(), 0);
        close(socket_fd);
        std::cout << "✅ 전달 완료: ota_prime_ecu" << std::endl;
    } else {
        std::cerr << "❌ ota_prime_ecu 연결 실패!" << std::endl;
    }

    _reply(0);
}

void ICStubImpl::notifyBatteryStatusChanged(int32_t batteryValue){
    if(batteryValue >=0 && batteryValue <= 100){
        qDebug()<<"bat has changed to"<<batteryValue;
        fireBatteryStatusChangedEvent(batteryValue);
    }
}

void ICStubImpl::notifyGearStatusChanged(std::string gearValue){
    fireGearStatusChangedEvent(gearValue);
}
void ICStubImpl::notifyLRSignStatusChanged(int32_t signValue){
    fireLrSignStatusChangedEvent(signValue);
}
void ICStubImpl::notifyDistStatusChanged(int32_t distValue){
    firePdcStatusChangedEvent(distValue);
}
void ICStubImpl::askNotify(int32_t ask){
    fireAskNotifyEvent(ask);
    std::cout << "fire update nofity" << std::endl;
}

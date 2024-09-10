#pragma once

#include <thread>
// #include <mutex>
// #include <condition_variable>
#include <functional>
#include <string>
#include "zmq.hpp"

class CatGrayRemoteObjects
{
public:
    enum RemoteSendFlags
    {
        NONE,
        DONTWAIT,
        SNDMORE
    };
    enum RemoteBindMode
    {
        INVALID_MODE,
        INPROC,
        IPC,
        TCP
    };

    enum RemoteType
    {
        INVALID_TYPE,
        PUB,
        SUB,
        SERVER,
        CLIENT
    };

    CatGrayRemoteObjects();
    ~CatGrayRemoteObjects();

    bool RemoteObjectInitialize(RemoteType remotetype,
                                RemoteBindMode remotebindmode,
                                const std::string addr);
    void RemoteobjectUnInitialize();

    zmq::socket_t* RemoteObjectSocket() const;

    std::string getRemoteAddr() const;

    RemoteBindMode getRemoteBindMode() const {
        return m_RemoteBindMode;
    }

    RemoteType getRemoteType() const {
        return m_RemoteType;
    }

    void setReadCallback(std::function<void(std::vector<zmq::message_t>, bool)> callback)
    {
        m_Read_Callback = callback;
    }

    std::function<void(std::vector<zmq::message_t>, bool)> readCallback() const {
        return m_Read_Callback;
    }
    bool start();
    void stop();

    bool isStart() const;

    bool send(const std::string& message, RemoteSendFlags flag = RemoteSendFlags::NONE);
    bool send(const std::string& message, uint32_t routing_id);

private:
    bool Initialize_PUB();
    void UnInitialize_PUB();
    bool Initialize_SUB();
    void UnInitialize_SUB();
    bool Initialize_SERVER();
    void UnInitialize_SERVER();
    bool Initialize_CLIENT();
    void UnInitialize_CLIENT();

    static void readThread(CatGrayRemoteObjects* object);


private:
    std::string m_Addr = "";
    std::thread* m_pReadData_Thread = nullptr;
    std::function<void(std::vector<zmq::message_t>, bool)> m_Read_Callback = nullptr;
    RemoteBindMode m_RemoteBindMode = INVALID_MODE;
    RemoteType m_RemoteType = INVALID_TYPE;

    zmq::context_t *ctx = nullptr;
    zmq::socket_t *socket = nullptr;
    bool remoteThreadIsStart = false;
    bool remoteIsStart = false;
};

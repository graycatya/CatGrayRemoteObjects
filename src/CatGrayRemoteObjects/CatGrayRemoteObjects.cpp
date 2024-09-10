#include "CatGrayRemoteObjects.h"
#include "zmq_addon.hpp"

std::string toZmqAddr(CatGrayRemoteObjects::RemoteBindMode remotebindmode, const std::string addr)
{
    switch (remotebindmode) {
        case CatGrayRemoteObjects::RemoteBindMode::INPROC:
            return std::string("inproc://") + addr;
        case CatGrayRemoteObjects::RemoteBindMode::IPC:
            return std::string("ipc://") + addr;
        case CatGrayRemoteObjects::RemoteBindMode::TCP:
            return std::string("tcp://") + addr;
        default:
            break;
    }
    return std::string("");
}

bool isStartRead(CatGrayRemoteObjects::RemoteType remptetype)
{
    switch (remptetype) {
    case CatGrayRemoteObjects::RemoteType::CLIENT:
        return true;
    case CatGrayRemoteObjects::RemoteType::SUB:
        return true;
    case CatGrayRemoteObjects::RemoteType::SERVER:
        return true;
    default:
        break;
    }
    return false;
}

zmq::send_flags toZmqSendFlag(CatGrayRemoteObjects::RemoteSendFlags flag)
{
    switch (flag) {
        case CatGrayRemoteObjects::RemoteSendFlags::SNDMORE:
            return zmq::send_flags::sndmore;
        case CatGrayRemoteObjects::RemoteSendFlags::DONTWAIT:
            return zmq::send_flags::dontwait;
        default:
            return zmq::send_flags::none;
    }

}

CatGrayRemoteObjects::CatGrayRemoteObjects()
{
    m_Addr = "";
    m_pReadData_Thread = nullptr;
    m_Read_Callback = nullptr;
    m_RemoteBindMode = INVALID_MODE;
    m_RemoteType = INVALID_TYPE;
}

CatGrayRemoteObjects::~CatGrayRemoteObjects()
{
    RemoteobjectUnInitialize();
}

void CatGrayRemoteObjects::RemoteobjectUnInitialize()
{
    if(socket)
    {
        switch (m_RemoteType) {
        case PUB:
            UnInitialize_PUB();
        case SUB:
            UnInitialize_SUB();
        case SERVER:
            UnInitialize_SERVER();
        case CLIENT:
            UnInitialize_CLIENT();
        default:
            break;
        }
        delete socket;
        socket = nullptr;
    }
    if(ctx)
    {
        delete ctx;
        ctx = nullptr;
    }
    m_Addr = "";
    m_RemoteBindMode = INVALID_MODE;
    m_RemoteType = INVALID_TYPE;
}

zmq::socket_t *CatGrayRemoteObjects::RemoteObjectSocket() const
{
    return socket;
}

std::string CatGrayRemoteObjects::getRemoteAddr() const
{
    return toZmqAddr(this->m_RemoteBindMode, this->m_Addr);
}

bool CatGrayRemoteObjects::start()
{
    bool ret = RemoteObjectInitialize(this->m_RemoteType, this->m_RemoteBindMode, this->m_Addr);
    if(ret)
    {
        remoteIsStart = true;
        if(isStartRead(this->m_RemoteType))
        {
            m_pReadData_Thread = new std::thread(this->readThread, this);
            //     [&errormessage, this](){
            //     remoteThreadIsStart = true;
            //     while (remoteThreadIsStart) {
            //         std::vector<zmq::message_t> trecv_msgs;
            //         zmq::recv_result_t result =
            //         zmq::recv_multipart(*socket, std::back_inserter(trecv_msgs));
            //         if((result && "recv failed") || *result == 2)
            //         {
            //             errormessage = false;
            //         } else {
            //             errormessage = true;
            //         }

            //         if(!remoteThreadIsStart)
            //         {
            //             break;
            //         }
            //         // if(trecv_msgs.size() > 1)
            //         // {
            //         //     message = std::string("[" + trecv_msgs[0].to_string() + "] " + trecv_msgs[1].to_string());
            //         // }
            //         // std::cout << " message: " << message << " - " << errormessage << " - trecv_msgs size: " << trecv_msgs.size()  <<  std::endl;
            //         // std::cout.flush();
            //         if(this->readCallback() != nullptr)
            //         {
            //             std::function<void(std::vector<zmq::message_t>, bool)> callback = std::move(this->readCallback());
            //             callback(trecv_msgs, errormessage);
            //         }
            //     }
            // });
        }
    }
    return ret;
}

void CatGrayRemoteObjects::stop()
{
    if(remoteIsStart)
    {
        if(remoteIsStart)
        {
            if(isStartRead(this->m_RemoteType))
            {
                remoteThreadIsStart = false;
                m_pReadData_Thread->join();
                delete m_pReadData_Thread;
                m_pReadData_Thread = nullptr;
            }
            remoteIsStart = false;

        }
    }
}

bool CatGrayRemoteObjects::isStart() const
{
    return remoteIsStart;
}

bool CatGrayRemoteObjects::send(const std::string& message, RemoteSendFlags flag)
{
    bool ret = false;
    if(remoteIsStart)
    {
        zmq::message_t zmq_message(message.size());
        memcpy(zmq_message.data(), message.data(), message.size());
        try {
            zmq::send_result_t result = socket->send(zmq_message, toZmqSendFlag(flag));
            if (result.has_value() && !*result) {
                ret = false;
            } else {
                ret = true;
            }
        } catch(const zmq::error_t& e) {
            ret = false;
        }
    }
    return ret;
}

bool CatGrayRemoteObjects::send(const std::string &message, uint32_t routing_id)
{
    bool ret = false;
    if(remoteIsStart)
    {
        zmq::message_t zmq_message(message.size());
        zmq_message.set_routing_id(routing_id);
        memcpy(zmq_message.data(), message.data(), message.size());
        try {
            zmq::send_result_t result = socket->send(zmq_message, zmq::send_flags::none);
            if (result.has_value() && !*result) {
                ret = false;
            } else {
                ret = true;
            }
        } catch(const zmq::error_t& e) {
            ret = false;
        }
    }
    return ret;
}

bool CatGrayRemoteObjects::Initialize_PUB()
{
    try {
        socket = new zmq::socket_t(*ctx, zmq::socket_type::pub);
        socket->bind(toZmqAddr(this->m_RemoteBindMode, this->m_Addr));
    } catch(...) {
        return false;
    }
    return true;
}

void CatGrayRemoteObjects::UnInitialize_PUB()
{
    try {
        socket->unbind(toZmqAddr(this->m_RemoteBindMode, this->m_Addr));
    } catch(...) {
        {}
    }
}

bool CatGrayRemoteObjects::Initialize_SUB()
{
    try {
        socket = new zmq::socket_t(*ctx, zmq::socket_type::sub);
        socket->connect(toZmqAddr(this->m_RemoteBindMode, this->m_Addr));
    } catch(...) {
        return false;
    }
    return true;
}

void CatGrayRemoteObjects::UnInitialize_SUB()
{
    try {
        socket->disconnect(toZmqAddr(this->m_RemoteBindMode, this->m_Addr));
    } catch(...) {
        {}
    }
}

bool CatGrayRemoteObjects::Initialize_SERVER()
{
    try {
#if defined(ZMQ_BUILD_DRAFT_API) && ZMQ_VERSION >= ZMQ_MAKE_VERSION(4, 2, 0)
        socket = new zmq::socket_t(*ctx, zmq::socket_type::server);
        socket->bind(toZmqAddr(this->m_RemoteBindMode, this->m_Addr));
#else
        return false;
#endif
    } catch(...) {
        return false;
    }
    return true;
}

void CatGrayRemoteObjects::UnInitialize_SERVER()
{
    try {
#if defined(ZMQ_BUILD_DRAFT_API) && ZMQ_VERSION >= ZMQ_MAKE_VERSION(4, 2, 0)
        socket->unbind(toZmqAddr(this->m_RemoteBindMode, this->m_Addr));
#else
        {}
#endif
    } catch(...) {
        {}
    }
}

bool CatGrayRemoteObjects::Initialize_CLIENT()
{
#if defined(ZMQ_BUILD_DRAFT_API) && ZMQ_VERSION >= ZMQ_MAKE_VERSION(4, 2, 0)
    socket = new zmq::socket_t(*ctx, zmq::socket_type::client);
    socket->connect(toZmqAddr(this->m_RemoteBindMode, this->m_Addr));
#else
    return false;
#endif
    return true;
}

void CatGrayRemoteObjects::UnInitialize_CLIENT()
{
    try {
#if defined(ZMQ_BUILD_DRAFT_API) && ZMQ_VERSION >= ZMQ_MAKE_VERSION(4, 2, 0)
        socket->disconnect(toZmqAddr(this->m_RemoteBindMode, this->m_Addr));
#else
        {}
#endif
    } catch(...) {
        {}
    }
}

void CatGrayRemoteObjects::readThread(CatGrayRemoteObjects* object)
{
    bool errormessage = false;
    object->remoteThreadIsStart = true;
    while (object->remoteThreadIsStart) {
        std::vector<zmq::message_t> trecv_msgs;
        zmq::recv_result_t result =
        zmq::recv_multipart(*object->socket, std::back_inserter(trecv_msgs));
        if((result && "recv failed") || *result == 2)
        {
            errormessage = false;
        } else {
            errormessage = true;
        }

        if(!object->remoteThreadIsStart)
        {
            break;
        }
        if(object->readCallback() != nullptr)
        {
            std::function<void(std::vector<zmq::message_t>, bool)> callback = std::move(object->readCallback());
            callback(std::move(trecv_msgs), errormessage);
        }
    }
}

bool CatGrayRemoteObjects::RemoteObjectInitialize(RemoteType remotetype, RemoteBindMode remotebindmode,
                                                  const std::string addr)
{
    bool ret = false;
    if(!ctx)
    {
        if(!ctx)
        {
            ctx = new zmq::context_t;
            m_RemoteType = remotetype;
            m_RemoteBindMode = remotebindmode;
            m_Addr = addr;
            switch (m_RemoteType) {
                case PUB:
                    ret = Initialize_PUB();
                    break;
                case SUB:
                    ret = Initialize_SUB();
                    break;
                case SERVER:
                    ret = Initialize_SERVER();
                    break;
                case CLIENT:
                    ret = Initialize_CLIENT();
                    break;
                default:
                    m_RemoteType = INVALID_TYPE;
                    m_RemoteBindMode = INVALID_MODE;
                    ret = false;
            }

            if(!ret)
            {
                RemoteobjectUnInitialize();
            }
        } else {
            ret = true;
        }
    } else {
        ret = true;
    }

    return ret;
}



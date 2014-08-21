// Copyright 2014 GSI, Inc. All rights reserved.
//
//
//

#ifndef __DDS__InfoChannel__
#define __DDS__InfoChannel__
// DDS
#include "ConnectionImpl.h"
#include "Options.h"

namespace dds
{
    class CInfoChannel : public CConnectionImpl<CInfoChannel>
    {
        CInfoChannel(boost::asio::io_service& _service)
            : CConnectionImpl<CInfoChannel>(_service)
            , m_isHandShakeOK(false)
        {
        }

        REGISTER_DEFAULT_ON_DISCONNECT_CALLBACKS
        REGISTER_DEFAULT_ON_HEADER_READ_CALLBACKS

      public:
        BEGIN_MSG_MAP(CInfoChannel)
        MESSAGE_HANDLER(cmdREPLY_HANDSHAKE_OK, on_cmdREPLY_HANDSHAKE_OK)
        MESSAGE_HANDLER(cmdSIMPLE_MSG, on_cmdSIMPLE_MSG)
        MESSAGE_HANDLER(cmdREPLY_PID, on_cmdREPLY_PID)
        MESSAGE_HANDLER(cmdREPLY_AGENTS_INFO, on_cmdREPLY_AGENTS_INFO)
        END_MSG_MAP()

        void setOptions(const dds::SOptions& _options)
        {
            m_options = _options;
        }

      private:
        // Message Handlers
        bool on_cmdREPLY_HANDSHAKE_OK(const CProtocolMessage& _msg);
        bool on_cmdSIMPLE_MSG(const CProtocolMessage& _msg);
        bool on_cmdREPLY_PID(const CProtocolMessage& _msg);
        bool on_cmdREPLY_AGENTS_INFO(const CProtocolMessage& _msg);

        // On connection handles
        void onConnected()
        {
            LOG(MiscCommon::info) << "Connected to the commander server";
        }
        void onFailedToConnect()
        {
            LOG(MiscCommon::log_stderr) << "Failed to connect to commander server.";
        }

      private:
        bool m_isHandShakeOK;
        dds::SOptions m_options;
    };
}

#endif

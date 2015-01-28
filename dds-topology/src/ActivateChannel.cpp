// Copyright 2014 GSI, Inc. All rights reserved.
//
//
//

// DDS
#include "ActivateChannel.h"
#include "ProgressDisplay.h"

using namespace MiscCommon;
using namespace dds;
using namespace std;

void CActivateChannel::onHandshakeOK()
{
    pushMsg<cmdACTIVATE_AGENT>();
}
void CActivateChannel::onHandshakeERR()
{
}

bool CActivateChannel::on_cmdSIMPLE_MSG(SCommandAttachmentImpl<cmdSIMPLE_MSG>::ptr_t _attachment)
{
    if (m_options.m_verbose)
    {
        if (!_attachment->m_sMsg.empty())
            LOG((_attachment->m_msgSeverity == fatal || _attachment->m_msgSeverity == error) ? log_stderr : log_stdout)
                << "Server reports: " << _attachment->m_sMsg;
    }
    else
    {
        LOG(static_cast<ELogSeverityLevel>(_attachment->m_msgSeverity)) << _attachment->m_sMsg;
    }

    // stop communication if a fatal error is recieved
    if (_attachment->m_msgSeverity == fatal)
        stop();
    return true;
}

bool CActivateChannel::on_cmdSHUTDOWN(SCommandAttachmentImpl<cmdSHUTDOWN>::ptr_t /*_attachment*/)
{
    // Close communication channel
    stop();
    return true;
}

bool CActivateChannel::on_cmdPROGRESS(SCommandAttachmentImpl<cmdPROGRESS>::ptr_t _attachment)
{
    if (m_options.m_verbose)
        return true;

    int completed = _attachment->m_completed + _attachment->m_errors;
    if (completed < _attachment->m_total)
    {
        cout << getProgressDisplayString(completed, _attachment->m_total);
        cout.flush();
    }
    else
    {
        cout << getProgressDisplayString(completed, _attachment->m_total) << endl;
        cout << "Tasks activated: " << _attachment->m_completed << " errors: " << _attachment->m_errors
             << " total: " << _attachment->m_total << endl;
    }
    return true;
}

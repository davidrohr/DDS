// Copyright 2019 GSI, Inc. All rights reserved.
//
//
//

#ifndef DDS_TOOLSPROTOCOL_H
#define DDS_TOOLSPROTOCOL_H

// STD
#include <chrono>
#include <ostream>
#include <string>
//// BOOST
#include <boost/property_tree/ptree.hpp>
// DDS
#include "Intercom.h"
#include "ToolsProtocolCore.h"

namespace dds
{
    namespace tools_api
    {
        /// \brief Structure holds information of a done response.
        DDS_TOOLS_DECLARE_DATA_CLASS(SBaseResponseData, SDoneResponseData, "done")

        /// \brief Structure holds information of a message response.
        struct SMessageResponseData : SBaseResponseData<SMessageResponseData>
        {
            std::string m_msg;                                                                  ///< Message text.
            dds::intercom_api::EMsgSeverity m_severity = dds::intercom_api::EMsgSeverity::info; ///< Message severity.

          private:
            friend SBaseData<SMessageResponseData>;
            friend SBaseResponseData<SMessageResponseData>;
            void _fromPT(const boost::property_tree::ptree& _pt);
            void _toPT(boost::property_tree::ptree& _pt) const;
            static constexpr const char* _protocolTag = "message";

          public:
            /// \brief Equality operator.
            bool operator==(const SMessageResponseData& _val) const;
            /// \brief Ostream operator.
            friend std::ostream& operator<<(std::ostream& _os, const SMessageResponseData& _data);
        };

        /// \brief Structure holds information of a progress response.
        struct SProgressResponseData : SBaseResponseData<SProgressResponseData>
        {
            uint32_t m_completed = 0;
            uint32_t m_total = 0;
            uint32_t m_errors = 0;
            uint32_t m_time = 0;
            uint16_t m_srcCommand = 0; ///< Reserved for internal use

            SProgressResponseData();
            SProgressResponseData(
                uint16_t _srcCmd, uint32_t _completed, uint32_t _total, uint32_t _errors, uint32_t _time = 0);

          private:
            friend SBaseData<SProgressResponseData>;
            friend SBaseResponseData<SProgressResponseData>;
            void _fromPT(const boost::property_tree::ptree& _pt);
            void _toPT(boost::property_tree::ptree& _pt) const;
            static constexpr const char* _protocolTag = "progress";

          public:
            /// \brief Equality operator.
            bool operator==(const SProgressResponseData& _val) const;
            /// \brief Ostream operator.
            friend std::ostream& operator<<(std::ostream& _os, const SProgressResponseData& _data);
        };

        /// \brief Structure holds information of a submit request.
        struct SSubmitRequestData : SBaseRequestData<SSubmitRequestData>
        {
            std::string m_rms;        ///< RMS.
            uint32_t m_instances = 0; ///< Number of instances.
            uint32_t m_slots = 0;     /// < Number of task slots.
            std::string m_config;     ///< Path to the configuration file.
            std::string m_pluginPath; ///< Optional. A plug-in's directory search path

          private:
            friend SBaseData<SSubmitRequestData>;
            void _fromPT(const boost::property_tree::ptree& _pt);
            void _toPT(boost::property_tree::ptree& _pt) const;
            static constexpr const char* _protocolTag = "submit";

          public:
            /// \brief Equality operator.
            bool operator==(const SSubmitRequestData& _val) const;
            /// \brief Ostream operator.
            friend std::ostream& operator<<(std::ostream& _os, const SSubmitRequestData& _data);
        };

        /// \brief Request class of submit.
        using SSubmitRequest = SBaseRequestImpl<SSubmitRequestData, SEmptyResponseData>;

        /// \brief Structure holds information of topology request.
        struct STopologyRequestData : SBaseResponseData<STopologyRequestData>
        {
            enum class EUpdateType : uint8_t
            {
                UPDATE = 0,
                ACTIVATE,
                STOP
            };
            EUpdateType m_updateType = EUpdateType::UPDATE; ///< Topology update type: Update, Activate, Stop
            std::string m_topologyFile;                     ///< A topology file to process
            bool m_disableValidation = false; ///< A flag to disiable topology validation before processing it.

          private:
            friend SBaseData<STopologyRequestData>;
            void _fromPT(const boost::property_tree::ptree& _pt);
            void _toPT(boost::property_tree::ptree& _pt) const;
            static constexpr const char* _protocolTag = "topology";

          public:
            /// \brief Equality operator.
            bool operator==(const STopologyRequestData& _val) const;
            /// \brief Ostream operator.
            friend std::ostream& operator<<(std::ostream& _os, const STopologyRequestData& _data);
        };

        /// \brief Request class of topology.
        using STopologyRequest = SBaseRequestImpl<STopologyRequestData, SEmptyResponseData>;

        /// \brief Structure holds information of a getlog request.
        DDS_TOOLS_DECLARE_DATA_CLASS(SBaseRequestData, SGetLogRequestData, "getlog")

        /// \brief Request class of getlog.
        using SGetLogRequest = SBaseRequestImpl<SGetLogRequestData, SEmptyResponseData>;

        /// \brief Structure holds information of commanderInfo response.
        struct SCommanderInfoResponseData : SBaseResponseData<SCommanderInfoResponseData>
        {
            pid_t m_pid = 0;                  ///< PID of the commander
            std::string m_activeTopologyName; ///< Name of active topology, empty if none is active
            std::string m_activeTopologyPath; ///< Filepath of active topology, empty if none is active

          private:
            friend SBaseData<SCommanderInfoResponseData>;
            friend SBaseResponseData<SCommanderInfoResponseData>;
            void _fromPT(const boost::property_tree::ptree& _pt);
            void _toPT(boost::property_tree::ptree& _pt) const;
            static constexpr const char* _protocolTag = "commanderInfo";

          public:
            /// \brief Equality operator.
            bool operator==(const SCommanderInfoResponseData& _val) const;
            /// \brief Ostream operator.
            friend std::ostream& operator<<(std::ostream& _os, const SCommanderInfoResponseData& _data);
        };

        /// \brief Structure holds information of a commanderInfo request.
        DDS_TOOLS_DECLARE_DATA_CLASS(SBaseRequestData, SCommanderInfoRequestData, "commanderInfo")

        /// \brief Request class of commanderInfo.
        using SCommanderInfoRequest = SBaseRequestImpl<SCommanderInfoRequestData, SCommanderInfoResponseData>;

        /// \brief Structure holds information of agentInfo response.
        struct SAgentInfoResponseData : SBaseResponseData<SAgentInfoResponseData>
        {
            uint32_t m_index{ 0 };                                                   ///< Index of the current agent
            uint64_t m_agentID{ 0 };                                                 ///< Agent ID
            std::chrono::milliseconds m_startUpTime{ std::chrono::milliseconds(0) }; ///< Agent's startup time
            std::string m_username;                                                  ///< Username
            std::string m_host;                                                      ///< Hostname
            std::string m_DDSPath;                                                   ///< DDS path
            uint32_t m_agentPid{ 0 };                                                ///< Agent's process ID
            uint32_t m_nSlots{ 0 };                                                  ///< Number of task slots

          private:
            friend SBaseData<SAgentInfoResponseData>;
            friend SBaseResponseData<SAgentInfoResponseData>;
            void _fromPT(const boost::property_tree::ptree& _pt);
            void _toPT(boost::property_tree::ptree& _pt) const;
            static constexpr const char* _protocolTag = "agentInfo";

          public:
            /// \brief Equality operator.
            bool operator==(const SAgentInfoResponseData& _val) const;
            /// \brief Ostream operator.
            friend std::ostream& operator<<(std::ostream& _os, const SAgentInfoResponseData& _data);
        };

        /// \brief Structure holds information of agentInfo request.
        DDS_TOOLS_DECLARE_DATA_CLASS(SBaseRequestData, SAgentInfoRequestData, "agentInfo")

        /// \brief Request class of agentInfo.
        using SAgentInfoRequest = SBaseRequestImpl<SAgentInfoRequestData, SAgentInfoResponseData>;

        /// \brief Structure holds information of agentCount response.
        struct SAgentCountResponseData : SBaseResponseData<SAgentCountResponseData>
        {
            uint32_t m_activeSlotsCount = 0;    ///< The number of online slots
            uint32_t m_idleSlotsCount = 0;      ///< The count of idle slots
            uint32_t m_executingSlotsCount = 0; ///< The count of executing slots

          private:
            friend SBaseData<SAgentCountResponseData>;
            friend SBaseResponseData<SAgentCountResponseData>;
            void _fromPT(const boost::property_tree::ptree& _pt);
            void _toPT(boost::property_tree::ptree& _pt) const;
            static constexpr const char* _protocolTag = "agentCount";

          public:
            /// \brief Equality operator.
            bool operator==(const SAgentCountResponseData& _val) const;
            /// \brief Ostream operator.
            friend std::ostream& operator<<(std::ostream& _os, const SAgentCountResponseData& _data);
        };

        /// \brief Structure holds information of agentCount response.
        DDS_TOOLS_DECLARE_DATA_CLASS(SBaseRequestData, SAgentCountRequestData, "agentCount")

        /// \brief Request class of agentCount.
        using SAgentCountRequest = SBaseRequestImpl<SAgentCountRequestData, SAgentCountResponseData>;

    } // namespace tools_api
} // namespace dds

#endif /* DDS_TOOLSPROTOCOL_H */

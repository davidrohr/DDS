// Copyright 2014 GSI, Inc. All rights reserved.
//
//
//
// DDS
#include "BOOSTHelper.h"
#include "DDSHelper.h"
#include "ErrorCode.h"
#include "Process.h"
#include "StatChannel.h"
#include "SysHelper.h"
#include "UserDefaults.h"

using namespace std;
using namespace MiscCommon;
using namespace dds;
using namespace dds::stat_cmd;
using namespace dds::user_defaults_api;
using boost::asio::ip::tcp;

//=============================================================================
int main(int argc, char* argv[])
{
    // Command line parser
    SOptions_t options;

    try
    {
        CUserDefaults::instance(); // Initialize user defaults
        Logger::instance().init(); // Initialize log

        vector<std::string> arguments(argv + 1, argv + argc);
        ostringstream ss;
        copy(arguments.begin(), arguments.end(), ostream_iterator<string>(ss, " "));
        LOG(info) << "Starting with arguments: " << ss.str();

        if (!ParseCmdLine(argc, argv, &options))
            return EXIT_SUCCESS;
    }
    catch (exception& e)
    {
        LOG(log_stderr) << e.what();
        return EXIT_FAILURE;
    }

    try
    {
        string sHost;
        string sPort;
        // We connect to UI commander channel.
        findCommanderUI(&sHost, &sPort);

        boost::asio::io_context io_context;

        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::asio::ip::tcp::resolver::query query(sHost, sPort);

        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

        CStatChannel::connectionPtr_t client = CStatChannel::makeNew(io_context, 0);
        client->setOptions(options);
        client->connect(iterator);

        io_context.run();
    }
    catch (exception& e)
    {
        LOG(error) << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

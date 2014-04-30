// Copyright 2014 GSI, Inc. All rights reserved.
//
//
//

#ifndef __DDS__Task__
#define __DDS__Task__

// DDS
#include "TopoElement.h"
#include "Port.h"
// STD
#include <string>
#include <memory>

namespace dds
{
    class CTask : public CTopoElement
    {
      public:
        /// \brief Constructor.
        CTask();

        /// \brief Destructor.
        virtual ~CTask();

        /// Accessors
        std::string getExec() const;
        size_t getNofPorts() const;
        PortPtr_t getPort(size_t _i) const;
        const PortPtrVector_t& getPorts() const;

        /// Modifiers
        void setExec(const std::string& _exec);
        void setPorts(const PortPtrVector_t& _ports);
        void addPort(PortPtr_t& _port);

        /// \brief If parent is a group than return N, else return 1.
        size_t getTotalCounter() const;

        /// \brief Inherited from DDSTopoElement.
        virtual size_t getNofTasks() const;

        /// \brief Inherited from DDSTopoElement.
        virtual size_t getTotalNofTasks() const;

        /// \brief Inherited from DDSTopoElement.
        virtual void initFromPropertyTree(const std::string& _name, const boost::property_tree::ptree& _pt);

        /// \brief Returns string representation of an object.
        /// \return String representation of an object.
        virtual std::string toString() const;

        /// \brief Operator << for convenient output to ostream.
        /// \return Insertion stream in order to be able to call a succession of
        /// insertion operations.
        friend std::ostream& operator<<(std::ostream& _strm, const CTask& _task);

      private:
        std::string m_exec;      ///> Path to executable
        PortPtrVector_t m_ports; ///> Ports
    };

    typedef std::shared_ptr<CTask> TaskPtr_t;
    typedef std::vector<TaskPtr_t> TaskPtrVector_t;
}
#endif /* defined(__DDS__Task__) */
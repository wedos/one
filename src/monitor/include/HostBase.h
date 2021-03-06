/* -------------------------------------------------------------------------- */
/* Copyright 2002-2019, OpenNebula Project, OpenNebula Systems                */
/*                                                                            */
/* Licensed under the Apache License, Version 2.0 (the "License"); you may    */
/* not use this file except in compliance with the License. You may obtain    */
/* a copy of the License at                                                   */
/*                                                                            */
/* http://www.apache.org/licenses/LICENSE-2.0                                 */
/*                                                                            */
/* Unless required by applicable law or agreed to in writing, software        */
/* distributed under the License is distributed on an "AS IS" BASIS,          */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   */
/* See the License for the specific language governing permissions and        */
/* limitations under the License.                                             */
/* -------------------------------------------------------------------------- */
#ifndef HOST_BASE_H_
#define HOST_BASE_H_

#include <set>
#include "BaseObject.h"
#include "ObjectCollection.h"
#include "Host.h"   // For HostState, can we moved it to reduce dependencies?


// Class storing Host data, it shouldn't contain any logic
// Scheduler, Monitor, oned should derive from this class
// to reduce amount of copy/pasted code
// Maybe it's not needed to derive, it could be used directly
// logic could be outside the class
class HostBase : public BaseObject, public ClusterableSingle
{
public:
    explicit HostBase(const std::string &xml_string)
        : BaseObject(xml_string)
        , ClusterableSingle(-1, "")
    {
        init_attributes();
    }

    explicit HostBase(const xmlNodePtr node)
        : BaseObject(node)
        , ClusterableSingle(-1, "")
    {
        init_attributes();
    }

    /**
     * Rebuilds the object from an xml formatted string
     * @param xml_str The xml-formatted string
     * @return 0 on success, -1 otherwise
     */
    int from_xml(const std::string &xml_str) override;

    /**
     * Print object to xml string
     *  @return xml formatted string
     */
    std::string to_xml() const override;

    int cluster_id() const
    {
        return ClusterableSingle::cluster_id;
    };

    Host::HostState state() const
    {
        return _state;
    }

    Host::HostState prev_state() const
    {
        return _prev_state;
    }

    /**
     * Retrieves VMM mad name
     *    @return string vmm mad name
     */
    const std::string& vmm_mad() const
    {
        return _vmm_mad;
    };

    /**
     * Retrieves IM mad name
     *    @return string im mad name
     */
    const std::string& im_mad() const
    {
        return _im_mad;
    };

    const std::set<int>& vm_ids() const
    {
        return _vm_ids.get_collection();
    }

    void vm_ids(const std::set<int>& ids);

    bool public_cloud() const
    {
        return _public_cloud;
    }

    time_t last_monitored() const { return _last_monitored; }

    void last_monitored(time_t lm) { _last_monitored = lm; }

    bool monitor_in_progress() const { return _monitor_in_progress; }

    void monitor_in_progress(bool monitoring) { _monitor_in_progress = monitoring; }

    /**
     *  Prints the Host information to an output stream. This function is used
     *  for logging purposes.
     */
    friend ostream& operator<<(ostream& o, const HostBase& host);

protected:
    int init_attributes();

private:
    Host::HostState _state      = Host::HostState::INIT;
    Host::HostState _prev_state = Host::HostState::INIT;

    std::string _vmm_mad;
    std::string _im_mad;

    bool _public_cloud = false;

    ObjectCollection _vm_ids{"VMS"};

    Template _obj_template;

    time_t   _last_monitored = 0;
    bool     _monitor_in_progress = false;
};

#endif // HOST_BASE_H_

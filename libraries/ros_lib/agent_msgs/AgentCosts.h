#ifndef _ROS_agent_msgs_AgentCosts_h
#define _ROS_agent_msgs_AgentCosts_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace agent_msgs
{

  class AgentCosts : public ros::Msg
  {
    public:
      typedef int16_t _agentID_type;
      _agentID_type agentID;
      typedef float _traversal_time_type;
      _traversal_time_type traversal_time;
      typedef float _policy_output_type;
      _policy_output_type policy_output;
      typedef bool _wait_type;
      _wait_type wait;

    AgentCosts():
      agentID(0),
      traversal_time(0),
      policy_output(0),
      wait(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_agentID;
      u_agentID.real = this->agentID;
      *(outbuffer + offset + 0) = (u_agentID.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_agentID.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->agentID);
      offset += serializeAvrFloat64(outbuffer + offset, this->traversal_time);
      offset += serializeAvrFloat64(outbuffer + offset, this->policy_output);
      union {
        bool real;
        uint8_t base;
      } u_wait;
      u_wait.real = this->wait;
      *(outbuffer + offset + 0) = (u_wait.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->wait);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_agentID;
      u_agentID.base = 0;
      u_agentID.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_agentID.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->agentID = u_agentID.real;
      offset += sizeof(this->agentID);
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->traversal_time));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->policy_output));
      union {
        bool real;
        uint8_t base;
      } u_wait;
      u_wait.base = 0;
      u_wait.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->wait = u_wait.real;
      offset += sizeof(this->wait);
     return offset;
    }

    const char * getType(){ return "agent_msgs/AgentCosts"; };
    const char * getMD5(){ return "c4b0d46a5d9bd4207f957171a0f1d9d6"; };

  };

}
#endif
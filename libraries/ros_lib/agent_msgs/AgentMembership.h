#ifndef _ROS_agent_msgs_AgentMembership_h
#define _ROS_agent_msgs_AgentMembership_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace agent_msgs
{

  class AgentMembership : public ros::Msg
  {
    public:
      typedef const char* _robot_name_type;
      _robot_name_type robot_name;
      typedef int16_t _parent_type;
      _parent_type parent;
      typedef int16_t _child_type;
      _child_type child;

    AgentMembership():
      robot_name(""),
      parent(0),
      child(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_robot_name = strlen(this->robot_name);
      varToArr(outbuffer + offset, length_robot_name);
      offset += 4;
      memcpy(outbuffer + offset, this->robot_name, length_robot_name);
      offset += length_robot_name;
      union {
        int16_t real;
        uint16_t base;
      } u_parent;
      u_parent.real = this->parent;
      *(outbuffer + offset + 0) = (u_parent.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_parent.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->parent);
      union {
        int16_t real;
        uint16_t base;
      } u_child;
      u_child.real = this->child;
      *(outbuffer + offset + 0) = (u_child.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_child.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->child);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_robot_name;
      arrToVar(length_robot_name, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_robot_name; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_robot_name-1]=0;
      this->robot_name = (char *)(inbuffer + offset-1);
      offset += length_robot_name;
      union {
        int16_t real;
        uint16_t base;
      } u_parent;
      u_parent.base = 0;
      u_parent.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_parent.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->parent = u_parent.real;
      offset += sizeof(this->parent);
      union {
        int16_t real;
        uint16_t base;
      } u_child;
      u_child.base = 0;
      u_child.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_child.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->child = u_child.real;
      offset += sizeof(this->child);
     return offset;
    }

    const char * getType(){ return "agent_msgs/AgentMembership"; };
    const char * getMD5(){ return "93e7816f0431fce6fde15c13c73acf23"; };

  };

}
#endif
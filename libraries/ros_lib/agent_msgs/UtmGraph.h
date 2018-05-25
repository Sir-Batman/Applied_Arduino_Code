#ifndef _ROS_agent_msgs_UtmGraph_h
#define _ROS_agent_msgs_UtmGraph_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace agent_msgs
{

  class UtmGraph : public ros::Msg
  {
    public:
      uint32_t actual_traversal_costs_length;
      typedef float _actual_traversal_costs_type;
      _actual_traversal_costs_type st_actual_traversal_costs;
      _actual_traversal_costs_type * actual_traversal_costs;
      uint32_t policy_output_costs_length;
      typedef float _policy_output_costs_type;
      _policy_output_costs_type st_policy_output_costs;
      _policy_output_costs_type * policy_output_costs;
      uint32_t wait_to_enter_length;
      typedef bool _wait_to_enter_type;
      _wait_to_enter_type st_wait_to_enter;
      _wait_to_enter_type * wait_to_enter;

    UtmGraph():
      actual_traversal_costs_length(0), actual_traversal_costs(NULL),
      policy_output_costs_length(0), policy_output_costs(NULL),
      wait_to_enter_length(0), wait_to_enter(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->actual_traversal_costs_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->actual_traversal_costs_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->actual_traversal_costs_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->actual_traversal_costs_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->actual_traversal_costs_length);
      for( uint32_t i = 0; i < actual_traversal_costs_length; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->actual_traversal_costs[i]);
      }
      *(outbuffer + offset + 0) = (this->policy_output_costs_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->policy_output_costs_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->policy_output_costs_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->policy_output_costs_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->policy_output_costs_length);
      for( uint32_t i = 0; i < policy_output_costs_length; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->policy_output_costs[i]);
      }
      *(outbuffer + offset + 0) = (this->wait_to_enter_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->wait_to_enter_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->wait_to_enter_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->wait_to_enter_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->wait_to_enter_length);
      for( uint32_t i = 0; i < wait_to_enter_length; i++){
      union {
        bool real;
        uint8_t base;
      } u_wait_to_enteri;
      u_wait_to_enteri.real = this->wait_to_enter[i];
      *(outbuffer + offset + 0) = (u_wait_to_enteri.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->wait_to_enter[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t actual_traversal_costs_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      actual_traversal_costs_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      actual_traversal_costs_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      actual_traversal_costs_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->actual_traversal_costs_length);
      if(actual_traversal_costs_lengthT > actual_traversal_costs_length)
        this->actual_traversal_costs = (float*)realloc(this->actual_traversal_costs, actual_traversal_costs_lengthT * sizeof(float));
      actual_traversal_costs_length = actual_traversal_costs_lengthT;
      for( uint32_t i = 0; i < actual_traversal_costs_length; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->st_actual_traversal_costs));
        memcpy( &(this->actual_traversal_costs[i]), &(this->st_actual_traversal_costs), sizeof(float));
      }
      uint32_t policy_output_costs_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      policy_output_costs_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      policy_output_costs_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      policy_output_costs_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->policy_output_costs_length);
      if(policy_output_costs_lengthT > policy_output_costs_length)
        this->policy_output_costs = (float*)realloc(this->policy_output_costs, policy_output_costs_lengthT * sizeof(float));
      policy_output_costs_length = policy_output_costs_lengthT;
      for( uint32_t i = 0; i < policy_output_costs_length; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->st_policy_output_costs));
        memcpy( &(this->policy_output_costs[i]), &(this->st_policy_output_costs), sizeof(float));
      }
      uint32_t wait_to_enter_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      wait_to_enter_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      wait_to_enter_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      wait_to_enter_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->wait_to_enter_length);
      if(wait_to_enter_lengthT > wait_to_enter_length)
        this->wait_to_enter = (bool*)realloc(this->wait_to_enter, wait_to_enter_lengthT * sizeof(bool));
      wait_to_enter_length = wait_to_enter_lengthT;
      for( uint32_t i = 0; i < wait_to_enter_length; i++){
      union {
        bool real;
        uint8_t base;
      } u_st_wait_to_enter;
      u_st_wait_to_enter.base = 0;
      u_st_wait_to_enter.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->st_wait_to_enter = u_st_wait_to_enter.real;
      offset += sizeof(this->st_wait_to_enter);
        memcpy( &(this->wait_to_enter[i]), &(this->st_wait_to_enter), sizeof(bool));
      }
     return offset;
    }

    const char * getType(){ return "agent_msgs/UtmGraph"; };
    const char * getMD5(){ return "78ae73cbbca6891c9d4edc4deb980bba"; };

  };

}
#endif
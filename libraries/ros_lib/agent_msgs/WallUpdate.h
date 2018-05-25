#ifndef _ROS_agent_msgs_WallUpdate_h
#define _ROS_agent_msgs_WallUpdate_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace agent_msgs
{

  class WallUpdate : public ros::Msg
  {
    public:
      typedef int16_t _total_changes_type;
      _total_changes_type total_changes;
      uint32_t type_length;
      typedef bool _type_type;
      _type_type st_type;
      _type_type * type;
      uint32_t data_length;
      typedef float _data_type;
      _data_type st_data;
      _data_type * data;

    WallUpdate():
      total_changes(0),
      type_length(0), type(NULL),
      data_length(0), data(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_total_changes;
      u_total_changes.real = this->total_changes;
      *(outbuffer + offset + 0) = (u_total_changes.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_total_changes.base >> (8 * 1)) & 0xFF;
      offset += sizeof(this->total_changes);
      *(outbuffer + offset + 0) = (this->type_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->type_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->type_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->type_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->type_length);
      for( uint32_t i = 0; i < type_length; i++){
      union {
        bool real;
        uint8_t base;
      } u_typei;
      u_typei.real = this->type[i];
      *(outbuffer + offset + 0) = (u_typei.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->type[i]);
      }
      *(outbuffer + offset + 0) = (this->data_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->data_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->data_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->data_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->data_length);
      for( uint32_t i = 0; i < data_length; i++){
      union {
        float real;
        uint32_t base;
      } u_datai;
      u_datai.real = this->data[i];
      *(outbuffer + offset + 0) = (u_datai.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_datai.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_datai.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_datai.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->data[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        int16_t real;
        uint16_t base;
      } u_total_changes;
      u_total_changes.base = 0;
      u_total_changes.base |= ((uint16_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_total_changes.base |= ((uint16_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->total_changes = u_total_changes.real;
      offset += sizeof(this->total_changes);
      uint32_t type_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      type_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      type_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      type_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->type_length);
      if(type_lengthT > type_length)
        this->type = (bool*)realloc(this->type, type_lengthT * sizeof(bool));
      type_length = type_lengthT;
      for( uint32_t i = 0; i < type_length; i++){
      union {
        bool real;
        uint8_t base;
      } u_st_type;
      u_st_type.base = 0;
      u_st_type.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->st_type = u_st_type.real;
      offset += sizeof(this->st_type);
        memcpy( &(this->type[i]), &(this->st_type), sizeof(bool));
      }
      uint32_t data_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      data_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      data_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      data_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->data_length);
      if(data_lengthT > data_length)
        this->data = (float*)realloc(this->data, data_lengthT * sizeof(float));
      data_length = data_lengthT;
      for( uint32_t i = 0; i < data_length; i++){
      union {
        float real;
        uint32_t base;
      } u_st_data;
      u_st_data.base = 0;
      u_st_data.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_data.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_data.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_data.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_data = u_st_data.real;
      offset += sizeof(this->st_data);
        memcpy( &(this->data[i]), &(this->st_data), sizeof(float));
      }
     return offset;
    }

    const char * getType(){ return "agent_msgs/WallUpdate"; };
    const char * getMD5(){ return "18635044d2c4d46e35eadd410b80e465"; };

  };

}
#endif
// -*- mode: c++ -*-
/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2014, JSK Lab
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the JSK Lab nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/


#ifndef JSK_PCL_ROS_UTILS_COLORIZE_DISTANCE_FROM_PLANE_H_
#define JSK_PCL_ROS_UTILS_COLORIZE_DISTANCE_FROM_PLANE_H_

#include <pcl_ros/pcl_nodelet.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/synchronizer.h>

#include <sensor_msgs/PointCloud2.h>
#include <jsk_recognition_msgs/ClusterPointIndices.h>
#include <jsk_recognition_msgs/ModelCoefficientsArray.h>
#include <jsk_recognition_msgs/PolygonArray.h>
#include <dynamic_reconfigure/server.h>
#include <jsk_pcl_ros_utils/ColorizeDistanceFromPlaneConfig.h>
#include "jsk_recognition_utils/geo_util.h"
#include "jsk_topic_tools/connection_based_nodelet.h"

namespace jsk_pcl_ros_utils
{
  class ColorizeDistanceFromPlane: public jsk_topic_tools::ConnectionBasedNodelet
  {
  public:
    typedef pcl::PointXYZRGB PointT;
    typedef boost::shared_ptr<ColorizeDistanceFromPlane> Ptr;
    typedef message_filters::sync_policies::ExactTime<
      sensor_msgs::PointCloud2,
      jsk_recognition_msgs::ModelCoefficientsArray,
      jsk_recognition_msgs::PolygonArray
      > SyncPolicy;
    typedef ColorizeDistanceFromPlaneConfig Config;
    virtual ~ColorizeDistanceFromPlane();
  protected:
    ////////////////////////////////////////////////////////
    // methods
    ////////////////////////////////////////////////////////
    virtual void onInit();
    
    virtual void colorize(const sensor_msgs::PointCloud2::ConstPtr& cloud,
                          const jsk_recognition_msgs::ModelCoefficientsArray::ConstPtr& coefficients,
                          const jsk_recognition_msgs::PolygonArray::ConstPtr& polygons);

    virtual double distanceToConvexes(
      const PointT& p, const std::vector<jsk_recognition_utils::ConvexPolygon::Ptr>& convexes);
    
    virtual uint32_t colorForDistance(const double d);
    
    virtual void configCallback(Config &config, uint32_t level);

    virtual void subscribe();
    virtual void unsubscribe();
    
    ////////////////////////////////////////////////////////
    // ROS variabels
    ////////////////////////////////////////////////////////
    ros::Publisher pub_;
    boost::shared_ptr<message_filters::Synchronizer<SyncPolicy> >sync_;
    message_filters::Subscriber<sensor_msgs::PointCloud2> sub_input_;
    message_filters::Subscriber<jsk_recognition_msgs::ModelCoefficientsArray> sub_coefficients_;
    message_filters::Subscriber<jsk_recognition_msgs::PolygonArray> sub_polygons_;
    boost::shared_ptr <dynamic_reconfigure::Server<Config> > srv_;
    boost::mutex mutex_;
    
    ////////////////////////////////////////////////////////
    // varibales to configure colorization
    ////////////////////////////////////////////////////////
    double max_distance_;
    double min_distance_;
    bool only_projectable_;
    
  private:
    
  };
}

#endif

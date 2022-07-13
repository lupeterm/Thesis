/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2017, PickNik Consulting
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
 *   * Neither the name of the Univ of CO, Boulder nor the names of its
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

/* Author: Dave Coleman
   Desc:   Demo implementation of rviz_visual_tools
           To use, add a Rviz Marker Display subscribed to topic /rviz_visual_tools
*/

// ROS
#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
// For visualizing things in rviz
#include <rviz_visual_tools/rviz_visual_tools.h>

// C++
#include <string>
#include <vector>
#include <boost/foreach.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include <algorithm>

// RSPD
#include "RSPD/point.h"
#include "RSPD/pointcloud.h"
#include "RSPD/planedetector.h"
#include "RSPD/normalestimator.h"
#include "RSPD/boundaryvolumehierarchy.h"
#include "RSPD/connectivitygraph.h"

// OPS
#include "OPS/Ops.h"

// 3DKHT
#include "3DKHT/octree_t.h"
#include "3DKHT/settings.h"
#include "3DKHT/plane_t.h"
#include "3DKHT/hough.h"
#include "3DKHT/accumulatorball_t.h"

namespace rvt = rviz_visual_tools;
typedef pcl::PointCloud<pcl::PointXYZRGBNormal> PCLPointCloud;

enum ALGO
{
    RSPD,
    OPS
};

namespace rviz_visual_tools
{

    class PlaneDetectionHub
    {
    private:
        // A shared node handle
        ros::NodeHandle nh_;

        // For visualizing things in rviz
        rvt::RvizVisualToolsPtr visual_tools_;
        std::string name_;

    public:
        ros::Subscriber sub;
        hough_settings settings;

        /**
         * \brief Constructor
         */
        PlaneDetectionHub(std::string alg, const std::string subTopic, const std::string frame) : name_("PlaneDetection")
        {
            visual_tools_.reset(new rvt::RvizVisualTools(frame, "/plane_visualization"));
            visual_tools_->loadMarkerPub(); // create publisher before waiting
            if (alg == "rspd")
            {
                sub = nh_.subscribe(subTopic, 1000, &PlaneDetectionHub::callback_RSPD, this);
            }
            if (alg == "ops")
            {
                sub = nh_.subscribe(subTopic, 1000, &PlaneDetectionHub::callback_OPS, this);
            }
            if (alg == "3dkht")
            {
                sub = nh_.subscribe(subTopic, 1000, &PlaneDetectionHub::callback_3DKHT, this);
            }
            // Clear messages
            visual_tools_->deleteAllMarkers();
            visual_tools_->enableBatchPublishing();
        }

        void callback_3DKHT(const PCLPointCloud::ConstPtr &msg)
        {
            ROS_INFO("Received MapCloud");
            octree_t father;
            accumulatorball_t *accum;

            father.m_middle = Vector4d(0, 0, 0);
            father.m_level = 0;
            father.m_root = &father;
            std::vector<plane_t> planes_out;

            double max_distance = 0.0;
            int point_num = 0;
            double mix, miy, miz, max, may, maz;

            // add points to octree
            BOOST_FOREACH (const pcl::PointXYZRGBNormal &pt, msg->points)
            {
                Vector4d point, color(160, 160, 160);
                point.x = pt.x;
                point.y = pt.y;
                point.z = pt.z;
                color.x = pt.r;
                color.y = pt.g;
                color.z = pt.b;
                father.m_points.push_back(point);
                father.m_centroid += point;
                father.m_indexes.push_back(point_num++);
                father.m_colors.push_back(color / 255.0);
                mix = std::min(mix, point.x);
                miy = std::min(miy, point.y);
                miz = std::min(miz, point.z);
                max = std::max(max, point.x);
                may = std::max(may, point.y);
                maz = std::max(maz, point.z);
            }
            Vector4d centroid = father.m_centroid / father.m_points.size();
            father.m_centroid = Vector4d();
            father.m_size = max_distance * 2.0;
            auto t_start = std::chrono::high_resolution_clock::now();
            accum = kht3d(planes_out, father, settings);
            auto t_elasped = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t_start).count() / 1000.0;
            std::cout << "Hough Transform:                Total [" << t_elasped << "]   " << 1.0 / t_elasped << " fps\n";
            std::cout << "3-D KHT ended: " << planes_out.size() << " planes found" << std::endl;
        }

        void callback_OPS(const pcl::PointCloudXYZ::Ptr &msg)
        {
            ROS_INFO("Received MapCloud");
            std::vector<OPSPlane> planes;
            planes = process(msg); // [[inliers], (normal, centroid))]
            geometry_msgs::Vector3 scale;
            scale.x = 0.02;
            scale.y = 0.02;
            scale.z = 0.02;
            visual_tools_->deleteAllMarkers();

            // for (auto p : planes)
            for (size_t i = 0; i < planes.size(); i++)
            {

                std::vector<geometry_msgs::Point> ps;
                for (auto in : planes[i].first)
                {
                    geometry_msgs::Point point;
                    point.x = in.x;
                    point.y = in.y;
                    point.z = in.z;
                    ps.push_back(point);
                }
                visual_tools_->publishSpheres(ps, visual_tools_->intToRvizColor(2), scale);
            }
            visual_tools_->trigger();
        }

        void callback_RSPD(const PCLPointCloud::ConstPtr &msg)
        {
            ROS_INFO("Received MapCloud");
            Eigen::Isometry3d pose1 = Eigen::Isometry3d::Identity();
            double x_width = 0.15;
            double y_width = 0.05;

            std::vector<Point3d> points;
            BOOST_FOREACH (const pcl::PointXYZRGBNormal &pt, msg->points)
            {
                points.push_back(Point3d(Eigen::Vector3f(pt.x, pt.y, pt.z)));
            }
            PointCloud3d *pointCloud = new PointCloud3d(points);
            ROS_INFO("Number of samples in Mapcloud: %zu", pointCloud->size());

            // ROS_INFO("Estimating normals..");
            size_t normalsNeighborSize = 30;
            Octree octree(pointCloud);
            octree.partition(10, 30);
            ConnectivityGraph *connectivity = new ConnectivityGraph(pointCloud->size());
            pointCloud->connectivity(connectivity);
            NormalEstimator3d estimator(&octree, normalsNeighborSize, NormalEstimator3d::QUICK);
            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

            for (size_t i = 0; i < pointCloud->size(); i++)
            {
                if (i % 10000 == 0)
                {
                    std::cout << i / float(pointCloud->size()) * 100 << "%..." << std::endl;
                }
                NormalEstimator3d::Normal normal = estimator.estimate(i);
                connectivity->addNode(i, normal.neighbors);
                (*pointCloud)[i].normal(normal.normal);
                (*pointCloud)[i].normalConfidence(normal.confidence);
                (*pointCloud)[i].curvature(normal.curvature);
            }
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

            std::cout << "Normal Estimation took " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
            ROS_INFO("Detecting planes..");

            PlaneDetector detector(pointCloud);
            detector.minNormalDiff(0.5f);
            detector.maxDist(0.258819f);
            detector.outlierRatio(0.75f);
            std::set<Plane *> planes = detector.detect();
            ROS_INFO("Found %zu planes!", planes.size());
            ROS_INFO("Publishing results to Rviz...");

            visual_tools_->deleteAllMarkers();
            Eigen::Vector3f v1, v2, v3, v4;
            for (Plane *plane : planes)
            {
                v1 = plane->center() + plane->basisU() + plane->basisV();
                v2 = plane->center() + plane->basisU() - plane->basisV();
                v3 = plane->center() - plane->basisU() + plane->basisV();
                v4 = plane->center() - plane->basisU() - plane->basisV();
                geometry_msgs::Polygon poly;
                poly.points.push_back(eigen_to_point(v1));
                poly.points.push_back(eigen_to_point(v2));
                poly.points.push_back(eigen_to_point(v4));
                poly.points.push_back(eigen_to_point(v3));
                visual_tools_->publishPolygon(poly);
            }
            visual_tools_->trigger();
            delete pointCloud;
        }
        geometry_msgs::Point32 eigen_to_point(Eigen::Vector3f e)
        {
            geometry_msgs::Point32 p;
            p.x = e[0];
            p.y = e[1];
            p.z = e[2];
            return p;
        }

        void publishLabelHelper(const Eigen::Isometry3d &pose, const std::string &label)
        {
            Eigen::Isometry3d pose_copy = pose;
            pose_copy.translation().x() -= 0.2;
            visual_tools_->publishText(pose_copy, label, rvt::WHITE, rvt::XXLARGE, false);
        }

        void showplane(double a, double b, double c, double d)
        {
            Eigen::Isometry3d pose1 = Eigen::Isometry3d::Identity();
            double x_width = 0.15;
            double y_width = 0.05;

            int step = 1.5;

            visual_tools_->publishABCDPlane(a, b, c, d, rvt::MAGENTA, x_width, y_width);
            visual_tools_->trigger();
        }
    };
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "visual_tools_demo");
    ROS_INFO_STREAM("Plane Detection");
    std::string topic, frame, algo;
    std::cout << "#arguments: " << argc << std::endl;
    topic = argv[1];
    std::cout << "setting Topic to " << topic << std::endl;
    frame = argv[2];
    std::cout << "setting Frame to " << frame << std::endl;
    algo = argv[3];
    std::cout << "setting Algorithm to " << algo << std::endl;
    rviz_visual_tools::PlaneDetectionHub demo(algo, topic, frame);
    ros::spin();

    ROS_INFO_STREAM("Shutting down.");

    return 0;
}

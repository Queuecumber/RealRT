#pragma once

#include "Shape.hpp"
#include <opencv2/opencv.hpp>

namespace RealRT
{

    class Sphere : public Shape
    {
    public:
        Sphere(MATERIAL<> mat = BLANK, const cv::Vec3d &center = cv::Vec3d::zeros(3), double radius = 2.5);
        ~Sphere(void);

        double Intersect(const Ray &incident, bool &flipNormals) const;
        cv::Vec3d Normal(const cv::Point3d &pt) const;

        double Radius(void) const;
        cv::Vec3d Center(void) const;

    private:
        cv::Point3d _Center;
        double _Radius;
    };

}

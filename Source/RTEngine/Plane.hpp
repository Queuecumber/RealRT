#pragma once
#include "Shape.hpp"

#include <opencv2/opencv.hpp>

namespace RealRT
{

    class Plane : public Shape
    {
    public:
        Plane(MATERIAL<> mat = MATERIAL<>(), const cv::Vec3d &normal = cv::Vec3d(0.0, 0.0, 0.0), double d = 5);

        cv::Vec3d Normal(const cv::Point3d &pt) const;

        double Intersect(const Ray &incident, bool &flipNormals) const;

        bool IsLight(void) const;

        ~Plane(void);

    private:
        cv::Vec3d _Normal;
        double _D;
    };

}

#pragma once
#include <opencv2/opencv.hpp>

namespace RealRT
{

    class Ray
    {
    public:
        Ray(void);
        Ray(const cv::Point3d &origin, const cv::Vec3d &direction);
        Ray(const Ray &cp);

        cv::Point3d Origin(void) const;
        cv::Vec3d Direction(void) const;

        ~Ray(void);

    private:
        cv::Point3d _Origin;
        cv::Vec3d _Direction;
    };

}

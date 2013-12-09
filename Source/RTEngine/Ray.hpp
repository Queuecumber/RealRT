#pragma once

namespace RealRT
{
    class Vector3D;

    class Ray
    {
        public:
            Ray(void) = default;
            Ray(const Vector3D &origin, const Vector3D &direction);
            Ray(const Ray &cp);

            Vector3D Origin(void) const;
            Vector3D Direction(void) const;

        private:
            Vector3D _Origin;
            Vector3D _Direction = {0.f, 0.f, 1.f};
    };

}

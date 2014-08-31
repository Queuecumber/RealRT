#pragma once
#include <memory>

namespace RealRT
{
    class Material;
    class Vector3D;

    class Light
    {
        public:
            Light(std::shared_ptr<const Material> mat);
            virtual ~Light(void) = default;

            virtual Vector3D Position(void) const = 0;

            const std::shared_ptr<const Material> LightMaterial(void) const;

        private:
            std::shared_ptr<const Material> _Material;
    };
}

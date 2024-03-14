#include "utils.h"
#include "Vec3.h"
#include "Hittable.h"

class Quad : public Hittable {
    public:
        Quad(const Point3& q_, const Vec3& u_, const Vec3& v_, std::shared_ptr<Material> mat)
            : q{q_}, u{u_}, v{v_}, norm{ unit_vector(cross(u, v)) }, material{mat} {}
        
        bool hit(const Ray& r, double t_min, double t_max, HitRecord& re) const override;

    private:
        Point3 q;
        Vec3 u;
        Vec3 v;
        Vec3 norm;
        std::shared_ptr<Material> material;
};

// hittable_list.hpp
#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include "hittable.hpp"
#include <vector>
#include <memory>

class HittableList : public Hittable {
public:
    std::vector<std::shared_ptr<Hittable>> objects;

    HittableList();
    HittableList(std::shared_ptr<Hittable> object);

    void clear();
    void add(std::shared_ptr<Hittable> object);

    bool hit(const Ray& r, interval ray_t, HitRecord& rec) const override;
};

#endif // HITTABLE_LIST_HPP
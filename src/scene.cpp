#include <scene.h>
#include <random>

void Scene::GenerateRandomScene() {
    const size_t sphereNmb = 128;
    spheres_.clear();
    materials_.clear();
    spheres_.resize(sphereNmb);
    materials_.resize(sphereNmb);
    std::random_device rd;
    std::mt19937 g(rd());

    const std::uniform_real_distribution<float> xyDist{-20.0f, 20.0f};
    const std::uniform_real_distribution<float> zDist{-5.0f, -100.0f};
    const std::uniform_real_distribution<float> radiusDist{1.0f, 3.0f};
    const std::uniform_real_distribution<float> dist{0.0f, 1.0f};

    const std::uniform_int_distribution<int> boolDist{0, 1};
    for(size_t i = 0; i < sphereNmb; i++)
    {
        Sphere sphere;

        sphere.center = Vec3f(xyDist(g), xyDist(g), zDist(g));
        sphere.radius = radiusDist(g);
        spheres_[i] = sphere;

        Material material;
        material.baseColor = Color (dist(g), dist(g), dist(g));
        if(boolDist(g))
            material.emissionColor = material.baseColor;
        material.reflection = boolDist(g);

        materials_[i] = material;

    }
}

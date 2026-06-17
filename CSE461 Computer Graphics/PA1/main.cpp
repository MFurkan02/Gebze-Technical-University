// Includes
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <sstream>
#include <chrono>
#include <ctime>  
#include "pugi/pugixml.hpp" // For XML parsing

// Project-specific includes
#include "AmbientLight.h"
#include "Color.h"
#include "Camera.h"
#include "TriangularLight.h"
#include "Material.h"
#include "Mesh.h"
#include "PointLight.h"
#include "Ray.h"
#include "Light.h"
#include "Hittable.h"
#include "HittableObjs.h"
#include "Vector3.h"

// For image output
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

// Texture and Normal structure definitions
struct Texture {
    std::vector<std::tuple<float, float>> coords;
    std::string imagePath;
};

struct Normal {
    std::vector<Vector3> normals;
};

// Global mutex for multithreading synchronization
std::mutex mtx;

// Struct holding world data: scene objects, lights, background color, and settings
struct World {
    HittableObjs objects;
    Color background;
    std::vector<shared_ptr<Light>> lights;
    shared_ptr<Light> ambientLight;
    int maxRayDepth;
};

// Attach a material to a hittable object based on its ID
void addMatterial(Hittable & hittable, std::string id, const std::vector<shared_ptr<Material>> &material_list) {
    for (const auto & material : material_list) {
        if (id == material->material_id) {
            hittable.material = material;
            return;
        }
    }
    std::cerr << "Material not found for id: " << id << std::endl;
    hittable.material = nullptr;
}

// Recursive function for ray shading including ambient, direct light, and mirror reflection
Color shade_ray(const Ray& r, int depth, const World& world) {
    if (depth <= 0)
        return Color(0, 0, 0);

    Hit_record rec;
    bool is_hit = world.objects.hit(r, 0, infinity, rec);

    if (is_hit) {
        Color c = world.ambientLight->illuminate(r, rec);

        for (auto light : world.lights) {
            Vector3 to_light = (light->dir(rec.p));
            auto distance = to_light.length();
            to_light = unit_vector(to_light);

            double epsilon = 1e-7;
            Hit_record shadow_rec;
            bool is_shadow = world.objects.hit(Ray(rec.p, to_light), epsilon, distance, shadow_rec);

            if (!is_shadow) {
                c += light->illuminate(r, rec);
                fix_color(c);

                // Mirror reflection
                Color km = rec.hit_surface_p->material->mirrorReflection;
                if (!(km.x() == 0 && km.y() == 0 && km.z() == 0)) {
                    Vector3 reflectRay_dir = reflect(-to_light, rec.normal);
                    Ray reflectRay = Ray(rec.p + epsilon * reflectRay_dir, reflectRay_dir);
                    Color reflectColor = shade_ray(reflectRay, depth - 1, world) * km;
                    c = 0.75 * c + 0.25 * reflectColor;
                }
            }
        }

        return c;
    }

    // If no hit, return background color
    return world.background;
}

// Worker function for each thread to compute pixel colors in a specific image chunk
void process_chunk(int start, int end, int ny, int nx, const Camera& camera, const World& world, std::vector<std::vector<Color>>& pixel_colors) {
    for (int j = start; j < end; j++) {
        for (int i = nx - 1; i > 0; i--) {
            double left = camera.get_left();
            double right = camera.get_rightPlane();
            double top = camera.get_top();
            double bottom = camera.get_bottom();

            auto u = left + (right - left) * (i + 0.5) / nx;
            auto v = bottom + (top - bottom) * (j + 0.5) / ny;
            Vector3 ray_o = camera.get_position();
            Vector3 ray_d = (u * camera.get_u()) + (v * camera.get_v()) - camera.get_w() * camera.neardistance();
            Ray r = Ray(ray_o, ray_d);
            Color pixel_color = shade_ray(r, world.maxRayDepth, world);

            // Synchronize writing to shared pixel buffer
            std::lock_guard<std::mutex> lock(mtx);
            pixel_colors[j][i] = pixel_color;
        }
    }

    // Logging thread completion
    std::cout << "Thread " << std::this_thread::get_id() << " finished processing chunk from " << start << " to " << end << "\n";	
}

// Utility function: Converts space-separated string to Vector3
Vector3 stringToVector(const std::string& str) {
    std::istringstream stream(str);
    double x, y, z;
    stream >> x >> y >> z;
    return Vector3(x, y, z);
}

// XML parsing functions

// Parse a PointLight node from XML
shared_ptr<PointLight> parsePointLight(const pugi::xml_node& lightNode) {
    auto pos = stringToVector(lightNode.child("position").text().as_string());
    auto intensity = stringToVector(lightNode.child("intensity").text().as_string());
    return make_shared<PointLight>(PointLight::fromIntensity(pos, intensity));
}

// Parse a TriangularLight node from XML
shared_ptr<TriangularLight> parseTriLight(const pugi::xml_node& lightNode) {
    auto v1 = stringToVector(lightNode.child("vertex1").text().as_string());
    auto v2 = stringToVector(lightNode.child("vertex2").text().as_string());
    auto v3 = stringToVector(lightNode.child("vertex3").text().as_string());
    auto intensity = stringToVector(lightNode.child("intensity").text().as_string());
    return make_shared<TriangularLight>(TriangularLight::fromTriangle(v1, v2, v3, intensity));
}

// Parse a Material node from XML
shared_ptr<Material> parseMaterial(const pugi::xml_node& materialNode) {
    std::string id = materialNode.attribute("id").as_string();
    auto ka = stringToVector(materialNode.child("ambient").text().as_string());
    auto kd = stringToVector(materialNode.child("diffuse").text().as_string());
    auto ks = stringToVector(materialNode.child("specular").text().as_string());
    auto km = stringToVector(materialNode.child("mirrorreflactance").text().as_string());
    double ph = materialNode.child("phongexponent").text().as_double();
    double tf = materialNode.child("texturefactor").text().as_double();
    return make_shared<Material>(Material(id, ka, kd, ks, km, ph, tf));
}

// Parse a single vertex array from XML
void parseVertex(const pugi::xml_node& vertexNode, std::vector<Vector3>& vertices) {
    std::string str = vertexNode.text().as_string();
    std::istringstream stream(str);
    double x, y, z;
    while (stream >> x >> y >> z) {
        vertices.push_back(Vector3(x, y, z));
    }
}

// Parse a Mesh from XML using global vertex list
shared_ptr<Mesh> parseMesh(const pugi::xml_node& objectNode, const std::vector<Vector3>& vertices) {
    std::vector<std::vector<int>> faces;
    std::string str = objectNode.child("faces").text().as_string();
    std::istringstream stream(str);
    int x, y, z;
    while (stream >> x >> y >> z) {
        faces.push_back(std::vector<int>({x, y, z}));
    }
    return make_shared<Mesh>(Mesh(vertices, faces));
}

// Save the rendered image to disk as a PNG file
void save_as_png(const std::string& filename, const std::vector<std::vector<Color>>& pixel_colors, int width, int height) {
    std::vector<unsigned char> image_data(width * height * 3); // RGB

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            int index = (j * width + i) * 3;
            const Color& pixel = pixel_colors[j][i];
            image_data[index + 0] = static_cast<unsigned char>(255.999 * pixel.x());
            image_data[index + 1] = static_cast<unsigned char>(255.999 * pixel.y());
            image_data[index + 2] = static_cast<unsigned char>(255.999 * pixel.z());
        }
    }

    // Use stb_image_write to export image
    if (stbi_write_png(filename.c_str(), width, height, 3, image_data.data(), width * 3)) {
        std::clog << "Image saved as " << filename << "\n";
    } else {
        std::cerr << "Failed to save image as " << filename << "\n";
    }
}

int main() {
    // === Load XML Scene ===
    World world;
    std::string filename = "scene2.xml";
    pugi::xml_document doc;
    if (!doc.load_file(filename.c_str())) {
        std::cerr << "Failed to load XML file: " << filename << std::endl;
        return 1;
    }

    // === Scene Settings ===
    world.maxRayDepth = doc.child("scene").child("maxraytracedepth").text().as_int();

    // Parse background color (R G B format) and normalize
    world.background = stringToVector(doc.child("scene").child("backgroundColor").text().as_string());
    world.background.set_x(world.background.x() / 255.0);
    world.background.set_y(world.background.y() / 255.0);
    world.background.set_z(world.background.z() / 255.0);

    // === Camera Settings ===
    Vector3 pos = stringToVector(doc.child("scene").child("camera").child("position").text().as_string());
    Vector3 gaze = stringToVector(doc.child("scene").child("camera").child("gaze").text().as_string());
    Vector3 up = stringToVector(doc.child("scene").child("camera").child("up").text().as_string());

    double l, r, b, t, nearDist;
    std::istringstream np(doc.child("scene").child("camera").child("nearPlane").text().as_string());
    np >> l >> r >> b >> t;
    nearDist = doc.child("scene").child("camera").child("neardistance").text().as_double();

    double resX, resY;
    std::istringstream resStream(doc.child("scene").child("camera").child("imageresolution").text().as_string());
    resStream >> resX >> resY;

    Camera camera(pos, gaze, up, l, r, b, t, nearDist, resX, resY);

    // === Light Setup ===
    world.ambientLight = std::make_shared<AmbientLight>(AmbientLight::from_rgb(
        stringToVector(doc.child("scene").child("lights").child("ambientlight").text().as_string())
    ));

    for (auto lightNode : doc.child("scene").child("lights").children()) {
        std::string name = lightNode.name();
        if (name == "pointlight")
            world.lights.push_back(parsePointLight(lightNode));
        else if (name == "triangularlight")
            world.lights.push_back(parseTriLight(lightNode));
    }

    // === Materials ===
    std::vector<std::shared_ptr<Material>> material_list;
    for (auto matNode : doc.child("scene").child("materials").children())
        material_list.push_back(parseMaterial(matNode));

    // === Vertex Data ===
    std::vector<Vector3> vertices;
    parseVertex(doc.child("scene").child("vertexdata"), vertices);

    // === Texture Data ===
    Texture texture;
    for (auto coordNode : doc.child("scene").child("texturedata").children()) {
        std::istringstream cs(coordNode.text().as_string());
        float u, v;
        cs >> u >> v;
        texture.coords.emplace_back(u, v);
    }
    texture.imagePath = doc.child("scene").child("textureimage").text().as_string();

    // === Normal Data ===
    Normal normalData;
    for (auto normalNode : doc.child("scene").child("normaldata").children()) {
        std::istringstream ns(normalNode.text().as_string());
        double x, y, z;
        ns >> x >> y >> z;
        normalData.normals.emplace_back(x, y, z);
    }

    // === Objects (Meshes) ===
    for (auto objNode : doc.child("scene").child("objects").children()) {
        auto material_id = objNode.child("materialid").text().as_string();
        auto mesh = parseMesh(objNode, vertices);
        addMatterial(*mesh, material_id, material_list);
        world.objects.add(mesh);
    }

    // === Rendering ===
    int nx = camera.get_nx(), ny = camera.get_ny();
    std::vector<std::vector<Color>> pixel_colors(ny + 1, std::vector<Color>(nx + 1));

    std::clog << "Rendering " << nx << " x " << ny << " with max ray depth " << world.maxRayDepth << "\n";
    auto start_time = std::chrono::system_clock::now();

    unsigned int num_threads = std::thread::hardware_concurrency();
    int chunk_size = ny / num_threads;

    std::vector<std::thread> threads;
    for (int t = 0; t < num_threads; ++t) {
        int start = t * chunk_size;
        int end = (t == num_threads - 1) ? ny : (t + 1) * chunk_size;
        threads.emplace_back(process_chunk, start, end, ny, nx, std::ref(camera), std::ref(world), std::ref(pixel_colors));
    }

    for (auto& thread : threads) thread.join();

    auto end_time = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::clog << "Rendering finished in " << elapsed.count() << "s\n";

    // === Output Image ===
    for (int j = 0; j < ny; ++j) {
        for (int i = nx; i > 0; --i) {
            std::lock_guard<std::mutex> lock(mtx);
            write_color(std::cout, pixel_colors[j][i]);
        }
        std::cout << "\n";
    }

    save_as_png("output.png", pixel_colors, nx, ny);
    std::clog << "Image saved as output.png\n";

    return 0;
}

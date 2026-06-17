CXX = g++
CXXFLAGS = -std=c++17 -O2

# List source files and header files
SRCS = main.cpp 
HDRS = Ray.h Vector3.h Material.h Hittable.h HittableObjs.h Light.h PointLight.h AmbientLight.h TriangularLight.h Camera.h Color.h Mesh.h 

# Libraries to link with
LIBS = -lpugixml

# Target executable
raytracer: $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) $(SRCS) $(LIBS) -o raytracer

# Clean up compiled files
clean:
	rm -f raytracer *.ppm

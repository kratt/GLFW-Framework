//Author: Sören Pirk
//Date: 22.01.2013

#ifndef SPLINE
#define SPLINE

#include <vector>
#include <glm/vec3.hpp>

class Spline
{
public:
    enum Config
    {
        CATMULL_ROM = 0,
        CUBIC, 
        HERMITE,
        PICEWISE_HERMITE,
        COSINE,
        LINEAR,
        KOCHANEK_BARTEL,
        ROUNDED_CATMULL_ROM,
        BSPLINE
    };
    
    Spline(Config conf = CATMULL_ROM);
   ~Spline();

   void addPoint(const glm::vec3 &v);
   void clear();
   glm::vec3 interpolatedPoint(float t, Config conf = CATMULL_ROM);
   glm::vec3 point(int n) const;
   int numPoints() const;
   void render(Config conf = CATMULL_ROM);

   void bounds(int &p);

   static glm::vec3 linearInterpolation(const glm::vec3 &p0, const glm::vec3 &p1, float t);
   static glm::vec3 catmullRomInterpolation(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, float t);
   static glm::vec3 roundedCatmullRomInterpolation(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, 
                                                 float t);
   static glm::vec3 cubicInterpolation(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, float t);
   static glm::vec3 bSplineInterpolation(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3, const glm::vec3 &p4, double t);

   static glm::vec3 hermiteInterpolation(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3,
	   float t, float tension = 0.0, float bias = 0.0);

   static glm::vec3 picewiseHermiteInterpolation(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &startTangent, 
                                               const glm::vec3 &endTangent, float t);
   static glm::vec3 kochanekBartelInterpolation(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c, const glm::vec3 &d, 
                                              float t, float tension = 0.0, float bias = 0.0, float continuity = 0.0f);
   std::vector<glm::vec3> m_points;

   glm::vec3 m_phantomStart;
   glm::vec3 m_phantomEnd;

private:    
    float m_deltaT;
    Config m_config;
};

#endif


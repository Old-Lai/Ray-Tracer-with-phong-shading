#pragma once

#include "ofMain.h"
#include <glm/gtx/intersect.hpp>
#include "ofxGui.h"

//-------------------- From Project 2 starter file --------------------
class Ray {
public:
   Ray(glm::vec3 p, glm::vec3 d) { this->p = p; this->d = d; }
   void draw(float t) { ofDrawLine(p, p + t * d); }

   glm::vec3 evalPoint(float t) {
      return (p + t * d);
   }

   glm::vec3 p, d;
};

class SceneObject {
public:
   virtual void draw() = 0;
   virtual bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) { cout << "SceneObject::intersect" << endl; return false; }

   glm::vec3 position = glm::vec3(0, 0, 0);

   ofColor diffuseColor = ofColor::grey;
   ofColor specularColor = ofColor::lightGray;

//   vector<glm::vec3> points;

   //keep count of if object is textured
   bool textured = false;
   virtual ofColor getDiffFromTexture(float u, float v){}
   virtual ofColor getSpecFromTexture(float u, float v){}
   virtual void loadTexture(string diffDir, string specDir){}
   ofImage diffTexture, specTexture;
};

class Sphere: public SceneObject {
public:
   Sphere(glm::vec3 p, float r, ofColor diffuse = ofColor::lightGray, bool textured = false) { position = p; radius = r; diffuseColor = diffuse; this->textured = textured;}
   Sphere() {}
   bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
      return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
   }
   void draw()  {
      ofDrawSphere(position, radius);
   }

   float radius = 1.0;
};

//class Triangle{
//public:
//   //stores triangles as 3 vertices per triangle
//   int vertInd[3];
//   Triangle(int i1, int i2, int i3) {vertInd[0] = i1, vertInd[1] = i2, vertInd[2] = i3;}
//};
//
//class Mesh : public SceneObject {
//public:
//   vector<glm::vec3> vertices;
//   vector<Triangle> triangles;
//
//   Mesh(glm::vec3 p, string fileDir, ofColor diffuse = ofColor::cyan){
//      position = p;
//      diffuseColor = diffuse;
//      readMesh(fileDir);
//   }
//
//   bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
//      float dist;
//      glm::vec2 baryPos;
//      bool hit;
//      for(int i = 0; i < triangles.size(); i++){
//         hit = glm::intersectRayTriangle(ray.p, ray.d, vertices[triangles[i].vertInd[0]], vertices[triangles[i].vertInd[1]], vertices[triangles[i].vertInd[2]], baryPos, dist);
//         if(hit){
//            glm::vec3 edge1 = vertices[triangles[i].vertInd[1]]-vertices[triangles[i].vertInd[0]], edge2 = vertices[triangles[i].vertInd[2]]-vertices[triangles[i].vertInd[0]];
////            point = vertices[triangles[i].vertInd[0]] + glm::normalize(edge1) * baryPos.x + glm::normalize(edge2) * baryPos.y;
//            Ray r = ray;
//            point = r.evalPoint(dist);
//            points.push_back(point);
//            float nx, ny, nz;
//            nx = (edge1.y * edge2.z) - (edge1.z * edge2.y);
//            ny = (edge1.z * edge2.x) - (edge1.x * edge2.z);
//            nz = (edge1.x * edge2.y) - (edge1.y * edge2.x);
//            glm::vec3 faceNorm(nx, ny, nz), test(0,1,0);
//            cout << glm::normalize(faceNorm) << endl;
//            normal = glm::normalize(faceNorm);
//         }
//      }
//      return hit;
//   }
//   void draw() {
//      for (int i = 0; i < triangles.size(); i++){
//         int x = triangles[i].vertInd[0];
//         int y = triangles[i].vertInd[1];
//         int z = triangles[i].vertInd[2];
//         ofSetColor(diffuseColor);
//         ofNoFill();
//         ofDrawTriangle(vertices[x], vertices[y], vertices[z]);
//      }
//   }
//
//   void readMesh(string fileDir);
//};

class Plane: public SceneObject {
public:
   int scale = 8;
   Plane(glm::vec3 p, glm::vec3 n, ofColor diffuse = ofColor::darkOliveGreen, float w = 20, float h = 20) {
      position = p; normal = n;
      width = w;
      height = h;
      plane.rotateDeg(90, 1, 0, 0);
      diffuseColor = diffuse;
   }
   Plane() { }
   glm::vec3 normal = glm::vec3(0, 1, 0);
   bool intersect(const Ray &ray, glm::vec3 & point, glm::vec3 & normal);
   void draw() {
      plane.setPosition(position);
      plane.setWidth(width);
      plane.setHeight(height);
      plane.setResolution(4, 4);
      plane.drawWireframe();
   }
   ofPlanePrimitive plane;
   float width = 20;
   float height = 20;

   void loadTexture(string diffDir, string specDir){
      diffTexture.load(diffDir.c_str());
      specTexture.load(specDir.c_str());
      textured = true;
   }

   ofColor getDiffFromTexture(float u, float v){
      u = u / scale;
      v = v / scale;
//      cout << u << "   " << v << endl;
      int nx = diffTexture.getWidth();
      int ny = diffTexture.getHeight();
//      cout << nx << "   " << ny << endl;
      int i;
      int j;
      if(u >= 0)
         i = fmod(u * nx - 0.5, nx);
      else
         i = nx - fmod(-u * nx - 0.5, nx);
      if(v >= 0)
         j = fmod(v * ny - 0.5, ny);
      else
         j = ny - fmod(-v * ny - 0.5, ny);
//      cout << "i: " << i << "  j: " << j << endl;
//      cout << texture.getColor(0, 0) << endl;
      return diffTexture.getColor(i,j);
   }

   ofColor getSpecFromTexture(float u, float v){
      u = u / scale;
      v = v / scale;

      int nx = specTexture.getWidth();
      int ny = specTexture.getHeight();

      int i;
      int j;
      if(u > 0)
         i = fmod(u * nx - 0.5, nx);
      else
         i = nx - fmod(-u * nx - 0.5, nx);
      if(v > 0)
         j = fmod(v * ny - 0.5, ny);
      else
         j = ny - fmod(-v * ny - 0.5, ny);

      return specTexture.getColor(i,j);
   }
};

class  ViewPlane: public Plane {
public:
   ViewPlane(glm::vec2 p0, glm::vec2 p1) { min = p0; max = p1; }

   ViewPlane() {
      min = glm::vec2(-3, -2);
      max = glm::vec2(3, 2);
      position = glm::vec3(0, 0, 5);
      normal = glm::vec3(0, 0, 1);
   }

   void setSize(glm::vec2 min, glm::vec2 max) { this->min = min; this->max = max; }
   float getAspect() { return width() / height(); }

   glm::vec3 toWorld(float u, float v);

   void draw() {
      ofDrawRectangle(glm::vec3(min.x, min.y, position.z), width(), height());
   }


   float width() {
      return (max.x - min.x);
   }
   float height() {
      return (max.y - min.y);
   }

   glm::vec2 topLeft() { return glm::vec2(min.x, max.y); }
   glm::vec2 topRight() { return max; }
   glm::vec2 bottomLeft() { return min;  }
   glm::vec2 bottomRight() { return glm::vec2(max.x, min.y); }

   glm::vec2 min, max;
};

class RenderCam: public SceneObject {
public:
   RenderCam() {
      position = glm::vec3(0, 0, 10);
      aim = glm::vec3(0, 0, -1);
   }
   Ray getRay(float u, float v);
   void draw() { ofDrawBox(position, 1.0); };
   void drawFrustum();

   glm::vec3 aim;
   ViewPlane view;
};

//-------------------- End of project 2 starter file --------------------

class Light: public SceneObject {
public:
   float intensity = 1;

   Light(glm::vec3 p, float intensity = 1, ofColor diffuse = ofColor::white) {
      position = p;
      this->intensity = intensity;
      diffuseColor = diffuse;
   }
   Light() {}

   void draw() { ofDrawSphere(position, 0.1); }
};

class AreaLight: public SceneObject {
public:
   float intensity = 1;
   float ambientIntensity = 3;
   vector<Light> lightArr;

   AreaLight(glm::vec3 positionOffset, float intensity = 1, ofColor diffuse = ofColor::white) {
      position = positionOffset;
      this->intensity = intensity;
      diffuseColor = diffuse;

      lightArr.push_back(Light(position)); //placeholder befor file load
   }

   void draw() {
      for(int i = 0; i < lightArr.size(); i++){
         ofSetColor(diffuseColor);
         ofDrawSphere(lightArr[i].position, 0.1);
      }
   }

   void updateIntensity(){
      //this spreads the intensity to all lights so the endresult is an area
      //light with the wanted itensity
      for(int i = 0; i < lightArr.size(); i++){
         lightArr[i].intensity = intensity / lightArr.size();
      }
   }
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

      void progressBar(float progress, int &prevPos);
      void rayTrace();

      void readFile(string fileDir);

      bool bShowImage = false;

      ofEasyCam  mainCam;
      ofCamera previewCam;
      ofCamera  *theCam;

      RenderCam renderCam;

      vector<SceneObject *> scene;

      vector<AreaLight> lights;
      int selectedLightIndx;

      //Shader
      ofColor phong(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse,
                 const ofColor specular, float power);
      Boolean inShadow(Ray r);

      ofImage image;
      int imageWidth = 1200;
      int imageHeight = 800;

      ofImage diffTexture, specTexture;

      ofxPanel gui;
      ofxIntSlider powerSlider, modelSelectSlider;
      ofxFloatSlider lightIntensitySlider, phongCoeffSlider, lambertCoeffSlider, ambiCoeffSlider;
};

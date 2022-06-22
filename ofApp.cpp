#include "ofApp.h"

//-------------------- From Project 2 --------------------
bool Plane::intersect(const Ray &ray, glm::vec3 & point, glm::vec3 & normalAtIntersect) {
   float dist;
   bool hit = glm::intersectRayPlane(ray.p, ray.d, position, this->normal, dist);
   if (hit) {
      Ray r = ray;
      point = r.evalPoint(dist);
      normalAtIntersect = this->normal;
   }
   return (hit);
}

glm::vec3 ViewPlane::toWorld(float u, float v) {
   float w = width();
   float h = height();
   return (glm::vec3((u * w) + min.x, (v * h) + min.y, position.z));
}

Ray RenderCam::getRay(float u, float v) {
   glm::vec3 pointOnPlane = view.toWorld(u, v);
   return(Ray(position, glm::normalize(pointOnPlane - position)));
}
//-------------------------------------------------------

//--------------------------------------------------------------
void ofApp::setup(){
   theCam = &mainCam;
   previewCam.setPosition(renderCam.position);
   previewCam.lookAt(renderCam.aim);
   mainCam.setDistance(30);
   mainCam.lookAt(glm::vec3(0,0,0));
   ofSetBackgroundColor(ofColor::black);

   ofEnableDepthTest();

   scene.push_back(new Plane(glm::vec3(0,-1,0), glm::vec3(0,1,0), ofColor::lawnGreen));
   scene.push_back(new Sphere(glm::vec3(-1,-0.5,0), 1, ofColor::lightSkyBlue));
   scene.push_back(new Sphere(glm::vec3(-1,0.8,0), 0.6, ofColor::lightSkyBlue));
   scene.push_back(new Sphere(glm::vec3(-1,1.5,0), 0.3, ofColor::lightSkyBlue));
   scene.push_back(new Sphere(glm::vec3(-1,1.45,0.3), 0.05, ofColor::lightSalmon));
   scene.push_back(new Sphere(glm::vec3(-1.1,1.55,0.28), 0.02, ofColor::lightGray));
   scene.push_back(new Sphere(glm::vec3(-0.9,1.55,0.28), 0.02, ofColor::lightGray));
   scene.push_back(new Sphere(glm::vec3(-1,0.7,0.63), 0.1, ofColor::gray));
   scene.push_back(new Sphere(glm::vec3(-1,1,0.59), 0.1, ofColor::gray));

   scene.push_back(new Sphere(glm::vec3(-1.6,1,0), 0.2, ofColor::gray));
   scene.push_back(new Sphere(glm::vec3(-1.8,1.025,0), 0.2, ofColor::gray));
   scene.push_back(new Sphere(glm::vec3(-2,1.2,0), 0.2, ofColor::gray));
   scene.push_back(new Sphere(glm::vec3(-2.1,1.4,0), 0.2, ofColor::gray));

   scene.push_back(new Sphere(glm::vec3(-0.4,1,0), 0.2, ofColor::gray));
   scene.push_back(new Sphere(glm::vec3(-0.2,0.975,0), 0.2, ofColor::gray));
   scene.push_back(new Sphere(glm::vec3(0,0.8,0), 0.2, ofColor::gray));
   scene.push_back(new Sphere(glm::vec3(0.1,0.6,0), 0.2, ofColor::gray));
//   scene.push_back(new Mesh(glm::vec3(0,-1,0), "/Users/henrylai/SynologyDrive/of_v0.11.2_osx_release/apps/myApps/Project2-Part3/bin/data/squareplane.obj"));

   selectedLightIndx = 0;
   AreaLight a1(glm::vec3(6,3,0), 3), a2(glm::vec3(0,3,0), 3), a3(glm::vec3(0,3,0), 3);
   lights.push_back(a1);
   lights.push_back(a2);
   lights.push_back(a3);

   //allocateing storage for image
   image.allocate(imageWidth, imageHeight, ofImageType::OF_IMAGE_COLOR);

   gui.setup();
   gui.add(modelSelectSlider.setup("Select models", 1, 1, 3));
   gui.add(powerSlider.setup("Phong Power", 30, 2, 100));
   gui.add(lightIntensitySlider.setup("Area light intensity", 3, 0, 8));
   gui.add(phongCoeffSlider.setup("Specular coefficient", 2, 0, 10));
   gui.add(lambertCoeffSlider.setup("Diffuse coefficient", 0.35, 0, 2));
   gui.add(ambiCoeffSlider.setup("Ambient coefficient", 1, 0, 10));

   scene[0]->loadTexture("16_Street Rounded cobble PBR texture_DIFF.jpg", "16_Street Rounded cobble PBR texture_SPEC.jpg");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
   theCam->begin();
   //drawing scene objects
   for(int i = 0; i < scene.size(); i++){
      ofSetColor(scene[i]->diffuseColor);
      scene[i]->draw();
   }

//   for(int i = 0; i < scene[1]->points.size(); i++)
//      ofDrawCircle(scene[1]->points[i], 0.01);
   //drawing lights as small spheres
   lights[selectedLightIndx].draw();
   //draw render camera
   ofSetColor(ofColor::white);
   renderCam.draw();

   theCam->end();
   //for image display on screen
   if(bShowImage){
      image.load("Output.png");
      image.draw(glm::vec3(0,0,0));
   }
   //draw gui
   ofDisableDepthTest();
   gui.draw();
   ofEnableDepthTest();
   //update selected model to slider value
   selectedLightIndx = modelSelectSlider - 1;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
   switch(key){
      case OF_KEY_F1:
         theCam = &mainCam;
         break;
      case OF_KEY_F2:
         theCam = &previewCam;
         break;
      case OF_KEY_F3:
         rayTrace();
         break;
      case OF_KEY_F4:
         bShowImage = !bShowImage;
         break;
      default:
         break;
   }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
   readFile(dragInfo.files[0]);
}

//--------------------------------------------------------------
void ofApp::progressBar(float progress, int &prevPos){
   //just a method to output a progressbar in console
   int barWidth = 50;
   int pos = barWidth * progress;

   if(pos != prevPos){
      cout << "[";
      for(int i = 0; i < barWidth; i++) {
         if(i < pos)
            cout << "=";
         else if(i == pos)
            cout << ">";
         else
            cout << " ";
      }
      cout << "]" << int(progress * 100) << "%\r";
      cout.flush();
   }

   prevPos = pos;
}

//--------------------------------------------------------------
void ofApp::rayTrace(){
   //update light intensity to slider value
   lights[selectedLightIndx].intensity = lightIntensitySlider;
   lights[selectedLightIndx].updateIntensity();

   float viewWidth = renderCam.view.width();
   float viewHeight = renderCam.view.height();
   float widthIncrament = 1.0 / imageWidth;
   float heightIncrament = 1.0 / imageHeight;
   bool hit = false;
   int closestObject;
   glm::vec3 closestIntersectPt, tempIntersectPt;
   glm::vec3 closestIntersectNorm, tempIntersectNorm;

   Ray renderRay(glm::vec3(0,0,0), glm::vec3(0,0,0)); //placeholder

   //progress bar variables
   float progress = 0.0;
   int previousPos = -1;

   //loop through each pixel
   for(int i = 1; i <= imageWidth; i++){
      for(int j = 1; j <= imageHeight; j++){
         //get ray for each pixel from the camera
         renderRay = renderCam.getRay(widthIncrament * (i-1) , heightIncrament * (j-1));
         //loop though scene object to check intersection
         for(int k = 0; k < scene.size(); k++){
            //find closest object
            if(scene[k]->intersect(renderRay, tempIntersectPt, tempIntersectNorm) && hit == false){
               closestObject = k;
               closestIntersectPt = tempIntersectPt;
               closestIntersectNorm = tempIntersectNorm;
               hit = true;
            }
            else if(scene[k]->intersect(renderRay, tempIntersectPt, tempIntersectNorm) && hit == true){
               if(glm::length(renderCam.position - tempIntersectPt) <= glm::length(renderCam.position - closestIntersectPt)){
                  closestObject = k;
                  closestIntersectPt = tempIntersectPt;
                  closestIntersectNorm = tempIntersectNorm;
               }
            }
         }

         //after figured out closest object, get the color of the object where ray is hit
         if(hit){
            ofColor diffuseCol;
            ofColor spectralCol;
            if(scene[closestObject]->textured){
               diffuseCol = scene[closestObject]->getDiffFromTexture(closestIntersectPt.x, closestIntersectPt.z);
               spectralCol = scene[closestObject]->getSpecFromTexture(closestIntersectPt.x, closestIntersectPt.z);
            }else{
               diffuseCol = scene[closestObject]->diffuseColor;
               spectralCol = scene[closestObject]->specularColor;
            }

            ofColor pShading = phong(closestIntersectPt, closestIntersectNorm, diffuseCol, spectralCol, powerSlider);
            image.setColor(i, imageHeight - j, pShading);

            hit = false;
         }
         else
            //backgroun color
            image.setColor(i, imageHeight - j, ofColor::black);
      }
      //Progress Bar updater -----------------
      progress += 1.0/imageWidth;
      if((i+1) == imageWidth)
         progress = 1;
      progressBar(progress, previousPos);
      //--------------------------------------
   }

   //save image
   image.save("Output.png");
}

//--------------------------------------------------------------
ofColor ofApp::phong(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse,
                     const ofColor specular, float power){
   ofColor retColor = 0;
   glm::vec3 lightPos;
   float lambertVal = 0;
   float phongVal = 0;
   float ambiVal = lights[selectedLightIndx].ambientIntensity;
   float lightIntensity;
   float r , tempVal, specCoeff = phongCoeffSlider, diffCoeff = lambertCoeffSlider, ambiCoeff = ambiCoeffSlider;

   for(int i = 0; i < lights[selectedLightIndx].lightArr.size(); i++){
      lightPos = lights[selectedLightIndx].lightArr[i].position;
      lightIntensity = lights[selectedLightIndx].lightArr[i].intensity;
      //lifted point away from surface for a small amount
      Ray pointToLight(p + (0.001 * norm), glm::normalize(lightPos - p));

      if(!inShadow(pointToLight)){
         r = glm::length(lightPos - p);
         //cosAngle part of lambart
         tempVal = glm::dot(norm, lightPos);
         //max between dot value and 0
         if(0 > tempVal)
            tempVal = 0;
         //lambart shading
         lambertVal += (diffCoeff * ((lightIntensity/(r*r)) * tempVal));

         //max between 0 and dot value part of phong
         tempVal = glm::dot(norm, glm::normalize((lightPos - p) + (renderCam.position - p)));
         if(0 > tempVal)
            tempVal = 0;

         //phong shading
         phongVal += (specCoeff * (lightIntensity/(r * r)) * glm::pow(tempVal, power));
         //ambient lighting
      }
   }
   //cout << lambertVal;
   retColor = diffuse * lambertVal;
   retColor += specular * phongVal;
   retColor += ambiCoeff * ambiVal;

   return retColor;
}

//--------------------------------------------------------------
Boolean ofApp::inShadow(Ray r){
   glm::vec3 p, n;
   for(int i = 0; i < scene.size(); i++){
      if(scene[i]->intersect(r, p, n))
         return true;
   }
   return false;
}

//--------------------------------------------------------------
void ofApp::readFile(string fileDir){
   ifstream file;
   file.open(fileDir.c_str());

   if(file.is_open()){
      string line, subStr;
      //clear vectors in selected arealight, prep for new data insertion.
      lights[selectedLightIndx].lightArr.clear();

      while (getline(file, line)) {
         //skipping empty lines
         if(line != "")
         {
            stringstream str(line);
            vector<string> tokens;

            while (getline(str, subStr, ' ')) {
               tokens.push_back(subStr);
            }

            if (tokens[0] == "v") {
               float v1 = stof(tokens[1]);
               float v2 = stof(tokens[2]);
               float v3 = stof(tokens[3]);
               //update position with offset for the selected area light.
               lights[selectedLightIndx].lightArr.push_back(Light((glm::vec3(v1, v2, v3) + lights[selectedLightIndx].position)));
            }
         }
      }

      file.close();
   } else {
      cout << "file not found" << endl;
   }
}

//void Mesh::readMesh(string fileDir){
//   ifstream file;
//   file.open(fileDir.c_str());
//
//   static const float shiftX = position.x, shiftY = position.y, shiftZ = position.z;
//
//   if (file.is_open()) {
//      string line, subStr;
//
//      vertices.clear();
//      triangles.clear();
//
//      cout << "file opened" << endl;
//
//      while (getline(file, line)) {
//         if(line != "")
//         {
//            stringstream str(line);
//            vector<string> tokens;
//
//            while (getline(str, subStr, ' ')) {
//               tokens.push_back(subStr);
//            }
//
//            if (tokens[0] == "v") {
//               float v1 = stof(tokens[1]);
//               float v2 = stof(tokens[2]);
//               float v3 = stof(tokens[3]);
//
//               vertices.push_back(glm::vec3(v1 + shiftX, v2 + shiftY, v3 + shiftZ));
//
//            } else if (tokens[0] == "f") {
//               stringstream ss;
//
//               int indx1 = tokens[1].find('/');
//               int indx2 = tokens[2].find('/');
//               int indx3 = tokens[3].find('/');
//
//               if(indx1 < 0) indx1 = tokens[1].length();
//               if(indx2 < 0) indx2 = tokens[2].length();
//               if(indx3 < 0) indx3 = tokens[3].length();
//
//               indx1 = stoi(tokens[1].substr(0, indx1));
//               indx2 = stoi(tokens[2].substr(0, indx2));
//               indx3 = stoi(tokens[3].substr(0, indx3));
//
//               triangles.push_back(Triangle(indx1 - 1, indx2 - 1, indx3 - 1));
//            }
//         }
//      }
//      file.close();
//   } else {
//      cout << "file not found" << endl;
//   }
//}



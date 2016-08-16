#pragma once

#include "glm/glm.hpp"

using namespace glm;
class Math{
public:
	static double pointLineSegmentDistance(vec2 p, vec2 v, vec2 w, vec2 &direction) {
		  // Return minimum distance between line segment vw and point p
		  const double l2 = glm::length(w-v) * glm::length(w-v);  // i.e. |w-v|^2 -  avoid a sqrt
		  if (l2 == 0.0){
		      direction = v - p;
		      return glm::distance(p, v);   // v == w case
		  }
		  // Consider the line extending the segment, parameterized as v + t (w - v).
		  // We find projection of point p onto the line.
		  // It falls where t = [(p-v) . (w-v)] / |w-v|^2
		  const double t = glm::dot(p - v, w - v) / l2;
		  if (t < 0.0){
		      direction = v - p;
		      return glm::distance(p, v); // Beyond the 'v' end of the segment
		  }
		  else if (t > 1.0){
		      direction = w - p;
		      return glm::distance(p, w);  // Beyond the 'w' end of the segment
		  }
		  const vec2 projection = v + (vec2(t,t) * (w - v));  // Projection falls on the segment
		  direction = projection - p;
		  return glm::distance(p, projection);
	}


};
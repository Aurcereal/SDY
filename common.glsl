// Math
float smin(float d1, float d2, float k) {
	float d = abs(d1-d2)/k;
	float w = -pow(max(0.0, 1.-d), 2.);
	float h = 0.25 * k * w;
	return min(d1, d2) + h;
}

// SDFs
float sdBox(vec3 p, vec3 dim) {
	p = abs(p) - dim*.5;
	float s = sqrt(max(p.x, 0.0)*p.x + max(p.y, 0.0)*p.y + max(p.z, 0.0)*p.z);
	return step(s, 0) * max(p.x, max(p.y, p.z)) + s;
}

// https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
vec2 rayBoxIntersect(vec3 ro, vec3 ird, vec3 dim) {
	vec3 lb = -dim*.5;
	vec3 rt = dim*.5;

	float t1 = (lb.x - ro.x) * ird.x;
	float t2 = (rt.x - ro.x) * ird.x;
	float t3 = (lb.y - ro.y) * ird.y;
	float t4 = (rt.y - ro.y) * ird.y;
	float t5 = (lb.z - ro.z) * ird.z;
	float t6 = (rt.z - ro.z) * ird.z;

	float tMin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tMax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	return vec2(tMin, tMax);
}
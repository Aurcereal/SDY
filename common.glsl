// SDFs
float sdBox(vec3 p, vec3 dim) {
	p = abs(p) - dim*.5;
	float s = sqrt(max(p.x, 0.0)*p.x + max(p.y, 0.0)*p.y + max(p.z, 0.0)*p.z);
	return step(s, 0) * max(p.x, max(p.y, p.z)) + s;
}
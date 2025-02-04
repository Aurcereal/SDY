float trace(vec3 ro, vec3 rd, vec2 searchBounds) {
	float sd, d = searchBounds.x;

	for(int i=0; i<MAXSTEPS; i++) {
		sd = sdf(ro+rd*d);
		d += sd;

		if(abs(sd) <= SDEPS) {
			return d;
		}

		if(sd >= searchBounds.y) {
			return MAXDIST;
		}
	}

	return MAXDIST;
}
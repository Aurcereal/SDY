float trace(vec3 ro, vec3 rd) {
	float sd, d = 0.0;

	for(int i=0; i<MAXSTEPS; i++) {
		sd = sdf(ro+rd*d);
		d += sd;

		if(abs(sd) <= SDEPS || d >= MAXDIST) {
			return d;
		}
	}

	return MAXDIST;
}
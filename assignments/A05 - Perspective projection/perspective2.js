function perspective(w, h, fov) {
	var a = w / h;
	var f = 100;
	var n = 0.1;
	var tanFovHalved = Math.tan(Math.atan(1)*4*fov/180/2);
	// Build a perspective projection matrix, for a viewport whose size is determined by parameters w (width) and h (height), and whose fov-y is passed in parameter fov. Near plane is n=0.1, and far plane f=100.
	var out = [
		1/(a * tanFovHalved), 0, 0, 0,
        0, 1/tanFovHalved, 0, 0,
        0, 0, (f+n)/(n-f), 2*f*n/(n-f),
        0, 0, -1, 0
	];

	return out;
}


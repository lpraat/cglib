function perspective() {
	// Build a perspective projection matrix, for a 16/9 viewport, with fov-y=90, near plane n=0.1, and far plane f=100.
	var out = [
		0.5625, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1.002, -0.2002,
		0, 0, -1, 0
	];

	return out;
}
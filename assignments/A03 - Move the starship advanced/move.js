function move() {
	// Rotate 30 degrees around an arbitrary axis passing through (1,1,0). The x-axis can be aligned to the arbitrary axis after a rotation of 15 degrees around the z-axis, and then 45 degrees around the y-axis.
	var R1 = [0.928525, 0.36519, 0.0669096, -0.293715,
		-0.317823, 0.875, -0.36519, 0.442823,
		-0.19191, 0.317823, 0.928525, -0.125913,
		0, 0, 0, 1,];

	// Double the size of an object, using as fixed point (1,1,0)
	var S1 = [
		2, 0, 0, -1,
		0, 2, 0, -1,
		0, 0, 2, 0,
		0, 0, 0, 1
	];

	// Mirror the starship along a plane passing through (1,2,0), and obtained rotating 38 degree around the y axis the xy plane
	var S2 = [
		0.241922, 0, -0.970296, 0.758078,
		0, 1, 0, 0,
		-0.970296, 0, -0.241922, 0.970296,
		0, 0, 0, 1
	];

	// The ship has been doubled in size, rotated 45 degrees around the x axis, 30 degrees around the y axis, and moved to (1,1,-2). Return the ship in its original position
	var I1 = [
		0.433013, 0, -0.25, -0.933013,
		0.176777, 0.353553, 0.306186, 0.0820423,
		0.176777, -0.353553, 0.306186, 0.789149,
		0, 0, 0, 1,];

	return [R1, S1, S2, I1];
}


function perspective() {
    // Build a perspective projection matrix, for a 16/9 viewport, with fov-y=90, near plane n=0.1, and far plane f=100.

    // having two screen you now have the width which is doubled so the ratio is 32/9
    // the left screen goes from left to 0 while the right one from 0 to right

    // given slide 36 in perspective matrix you can now find the new first row of the perspective matrix
    // left: 2n/-l 0 -1 0
    // right: 2n/r 0 1 0

    // 2n/-l == 2n/r = 2 / (32/9 * math.tan((90 * math.atan(1)*4) / 180 / 2)) == 0.5625
	var out = [
    0.5625, 0, 1, 0,
		0, 1, 0, 0,
		0, 0, -1.002, -0.2002,
		0, 0, -1, 0
	];

	return out;
}
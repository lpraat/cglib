function move() {
    // Translate of +3 on the x axis, and -5 on the z axis
    // glp::translate(3, 0, -5)
    var T1 =  [1.0,		0.0,		0.0,		3.0,
               0.0,		1.0,		0.0,		0.0,
               0.0,		0.0,		1.0,		-5.0,
               0.0,		0.0,		0.0,		1.0];

    // Rotate of 30 degrees on the y axis
    // glp::rotateY(-30.0f)
    var R1 =  [0.866025,		0.0,		-0.5,		0.0,
               0.0,		1.0,		0.0,		0.0,
               0.5,		0.0,		0.866025,		0.0,
               0.0,		0.0,		0.0,		1.0];

    // Make the starship 2 times bigger
    // glp::scale(2.0f)
    var S1 =  [2.0,		0.0,		0.0,		0.0,
               0.0,		2.0,		0.0,		0.0,
               0.0,		0.0,		2.0,		0.0,
               0.0,		0.0,		0.0,		1.0];

    // Make the starship 1.5 times longer on the x axis, and half on the other axis
    // glp::scale(1.5f, 0.5f, 0.5f);
    var S2 =  [1.5,		0.0,		0.0,		0.0,
               0.0,		0.5,		0.0,		0.0,
               0.0,		0.0,		0.5,		0.0,
               0.0,		0.0,		0.0,		1.0];

    // Mirror over the yz plane
    // glp::planarMirrorYZ();
    var S3 =  [-1.0,		0.0,		0.0,		0.0,
               0.0,		1.0,		0.0,		0.0,
               0.0,		0.0,		1.0,		0.0,
               0.0,		0.0,		0.0,		1.0];

    // Flatten over the zx plane
    // glp::scale(1.0f, 0.0f, 1.0f);
    var S4 =  [1.0,		0.0,		0.0,		0.0,
               0.0,		0.0,		0.0,		0.0,
               0.0,		0.0,		1.0,		0.0,
               0.0,		0.0,		0.0,		1.0];

    // Make a shear along the x axis, with a factor of 1 along the y axis
    // glp::shearX(1.0f, 0.0f).print();
    var H1 =  [1.0,		0.0,		0.0,		0.0,
               1.0,		1.0,		0.0,		0.0,
               0.0,		0.0,		1.0,		0.0,
               0.0,		0.0,		0.0,		1.0];

    return [T1, R1, S1, S2, S3, S4, H1];
}


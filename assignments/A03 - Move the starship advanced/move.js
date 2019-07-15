function move() {
    // Rotate 30 degrees around an arbitrary axis passing through (1,1,0). The x-axis can be aligned to the arbitrary axis after a rotation of 15 degrees around the z-axis, and then 45 degrees around the y-axis.
       // std::vector<glp::Mat4<float32>> matrices {
       //      glp::translate(1.0f, 1.0f, 0.0f), // Resets axis position
       //      glp::rotateY(45.0f),        // Resets axis direction
       //      glp::rotateZ(15.0f),        // Resets axis direction
       //      glp::rotateX(30.0f),       // Rotation
       //      glp::rotateZ(-15.0f),      // aligns axis with x
       //      glp::rotateY(-45.0f),      // aligns axis with x
       //      glp::translate(-1.0f, -1.0f, 0.0f) // bring axis to origin
    // };

    // glp::compose(matrices).print();
    var R1 = [
        0.928525, 0.36519, 0.0669096, -0.293715,
        -0.317823, 0.875, -0.36519, 0.442823,
        -0.19191, 0.317823, 0.928525, -0.125913,
        0, 0, 0, 1,];

    // Double the size of an object, using as fixed point (1,1,0)
    //     std::vector<glp::Mat4<float32>> matrices {
        // glp::translate(1.0f, 1.0f, 0.0f),
        // glp::scale(2.0f),
        // glp::translate(-1.0f, -1.0f, 0.0f)
    // };

    // glp::compose(matrices).print();
    var S1 = [
        2, 0, 0, -1,
        0, 2, 0, -1,
        0, 0, 2, 0,
        0, 0, 0, 1
    ];

    // Mirror the starship along a plane passing through (1,2,0), and obtained rotating 38 degree around the y axis the xy plane
        // glp::translate(1.0f, 2.0f, 0.0f),
        // glp::rotateY(38.0f),
        // glp::planarMirrorXY(),
        // glp::rotateY(-38.0f),
        // glp::translate(-1.0f, -2.0f, 0.0f)
    // };

    // glp::compose(matrices).print();
    var S2 = [
        0.241922, 0, -0.970296, 0.758078,
        0, 1, 0, 0,
        -0.970296, 0, -0.241922, 0.970296,
        0, 0, 0, 1
    ];

    // The ship has been doubled in size, rotated    std::vector<glp::Mat4<float32>> matrices {
    // glp::rotateY(-30.0f),
    // glp::rotateX(-45.0f),
    // glp::scale(0.5f)
    // };

    // glp::compose(matrices).print(); 45 degrees around the x axis, 30 degrees around the y axis, and moved to (1,1,-2). Return the ship in its original position
    var I1 = [
        0.433013, 0, -0.25, -0.933013,
        0.176777, 0.353553, 0.306186, 0.0820423,
        0.176777, -0.353553, 0.306186, 0.789149,
        0, 0, 0, 1,];

    return [R1, S1, S2, I1];
}


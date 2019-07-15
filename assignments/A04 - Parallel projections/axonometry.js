function axonometry() {
    // Make an isometric view, w = 40, a = 16/9, n = 1, f = 101.
    // glp::isometricProjection<float32>(40.0f, 1.0f, 101.0f, 16.0f/9.0f)
    var A1 = [
        0.0176777, 0, 0.0176777, 0,
        0.0181424, 0.0362907, -0.0181424, 0,
        0.0115476, -0.0115458, -0.0115476, -1.02,
        0, 0, 0, 1
    ];

    // Make a dimetric view, w = 40, a = 16/9, n = 1, f = 101, rotated 20 around the x-axis
    // glp::dimetricProjection<float32>(20, 40, 1, 101, 16.0f / 9.0f)
    var A2 = [
        0.0176777, 0, 0.0176777, 0,
        0.0107487, 0.0417641, -0.0107487, 0,
        0.0132893, -0.0068404, -0.0132893, -1.02,
        0, 0, 0, 1
    ];

    // Make a trimetric view, w = 40, a = 16/9, n = 1, f = 101, rotated -30 around the x-axis and 30 around the y-axis
    // glp::trimetricProjection<float32>(-30, 30, 40, 1, 101, 16.0f / 9.0f)
    var A3 = [
        0.0216506, 0, 0.0125, 0,
        -0.0111111, 0.03849, 0.019245, 0,
        0.00866025, 0.01, -0.015, -1.02,
        0, 0, 0, 1
    ];

    // Make an cavalier projection view, w = 40, a = 16/9, n = 1, f = 101, at 45 degrees
    // glp::cavalierProjection<float32>(45, 40, 1, 101, 16.0f / 9.0f)
    var O1 = [
        0.025, 0, -0.0176777, 0,
        0, 0.0444444, -0.031427, 0,
        0, 0, -0.02, -1.02,
        0, 0, 0, 1
    ];

    // Make a cabinet projection view, w = 40, a = 16/9, n = 1, f = 101, at 60 degrees
    // glp::cabinetProjection<float32>(60, 40, 1, 101, 16.0f/9.0f)
    var O2 = [
        0.025, 0, -0.00625, 0,
        0, 0.0444444, -0.019245, 0,
        0, 0, -0.02, -1.02,
        0, 0, 0, 1
    ];

    return [A1, A2, A3, O1, O2];
}


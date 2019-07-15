function parallel() {
    // Build a parallel projection matrix, for a 16/9 viewport, with halfwidt w=40, near plane n=1, and far plane f=101.
    // glp::orthogonalProjection(40.0f, 1.0f, 101.0f, 16.0f/9.0f
    var out = [
        0.025, 0, 0, 0,
        0, 0.0444444, 0, 0,
        0, 0, -0.02, -1.02,
        0, 0, 0, 1
    ];

    return out;
}


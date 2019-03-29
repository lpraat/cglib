function parallel() {
    // Build a parallel projection matrix, for a 16/9 viewport, with halfwidt w=40, near plane n=1, and far plane f=101.
    var out = [
        0.34727, 0, 0, 0,
        0, 0.61737, 0, 0,
        0, 0, -1.00198, -0.200198,
        0, 0, -1, 0
    ];

    return out;
}

